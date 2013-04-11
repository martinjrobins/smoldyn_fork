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

#ifndef CPX_PLEXFAMILY_H
#define CPX_PLEXFAMILY_H

/*! \defgroup plexStructGroup Structure
  \ingroup plexGroup
  \brief Structural equivalence, structural families of complexes. */

/*! \file plexFamily.hh
  \ingroup plexStructGroup
  \brief Defines plexFamily, a structural family of species of complexes. */

#include <vector>
#include <map>
#include <functional>
#include "utl/autoCache.hh"
#include "utl/autoVector.hh"
#include "fnd/featureMap.hh"
#include "fnd/sensitive.hh"
#include "fnd/newSpeciesStimulus.hh"
#include "cpx/cxOmni.hh"
#include "cpx/cxMol.hh"
#include "cpx/cxBinding.hh"
#include "cpx/cxSite.hh"
#include "cpx/queryAlloList.hh"
#include "cpx/omniPlex.hh"
#include "cpx/omniStructureQuery.hh"
#include "cpx/knownBindings.hh"

namespace cpx
{
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class omniPlex;
    
    /*! \ingroup plexStructGroup
      
      \brief A structural family of species of complexes.
      
      The parameter that is used to classify complexes is the vector
      of molParams of the complex.  All other parameters of the
      complex are computed from these, together with the allostery
      properties of the structural family. */
    template<class molT,
             class plexT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class plexFamily :
        public utl::autoCache<std::vector<molParam>, plexSpeciesT>,
        public fnd::sensitive<fnd::newSpeciesStimulus<plexSpeciesT> >
    {
        //   There are two phases of manipulations to the plexFamilies that must
        //   be done before runtime:
        
        //   Pass 1:
        //     - construction
        //     - setting allosteric sites that are explicitly specified by
        //       "allo-site" commands.
        
        //   Pass 2:
        //     - connection to features("behaviorize"), including omniplex
        //     features.
        //     - setting allosteric sites from omniplexes, using the omniplex
        //     feature map to locate the relevant omniplexes.
        
        //   Passes 1 and 2 should be complete for omniplex families before
        //   pass 2 is done for other plex families.
        
    public:
        typedef molT molType;
        typedef plexT plexType;
        typedef plexSpeciesT plexSpeciesType;
        typedef plexFamilyT plexFamilyType;
        typedef omniPlexT omniPlexType;
        
        //        typedef typename plexSpeciesT::queryDumpableType dumpableType;
        
        typedef fnd::feature<cxBinding<plexSpeciesType, plexFamilyType> > bindingFeatureType;
        typedef queryAllosteryList<plexSpeciesType, omniPlexType> queryAlloListType;
        
        typedef andPlexQueries<plexSpeciesType, omniPlexType> queryType;


        std::string getPlexFamilyName() const
        {
            // This is from the a mzrPlex, NOT a mzrPlexSpecies, so the name
            // is informative, and not canonical.
            return paradigm.getName();
        }

    protected:
        
        class doBehaviorizeSite;
        friend class doBehaviorizeSite;
        
        class doBehaviorizeOmni;
        friend class doBehaviorizeOmni;
        
        class connectFamilyToSatisfiedOmni;
        friend class connectFamilyToSatisfiedOmni;
        
        class applyOmniMods;
        friend class applyOmniMods;
        
        // The first plex in this species that was seen.  This determines
        // the "official" ordering of the mols and bindings.
        plexType paradigm;
        
        // The omniPlexes with structure given by the paradigm of this
        // plexFamily.  Putting these here makes it possible, after determining
        // that a new plexSpecies has structure with this plexFamily's structure
        // as a subcomplex, to apply the structural tests of just the omniPlexes
        // associated with this plexFamily's structure.
        //
        // These omniPlexes are memory managed by this plexFamily.
        utl::autoVector<omniPlexType> omniPlexes;
        
        // Map of site specs of free binding sites to corresponding features.
        typedef fnd::featureMap<cxSite<plexSpeciesType,
                                       plexFamilyType> > siteMapType;
        siteMapType freeSiteFeatures;
        
        // Map of binding specs to corresponding features.
        typedef fnd::featureMap<cxBinding<plexSpeciesType,
                                          plexFamilyType> > bindingMapType;
        bindingMapType bindingFeatures;
        
        // Map of mol specs to corresponding features.
        typedef fnd::featureMap<cxMol<plexSpeciesType,
                                      plexFamilyType> > molMapType;
        molMapType molFeatures;
        
        // Map of subcomplex specs to corresponding features.
        typedef fnd::featureMap<cxOmni<molType,
                                       plexSpeciesType,
                                       plexFamilyType,
                                       omniPlexType> > omniMapType;
        omniMapType omniFeatures;
        
        // Mapping of state queries to allosteric site maps.  This is used
        // to add allosteric properties connected with this structural family
        // itself to new species created in this family.
        queryAlloListType alloStateList;
        
        ///////////////////
        
        // Attaches this plexFamily to the bindingFeatures of all its bindings.
        //
        // This enables the family to notify reaction families of new member
        // species of this plexFamily. For example, there is a decomposition
        // reaction family that handles decompositions of bindings between
        // these two mols at these two sites.  This allows each such
        // reaction family to construct new reactions for the new member
        // species.
        void
        behaviorizeBinding( const bindingSpec& rSpec );
        
        // Attaches this plexFamily to the molFeatures of all its mols.
        // See behaviorizeBinding above.
        void
        behaviorizeMol( const molSpec& rSpec );
        
        // This is needed to look up binding features from pairs of binding sites.
        // This is expected to reside in the plexUnit.
        knownBindings<molType, bindingFeatureType>& rKnownBindings;
        
        // This is needed to find the distinguished subcomplexes of new complexes.
        // Structural information given by this family's paradim determines which
        // omniplex features associated to these families could be displayed by
        // complexes in this family; these are put into the omniFeatures map.
        // State informtion connected with individual new species determine which
        // species in this family display which features.
        std::set<plexFamilyType*>& rOmniPlexFamilies;
        
    public:
        
        // Constructs a plexFamily for the combinatorial structure given
        // by rParadigm, which becomes the paradigm of the new plexFamily.
        //
        // This minimal constructor doesn't calculate the default parameter,
        // for example, and is used prior to looking for subplexes, etc.
        plexFamily( const plexT& rParadigm,
                    knownBindings<molType, bindingFeatureType>& refKnownBindings,
                    std::set<plexFamilyType*>& refOmniplexFamilies );
        
        // Notify the features, dumpables of a new species.
        void
        respond( const fnd::newSpeciesStimulus<plexSpeciesType>& rStim );

        void 
        dumpablesRespond( const fnd::newSpeciesStimulus<plexSpeciesType>& rStim );
        
        void
        accumulateSpecies( std::vector<plexSpeciesType*>& rAllSpecies );
        
        const plexType&
        getParadigm( void ) const
        {
            return paradigm;
        }
        
        plexSpeciesType*
        makeMember( const std::vector<molParam>& rMolParams );
        
        virtual
        plexSpeciesType*
        constructSpecies( const cpx::siteToShapeMap& rSiteParams,
                          const std::vector<molParam>& rMolParams ) = 0;
        
        // Inserts an omniPlex, which should be in this plexFamily's structural
        // class, into this plexFamily's list of omniPlexes.  Later, a new
        // structural family that has this plexFamily's structure as a subcomplex
        // can run down this list looking for omniPlexes whose structural queries
        // are satisfied, and connect to their omniPlexFeatures.
        //
        // This plexFamily memory-manages its list of omniPlexes, too.
        //
        // This is used by the omniPlex constructor, which automatically
        // installs new omniPlexes into their plexFamilies.
        void
        addOmniPlex( omniPlexType* pOmniPlex )
        {
            omniPlexes.push_back( pOmniPlex );
        }
        
        // This routine is probably obsoleted by the new treatment of omniPlexes.
        //
        // It's only used now in plexFamily code, I think, and as an accessor
        // of a private variable, it should go away.
        const queryAlloListType&
        getAlloStateList( void ) const
        {
            return alloStateList;
        }
        
        // This is so that one can set the allostery properties of this
        // plexFamily.
        void
        addAlloQueryAndMap( const queryType* pQuery,
                            const siteToShapeMap& rSiteToShapeMap )
        {
            alloStateList.addQueryAndMap( pQuery,
                                          rSiteToShapeMap );
        }
        
        // Connects this plexFamily to all its features, and thereby
        // to all the reactionFamilies that are sensitive to it by virtue
        // of connection to a structural feature.  For example, free
        // sites are features to which families of dimerization reactions
        // are sensitive.
        //
        // This should be done only after all the omniPlexFamilies have
        // been put through passes 1 and 2.
        void
        connectToFeatures( void );


        // Makes the vector of default molParams, which is useful as a starting
        // point for constructing the molParams of a new species.
        std::vector<molParam>
        makeDefaultMolParams( void ) const;
    };
}

#include "cpx/plexFamilyImpl.hh"

#endif // CPX_PLEXFAMILY_H
