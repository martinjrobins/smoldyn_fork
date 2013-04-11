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

#ifndef CPX_QUERYALLOLIST_H
#define CPX_QUERYALLOLIST_H

#include <list>
#include "cpx/plexQuery.hh"


namespace cpx
{
    // This lists the allosteric shapes that a complex takes on when
    // its components satisfy state queries.  This optimizes traversal,
    // since we must run through all the queries for every new species
    // in the plex family.
    template<class plexSpeciesT,
             class omniPlexT>
    class queryAllosteryList :
        public std::list<std::pair<const andPlexQueries<plexSpeciesT,
                                                        omniPlexT>*,
                                   siteToShapeMap> >
    {
    public:
        typedef andPlexQueries<plexSpeciesT,
                               omniPlexT> queryType;
        
        typedef subPlexSpec<omniPlexT> specType;
        
        // This is used to add allostery to a plexFamily.
        //
        // One adds a plexQuery and a specification of all the shapes of the
        // complex's binding sites.  When a new species of complex in the
        // plexFamily appears, then then the query is applied.  If it tests true,
        // then the associated shapes are overlaid on the new species's binding
        // site shapes.
        void
        addQueryAndMap( const queryType* pQuery,
                        const siteToShapeMap& rSiteToShapeMap );
        
        // Decided, for reasons unknown, to use overloading for the
        // version of this function that works through the injection
        // of a subcomplex into a complex.
        
        // Runs down the list, seeing which queries are satisfied
        // by the plexParam.  When a query, for now a query about
        // the states of the mols in the plex, is satisfied, then
        // the site shapes in the plexParam are set from the siteShapeMap
        // associated to the query.
        //
        // This is used in the construction of new plexSpecies that belong to a
        // plexFamily for which allosteric states have been declared (an
        // allosteric-plex.)
        void
        setSatisfiedQuerySiteShapes( plexSpeciesT& rSpecies ) const;
        
        // Same as the above, but the queries are pushed forward through
        // the injection, as are the changes in site shapes.
        //
        // This is used in the construction of new plexSpecies in which
        // allosteric subcomplexes have been detected.
        void
        setSatisfiedQuerySiteShapes( plexSpeciesT& rSpecies,
                                     const specType& rSubPlexSpec ) const;
    };
}

#include "cpx/queryAlloListImpl.hh"

#endif // CPX_QUERYALLOLIST_H
