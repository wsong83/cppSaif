# Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk>
# Advanced Processor Technologies Group, School of Computer Science
# University of Manchester, Manchester M13 9PL UK
#
# This source code is free software; you can redistribute it
# and/or modify it in source code form under the terms of the GNU
# General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
#
# 07/02/2014 Wei Song
#
#

INCDIRS = -I.
TARGETS = saif_db.o saif_parser.o
LINK_FLAGS = -lgmp -lgmpxx


all: saif.o saif_util.o $(TARGETS) test

saif.o: saif.cc saif.y saif_util.hpp saif_db.hpp
$(CXX) $(INCDIRS) $(CXXFLAGS) -c $< -o $@

saif_util.o: saif_util.cpp saif_util.hpp saif_db.hpp
$(CXX) $(INCDIRS) $(CXXFLAGS) -c $< -o $@

$(TARGETS): %.o:%.cpp %.hpp
$(CXX) $(INCDIRS) $(CXXFLAGS) -c $< -o $@

test: test.cpp
$(CXX) $< saif.o saif_util.o $(TARGETS) $(LINK_FLAGS) -o $@

bison: saif.cc

saif.cc: saif.y
$(BISON_EXE) $<

.PHONY: clean

clean:
-rm *.o *.output
-rm saif.cc saif.hh stack.hh location.hh position.hh


