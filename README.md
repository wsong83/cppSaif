cppSaif
=======

(version 1.0)

A C++ parser for SAIF (Switching Activity Interchange Format) file.

=======

File list:

  AUTHORS                          Author list  
  COPYING                          Copy right information
  LICENSE                          License file
  src                              Source code
    Makefile                       GNU Make file
    saif.y                         Bison grammer file
    saif_db.hpp                    SAIF database (stor the abstract syntex tree)
    saif_db.cpp                    
    saif_parser.hpp                SAIF parser wrapper
    saif_parser.cpp                
    saif_util.hpp                  SAIF Lexer and utility classes
    saif_util.cpp  
    test.cpp                       A simple test program
  Makefile                         Globale Make file
  README                           Read me file
  README.md

=======

Library requirement

GNU G++ (> 4.1)
GNU BOOST Library
GNU MP Bignum Library
Bison

=======

Compile instruction

  make
