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
#include "cpx/modMolStateQuery.hh"
#include "mol/mzrModMol.hh"
#include "ftr/ftrEltName.hh"
#include "ftr/uniMolGen.hh"
#include "ftr/uniMolFam.hh"
#include "ftr/parseUniMolGen.hh"
#include <libxml++/libxml++.h>
#include "mzr/mzrSpeciesDumpable.hh"

namespace ftr
{
    bnd::mzrModMol*
    parseEnablingMol( xmlpp::Element* pEnablingMolElt,
                      bnd::molUnit& rMolUnit )
        throw( utl::xcpt )
    {
        std::string enablingMolName
            = utl::dom::mustGetAttrString( pEnablingMolElt,
                                           eltName::enablingMol_nameAttr );
        
        return rMolUnit.mustFindModMol( enablingMolName,
                                        pEnablingMolElt );
    }
    
    class parseEnablingModSiteRef :
        public std::unary_function<xmlpp::Node*, void>
    {
        bnd::mzrModMol* pEnablingMol;
        cpx::andMolStateQueries* pQuery;
        bnd::molUnit& rMolUnit;
        mzr::mzrUnit& rMzrUnit;
        
    public:
        parseEnablingModSiteRef( bnd::mzrModMol* pEnablingModMol,
                                 cpx::andMolStateQueries* pEnableQuery,
                                 bnd::molUnit& refMolUnit,
                                 mzr::mzrUnit& refMzrUnit ) :
            pEnablingMol( pEnablingModMol ),
            pQuery( pEnableQuery ),
            rMolUnit( refMolUnit ),
            rMzrUnit( refMzrUnit )
        {}
        
        void
        operator()( xmlpp::Node* pModSiteRefNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pModSiteRefElt
                = utl::dom::mustBeElementPtr( pModSiteRefNode );
            
            // Parse the modification site name and convert it into an index.
            std::string modSiteName
                = utl::dom::mustGetAttrString( pModSiteRefElt,
                                               eltName::modSiteRef_nameAttr );
            int modSiteNdx
                = pEnablingMol->mustGetModSiteNdx( modSiteName,
                                                   pModSiteRefElt );
            
            // Parse the modifcation name, and get the modification.
            xmlpp::Element* pModRefElt
                = utl::dom::mustGetUniqueChild( pModSiteRefElt,
                                                eltName::modRef );
            std::string modName
                = utl::dom::mustGetAttrString( pModRefElt,
                                               eltName::modRef_nameAttr );
            const cpx::modification* pModToSee
                = rMolUnit.mustGetMod( modName,
                                       pModRefElt );
            
            // Construct the query of modification state, and register it for
            // memory management.
            cpx::modMolStateQuery* pModMolStateQuery
                = new cpx::modMolStateQuery( modSiteNdx,
                                             pModToSee );
            rMzrUnit.addQuery( pModMolStateQuery );
            
            // Add the query to the compound query about the state of the "focus"
            // mol.
            pQuery->addQuery( pModMolStateQuery );
        }
    };
    
    cpx::andMolStateQueries*
    parseEnablingModifications( xmlpp::Element* pEnablingModsElt,
                                bnd::mzrModMol* pEnablingMol,
                                bnd::molUnit& rMolUnit,
                                mzr::mzrUnit& rMzrUnit )
        throw( utl::xcpt )
    {
        cpx::andMolStateQueries* pQuery
            = new cpx::andMolStateQueries();
        rMzrUnit.addQuery( pQuery );
        
        xmlpp::Node::NodeList modSiteRefNodes
            = pEnablingModsElt->get_children( eltName::modSiteRef );
        
        std::for_each( modSiteRefNodes.begin(),
                       modSiteRefNodes.end(),
                       parseEnablingModSiteRef( pEnablingMol,
                                                pQuery,
                                                rMolUnit,
                                                rMzrUnit ) );
        return pQuery;
    }
    
    class parseModExch :
        public std::unary_function<xmlpp::Node*, molModExchange>
    {
        const bnd::molUnit& rMolUnit;
        const bnd::mzrModMol* pEnabling;
    public:
        parseModExch( const bnd::molUnit& refMolUnit,
                      const bnd::mzrModMol* pEnablingModMol ) :
            rMolUnit( refMolUnit ),
            pEnabling( pEnablingModMol )
        {}
        
        molModExchange
        operator()( xmlpp::Node* pModificationExchangeNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pModificationExchangeElt
                = utl::dom::mustBeElementPtr( pModificationExchangeNode );
            
            // Parse the name of the modification site to be changed, and
            // convert it to site index.
            xmlpp::Element* pModSiteRefElt
                = utl::dom::mustGetUniqueChild( pModificationExchangeElt,
                                                eltName::modSiteRef );
            std::string modSiteName
                = utl::dom::mustGetAttrString( pModSiteRefElt,
                                               eltName::modSiteRef_nameAttr );
            int modSiteNdx
                = pEnabling->mustGetModSiteNdx( modSiteName,
                                                pModSiteRefElt );
            
            // Parse the name of the modification to be substituted in, and
            // look up the corresponding modification.
            xmlpp::Element* pInstalledModRefElt
                = utl::dom::mustGetUniqueChild( pModificationExchangeElt,
                                                eltName::installedModRef );
            std::string modName
                = utl::dom::mustGetAttrString( pInstalledModRefElt,
                                               eltName::installedModRef_nameAttr );
            const cpx::modification* pMod
                = rMolUnit.mustGetMod( modName,
                                       pInstalledModRefElt );
            
            return molModExchange( modSiteNdx,
                                   pMod );
        }
    };
    
    std::vector<molModExchange>
    parseModificationExchanges( xmlpp::Element* pModificationExchangesElt,
                                const bnd::mzrModMol* pEnablingModMol,
                                const bnd::molUnit& rMolUnit )
        throw( utl::xcpt )
    {
        
        xmlpp::Node::NodeList modificationExchangeNodes
            = pModificationExchangesElt->get_children( eltName::modificationExchange );
        
        std::vector<molModExchange> modificationExchanges
            = std::vector<molModExchange> ( modificationExchangeNodes.size() );
        
        // I get a peculiar name resolution problem unless I explicitly
        // call the ftr namespace here; I get incorrect linkage with
        // the parseModificationExchange class from parseOmniGen.cc !!!!
        std::transform( modificationExchangeNodes.begin(),
                        modificationExchangeNodes.end(),
                        modificationExchanges.begin(),
                        parseModExch( rMolUnit,
                                      pEnablingModMol ) );
        
        return modificationExchanges;
    }
    
    mzr::mzrSpecies*
    parseAdditionalReactantSpecies( xmlpp::Element* pAdditionalReactantSpeciesElt,
                                    const mzr::mzrUnit& rMzrUnit )
        throw( utl::xcpt )
    {
        std::string speciesName
            = utl::dom::mustGetAttrString
            ( pAdditionalReactantSpeciesElt,
              eltName::additionalReactantSpecies_nameAttr );
        
        return rMzrUnit.mustFindSpecies( speciesName,
                                         pAdditionalReactantSpeciesElt );
    }
    
    mzr::mzrSpecies*
    parseAdditionalProductSpecies( xmlpp::Element* pAdditionalProductSpeciesElt,
                                   const mzr::mzrUnit& rMzrUnit )
        throw( utl::xcpt )
    {
        std::string speciesName
            = utl::dom::mustGetAttrString
            ( pAdditionalProductSpeciesElt,
              eltName::additionalProductSpecies_nameAttr );
        
        return rMzrUnit.mustFindSpecies( speciesName,
                                         pAdditionalProductSpeciesElt );
    }
    
    void
    parseUniMolGen::
    operator()( xmlpp::Node* pUniMolGenNode ) const
        throw( utl::xcpt )
    {
        xmlpp::Element* pUniMolGenElt
            = utl::dom::mustBeElementPtr( pUniMolGenNode );
        
        // Parse the enabling mol.
        xmlpp::Element* pEnablingMolElt
            = utl::dom::mustGetUniqueChild( pUniMolGenElt,
                                            eltName::enablingMol );
        bnd::mzrModMol* pEnablingModMol
            = parseEnablingMol( pEnablingMolElt,
                                rMolUnit );
        
        // Parse the enabling modifications.
        xmlpp::Element* pEnablingModsElt
            = utl::dom::mustGetUniqueChild( pUniMolGenElt,
                                            eltName::enablingModifications );
        cpx::andMolStateQueries* pQuery
            = parseEnablingModifications( pEnablingModsElt,
                                          pEnablingModMol,
                                          rMolUnit,
                                          rMzrUnit );
        
        // Parse the modification exchanges.
        xmlpp::Element* pModificationExchangesElt
            = utl::dom::mustGetUniqueChild( pUniMolGenElt,
                                            eltName::modificationExchanges );
        
        std::vector<molModExchange> modificationExchanges
            = parseModificationExchanges( pModificationExchangesElt,
                                          pEnablingModMol,
                                          rMolUnit );
        
        // Parse the reation rate.
        //
        // (This is out of order, so that we can use the rate in the construction
        // of the rate extrapolator.  Which rate extrapolator is used depends
        // on whether there is an additional reactant.)
        xmlpp::Element* pRateElt
            = utl::dom::mustGetUniqueChild( pUniMolGenElt,
                                            eltName::rate );
        double rate
            = utl::dom::mustGetAttrDouble( pRateElt,
                                           eltName::rate_valueAttr );
        
        // Parse the additional reactant species, if any.
        //
        // If there is one, use the binary reaction rate extrapolator,
        // which requires that the additional reactant be massive.
        // If there isn't one, use the unary reaction rate extrapolator,
        // which doesn't really do anything.
        xmlpp::Element* pAdditionalReactantSpeciesElt
            = utl::dom::getOptionalChild( pUniMolGenElt,
                                          eltName::additionalReactantSpecies );
        
        mzr::mzrSpecies* pAdditionalReactantSpecies = 0;
        uniMolExtrapolator* pExtrapolator = 0;
        
        if(rMzrUnit.rMolzer.getRateExtrapolation() )
        {
            
            pAdditionalReactantSpecies
                = parseAdditionalReactantSpecies(pAdditionalReactantSpeciesElt,
                                                 rMzrUnit);
            
            fnd::massive* pMassive
                = fnd::mustBeMassiveSpecies(pAdditionalReactantSpecies,
                                            pAdditionalReactantSpeciesElt);
            
            pExtrapolator = new uniMolMassExtrap(rate,
                                                 pEnablingModMol,
                                                 pMassive);
            
        }
        else
        {
            pExtrapolator = new uniMolMassExtrap(rate);
        }
        
        pExtrapolator = new uniMolMassExtrap( rate );
        
        
        // Parse the additional product species, if any.
        xmlpp::Element* pAdditionalProductSpeciesElt
            = utl::dom::getOptionalChild( pUniMolGenElt,
                                          eltName::additionalProductSpecies );
        
        mzr::mzrSpecies* pAdditionalProductSpecies = 0;
        if ( pAdditionalProductSpeciesElt )
        {
            pAdditionalProductSpecies
                = parseAdditionalProductSpecies( pAdditionalProductSpeciesElt,
                                                 rMzrUnit );
        }
        
        // Construct the reaction family and register it for memory management.
        uniMolFam* pFamily
            = new uniMolFam( rMzrUnit,
                             rPlexUnit,
                             pEnablingModMol,
                             pQuery,
                             modificationExchanges,
                             pAdditionalReactantSpecies,
                             pAdditionalProductSpecies,
                             pExtrapolator );
        rMzrUnit.addReactionFamily( pFamily );
        
        // Connect the family's reaction generator to the mol's feature.
        // Note that mol inherits from feature.
        pEnablingModMol->addSensitive( pFamily->getRxnGen() );
    }
}
