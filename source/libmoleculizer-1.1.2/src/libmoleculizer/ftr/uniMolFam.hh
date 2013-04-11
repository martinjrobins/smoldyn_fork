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

#ifndef FTR_UNIMOLFAM_H
#define FTR_UNIMOLFAM_H

#include "utl/autoVector.hh"
#include "ftr/uniMolGen.hh"

namespace ftr
{
    class uniMolFam :
        public utl::autoVector<mzr::mzrReaction>
    {
        uniMolRxnGen rxnGen;
        
    public:
        uniMolFam( mzr::mzrUnit& refMzrUnit,
                   plx::plexUnit& refPlexUnit,
                   bnd::mzrModMol* pEnablingModMol,
                   cpx::andMolStateQueries* pAndMolQueries,
                   const std::vector<molModExchange>& rModExchanges,
                   mzr::mzrSpecies* pAuxReactant,
                   mzr::mzrSpecies* pAuxProduct,
                   const uniMolExtrapolator* pUniMolExtrapolator ) :
            rxnGen( refMzrUnit,
                    refPlexUnit,
                    pEnablingModMol,
                    pAndMolQueries,
                    rModExchanges,
                    pAuxReactant,
                    pAuxProduct,
                    this,
                    pUniMolExtrapolator )
        {}
        
        uniMolRxnGen*
        getRxnGen( void )
        {
            return &rxnGen;
        }
    };
}

#endif // FTR_UNIMOLFAM_H
