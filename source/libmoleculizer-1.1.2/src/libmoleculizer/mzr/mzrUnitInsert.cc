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

#include <libxml++/libxml++.h>
#include "mzr/mzrUnit.hh"
#include "mzr/dumpUtils.hh"
#include "mzr/moleculizer.hh"

namespace mzr
{
    
    class insertFamilyReactions :
        public std::unary_function<const std::vector<mzrReaction>*, void>
    {
        // This may change to a different element containing just the
        // automatically generated reactions.
        xmlpp::Element* pTagReactionsElt;
        
    public:
        insertFamilyReactions( xmlpp::Element* pTagReactionsElement ) :
            pTagReactionsElt( pTagReactionsElement )
        {}
        
        void
        operator()( const std::vector<mzrReaction*>* pFamily ) const
            throw( std::exception )
        {
            std::for_each( pFamily->begin(),
                           pFamily->end(),
                           std::bind2nd( std::mem_fun( &mzrReaction::insertElt ),
                                         pTagReactionsElt ) );
        }
    };
    
    class insertExplicitSpeciesTag :
        public std::unary_function<utl::catalog<mzrSpecies>::value_type, void>
    {
        xmlpp::Element* pExplicitSpeciesTagsElt;
    public:
        insertExplicitSpeciesTag( xmlpp::Element* pExplicitSpeciesTagsElement ) :
            pExplicitSpeciesTagsElt( pExplicitSpeciesTagsElement )
        {}
        
        void
        operator()( const argument_type& rNameSpeciesPair ) const
            throw( std::exception )
        {
            xmlpp::Element* pExplicitSpeciesTagElt
                = pExplicitSpeciesTagsElt->add_child( eltName::explicitSpeciesTag );
            
            pExplicitSpeciesTagElt
                ->set_attribute( eltName::explicitSpeciesTag_nameAttr,
                                 rNameSpeciesPair.first );
            
            pExplicitSpeciesTagElt
                ->set_attribute( eltName::explicitSpeciesTag_tagAttr,
                                 rNameSpeciesPair.second->getTag() );
        }
    };
    
    void
    mzrUnit::insertStateElts( xmlpp::Element* pUnitStatesElt ) throw( std::exception )
    {

        pUnitStatesElt->add_child("mzr-unit-state");

//         // Model elements.
//         xmlpp::Element* pModelElt
//             = utl::dom::mustGetUniqueChild( pRootElt,
//                                             eltName::model );
        
//         xmlpp::Element* pExplicitSpeciesTagsElt
//             = utl::dom::mustGetUniqueChild( pModelElt,
//                                             eltName::explicitSpeciesTags );
        
//         // Give tags for named species.
//         std::for_each( speciesByName.begin(),
//                        speciesByName.end(),
//                        insertExplicitSpeciesTag( pExplicitSpeciesTagsElt ) );
        
//         // Give all the reactions, using tags to refer to species.
//         xmlpp::Element* pTagReactionsElt
//             = utl::dom::mustGetUniqueChild( pModelElt,
//                                             eltName::tagReactions );
        
//         // First the reactions that weren't automatically generated.
//         std::for_each( userReactions.begin(),
//                        userReactions.end(),
//                        std::bind2nd( std::mem_fun( &mzrReaction::insertElt ),
//                                      pTagReactionsElt ) );
        
//         // Now the reactions that were automatically generated.
//         std::for_each( reactionFamilies.begin(),
//                        reactionFamilies.end(),
//                        insertFamilyReactions( pTagReactionsElt ) );
    }
}
