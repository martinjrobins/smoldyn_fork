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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2008
//     * Combining all Larry's original dom execption files into one mass
//       file
//

#include <sstream>
#include "domXcpt.hh"
#include <libxml++/libxml++.h>

namespace utl
{
    namespace dom
    {
        std::string
        badChildCountXcpt::
        mkGeneralMsg( const xmlpp::Node* pParentNode,
                      const std::string& rChildName,
                      int requiredCount,
                      int actualCount )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pParentNode )
                      << "Expected "
                      << requiredCount
                      << " "
                      << rChildName
                      << " elements in "
                      << pParentNode->get_name()
                      << " element; got "
                      << actualCount
                      << ".";
            return msgStream.str();
        }
        
        std::string
        badChildCountXcpt::
        mkChoiceMsg( const xmlpp::Node* pParentNode,
                     int actualCount )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pParentNode )
                      << "Expected one child of "
                      << pParentNode->get_name()
                      << " node among alternatives "
                      << "offered by choice, got "
                      << actualCount
                      << ".";
            return msgStream.str();
        }
        
        std::string
        badChildCountXcpt::
        mkOneOrMoreMsg( const xmlpp::Node* pParentNode )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pParentNode )
                      << "Expected one or more children of "
                      << pParentNode->get_name()
                      << " node among alternatives, got none.";
            return msgStream.str();
        }
        
        // In support of RNG's "optional" construct.
        std::string
        badChildCountXcpt::
        mkZeroOrOneMsg( const xmlpp::Node* pParentNode,
                        const std::string& rChildName,
                        int actualCount )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pParentNode )
                      << "Expected zero or one "
                      << rChildName
                      << " elements in "
                      << pParentNode->get_name()
                      << " element; got "
                      << actualCount
                      << ".";
            return msgStream.str();
        }
        
        badChildCountXcpt::
        badChildCountXcpt( const std::string& rMsg ) :
            xcpt( rMsg )
        {}
        
        badChildCountXcpt
        badChildCountXcpt::
        general( const xmlpp::Node* pParentNode,
                 const std::string& rChildName,
                 int requiredCount,
                 int actualCount )
            throw()
        {
            return badChildCountXcpt( mkGeneralMsg( pParentNode,
                                                    rChildName,
                                                    requiredCount,
                                                    actualCount ) );
        }
        
        badChildCountXcpt
        badChildCountXcpt::
        choice( const xmlpp::Node* pParentNode,
                int actualCount )
            throw()
        {
            return badChildCountXcpt( mkChoiceMsg( pParentNode,
                                                   actualCount ) );
        }
        
        badChildCountXcpt
        badChildCountXcpt::
        oneOrMore( const xmlpp::Node* pParentNode )
            throw()
        {
            return badChildCountXcpt( mkOneOrMoreMsg( pParentNode ) );
        }
        
        badChildCountXcpt
        badChildCountXcpt::
        zeroOrOne( const xmlpp::Node* pParentNode,
                   const std::string& rChildName,
                   int actualCount )
            throw()
        {
            return badChildCountXcpt( mkZeroOrOneMsg( pParentNode,
                                                      rChildName,
                                                      actualCount ) );
        }
        
        std::string
        badDoubleAttrXcpt::
        mkMsg( const xmlpp::Element* pElement,
               const std::string& rAttrName,
               const std::string& rBadAttrValue )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pElement )
                      << "Expected double for value of "
                      << rAttrName
                      << " attribute; got `"
                      << rBadAttrValue
                      << "'.";
            return msgStream.str();
        }
        
        badDoubleAttrXcpt::
        badDoubleAttrXcpt( const xmlpp::Element* pElement,
                           const std::string& rAttrName,
                           const std::string& rBadAttrValue ) throw() :
            xcpt( mkMsg( pElement,
                         rAttrName,
                         rBadAttrValue ) )
        {}
        
        badElementCastXcpt::
        badElementCastXcpt( const xmlpp::Node* pNode )
            throw() :
            xcpt( mkMsg( pNode ) )
        {}
        
        std::string
        badElementCastXcpt::
        mkMsg( const xmlpp::Node* pNode )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pNode )
                      << "Could not cast "
                      << pNode->get_name()
                      << " node to xmlpp::Element.";
            return msgStream.str();
        }
        
        std::string
        badIntAttrXcpt::
        mkMsg( const xmlpp::Element* pOffendingElement,
               const std::string& rAttrName,
               const std::string& rBadAttrValue )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pOffendingElement )
                      << "Expected integer for value of "
                      << rAttrName
                      << " attribute; got `"
                      << rBadAttrValue
                      << "'.";
            return msgStream.str();
        }
        
        badIntAttrXcpt::
        badIntAttrXcpt( const xmlpp::Element* pOffendingElement,
                        const std::string& rAttrName,
                        const std::string& rBadAttrValue ) :
            xcpt( mkMsg( pOffendingElement,
                         rAttrName,
                         rBadAttrValue ) )
        {}
        
        std::string
        badNNDoubleAttrXcpt::
        mkMsg( const xmlpp::Element* pOffendingElement,
               const std::string& rAttrName,
               double badDoubleValue )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pOffendingElement )
                      << "Expected "
                      << rAttrName
                      << " attribute to be non-negative; got "
                      << badDoubleValue
                      << ".";
            return msgStream.str();
        }
        
        badNNDoubleAttrXcpt::
        badNNDoubleAttrXcpt( const xmlpp::Element* pOffendingElement,
                             const std::string& rAttrName,
                             double badDoubleValue )
            throw() :
            xcpt( mkMsg( pOffendingElement,
                         rAttrName,
                         badDoubleValue ) )
        {}
        
        std::string
        badPosDoubleAttrXcpt::
        mkMsg( const xmlpp::Element* pOffendingElement,
               const std::string& rAttrName,
               double badDoubleValue )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pOffendingElement )
                      << "Expected "
                      << rAttrName
                      << " attribute to be positive; got "
                      << badDoubleValue
                      << ".";
            return msgStream.str();
        }
        
        badPosDoubleAttrXcpt::
        badPosDoubleAttrXcpt( const xmlpp::Element* pOffendingElement,
                              const std::string& rAttrName,
                              double badDoubleValue )
            throw() :
            xcpt( mkMsg( pOffendingElement,
                         rAttrName,
                         badDoubleValue ) )
        {}
        
        std::string
        missingAttrXcpt::
        mkMsg( const xmlpp::Element* pElt,
               const std::string& rAttrName )
        {
            std::ostringstream msgStream;
            
            msgStream << xcpt::mkMsg( pElt )
                      << "'"
                      << pElt->get_name()
                      << "\" element has no \""
                      << rAttrName
                      << "\" attribute.";
            return msgStream.str();
        }
        
        missingAttrXcpt::
        missingAttrXcpt( const xmlpp::Element* pElt,
                         const std::string& rAttrName )
            throw() :
            xcpt( mkMsg( pElt,
                         rAttrName ) )
        {}
        
        std::string
        badNNIntAttrXcpt::
        mkMsg( const xmlpp::Element* pOffendingElt,
               const std::string& rAttrName,
               int badAttrValue )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pOffendingElt )
                      << "Expected non-negative integer for value of "
                      << rAttrName
                      << " attribute; got "
                      << badAttrValue
                      << ".";
            return msgStream.str();
        }
        
        badNNIntAttrXcpt::
        badNNIntAttrXcpt( const xmlpp::Element* pOffendingElt,
                          const std::string& rAttrName,
                          int badAttrValue ) :
            xcpt( mkMsg( pOffendingElt,
                         rAttrName,
                         badAttrValue ) )
        {}
        
        badPosIntAttrXcpt::badPosIntAttrXcpt( const xmlpp::Element* pOffendingElt,
                                              const std::string& rAttrName,
                                              int badAttrValue )
            :
            xcpt( mkMsg( pOffendingElt,
                         rAttrName,
                         badAttrValue ) )
        {}
        
        std::string
        badPosIntAttrXcpt::
        mkMsg( const xmlpp::Element* pOffendingElt,
               const std::string& rAttrName,
               int badAttrValue )
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg( pOffendingElt )
                      << "Expected positive integer for value of "
                      << rAttrName
                      << " attribute; got "
                      << badAttrValue
                      << ".";
            return msgStream.str();
        }
        
        
    } // Namespace dom
}
