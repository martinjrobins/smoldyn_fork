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

#ifndef OMNIPLEX_H
#define OMNIPLEX_H

#include "cpx/plexQuery.hh"
#include "cpx/omniPlexFeature.hh"
#include "cpx/omniStructureQuery.hh"

namespace cpx
{
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class omniPlex
    {
    public:
        typedef molT molType;
        typedef plexT plexType;
        typedef plexSpeciesT plexSpeciesType;
        typedef plexFamilyT plexFamilyType;
        typedef omniPlexT omniPlexType;
        
        typedef
        fnd::query<omniStructureQueryArg<plexType> >
        structureQueryType;
        
        typedef andPlexQueries<plexSpeciesType,
                               omniPlexType> stateQueriesType;
        
        typedef omniPlexFeature<molType,
                                plexSpeciesType,
                                plexFamilyType,
                                omniPlexType> omniFeatureType;
        
    private:
        // Pointer to the plexFamily of this omniplex.
        //
        // One side-effect of this way of working is that one can't get from a
        // plexFamily to all of its associated omniplex stuff, since the same
        // plexFamily may be an omniplex in more than one way now.
        plexFamilyType* pFamily;
        
        // Reactions that want to listen for species that satisfy the
        // structural queries should listen to this feature.
        omniFeatureType subPlexFeature;
        
        // Structural queries that must be statisfied for this
        // omniplex to be connected to a plexFamily as one of its features.
        //
        // In order for compound queries like this to be copyable, it's necessary
        // (really easiest) to memory-manage queries centrally, in the plexUnit.
        fnd::andQueries<structureQueryType>* pStructureQueries;
        
        // State queries that must be satisfied for a plexSpecies
        // to be (allo)sterically modified as given by the siteShapeMap.
        //
        // In order for compound queries like this to be copyable, it's necessary
        // (really easiest) to memory-manage queries centrally, in the plexUnit.
        // This andPlexQueries object should be registered with plexUnit before
        // incorporation into the omniPlex.
        stateQueriesType* pStateQueries;
        
        // Map giving allosteric site shapes.  When a plexFamily is recognized, it
        // may pass the structureQueries above and be connected to this omni.
        // Then, when a new plexSpecies in that family is created, its state may
        // pass the stateQueries above.  If so, then the shapes of the sites in
        // the new plexSpecies that correspond to the sites in this map are set to
        // the shapes given by this map.
        //
        // This is only used in allosteric omnis; it remains empty as constructed
        // otherwise.
        siteToShapeMap alloSiteMap;
        
    public:
        template<typename structureQueryType>
        omniPlex( plexFamilyType* pPlexFamily,
                  fnd::andQueries<structureQueryType>* pOmniStructureQueries,
                  stateQueriesType* pOmniStateQueries ) :
            pFamily( pPlexFamily ),
            pStructureQueries( pOmniStructureQueries ),
            pStateQueries( pOmniStateQueries )
        {}
        
        plexFamilyType*
        getFamily( void ) const
        {
            return pFamily;
        }
        
        // For use in plexFamily::connectToFeatures.
        const fnd::andQueries<structureQueryType>&
        getStructureQuery( void ) const
        {
            return *pStructureQueries;
        }
        
        // Used in omniPlexFeature::notifyNew.  Perhaps omniPlexFeature should
        // be a friend?
        const stateQueriesType*
        getStateQuery( void ) const
        {
            return pStateQueries;
        }
        
        // Replaces plexFamily::getSubPlexFeature, now that a complex can be omni
        // in more than one way.
        omniFeatureType*
        getSubPlexFeature( void )
        {
            return &subPlexFeature;
        }
        
        // For use in plex allostery routines.
        const siteToShapeMap&
        getSiteToShapeMap( void ) const
        {
            return alloSiteMap;
        }
        
        // For use in the parser.
        siteToShapeMap&
        getSiteToShapeMap( void )
        {
            return alloSiteMap;
        }
    };
}

#endif // OMNIPLEX_H
