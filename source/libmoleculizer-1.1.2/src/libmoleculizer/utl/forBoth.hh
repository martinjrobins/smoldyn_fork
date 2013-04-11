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

#ifndef UTL_FORBOTH_H
#define UTL_FORBOTH_H

namespace utl
{
    // A binary version of std::for_each.
    template<class inputIter1, class inputIter2, class binaryOp>
    binaryOp
    for_both( inputIter1 first1,
              inputIter1 last1,
              inputIter2 first2,
              binaryOp op )
    {
        while ( first1 != last1 )
        {
            op( *first1, *first2 );
            // Postincrement is costly for some iterators.
            ++first1;
            ++first2;
        }
        return op;
    }
}

#endif // UTL_FORBOTH_H
