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

#ifndef PLEX_NOOMNIFORNODEXCPT_H
#define PLEX_NOOMNIFORNODEXCPT_H

#include "utl/dom.hh"

namespace plx
{
    // Thrown when a client of the plexUnit wants to know what omniplex the
    // plexUnit parsed for one of the client's nodes.  If the plexUnit doesn't
    // know, it probably means that the client unit didn't register an Xpath for
    // the node with the plexUnit, so the plexUnit didn't really parse the
    // omniplex.  (This might be a common error in new unit implementation.)
    class noOmniForNodeXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const xmlpp::Node* pParentNode = 0 );
        
    public:
        noOmniForNodeXcpt( const xmlpp::Node* pParentNode = 0 ) :
            utl::xcpt( mkMsg( pParentNode ) )
        {}
    };
}

#endif // PLEX_NOOMNIFORNODEXCPT_H
