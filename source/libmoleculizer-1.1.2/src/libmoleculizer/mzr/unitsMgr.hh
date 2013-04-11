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

#ifndef UNITSMGR_H
#define UNITSMGR_H

#include "utl/autoVector.hh"
#include "plex/plexUnit.hh"
#include "mzr/unit.hh"

namespace mzr
{
    class mzrUnit;
}
namespace bnd
{
    class molUnit;
}
namespace plx
{
    class plexUnit;
}
namespace dimer
{
    class dimerUnit;
}
namespace stoch
{
    class stochUnit;
}
namespace ftr
{
    class ftrUnit;
}

namespace nmr
{
    class nmrUnit;
}

namespace mzr
{
    class moleculizer;
    
    // As a vector, the units should be ordered in output order, so that if each
    // unit arranges its elements correctly (under various heads in the output
    // document) and the units are ordered thus, then all the elements in the
    // output doc should appear in the order demanded by the schema.
    class unitsMgr :
        public utl::autoVector<unit>
    {
    public:
        
        nmr::nmrUnit* pNmrUnit;
        mzr::mzrUnit* pMzrUnit;
        bnd::molUnit* pMolUnit;
        plx::plexUnit* pPlexUnit;
        stoch::stochUnit* pStochUnit;
        dimer::dimerUnit* pDimerUnit;
        ftr::ftrUnit* pFtrUnit;
        
        // Now the moleculizer unit is also a recorder of species..
        // which is a ReactionNetworkDescription<mzrSpecies, mzrReactions>.
        unitsMgr( moleculizer& rMoleculizer );
        
        // Prepares the overall input capabilities of moleculizer from
        // the several input capabilities of the units.
        void
        unionInputCaps( inputCapabilities& rUnion );
    };
}

#endif // UNITSMGR_H
