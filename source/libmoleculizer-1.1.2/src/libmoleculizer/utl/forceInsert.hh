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

#ifndef UTL_FORCEINSERT_H
#define UTL_FORCEINSERT_H

#include <utility>
#include <functional>
#include <algorithm>

namespace utl
{
    template<class mapClass>
    void
    forceInsert( mapClass& rTargetMap,
                 const typename mapClass::value_type& rKeyValuePair )
    {
        // Attempt to insert the key/value pair.  This will not succeed if
        // the key is already associated to a value by the map.
        typename std::pair<typename mapClass::iterator, bool> insertResult
            = rTargetMap.insert( rKeyValuePair );
        
        // If the key was already mapped, reset the value associated to it.
        if ( ! insertResult.second )
        {
            typename mapClass::iterator iEntry = insertResult.first;
            iEntry->second = rKeyValuePair.second;
        }
    }
    
    template<class mapClass>
    class forceInsertOne :
        public std::unary_function<typename mapClass::value_type, void>
    {
        mapClass& rTarget;
        
    public:
        forceInsertOne( mapClass& rTargetMap ) :
            rTarget( rTargetMap )
        {}
        
        void operator()( const typename mapClass::value_type& rEntry ) const
        {
            forceInsert( rTarget,
                         rEntry );
        }
    };
    
    template<class mapClass>
    void
    forceInsert( mapClass& rTargetMap,
                 typename mapClass::const_iterator startIter,
                 typename mapClass::const_iterator stopIter )
    {
        std::for_each( startIter,
                       stopIter,
                       forceInsertOne<mapClass> ( rTargetMap ) );
    }
}

#endif // UTL_FORCEINSERT_H
