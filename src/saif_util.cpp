/*
 * Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk> 
 *    Advanced Processor Technologies Group, School of Computer Science
 *    University of Manchester, Manchester M13 9PL UK
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

/* 
 * Helper classes for the saif parser
 * 06/02/2014   Wei Song
 *
 *
 */

#include "saif_util.hpp"
#include "saif.hh"
#include <boost/lexical_cast.hpp>

using namespace vcd;
using std::string;

#define TType saif_parser::token 

saif::SaifLexer::SaifLexer(std::istream * i) 
  : istm(i), sub_state(0), m_string("") { state.push_back(S_BEGIN); }

int saif::SaifLexer::lexer(vcd_token_type * rv) {
  while(true) {
    if(buf.empty()) {
      if(istm->eof()) return 0;
      else std::getline(*istm, buf);
    }

    std::cout << "buf: " << buf;

    string token = next_token();
    if(token.empty()) continue;
    else {
      if(!validate_token(token, rv, rvt)) 
        continue;
      std::cout << " |token: " << rvt << std::endl;
      return rvt;
    }
  }
}

string saif::SaifLexer::next_token() {
  // get rid of starting blanks
  buf.erase(0, buf.find_first_not_of(' '));
  if(buf.empty()) return buf;

  unsigned int loc = buf.find_first_of("\"() ");
  if(loc == 0) loc = 1;
  string t = buf.substr(0, loc);
  buf.erase(0, loc);
  return t;
}

bool saif::SaifLexer::token_helper(int vrvt, 
                                   bool push_state, unsigned int new_state,
                                   unsigned int sub_state,
                                   bool pop_state,
                                   int& rvt,
                                   bool breturn) {
  if(push_state)
    state.push_back(std::pair<unsigned int, unsigned int>(new_state, 0));
  
  state.back().second = sub_state;
  
  if(pop_state)
    state.pop_back();

  rvt = vrvt;

  return breturn;
}

bool saif::SaifLexer::validate_token(const string& t, vcd_token_type * tt, int& rvt) {
  switch(state.back().first) {
  case S_BEGIN: {
    if(t == "(")           
      return token_helper('(',                      true, S_BEGIN,     0, false, rvt, true);
    if(t == "SAIFILE")    
      return token_helper(TType::SKeySAIFILE,       true, S_BEGIN,     0, false, rvt, true);
    if(t == "SAIFVERSION")
      return token_helper(TType::SKeySAIFVERSION,   true, S_DEF,       0, false, rvt, true);
    if(t == "DIRECTION")  
      return token_helper(TType::SKeyDIRECTION,     true, S_DEF,       0, false, rvt, true);
    if(t == "DESIGN")     
      return token_helper(TType::SKeyDESIGN,        true, S_DEF,       0, false, rvt, true);
    if(t == "DATE")       
      return token_helper(TType::SKeyDATE,          true, S_DEF,       0, false, rvt, true);
    if(t == "VENDOR")     
      return token_helper(TType::SKeyVENDOR,        true, S_DEF,       0, false, rvt, true);
    if(t == "PROGRAM_NAME")
      return token_helper(TType::SKeyPROGRAME_NAME, true, S_DEF,       0, false, rvt, true);
    if(t == "VERSION")    
      return token_helper(TType::SKeyVERSION,       true, S_DEF,       0, false, rvt, true);
    if(t == "DIVIDER")    
      return token_helper(TType::SKeyDIVIDER,       true, S_DIV,       0, false, rvt, true);
    if(t == "TIMESCALE")  
      return token_helper(TType::SKeyTIMESCALE,     true, S_TIMESCALE, 0, false, rvt, true);
    if(t == "DURATION")   
      return token_helper(TType::SKeyDURATION,      true, S_DUR,       0, false, rvt, true);
    if(t == "INSTANCE")   
      return token_helper(TType::SKeyINSTANCE,      true, S_INST,      0, false, rvt, true);
    if(t == ")")          
      return token_helper(')',                      false, 0,          0, true,  rvt, true);
    assert(0 == "should not reach here");
    break;
  }
  case S_DEF: {
    if(t== "\"") {
      if(m_string == "")  
        return token_helper(0,                      true, S_STR,       0, false,  rvt, false);
      else {
        tt->tStr = m_string;
        m_string = "";
        return token_helper(TType::SString,         false, 0,          0, true,   rvt, true);
      }
    } else {
      tt->tStr = t;
      return token_helper(TType::SString,           false, 0,          0, true,   rvt, true);
    }
  }
  case S_DIV: {
    tt->tStr = t;
    return token_helper(TType::SString,             false, 0,          0, true,   rvt, true);
  }
  case S_TIMESCALE: {
    switch(state.back().second) {
    case 0: {
      tt->tNum = mpz_class(t);
      return token_helper(TType::SNum,              false, 0,          1, false,  rvt, true);
    }
    case 1: {
      tt->tVar = t;
      return token_helper(TType::SVar,              false, 0,          0, true,   rvt, true);
    }
    default:
      assert(0 == "should not reach here");
    }
    break;
  }
  case S_DUR: {
    tt->tNum = mpz_class(t);
    return token_helper(TType::SNum,                false, 0,          0, true,   rvt, true);
  }
  case S_INST: {
    switch(state.back().second) {
    case 0: {
      if(t == "\"") {
        if(m_string == "")
          return token_helper(0,                    true, S_STR,       0, false,  rvt, false);
        else {
          tt->tStr = m_string;
          m_string = "";
          return token_helper(TType::SString,       false, 0,          0, false,  rvt, true);
        }
      } else {
        tt->tStr = t;
        return token_helper(TType::SVar,            false, 0,          1, false,  rvt, true);
      }
    }
    case 1: {
      if(t== "(") 
        return token_helper('(',                    false, 0,          2, false,  rvt, true);
      else {
        assert(0 == "should not reach here");
      }
      break;
    }
    case 2: {
      if(t == "INSTANCE")
        return token_helper(TType::SKeyINSTANCE,    true, S_INST,      0, false,  rvt, true);
      if(t == "PORT")
        return token_helper(TType::SKeyPORT,        true,  S_PORT,     0, false,  rvt, true);
      if(t == ")")
        return token_helper(')',                    false, 0,          0, true,   rvt, true);
      assert(0 == "should not reach here");
      break;
    }
    default:
      assert(0 == "should not reach here");
    }
    break;
  }
  case S_PORT: {
    if(t == "(") 
      return token_helper('(',                      true, S_SIG,       0, false,  rvt, true);
    
    if(t == ")")
      return token_helper(')',                      false, 0,          0, true,   rvt, true);
    assert(0 == "should not reach here");
    break;
  }
  case S_SIG: {
    switch(state.back().second) {
    case 0: {
      return token_helper(TType::SVar,              false, 0,          1, false,  rvt, true);
    }
    case 1: {
      if(t == "(")
        return token_helper('(',                    false, 0,          2, false,  rvt, true);
      if(t == ")")
        return token_helper(')',                    false, 0,          0, true,   rvt, true);
      assert(0 == "should not reach here");
      break;
    }
    case 2: {
      if(t == "T0")
        return token_helper(TType::SKeyT0,          false, 0,          3, false,  rvt, true);
      if(t == "T1")
        return token_helper(TType::SKeyT1,          false, 0,          3, false,  rvt, true);
      if(t == "TX")
        return token_helper(TType::SKeyTX,          false, 0,          3, false,  rvt, true);
      if(t == "TZ")
        return token_helper(TType::SKeyTZ,          false, 0,          3, false,  rvt, true);
      if(t == "TC")
        return token_helper(TType::SKeyTC,          false, 0,          3, false,  rvt, true);
      if(t == "IG")
        return token_helper(TType::SKeyIG,          false, 0,          3, false,  rvt, true);
      assert(0 == "should not reach here");
      break;
    }
    case 3: {
      tt->tNum = mpz_class(t);
      return token_helper(TType::SNum,              false, 0,          4, false,  rvt, true);
    }
    case 4: {
      if(t == ")")
        return token_helper(')',                    false, 0,          1, false,  rvt, true);
      assert(0 == "should not reach here");
      break;
    }
    default:
      assert(0 == "should not reach here");
    }
    break;
  }
  case S_STR: {
    if(t == "\"")
      return token_helper(0,                         false, 0,          0, true,  rvt, false);
    else {
      m_string += " " + t;
      return false;
    }
  }
  default:
    assert(0 == "should not reach here");
  }
  return false;
}
        

    
    
    



    


    