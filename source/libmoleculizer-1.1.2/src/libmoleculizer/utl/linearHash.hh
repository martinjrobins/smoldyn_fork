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
//   Larry Lok, Research Fellow, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//

#ifndef LINEARHASH_H
#define LINEARHASH_H

/*! \file linearHash.hh
  \ingroup mzrGroup
  \brief Defines a linear-congruential hash function class.
  
  I don't think the hash functions given in our STL implementation
  look particularly promising.  Ints, chars, etc all hash to
  themselves, for example.  If I try to hash addresses, they'll all
  come out multiples of 4.  */

#include <cstddef>
#include <string>

namespace utl
{
    /*! \ingroup mzrGroup
      \brief Linear-congruential hash function class.
      
      This really could just as well, maybe better, have been a global
      function.
      
      \todo Use overloading to handle common types transparently.
      Make provision (another class?) to do accumulation of hash values.
      This provision could be used in the "common types" code.
      
    */
    class linearHash
    {
        // These will need to be adjusted, I expect.  Or maybe not.
        static const size_t multiplier;
        static const size_t summand;
    public:
        size_t operator()( const size_t& rData ) const;
        size_t operator()( const std::string& rString ) const;
    };
}

#endif
