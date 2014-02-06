// -*- Bison -*-
%skeleton "lalr1.cc"
%defines
%define namespace "saif"
%define parser_class_name "saif_parser"
%language "c++"
%output "saif.cc"
%parse-param {vcd::SaifLexer* lexer}
%parse-param {vcd::SaifDB* db}
%{

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
 * Bison grammer file for Saif file
 * 06/02/2014   Wei Song
 *
 *
 */

#include "saif_util.hpp"
#include "saif_db.hpp"

#define yylex lexer->lexer

  using namespace saif;

%}


%initial-action
{
}


///////////////////////////////////////////////////
// token definitions

%token SKeySAIFILE                  "SAIFILE"
%token SKeySAIFVERSION              "SAIFVERSION"
%token SKeyDIRECTION                "DIRECTION"
%token SKeyDESIGN                   "DESIGN"
%token SKeyDATE                     "DATE"
%token SKeyVENDOR                   "VENDOR"
%token SKeyPROGRAM_NAME             "PROGRAM_NAME"
%token SKeyVERSION                  "VERSION"
%token SKeyDIVIDER                  "DIVIDER"
%token SKeyTIMESCALE                "TIMESCALE"
%token SKeyDURATION                 "DURATION"
%token SKeyINSTANCE                 "INSTANCE"
%token SKeyPORT                     "PORT"
%token SKeyT0                       "T0"
%token SKeyT1                       "T1"
%token SKeyTZ                       "TZ"
%token SKeyTX                       "TX"
%token SKeyTC                       "TC"
%token SKeyIG                       "IG"

%token<tStr> SString
%token<tVar> SVar
%token<tNum> SNum

// the saif file

%start saif_file

saif_file
    : '(' "SAIFILE" saif_contents ')'

saif_contents
    : saif_line
    | saif_contents saif_line

saif_line
    : '(' "SAIFVERSION"  SString ')'   { db->version      = $3; }
    | '(' "DIRECTION"    SString ')'   { db->direction    = $3; }
    | '(' "DESIGN"')'               
    | '(' "DATE"         SString ')'   { db->date         = $3; }
    | '(' "VENDOR"       SString ')'   { db->vendor       = $3; }
    | '(' "PROGRAM_NAME" SString ')'   { db->program_name = $3; }
    | '(' "VERSION"      SString ')'   { db->tool_version = $3; }
    | '(' "DIVIDER"      SString ')'   { db->divider      = $3; }
    | '(' "TIMESCALE"    SNum SVar ')'    
    { 
      db->timescale    = std::pair<mpz_class, std::string>($3, $4); 
    }
    | '(' "DURATION"     SNum    ')'   { db->duration = $3; }
    | saif_instance                    

saif_insatnces
    : saif_insatnce
    | saif_insatnces saif_insatnce

saif_insatnce
    : '(' "INSTANCE" SString SVar instance_contents ')'
    | '(' "INSTANCE" SVar instance_contents ')'

instance_contents
    : port_list
    | port_list instances

port_list
    : '(' "PORT" signal_lists ')'

signal_lists
    : signal
    | signal_lists signal

signal
    : '(' SVar activities ')'

activities
    : activity
    | activities activity

activity
    : '(' "T0" SNum ')'
    | '(' "T1" SNum ')'
    | '(' "TX" SNum ')'
    | '(' "TZ" SNum ')'
    | '(' "TC" SNum ')'
    | '(' "IG" SNum ')'
