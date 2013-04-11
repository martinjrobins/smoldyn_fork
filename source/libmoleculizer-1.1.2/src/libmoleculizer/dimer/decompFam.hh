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

#ifndef DIMER_DECOMPFAM_H
#define DIMER_DECOMPFAM_H

#include "utl/autoVector.hh"
#include "dimer/decompRxnGen.hh"
#include <iostream>

namespace dimer
{
    class decompFam :
        public utl::autoVector<mzr::mzrReaction>
    {
        decompRxnGen decompGen;
        
    public:
        decompFam( mzr::mzrUnit& rMzrUnit,
                   plx::plexUnit& rPlexUnit,
                   decomposeExtrapolator* pExtrap ) :
            decompGen( this,
                       rMzrUnit,
                       rPlexUnit,
                       pExtrap )
        {}
        
        decompRxnGen*
        getRxnGen( void )
        {
            return &decompGen;
        }
    };
}

#endif // DIMER_DECOMPFAM_H
