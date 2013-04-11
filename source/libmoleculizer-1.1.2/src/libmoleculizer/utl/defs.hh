//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//        This file is part of Libmoleculizer
//
//        Copyright (C) 2001-2009 The Molecular Sciences Institute.
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// Moleculizer is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Moleculizer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Moleculizer; if not, write to the Free Software Foundation
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307,  USA
//
// END HEADER
//
// Original Author:
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute
//
// Modifing Authors:
//
//


#ifndef __DEFS_HPP
#define __DEFS_HPP

#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>
#include <cassert>

#include "xcpt.hh"

#include "AssocVector.h"
#include "autoCache.hh"
#include "autoCatalog.hh"
#include "autoVector.hh"


// Acknowledgements:
// The "DECLARE_XXX" macros were copied from stuff written by Koichi
// Takahashi in his work on the E-Cell Project <http://www.e-cell.org>.
//

#define DECLARE_TYPE( mydecl, mytype )          \
    typedef mydecl         mytype;                   \
    typedef mytype *       mytype ## Ptr;                      \
    typedef const mytype * mytype ## Cptr;                     \
    typedef mytype &       mytype ## Ref;                      \
    typedef const mytype & mytype ## Cref;                     \
    
/**
   Declare class , class pointer ,
   const pointer, class reference
   and const class reference types for classes. For example
   DECLARE_CLASS( Exception );
   @param tag The class being declared
*/

#define DECLARE_CLASS( tag )         \
    class   tag;                     \
    typedef tag *       tag ## Ptr;            \
    typedef const tag * tag ## Cptr;                    \
    typedef tag &       tag ## Ref;                     \
    typedef const tag & tag ## Cref;


// Some global types.
typedef int Integer;
typedef float Real;
typedef std::string String;

#endif
