/*
* Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk>
* Advanced Processor Technologies Group, School of Computer Science
* University of Manchester, Manchester M13 9PL UK
*
* This source code is free software; you can redistribute it
* and/or modify it in source code form under the terms of the GNU
* General Public License as published by the Free Software
* Foundation; either version 2 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/

/*
* A saif database
* 06/02/2014 Wei Song
*
*
*/

#include "saif_db.hpp"
#include <boost/foreach.hpp>

using std::string;

saif::SaifRecord::SaifRecord() {
  T0 = 0;
  T1 = 0;
  TX = 0;
  TZ = 0;
  TC = 0;
  IG = 0;
  TB = 0;
}

std::ostream& saif::SaifRecord::streamout( std::ostream& os) const {
  os << "(T0 " << T0.get_str() << ") ";
  os << "(T1 " << T1.get_str() << ") ";
  os << "(TX " << TX.get_str() << ") ";
  os << "(TZ " << TZ.get_str() << ") ";
  os << "(TC " << TC.get_str() << ") ";
  os << "(IG " << IG.get_str() << ") ";
  os << "(TB " << TB.get_str() << ") ";
  return os;
}

std::ostream& saif::SaifSignal::streamout( std::ostream& os) const {
  return streamout(os, "", "");
}

std::ostream& saif::SaifSignal::streamout( std::ostream& os, const std::string& sig, const std::string& dim) const {
  if(bits.empty()) {
    os << "(" << sig << dim << std::endl;
    os << "    " << *data << std::endl;
    os << ")" << std::endl;
  } else {
    std::pair<const int, boost::shared_ptr<SaifSignal> > it;
    BOOST_FOREACH(it, bits) {
      mpz_class m = it.first;
      m_dim = dim + "[" + m.get_str() + "]";
      it.second->streamout(os, sig, m_dim);
    }
  }
  return os;
}

std::ostream& saif::SaifInstance::streamout( std::ostream& os) const {
  return streamout(os, "");
}

std::ostream& saif::SaifInstance::streamout( std::ostream& os, const std::string name) const {
  os << "(INSTANCE ";

  if(module_name.empty())
    os << name << std::endl;
  else
    os << "\"" << module_name << "\" " << name << std::endl;

  if(!signals.empty()) {
    os << "(NET" << std::endl;
    std::pair<const string&, boost::shared_ptr<SaifSignal> > it;
    BOOST_FOREACH(it, signals) {
      it.second->streamout(os, it.first, "");
    }
    os << ")" << std::endl;
  }

  if(!instances.empty()) {
    std::pair<const string&, boost::shared_ptr<SaifInstance> > it;
    BOOST_FOREACH(it, instance) {
      it.second->streamout(os, it.first);
    }
  }

  return os;
}

