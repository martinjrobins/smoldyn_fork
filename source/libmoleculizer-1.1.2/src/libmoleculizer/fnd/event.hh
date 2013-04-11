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

#ifndef FND_EVENT_H
#define FND_EVENT_H

#include <exception>

namespace fnd
{
    enum eventResult
    {
        go,
        stop
    };
    
    template<class applicationType>
    class event
    {
    public:
        virtual
        ~event( void )
        {}
        
        // How the event happens. Typically, this is by making changes to state
        // variables to which the event is directly connected, for example, a
        // reaction event changes the populations of its substrate and product
        // species. But an event could also change something global, such as the
        // volume in a mixed-bag simulation, which can be made accessible via
        // pApp.
        virtual
        eventResult
        happen( applicationType& rApp )
            throw( std::exception ) = 0;
    };
}

#endif // FND_EVENT_H
