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

#ifndef MZR_CREATEEVENT_H
#define MZR_CREATEEVENT_H

#include "mzr/mzrEvent.hh"

namespace mzr
{
    class mzrSpecies;
    class mzrUnit;
    
    /*! \ingroup eventGroup
      \brief Abstract base class for all events.
      
      Basically, everything that happens in the simulation is an event.
      
      Aside from the pure virtual doEvent method, which can do just about
      anything, the event is enabled to keep track of its entry in the
      global queue of events.  This allows one to determine if the event
      is scheduled to happen and at what simulation time. */
    
    /*! \ingroup eventGroup
      \brief Event to create species members. */
    class createEvent :
        public mzrEvent
    {
        mzrSpecies* pSpeciesToCreate;
        int howMany;
        
        mzrUnit& rMzrUnit;
        
    public:
        createEvent( mzrSpecies* pSpecies,
                     int count,
                     mzrUnit& refMzrUnit );
        
        fnd::eventResult
        happen( moleculizer& rMolzer )
            throw( std::exception );
    };
}

#endif // MZR_CREATEEVENT_H
