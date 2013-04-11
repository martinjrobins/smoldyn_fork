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

#include "mol/molUnit.hh"
#include "mol/molEltName.hh"
#include <libxml++/libxml++.h>

namespace bnd
{
    class insertModification :
        public std::unary_function<cpx::modification, void>
    {
        xmlpp::Element* pModsElt;
        
    public:
        insertModification( xmlpp::Element* pModificationsElt ) :
            pModsElt( pModificationsElt )
        {}
        
        void
        operator()( const cpx::modification& rModification ) const
        {
            // Insert element for this modification into modifications section.
            xmlpp::Element* pModificationElt
                = pModsElt->add_child( eltName::modification );
            
            pModificationElt->set_attribute( eltName::modification_nameAttr,
                                             rModification.getName() );
            
            // Insert element for weight delta into this modification.
            xmlpp::Element* pWeightElt
                = pModificationElt->add_child( eltName::weightDelta );
            
            pWeightElt->set_attribute( eltName::weightDelta_daltonsAttr,
                                       utl::stringify<double> ( rModification.getWeightDelta() ) );
        }
    };
    
    class insertModElt :
        public std::unary_function<utl::autoCatalog<const cpx::modification>::value_type, void>
    {
        insertModification modInserter;
    public:
        insertModElt( xmlpp::Element* pModificationsElement ) :
            modInserter( pModificationsElement )
        {}
        
        void
        operator()( const argument_type& rEntry ) const throw( std::exception )
        {
            modInserter( *( rEntry.second ) );
        }
    };
    
    class insertMolElt :
        public std::unary_function<utl::autoCatalog<mzrMol>::value_type, void>
    {
        xmlpp::Element* pMolsElt;
    public:
        insertMolElt( xmlpp::Element* pMolsElement ) :
            pMolsElt( pMolsElement )
        {}
        
        void
        operator()( const argument_type& rEntry ) const
            throw( std::exception )
        {
            rEntry.second->insertElt( pMolsElt );
        }
    };
    
    void
    molUnit::insertStateElts( xmlpp::Element* pUnitStateElt )
        throw( std::exception )
    {

        pUnitStateElt->add_child("mol-unit-state");
        
//         // Get the model element
//         xmlpp::Element* pModelElt
//             = utl::dom::mustGetUniqueChild( pRootElt,
//                                             mzr::eltName::model );
        
//         // Get the units-states element.
//         xmlpp::Element* pUnitsStatesElt
//             = utl::dom::mustGetUniqueChild( pRootElt,
//                                             mzr::eltName::unitsStates );
        
//         // Insert the modifications element.
//         xmlpp::Element* pModificationsElt
//             = pUnitsStatesElt->add_child( eltName::modifications );
        
//         // Have each modification insert itself into the modifications section.
//         std::for_each( knownMods.begin(),
//                        knownMods.end(),
//                        insertModElt( pModificationsElt ) );
        
//         // Insert the mols element.
//         xmlpp::Element* pMolsElt
//             = pUnitsStatesElt->add_child( eltName::mols );
        
//         // Have each mol insert itself into the mols section.
//         // At this time, there is actually only one kind of mol.
//         std::for_each( molsByName.begin(),
//                        molsByName.end(),
//                        insertMolElt( pMolsElt ) );
    }
}
