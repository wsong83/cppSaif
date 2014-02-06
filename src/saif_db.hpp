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

#ifndef SAIF_DB_H_
#define SAIF_DB_H_

#include <string>
#include <map>
#include <vector>
#include <gmpxx.h>


namespace saif {

  // a switch record
  class SaifRecord {
  public:
    SaifRecord();
    mpz_class T0;
    mpz_class T1;
    mpz_class TX;
    mpz_class TZ;
    mpz_class TC;
    mpz_class IG;
  };

  // a signal (single or multi bits)
  class SaifSignal {
  public:
    std::map<int, SaifSignal> bits;
    SaifRecord data;
  };

  // instance
  class SaifInstance {
  public:
    std::map<std::string, SaifSignal> ports;
    std::map<std::string, SaifInstance> instances;
    std::string module_name;
  };

}

#endif