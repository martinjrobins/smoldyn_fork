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

#ifndef UTL_DOM_H
#define UTL_DOM_H

#include <string>
#include <set>

#include "utl/xcpt.hh"
#include "utl/domXcpt.hh"

namespace xmlpp
{
    class Node;
    class Element;
    class Document;
    class DomParser;
}

namespace utl
{
    namespace dom
    {
        xmlpp::Element*
        mustBeElementPtr( xmlpp::Node* pNode )
        throw( xcpt );
        
        const xmlpp::Element*
        mustBeElementPtr( const xmlpp::Node* pNode )
            throw( xcpt );
        
        xmlpp::Element*
        mustGetUniqueChild( const xmlpp::Node* pParentNode,
                            const std::string& rChildName )
            throw( xcpt );
        
        xmlpp::Element*
        mustGetUniqueChild( const xmlpp::Node* pParentNode,
                            const std::set<std::string>& rChoiceEltNames )
            throw( xcpt );
        
        // Returns null pointer if no child has the given name.  Throws
        // badChildCountXcpt if more than one child has the given name.
        xmlpp::Element*
        getOptionalChild( const xmlpp::Node* pParentNode,
                          const std::string& rChildName )
            throw( xcpt );
        
        std::string
        mustGetAttrString( const xmlpp::Element* pElement,
                           const std::string& rAttrName )
            throw( xcpt );
        
        double
        mustGetAttrDouble( const xmlpp::Element* pElement,
                           const std::string& rAttrName )
            throw( xcpt );
        
        double
        mustGetAttrPosDouble( const xmlpp::Element* pElement,
                              const std::string& AttrName )
            throw( xcpt );
        
        double
        mustGetAttrNNDouble( const xmlpp::Element* pElement,
                             const std::string& rAttrName )
            throw( xcpt );
        
        int
        mustGetAttrInt( const xmlpp::Element* pElement,
                        const std::string& rAttrName )
            throw( xcpt );
        
        int
        mustGetAttrPosInt( const xmlpp::Element* pElement,
                           const std::string& rAttrName )
            throw( xcpt );
        
        int
        mustGetAttrNNInt( const xmlpp::Element* pElement,
                          const std::string& rAttrName )
            throw( xcpt );
        
        // Inserts a stereotyped double-valued parameter element, with additional
        // elements giving the parameter value in scientific notation
        //
        // This is basically part of a fix to introduce scientific notation for
        // all parameter values so as to be able, using XSLT, to generate SBML and
        // other formats that have the fraction and exponent in separate XML
        // constructs.
        void
        addDoubleParamChild( xmlpp::Node* pParentNode,
                             const std::string& rChildName,
                             const std::string& rParameterName,
                             double parameterValue );

    namespace tmp
    {
	void addChildWithAttribute( xmlpp::Element* pParent, 
				    const std::string& element_name, 
				    const std::string& attribute_name,
				    const std::string& attribute_value);
	
    }



    }




}

#endif // UTL_DOM_H
