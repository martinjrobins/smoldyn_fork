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

#ifndef UTL_MESSAGE_H
#define UTL_MESSAGE_H

#include <iostream>
#include <string>

namespace utl
{
    class message
    {
        std::string msg;
    public:
        
        message( const std::string& rMessage ) throw() :
            msg( rMessage )
        {}
        
        message( const char* pMessage ) throw() :
            msg( pMessage )
        {}
        
        const std::string&
        getMessage( void ) const throw()
        {
            return msg;
        }
        
        void
        issue( void ) const throw()
        {
            std::cerr << getMessage() << std::endl;
        }
    };
}

#endif // UTL_MESSAGE_H
