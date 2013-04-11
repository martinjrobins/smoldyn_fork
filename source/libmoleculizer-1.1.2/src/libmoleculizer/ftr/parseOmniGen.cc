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

#include "fnd/fndXcpt.hh"
#include "mol/mzrModMol.hh"
#include "mol/badModMolXcpt.hh"
#include "plex/parserPlex.hh"
#include "plex/parseOmniPlex.hh"
#include "ftr/ftrEltName.hh"
#include "ftr/omniGen.hh"
#include "ftr/omniFam.hh"
#include "ftr/parseOmniGen.hh"
#include <libxml++/libxml++.h>
#include "mzr/mzrSpeciesDumpable.hh"

namespace ftr
{
    class parseSmallMolExchange :
        public std::unary_function<xmlpp::Node*, smallMolExchange>
    {
        bnd::molUnit& rMolUnit;
        const plx::parserPlex& rParsedPlex;
    public:
        parseSmallMolExchange( bnd::molUnit& refMolUnit,
                               const plx::parserPlex& refParsedPlex ) :
            rMolUnit( refMolUnit ),
            rParsedPlex( refParsedPlex )
        {}
        
        smallMolExchange
        operator()( xmlpp::Node* pSmallMolExchangeNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pSmallMolExchangeElt
                = utl::dom::mustBeElementPtr( pSmallMolExchangeNode );
            
            xmlpp::Element* pSmallMolInstanceRefElt
                = utl::dom::mustGetUniqueChild( pSmallMolExchangeElt,
                                                eltName::smallMolInstanceRef );
            
            std::string smallMolInstanceName
                = utl::dom::mustGetAttrString( pSmallMolInstanceRefElt,
                                               eltName::smallMolInstanceRef_nameAttr );
            cpx::molSpec exchangedMolSpec
                = rParsedPlex.mustGetMolNdxByName( pSmallMolInstanceRefElt,
                                                   smallMolInstanceName );
            
            xmlpp::Element* pSmallMolRefElt
                = utl::dom::mustGetUniqueChild( pSmallMolExchangeElt,
                                                eltName::smallMolRef );
            
            std::string smallMolName
                = utl::dom::mustGetAttrString( pSmallMolRefElt,
                                               eltName::smallMolRef_nameAttr );
            
            bnd::smallMol* pReplacementMol
                = rMolUnit.mustFindSmallMol( smallMolName,
                                             pSmallMolInstanceRefElt );
            
            return smallMolExchange( exchangedMolSpec,
                                     pReplacementMol );
        }
    };
    
    class parseModificationExchange :
        public std::unary_function<xmlpp::Node*, modificationExchange>
    {
        bnd::molUnit& rMolUnit;
        const plx::parserPlex& rParsedPlex;
    public:
        parseModificationExchange( bnd::molUnit& refMolUnit,
                                   const plx::parserPlex& refParsedPlex ) :
            rMolUnit( refMolUnit ),
            rParsedPlex( refParsedPlex )
        {
        }
        
        modificationExchange
        operator()( xmlpp::Node* pModificationExchangeNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pModificationExchangeElt
                = utl::dom::mustBeElementPtr( pModificationExchangeNode );
            
            xmlpp::Element* pModMolInstanceRefElt
                = utl::dom::mustGetUniqueChild( pModificationExchangeElt,
                                                eltName::modMolInstanceRef );
            
            // Get the instance name of the mol instance in which the modification
            // exchange is to take place, and get the index of the mol in the
            // complex.
            std::string modMolInstanceName
                = utl::dom::mustGetAttrString( pModMolInstanceRefElt,
                                               eltName::modMolInstanceRef_nameAttr );
            
            cpx::molSpec modMolSpec
                = rParsedPlex.mustGetMolNdxByName( pModMolInstanceRefElt,
                                                   modMolInstanceName );
            
            // Get the modification site name.
            xmlpp::Element* pModSiteRefElt
                = utl::dom::mustGetUniqueChild( pModMolInstanceRefElt,
                                                eltName::modSiteRef );
            
            std::string modSiteName
                = utl::dom::mustGetAttrString( pModSiteRefElt,
                                               eltName::modSiteRef_nameAttr );
            
            // Have to use the mol to translate modification site name to
            // modification index.
            const bnd::mzrModMol* pModMol
                = bnd::mustBeModMol
                ( rParsedPlex.mustGetMolByName( pModMolInstanceRefElt,
                                                modMolInstanceName ),
                  pModMolInstanceRefElt );
            
            int modSiteNdx
                = pModMol->mustGetModSiteNdx( modSiteName,
                                              pModSiteRefElt );
            
            // Get the name of the modification that is to replace the modification
            // currently found at the modification site.
            xmlpp::Element* pInstalledModRefElt
                = utl::dom::mustGetUniqueChild( pModificationExchangeElt,
                                                eltName::installedModRef );
            std::string modificationName
                = utl::dom::mustGetAttrString( pInstalledModRefElt,
                                               eltName::installedModRef_nameAttr );
            
            // Look up the modification using the molUnit.
            const cpx::modification* pModification
                = rMolUnit.mustGetMod( modificationName,
                                       pInstalledModRefElt );
            
            return modificationExchange( modMolSpec,
                                         modSiteNdx,
                                         pModification );
        }
    };
    
    void
    parseOmniGen::
    operator()( xmlpp::Node* pOmniGenNode ) const
        throw( utl::xcpt )
    {
        xmlpp::Element* pOmniGenElt
            = utl::dom::mustBeElementPtr( pOmniGenNode );
        
        // Parse the enabling omniplex.
        xmlpp::Element* pEnablingOmniElt
            = utl::dom::mustGetUniqueChild( pOmniGenElt,
                                            eltName::enablingOmniplex );
        plx::parserPlex parsedPlex;
        plx::mzrOmniPlex* pOmni
            = plx::findOmni( pEnablingOmniElt,
                             rMolUnit,
                             rPlexUnit,
                             parsedPlex );
        
        // Parse the small-mol exchanges.
        xmlpp::Element* pSmallMolExchangesElt
            = utl::dom::mustGetUniqueChild( pOmniGenElt,
                                            eltName::smallMolExchanges );
        xmlpp::Node::NodeList smallMolExchangeNodes
            = pSmallMolExchangesElt->get_children( eltName::smallMolExchange );
        std::vector<smallMolExchange> smallMolExchanges
            = std::vector<smallMolExchange> ( smallMolExchangeNodes.size() );
        std::transform( smallMolExchangeNodes.begin(),
                        smallMolExchangeNodes.end(),
                        smallMolExchanges.begin(),
                        parseSmallMolExchange( rMolUnit,
                                               parsedPlex ) );
        
        // Parse the modification exchanges.
        xmlpp::Element* pModificationExchangesElt
            = utl::dom::mustGetUniqueChild( pOmniGenElt,
                                            eltName::modificationExchanges );
        xmlpp::Node::NodeList modificationExchangeNodes
            = pModificationExchangesElt->get_children( eltName::modificationExchange );
        std::vector<modificationExchange> modificationExchanges
            = std::vector<modificationExchange> ( modificationExchangeNodes.size() );
        std::transform( modificationExchangeNodes.begin(),
                        modificationExchangeNodes.end(),
                        modificationExchanges.begin(),
                        parseModificationExchange( rMolUnit,
                                                   parsedPlex ) );
        
        // Parse additional reactant.
        xmlpp::Element* pAdditionalReactantSpeciesElt
            = utl::dom::getOptionalChild( pOmniGenElt,
                                          eltName::additionalReactantSpecies );
        mzr::mzrSpecies* pAdditionalReactantSpecies = 0;
        if ( pAdditionalReactantSpeciesElt )
        {
            std::string additionalReactantSpeciesName
                = utl::dom::mustGetAttrString
                ( pAdditionalReactantSpeciesElt,
                  eltName::additionalReactantSpecies_nameAttr );
            
            pAdditionalReactantSpecies
                = rMzrUnit.mustFindSpecies( additionalReactantSpeciesName,
                                            pAdditionalReactantSpeciesElt );
        }
        
        // Parse additional product.
        xmlpp::Element* pAdditionalProductSpeciesElt
            = utl::dom::getOptionalChild( pOmniGenElt,
                                          eltName::additionalProductSpecies );
        mzr::mzrSpecies* pAdditionalProductSpecies = 0;
        if ( pAdditionalProductSpeciesElt )
        {
            std::string additionalProductSpeciesName
                = utl::dom::mustGetAttrString
                ( pAdditionalProductSpeciesElt,
                  eltName::additionalProductSpecies_nameAttr );
            
            pAdditionalProductSpecies
                = rMzrUnit.mustFindSpecies( additionalProductSpeciesName,
                                            pAdditionalProductSpeciesElt );
        }
        
        // Parse the reaction rate, whose units depend on whether an additional
        // reactant species is given or not.
        xmlpp::Element* pRateElt
            = utl::dom::mustGetUniqueChild( pOmniGenElt,
                                            eltName::rate );
        double rate
            = utl::dom::mustGetAttrPosDouble( pRateElt,
                                              eltName::rate_valueAttr );
        
        // Construct the reaction rate exptrapolator.  The generated reactions
        // could be either unary or binary, depending on whether an additional
        // reactant is given, and there is a constructor for each of these cases.
        omniMassExtrap* pExtrapolator = 0;
        pExtrapolator = new omniMassExtrap( rate );
        
        // TODO IMPORTANT NOTICE -- temporarily eliminating rate extrapolation.
        //     if(pAdditionalReactantSpeciesElt)
        //       {
        // 	// Construct the default species of the triggering omniplex.
        // 	//
        // 	// The need to construct a default member of a plexFamily arises
        // 	// again.  Reinsitute it as a member variable of plexFamily?
        // 	plx::mzrPlexFamily* pTriggeringFamily = pOmni->getFamily();
        // 	std::vector<cpx::molParam> defaultParams
        // 	  = pTriggeringFamily->makeDefaultMolParams();
        // 	plx::mzrPlexSpecies* pDefaultTriggeringSpecies
        // 	  = pTriggeringFamily->makeMember(defaultParams);
        
        // 	// Make sure that the molecular weight of the additional reactant
        // 	// species can be determined.
        // 	fnd::massive* pAdditionalMassiveSpecies
        // 	  = fnd::mustBeMassiveSpecies(pAdditionalReactantSpecies,
        // 				      pAdditionalReactantSpeciesElt);
        
        // 	pExtrapolator = new omniMassExtrap(rate,
        // 					   pDefaultTriggeringSpecies,
        // 					   pAdditionalMassiveSpecies);
        //       }
        //     else
        //       {
        // 	pExtrapolator = new omniMassExtrap(rate);
        //       }
        
        // Construct the reaction family and register it for memory management.
        omniFam* pFamily
            = new omniFam( rMzrUnit,
                           rPlexUnit,
                           smallMolExchanges,
                           modificationExchanges,
                           pAdditionalReactantSpecies,
                           pAdditionalProductSpecies,
                           pExtrapolator );
        rMzrUnit.addReactionFamily( pFamily );
        
        // Connect the family's reaction generator to the triggering omniplex's
        // feature.
        pOmni->getSubPlexFeature()->insert( pFamily->getRxnGen() );
    }
}
