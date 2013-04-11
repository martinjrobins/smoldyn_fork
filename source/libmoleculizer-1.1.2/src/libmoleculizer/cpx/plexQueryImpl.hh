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

#ifndef CPX_PLEXQUERYIMPL_H
#define CPX_PLEXQUERYIMPL_H

#include "cpx/molState.hh"
#include "cpx/cpxXcpt.hh"

namespace cpx
{
    template<class plexQueryT>
    class returnsFalseTracked :
        public std::unary_function<plexQueryT*, bool>
    {
    public:
        typedef typename plexQueryT::plexSpeciesType plexSpeciesType;
        typedef typename plexQueryT::specType specType;
        
    private:
        const plexSpeciesType& rSpecies;
        const specType& rSpec;
        
    public:
        returnsFalseTracked( const plexSpeciesType& rPlexSpecies,
                             const specType& rSubPlexSpec ) :
            rSpecies( rPlexSpecies ),
            rSpec( rSubPlexSpec )
        {}
        
        bool
        operator()( const plexQueryT* pQuery ) const
        {
            return ! pQuery->applyTracked( rSpecies,
                                           rSpec );
        }
    };
    
    template<class plexSpeciesT,
             class omniPlexT>
    bool
    andPlexQueries<plexSpeciesT,
                   omniPlexT>::
    applyTracked( const plexSpeciesType& rPlexSpecies,
                  const specType& rSpec ) const
    {
        return
            this->queries.end()
            == std::find_if( this->queries.begin(),
                             this->queries.end(),
                             returnsFalseTracked<plexQueryType> ( rPlexSpecies,
                                                                  rSpec ) );
    }
    
    template<class plexSpeciesT,
             class omniPlexT>
    bool
    molStatePlexQuery<plexSpeciesT,
                      omniPlexT>::
    operator()
        ( const typename molStatePlexQuery::plexSpeciesType& rPlexSpecies ) const
    {
        return rQuery( rPlexSpecies.molParams[theMolSpec] );
    }
    
    template<class plexSpeciesT,
             class omniPlexT>
    bool
    molStatePlexQuery<plexSpeciesT,
                      omniPlexT>::
    applyTracked( const typename molStatePlexQuery::plexSpeciesType& rPlexSpecies,
                  const typename molStatePlexQuery::specType& rSpec ) const
    {
        // Map the mol index through the injection of the omniplex into
        // the complex.
        int molNdx = rSpec.getInjection().forward.molMap[theMolSpec];
        
        // Test the state of the mol at the mapped mol index.
        return rQuery( rPlexSpecies.molParams[molNdx] );
    }
}

#endif // CPX_PLEXQUERYIMPL_H
