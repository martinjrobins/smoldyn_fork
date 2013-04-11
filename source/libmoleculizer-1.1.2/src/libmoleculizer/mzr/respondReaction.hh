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

#ifndef MZR_RESPONDREACTION_H
#define MZR_RESPONDREACTION_H

#include "mzr/mzrReaction.hh"

namespace mzr
{
    // This can be used by events to reschedule affected reactions en masse.
    // For example, an event that changes the volume will need to reschedule
    // all reactions.
    class respondReaction :
        public std::unary_function<mzrReaction*, void>
    {
        mzrReactionStimulus stimulus;
    public:
        respondReaction( moleculizer& rMoleculizer ) :
            stimulus( rMoleculizer )
        {}
        
        void
        operator()( mzrReaction* pMzrReaction ) const
        {
            pMzrReaction->respond( stimulus );
        }
    };
}

#endif // MZR_RESPONDREACTION_H
