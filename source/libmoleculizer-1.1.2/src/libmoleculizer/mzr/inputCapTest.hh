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

#ifndef MZR_INPUTCAPTEST_H
#define MZR_INPUTCAPTEST_H

#include "utl/dom.hh"

namespace mzr
{
    class modelNodeNotInCap :
        public std::unary_function<xmlpp::Node*, bool>
    {
        const inputCapabilities& rInputCap;
    public:
        modelNodeNotInCap( const inputCapabilities& rInputCapabilities ) :
            rInputCap( rInputCapabilities )
        {}
        
        bool
        operator()( xmlpp::Node* pNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pElt
                = dynamic_cast<xmlpp::Element*>( pNode );
            
            // We want to return false if the node is not an element, for example,
            // if the node is a comment.
            //
            // Perhaps other tests might be worthwhile to rule out stray text nodes
            // or whatever else might appear as corrupting matter, but this whole
            // checking process was a little daffy.
            return ( pElt && ( ! rInputCap.handlesModelContentElt( pElt ) ) );
        }
    };
    
    class explicitSpeciesNodeNotInCap :
        public std::unary_function<xmlpp::Node*, bool>
    {
        const inputCapabilities& rInputCap;
    public:
        explicitSpeciesNodeNotInCap( const inputCapabilities& rInputCapabilities ) :
            rInputCap( rInputCapabilities )
        {}
        
        bool
        operator()( xmlpp::Node* pNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pElt
                = dynamic_cast<xmlpp::Element*>( pNode );
            
            // We want to return false if the node is not an element, for example,
            // if the node is a comment.
            return ( pElt && ( ! rInputCap.handlesExplictSpeciesContent( pElt ) ) );
        }
    };
    
    class speciesStreamNodeNotInCap :
        public std::unary_function<xmlpp::Node*, bool>
    {
        const inputCapabilities& rInputCap;
    public:
        speciesStreamNodeNotInCap( const inputCapabilities& rInputCapabilities ) :
            rInputCap( rInputCapabilities )
        {}
        
        bool
        operator()( xmlpp::Node* pNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pElt
                = dynamic_cast<xmlpp::Element*>( pNode );
            
            // We want to return false if the node is not an element, for example,
            // if the node is a comment.
            return ( pElt && ( ! rInputCap.handlesSpeciesStreamsContent( pElt ) ) );
        }
    };
    
    class eventNodeNotInCap :
        public std::unary_function<xmlpp::Node*, bool>
    {
        const inputCapabilities& rInputCap;
    public:
        eventNodeNotInCap( const inputCapabilities& rInputCapabilities ) :
            rInputCap( rInputCapabilities )
        {}
        
        bool
        operator()( xmlpp::Node* pNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pElt
                = dynamic_cast<xmlpp::Element*>( pNode );
            
            // We want to return false if the node is not an element, for example,
            // if the node is a comment.
            return ( pElt && ( ! rInputCap.handlesEventsContent( pElt ) ) );
        }
    };
    
    class reactionGenNotInCap :
        public std::unary_function<xmlpp::Node*, bool>
    {
        const inputCapabilities& rInputCap;
    public:
        reactionGenNotInCap( const inputCapabilities& rInputCapabilities ) :
            rInputCap( rInputCapabilities )
        {}
        
        bool
        operator()( xmlpp::Node* pNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pElt
                = dynamic_cast<xmlpp::Element*>( pNode );
            
            // We want to return false if the node is not an element, for example,
            // if the node is a comment.
            return ( pElt && ( ! rInputCap.handlesReactionGensContent( pElt ) ) );
        }
    };
}

#endif // MZR_INPUTCAPTEST_H
