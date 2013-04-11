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

#include "utl/dom.hh"
#include "utl/domXcpt.hh"
#include "utl/frexp10.hh"
#include "utl/utility.hh"
#include "utl/utlEltName.hh"
#include <libxml++/libxml++.h>

namespace utl
{
    namespace dom
    {
        std::string
        xcpt::
        mkMsg( const xmlpp::Node* pOffendingNode )
        {
            std::ostringstream msgStream;
            if ( pOffendingNode )
            {
                msgStream << pOffendingNode->get_path()
                          << ": ";
            }
            return msgStream.str();
        }
        
        xmlpp::Element*
        mustBeElementPtr( xmlpp::Node* pNode )
            throw( xcpt )
        {
            xmlpp::Element* pElt = dynamic_cast<xmlpp::Element*>( pNode );
            
            if ( 0 == pElt ) throw badElementCastXcpt( pNode );
            
            return pElt;
        }
        
        const xmlpp::Element*
        mustBeElementPtr( const xmlpp::Node* pNode )
            throw( xcpt )
        {
            const xmlpp::Element* pElt
                = dynamic_cast<const xmlpp::Element*>( pNode );
            
            if ( 0 == pElt ) throw badElementCastXcpt( pNode );
            
            return pElt;
        }
        
        xmlpp::Element*
        mustGetUniqueChild( const xmlpp::Node* pParentNode,
                            const std::string& rChildName )
            throw( xcpt )
        {
            const xmlpp::Node::NodeList children
                = pParentNode->get_children( rChildName );
            
            if ( 1 != children.size() )
                throw badChildCountXcpt::general( pParentNode,
                                                  rChildName,
                                                  1,
                                                  children.size() );
            
            xmlpp::Node* pChildNode = children.front();
            
            return mustBeElementPtr( pChildNode );
        };
        
        xmlpp::Element*
        getOptionalChild( const xmlpp::Node* pParentNode,
                          const std::string& rChildName )
            throw( xcpt )
        {
            const xmlpp::Node::NodeList children
                = pParentNode->get_children( rChildName );
            
            // Here trying out a better way to arrange different ways
            // of constructing the same exception for use under different
            // circumstances.
            int childCount = children.size();
            switch ( childCount )
            {
            case 0 :
                return 0;
            case 1 :
                return mustBeElementPtr( children.front() );
            default :
                throw badChildCountXcpt::zeroOrOne( pParentNode,
                                                    rChildName,
                                                    childCount );
            }
        }
        
        std::string
        mustGetAttrString( const xmlpp::Element* pElement,
                           const std::string& rAttrName )
            throw( xcpt )
        {
            xmlpp::Attribute* pAttr
                = pElement->get_attribute( rAttrName );
            
            if ( 0 == pAttr ) throw missingAttrXcpt( pElement,
                                                     rAttrName );
            return pAttr->get_value();
        }
        
        double
        mustGetAttrDouble( const xmlpp::Element* pElement,
                           const std::string& rAttrName )
            throw( xcpt )
        {
            std::string attrString
                = mustGetAttrString( pElement,
                                     rAttrName );
            
            double attrDouble = 0.0;
            if ( ! stringIsDouble( attrString,
                                   attrDouble ) )
                throw badDoubleAttrXcpt( pElement,
                                         rAttrName,
                                         attrString );
            
            return attrDouble;
        }
        
        double
        mustGetAttrPosDouble( const xmlpp::Element* pElement,
                              const std::string& rAttrName )
            throw( xcpt )
        {
            double attrDouble
                = mustGetAttrDouble( pElement,
                                     rAttrName );
            
            if ( attrDouble <= 0.0 )
                throw badPosDoubleAttrXcpt( pElement,
                                            rAttrName,
                                            attrDouble );
            return attrDouble;
        }
        
        double
        mustGetAttrNNDouble( const xmlpp::Element* pElement,
                             const std::string& rAttrName )
            throw( xcpt )
        {
            double attrDouble
                = mustGetAttrDouble( pElement,
                                     rAttrName );
            
            if ( attrDouble < 0.0 )
                throw badNNDoubleAttrXcpt( pElement,
                                           rAttrName,
                                           attrDouble );
            return attrDouble;
        }
        
        int
        mustGetAttrInt( const xmlpp::Element* pElement,
                        const std::string& rAttrName )
            throw( xcpt )
        {
            std::string attrString
                = mustGetAttrString( pElement,
                                     rAttrName );
            
            int attrInt = -1;
            if ( ! stringIsInt( attrString,
                                attrInt ) )
                throw badIntAttrXcpt( pElement,
                                      rAttrName,
                                      attrString );
            return attrInt;
        }
        
        int
        mustGetAttrPosInt( const xmlpp::Element* pElement,
                           const std::string& rAttrName )
            throw( xcpt )
        {
            int attrInt
                = mustGetAttrInt( pElement,
                                  rAttrName );
            if ( attrInt <= 0 )
                throw badPosIntAttrXcpt( pElement,
                                         rAttrName,
                                         attrInt );
            return attrInt;
        }
        
        int
        mustGetAttrNNInt( const xmlpp::Element* pElement,
                          const std::string& rAttrName )
            throw( xcpt )
        {
            int attrInt
                = mustGetAttrInt( pElement,
                                  rAttrName );
            if ( attrInt < 0 )
                throw badNNIntAttrXcpt( pElement,
                                        rAttrName,
                                        attrInt );
            return attrInt;
        }
        
        void
        addDoubleParamChild( xmlpp::Node* pParentNode,
                             const std::string& rChildName,
                             const std::string& rParameterName,
                             double parameterValue )
        {
            xmlpp::Element* pChildElt
                = pParentNode->add_child( rChildName );
            
            pChildElt->set_attribute( rParameterName,
                                      utl::stringify<double> ( parameterValue ) );
            
            xmlpp::Element* pSciNoteElt
                = pChildElt->add_child( eltName::sciNote );
            
            // Generate scientific notation for use in generating SBML etc.
            int exponent = 0;
            double fraction = utl::frexp10( parameterValue,
                                            exponent );
            
            pSciNoteElt->set_attribute( eltName::sciNote_fractionAttr,
                                        utl::stringify<double> ( fraction ) );
            
            pSciNoteElt->set_attribute( eltName::sciNote_exponentAttr,
                                        utl::stringify<int> ( exponent ) );
        }

	namespace tmp
	{
	    void addChildWithAttribute( xmlpp::Element* pParent,
					const std::string& element_name, 
					const std::string& attribute_name,
					const std::string& attribute_value)
	    {
		xmlpp::Element* pNewChildElement = pParent->add_child( element_name );
		pNewChildElement->set_attribute( attribute_name, attribute_value);
		return;
	    }
	}

    }


}
