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

#ifndef FND_QUERYIMPL_H
#define FND_QUERYIMPL_H

#include <functional>
#include <algorithm>

namespace fnd
{
    template<class queryT>
    class queryReturnsFalse :
        public std::unary_function<queryT*, bool>
    {
    public:
        typedef typename queryT::argType argType;
        
        const typename queryT::argType& rArg;
        
        queryReturnsFalse( const argType& rArgument ) :
            rArg( rArgument )
        {}
        
        bool
        operator()( const queryT* pQuery ) const
        {
            const queryT& rQuery = *pQuery;
            return ! rQuery( rArg );
        }
    };
    
    template<class queryT>
    bool
    andQueries<queryT>::
    operator()( const typename queryT::argType& rArg ) const
    {
        return queries.end() == std::find_if( queries.begin(),
                                              queries.end(),
                                              queryReturnsFalse<queryT> ( rArg ) );
    }
}

#endif // FND_QUERYIMPL_H
