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

#include "utl/defs.hh"
#include "mzr/mzrException.hh"
#include "mzr/mzrEltName.hh"
#include "mzr/moleculizer.hh"
#include "mzr/mzrReaction.hh"
#include "mzr/mzrUnit.hh"
#include <libxml++/libxml++.h>


namespace mzr
{
    class addProductSpecies :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzrUnit& rMzrUnit;
        mzrReaction* pReaction;
    public:
        addProductSpecies( mzrUnit& refMzrUnit,
                           mzrReaction* pParsedReaction ) :
            rMzrUnit( refMzrUnit ),
            pReaction( pParsedReaction )
        {}
        
        void
        operator()( xmlpp::Node* pProductSpeciesRefNode ) const throw( std::exception )
        {
            xmlpp::Element* pProductSpeciesRefElt
                = utl::dom::mustBeElementPtr( pProductSpeciesRefNode );
            
            // Get the name of the substrate species.
            std::string speciesName
                = utl::dom::mustGetAttrString( pProductSpeciesRefElt,
                                               eltName::productSpeciesRef_nameAttr );
            
            // Look up the species in moleculizer's BAD BAD BAD static catalog of
            // named species.
            mzrSpecies* pSpecies
                = rMzrUnit.mustFindSpecies( speciesName,
                                            pProductSpeciesRefElt );
            
            // Get the multiplicity of the product, which can be positive or
            // negative.
            int multiplicity
                = utl::dom::mustGetAttrInt( pProductSpeciesRefElt,
                                            eltName::productSpeciesRef_multAttr );
            
            // Add the product species/multiplicity to the reaction.
            pReaction->addProduct( pSpecies,
                                   multiplicity );
        }
    };
    
    class addSubstrateSpecies :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzrUnit& rMzrUnit;
        mzrReaction* pReaction;
    public:
        addSubstrateSpecies( mzrUnit& refMzrUnit,
                             mzrReaction* pParsedReaction ) :
            rMzrUnit( refMzrUnit ),
            pReaction( pParsedReaction )
        {}
        
        void
        operator()( xmlpp::Node* pSubstrateSpeciesRefNode ) const throw( std::exception )
        {
            // Cast the Node* to Element*, probably unnecessarily dynamically.
            xmlpp::Element* pSubstrateSpeciesRefElt
                = utl::dom::mustBeElementPtr( pSubstrateSpeciesRefNode );
            
            // Get the name of the substrate species.
            std::string speciesName
                = utl::dom::mustGetAttrString( pSubstrateSpeciesRefElt,
                                               eltName::substrateSpeciesRef_nameAttr );
            
            // Look up the species in moleculizer's BAD BAD BAD static catalog of
            // named species.
            mzrSpecies* pSpecies
                = rMzrUnit.mustFindSpecies( speciesName,
                                            pSubstrateSpeciesRefElt );
            
            // Get the multiplicity of the substrate, which must be positive.
            int multiplicity
                = utl::dom::mustGetAttrPosInt( pSubstrateSpeciesRefElt,
                                               eltName::substrateSpeciesRef_multAttr );
            
            // Add the substrate/multiplicity to the reaction.
            pReaction->addReactant( pSpecies,
                                    multiplicity );
        }
    };
    
    class installReaction :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzrUnit& rMzrUnit;
    public:
        installReaction( mzrUnit& refMzrUnit ) :
            rMzrUnit( refMzrUnit )
        {}
        
        void
        operator()( const xmlpp::Node* pReactionNode ) const throw( std::exception )
        {
            mzrReaction* pParsedReaction
                = new mzrReaction( rMzrUnit.globalVars.begin(),
                                   rMzrUnit.globalVars.end() );
            
            // Get the list of substrate nodes.
            xmlpp::Node::NodeList substrateSpeciesRefNodes
                = pReactionNode->get_children( eltName::substrateSpeciesRef );
            
            // Add the substrates to the reaction.
            std::for_each( substrateSpeciesRefNodes.begin(),
                           substrateSpeciesRefNodes.end(),
                           addSubstrateSpecies( rMzrUnit,
                                                pParsedReaction ) );
            
            // Get the list of product nodes.
            xmlpp::Node::NodeList productSpeciesRefNodes
                = pReactionNode->get_children( eltName::productSpeciesRef );
            
            // Add the products to the reaction.
            std::for_each( productSpeciesRefNodes.begin(),
                           productSpeciesRefNodes.end(),
                           addProductSpecies( rMzrUnit,
                                              pParsedReaction ) );
            
            // Get the reaction rate element.
            xmlpp::Element* pRateElt
                = utl::dom::mustGetUniqueChild( pReactionNode,
                                                eltName::rate );
            double rate
                = utl::dom::mustGetAttrDouble( pRateElt,
                                               eltName::rate_valueAttr );
            pParsedReaction->setRate( rate );
            
            // Add the reaction to its destruction pit.
            rMzrUnit.addUserReaction( pParsedReaction );
        }
    };
    
    void
    mzrUnit::parseDomInput( xmlpp::Element* pRootElement,
                            xmlpp::Element* pModelElement,
                            xmlpp::Element* pStreamElt ) throw( std::exception )
    {
        //////////////////////////////////////////////////////////////////
        // Get the explicit-reactions element.
        //
        // Eventually, there could be more than one kind of reaction,
        // so that this header element for all the different kinds isn't
        // useless.
        xmlpp::Element* pExplicitReactionsElt
            = utl::dom::mustGetUniqueChild( pModelElement,
                                            eltName::explicitReactions );
        
        // Get the list of reaction elements.
        xmlpp::Node::NodeList reactionNodes
            = pExplicitReactionsElt->get_children( eltName::reaction );
        
        // Install each reaction in moleculizer's autoVector.
        std::for_each( reactionNodes.begin(),
                       reactionNodes.end(),
                       installReaction( *this ) );
    }
}
