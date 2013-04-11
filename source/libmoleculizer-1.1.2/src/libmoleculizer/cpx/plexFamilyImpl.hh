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

#ifndef CPX_PLEXFAMILYIMPL_H
#define CPX_PLEXFAMILYIMPL_H

#include "cpx/cpxXcpt.hh"
#include "cpx/reportIsoSearch.hh"

namespace cpx
{
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    plexFamily( const plexType& rParadigm,
                knownBindings<molType, bindingFeatureType>& refKnownBindings,
                std::set<plexFamilyType*>& refOmniplexFamilies ) :
        paradigm( rParadigm ),
        rKnownBindings( refKnownBindings ),
        rOmniPlexFamilies( refOmniplexFamilies )
    {}
    
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    respond( const fnd::newSpeciesStimulus<plexSpeciesType>& rStim )
    {
        freeSiteFeatures.respond( rStim );
        bindingFeatures.respond( rStim );
        molFeatures.respond( rStim );
        omniFeatures.respond( rStim );
    }


    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    dumpablesRespond( const fnd::newSpeciesStimulus<plexSpeciesType>& rStim )
    {
        freeSiteFeatures.dumpablesRespond( rStim );
        bindingFeatures.dumpablesRespond( rStim );
        molFeatures.dumpablesRespond( rStim );
        omniFeatures.dumpablesRespond( rStim );
    }
    
    // For accumulating all the plexSpecies in order to update them
    // by zero when regenerating reaction network.
    template<class plexSpeciesT,
             class plexFamilyT>
    class accumulateOneSpecies :
        public std::unary_function<typename plexFamilyT::value_type, void>
    {
    public:
        std::vector<plexSpeciesT*>& rAllSpecies;
        
    public:
        accumulateOneSpecies( std::vector<plexSpeciesT*>& rAllPlexSpecies ) :
            rAllSpecies( rAllPlexSpecies )
        {}
        
        void
        operator()( const typename accumulateOneSpecies::argument_type& rEntry ) const
        {
            rAllSpecies.push_back( rEntry.second );
        }
    };
    
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    accumulateSpecies( std::vector<plexSpeciesType*>& rAllSpecies )
    {
        std::for_each( this->begin(),
                       this->end(),
                       accumulateOneSpecies<plexSpeciesType, plexFamilyType> ( rAllSpecies ) );
    }
    
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class plexFamily<molT,
                     plexT,
                     plexSpeciesT,
                     plexFamilyT,
                     omniPlexT>::applyOmniMods :
        public std::unary_function<typename plexFamily::omniMapType::value_type,
                                   void>
    {
    public:
        typedef plexSpeciesT plexSpeciesType;
        typedef omniPlexT omniPlexType;
        
        typedef subPlexSpec<omniPlexType> specType;
        
        typedef andPlexQueries<plexSpeciesType,
                               omniPlexType> queryType;
    private:
        plexSpeciesT& rSpecies;
        
    public:
        applyOmniMods( plexSpeciesType& refSpecies ) :
            rSpecies( refSpecies )
        {}
        
        void operator()( const typename applyOmniMods::argument_type& rEntry ) const
        {
            const specType& rSpec = rEntry.first;
            
            omniPlexType* pOmni = rSpec.getOmni();
            
            // Does the species pass the omniPlex's state query?
            const queryType* pQuery
                = pOmni->getStateQuery();
            
            if ( pQuery->applyTracked( rSpecies,
                                       rSpec ) )
            {
                // Get the omniplex's site to shape map.
                const siteToShapeMap& rOmniSiteToShapeMap
                    = pOmni->getSiteToShapeMap();
                
                // Insert the allosteric site shapes.
                rSpecies.siteParams.setSiteShapes( rOmniSiteToShapeMap,
                                                   rSpec );
            }
        }
    };
    
    
    // Remember that getMember(const std::vector<molParam>& rMolParams) is
    // the routine that's used routinely; it calls this and installs the
    // new species in the family.
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    plexSpeciesT*
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    makeMember( const std::vector<molParam>& rMolParams )
    {
        siteToShapeMap siteParams;
        
        // Construct an "initial cut" of the siteToShapeMap of the plex species
        // by asking the mols to look up the binding site shapes that they
        // associate to the states in which they are found.
        for ( int molNdx = 0;
              molNdx < ( int ) paradigm.mols.size();
              molNdx++ )
        {
            // Look up the allosteric forms of the sites on the mol,
            // as determined from the mol's state.
            molType* pMol = paradigm.mols[molNdx];
            const molState* pMolState = rMolParams[molNdx];
            const std::vector<siteParam>& rSiteParams
                = pMol->allostery( pMolState );
            
            // Install the allosteric siteParams into result.siteParams.
            for ( int siteNdx = 0;
                  siteNdx < ( int ) pMol->getSiteCount();
                  siteNdx++ )
            {
                siteParams.insert
                    ( siteToShapeMap::value_type( siteSpec( molNdx, siteNdx ),
                                                  rSiteParams[siteNdx] ) );
            }
        }
        
        // Construct the new plexSpecies.  This invocation likely has to change
        // in descendant types.
        plexSpeciesType* pNewSpecies
            = constructSpecies( siteParams,
                                rMolParams );
        
        // Modify the siteParams database as indicated by omniPlexes,
        // working "through" the injection.  This can't be done until the
        // species has been constructed, since omniFeatures pass incoming
        // species through a test.
        for_each( omniFeatures.begin(),
                  omniFeatures.end(),
                  applyOmniMods( *pNewSpecies ) );
        
        // Modify siteParams database as indicated by this plexFamily.
        // Similar to what is done for each omniPlex above, but without the
        // permutation.  Again, testing of the new species is involved.
        getAlloStateList().setSatisfiedQuerySiteShapes( *pNewSpecies );
        
        return pNewSpecies;
    }
    
    //   template<class omniPlexT>
    //   class omniHasParent :
    //     public std::unary_function<omniPlexT*, bool>
    //   {
    //     xmlpp::Node* pParent;
    //   public:
    //     omniHasParent(xmlpp::Node* pParentNode) :
    //       pParent(pParentNode)
    //     {}
    
    //     bool
    //     operator()(const typename omniHasParent::argument_type pOmniPlex) const
    //     {
    //       return pParent == (pOmniPlex->getParentNode());
    //     }
    //   };
    
    // Connects the plex family to its free site features; i.e. fills in
    // the freeSiteFeatures map.
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class plexFamily<molT,
                     plexT,
                     plexSpeciesT,
                     plexFamilyT,
                     omniPlexT>::doBehaviorizeSite :
        public std::unary_function<siteSpec, void>
    {
        // Do not use plexFamilyT here!
        plexFamily<molT, plexT, plexSpeciesT, plexFamilyT, omniPlexT>& rFamily;
    public:
        doBehaviorizeSite( plexFamily<molT, plexT, plexSpeciesT, plexFamilyT, omniPlexT>& rPlexFamily ) :
            rFamily( rPlexFamily )
        {}
        
        void
        operator()( const siteSpec& rSpec ) const
        {
            molType& rMol = * ( rFamily.getParadigm().mols[rSpec.molNdx()] );
            
            // The mol is also a vector of binding sites.
            typename molType::bindingSiteType& rBindingSite
                = rMol[rSpec.siteNdx()];
            
            // Each binding site is also a feature.
            rFamily.freeSiteFeatures[rSpec] = &rBindingSite;
        }
    };
    
    // Install the binding feature for the given binding into the
    // binding feature map.
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    behaviorizeBinding( const bindingSpec& rSpec )
    {
        const binding& rbinding = paradigm.bindings[rSpec];
        
        const siteSpec& rLeftSpec = rbinding.leftSite();
        molType* pLeftMol = paradigm.mols[rLeftSpec.molNdx()];
        
        const siteSpec& rRightSpec = rbinding.rightSite();
        molType* pRightMol = paradigm.mols[rRightSpec.molNdx()];
        
        // Try to look up the binding feature in the table.
        // findBindingFeature tries the "edge" in both directions.
        bindingFeatureType* pFeature
            = rKnownBindings.findFeature( pLeftMol,
                                          rLeftSpec.siteNdx(),
                                          pRightMol,
                                          rRightSpec.siteNdx() );
        if ( ! pFeature )
        {
            // This means that the user specified a complex containing
            // a binding without there being a corresponding dimerization-gen.
            const typename molType::bindingSiteType& rLeftSite
                = ( *pLeftMol )[rLeftSpec.siteNdx()];
            const typename molType::bindingSiteType& rRightSite
                = ( *pRightMol )[rRightSpec.siteNdx()];
            
            throw noKineticConstsXcpt::molsAndSites( pLeftMol->getName(),
                                                     rLeftSite.getName(),
                                                     pRightMol->getName(),
                                                     rRightSite.getName() );
        }
        else
        {
            // Install the binding feature.
            bindingFeatures[rSpec] = pFeature;
        }
    }
    
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    behaviorizeMol( const molSpec& rSpec )
    {
        molType* pMol = paradigm.mols[rSpec];
        
        // Install the sensitive feature.
        molFeatures[rSpec] = pMol;
    }
    
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class plexFamily<molT,
                     plexT,
                     plexSpeciesT,
                     plexFamilyT,
                     omniPlexT>::connectFamilyToSatisfiedOmni :
        public std::unary_function<omniPlexType*,
                                   void>
    {
        plexFamily& rFamily;
        const plexIso& rInjection;
    public:
        connectFamilyToSatisfiedOmni( plexFamily& rPlexFamily,
                                      const plexIso& rIsoPair ) :
            rFamily( rPlexFamily ),
            rInjection( rIsoPair )
        {}
        
        void
        operator()( omniPlexType* pOmniPlex ) const
        {
            const typename omniPlexType::structureQueryType& rQuery
                = pOmniPlex->getStructureQuery();
            
            omniStructureQueryArg<plexType> queryArg( rFamily.getParadigm(),
                                                      rInjection );
            
            if ( rQuery( queryArg ) )
            {
                // Add the omniPlex's feature to the featureMap of rFamily.
                rFamily.omniFeatures.addFeature
                    ( subPlexSpec<omniPlexType> ( pOmniPlex,
                                                  rInjection ),
                      pOmniPlex->getSubPlexFeature() );
            }
        }
    };
    
    // This function both searches for subPlexes in the plexFamily's paradigm
    // and installs the ones that it finds.
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class plexFamily<molT,
                     plexT,
                     plexSpeciesT,
                     plexFamilyT,
                     omniPlexT>::doBehaviorizeOmni
        : public std::unary_function<plexFamily*, void>
    {
        plexFamily& rFamily;
    public:
        doBehaviorizeOmni( plexFamily& rPlexFamily ) :
            rFamily( rPlexFamily )
        {}
        
        void
        operator()( plexFamily* pOmniFamily ) const
        {
            // Is there an injection of the omni structure into this structure?
            cpx::plexIso injection;
            if ( cpx::reportIsoSearch<plexType> ( pOmniFamily->getParadigm(),
                                                  rFamily.getParadigm(),
                                                  injection ).findInjection() )
            {
                // Attach this plex family to the features of those omniPlexes
                // (associated to the omni family) whose structural queries
                // are satisfied by the structure of rFamily.
                std::for_each( pOmniFamily->omniPlexes.begin(),
                               pOmniFamily->omniPlexes.end(),
                               connectFamilyToSatisfiedOmni( rFamily,
                                                             injection ) );
            }
        }
    };
    
    // Connects this plexFamily to all its features, and thereby
    // to all the reactionFamilies that are sensitive to it by virtue
    // of connection to a structural feature.  For example, free
    // sites are features to which families of dimerization reactions
    // are sensitive.
    //
    // This should be done only after all the omniPlex families have
    // been put through passes 1 and 2.
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    connectToFeatures( void )
    {
        // Locate the free structural sites.
        std::vector<siteSpec> freeSiteVector;
        paradigm.makeFreeSiteVector( freeSiteVector );
        
        // Install each free structural site's feature in the free-site
        // feature map.
        for_each( freeSiteVector.begin(),
                  freeSiteVector.end(),
                  doBehaviorizeSite( *this ) );
        
        // Install each binding's feature in the binding feature map.
        int bindingNdx = paradigm.bindings.size();
        while ( 0 < bindingNdx-- ) behaviorizeBinding(( bindingSpec ) bindingNdx );
        
        // Install each mol's feature in the mol feature map.
        int molNdx = paradigm.mols.size();
        while ( 0 < molNdx-- ) behaviorizeMol(( molSpec ) molNdx );
        
        // Install the features for omniplexes.
        for_each( rOmniPlexFamilies.begin(),
                  rOmniPlexFamilies.end(),
                  doBehaviorizeOmni( *this ) );
    }
    
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    std::vector<molParam>
    plexFamily<molT,
               plexT,
               plexSpeciesT,
               plexFamilyT,
               omniPlexT>::
    makeDefaultMolParams( void ) const
    {
        const std::vector<molType*>& rMolVector = getParadigm().mols;
        
        std::vector<molParam> defaultParams( rMolVector.size(), 0 );
        
        std::transform( rMolVector.begin(),
                        rMolVector.end(),
                        defaultParams.begin(),
                        std::mem_fun( &molType::getDefaultParam ) );
        
        return defaultParams;
    }
}

#endif // CPX_PLEXFAMILYIMPL_H
