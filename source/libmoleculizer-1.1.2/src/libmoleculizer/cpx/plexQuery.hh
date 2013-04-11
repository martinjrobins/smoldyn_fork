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

#ifndef PLEXQUERY_H
#define PLEXQUERY_H

#include <algorithm>
#include "fnd/query.hh"
#include "cpx/subPlexSpec.hh"
#include "cpx/molStateQuery.hh"

namespace cpx
{
    /*! \ingroup plexSpeciesGroup
      \brief Base class for queries about complexes.
      
      Adds to plexSpecies::paramQuery the need to map plexParam argument
      through a plex isomorphism. This way of applying the query is used
      in omniplex dumps.
    */
    template<class plexSpeciesT,
             class omniPlexT>
    class plexQuery :
        public fnd::query<plexSpeciesT>
    {
    public:
        typedef plexSpeciesT plexSpeciesType;
        typedef omniPlexT omniPlexType;
        typedef subPlexSpec<omniPlexType> specType;
        
        // Seem to be having some trouble overloading operator() to handle
        // this case.
        virtual bool
        applyTracked( const plexSpeciesT& rPlexSpecies,
                      const specType& rSpec ) const = 0;
    };
    
    /*! \brief Auxiliary function class to help do AND of a number of
      plexQueries. */
    template<class plexSpeciesT,
             class omniPlexT>
    class andPlexQueries :
        public fnd::andQueries<plexQuery<plexSpeciesT,
                                         omniPlexT> >
    {
    public:
        typedef plexSpeciesT plexSpeciesType;
        typedef omniPlexT omniPlexType;
        
        typedef plexQuery<plexSpeciesType,
                          omniPlexType> plexQueryType;
        
        typedef subPlexSpec<omniPlexType> specType;
        
        bool
        applyTracked( const plexSpeciesT& rPlexSpecies,
                      const specType& rSpec ) const;
    };
    
    // Applies a test of state to a particular mol instance in a complex.
    //
    // At this point in time, the only mols with variable state are modMols.
    template<class plexSpeciesT,
             class omniPlexT>
    class molStatePlexQuery :
        public plexQuery<plexSpeciesT,
                         omniPlexT>
    {
        // The index in the complex of the mol whose state is to be queried.
        molSpec theMolSpec;
        
        // Query for state of mol.
        const cpx::molStateQuery& rQuery;
        
    public:
        
        molStatePlexQuery( const molSpec& rMolSpec,
                           const cpx::molStateQuery& rMolStateQuery ) :
            theMolSpec( rMolSpec ),
            rQuery( rMolStateQuery )
        {}
        
        bool
        operator()
        ( const typename molStatePlexQuery::plexSpeciesType& rPlexSpecies ) const;
        
        bool
        applyTracked( const typename molStatePlexQuery::plexSpeciesType& rPlexSpecies,
                      const typename molStatePlexQuery::specType& rSpec ) const;
    };
}

#include "cpx/plexQueryImpl.hh"

#endif
