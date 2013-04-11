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

#ifndef UTL_FUNCINSERT_H
#define UTL_FUNCINSERT_H

#include "utl/forceInsert.hh"

namespace utl
{
    template<class mapClass, class functionClass>
    class funcInsertOne :
        public std::unary_function<typename mapClass::value_type, void>
    {
        mapClass& rTarget;
        const functionClass keyFunction;
    public:
        funcInsertOne( mapClass& rTargetMap,
                       const functionClass& rKeyFunction ) :
            rTarget( rTargetMap ),
            keyFunction( rKeyFunction )
        {}
        
        void operator()( const typename mapClass::value_type& rKeyValuePair ) const
        {
            forceInsert
                ( rTarget,
                  typename mapClass::value_type( keyFunction( rKeyValuePair.first ),
                                                 rKeyValuePair.second ) );
        }
    };
    
    /*! \ingroup mzrGroup
      \brief ForceInsert with remapping of keys.
      
      Function class that applies a given function to the key
      before force-inserting into a given map.  This is used
      to apply a plexMap to the sitesSpecs in (siteSpec, siteParam)
      pairs, where the plexMap is an injection of an omniplex into
      a plex. */
    template<class mapClass, class functionClass>
    void
    funcInsert( mapClass& rTargetMap,
                const functionClass& rKeyFunction,
                typename mapClass::const_iterator startIter,
                typename mapClass::const_iterator stopIter )
    {
        for_each( startIter,
                  stopIter,
                  funcInsertOne<mapClass, functionClass> ( rTargetMap,
                                                           rKeyFunction ) );
    }
    
}

#endif // UTL_FUNCINSERT_H
