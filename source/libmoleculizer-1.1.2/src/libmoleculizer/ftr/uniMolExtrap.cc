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

#include "fnd/pchem.hh"
#include "mol/mzrModMol.hh"
#include "ftr/uniMolExtrap.hh"
#include "fnd/pchem.hh"

namespace ftr
{
    uniMolMassExtrap::
    uniMolMassExtrap( double theRate,
                      const bnd::mzrModMol* pEnablingMol,
                      const fnd::massive* pMassiveAuxiliarySpecies ) :
        pMassive( pMassiveAuxiliarySpecies )
    {
        if ( pMassive )
        {
            cpx::molParam pEnablingMolDefaultState
                = pEnablingMol->getDefaultState();
            
            rateOrInvariant
                = fnd::bindingInvariant( theRate,
                                         pEnablingMolDefaultState->getMolWeight(),
                                         pMassive->getWeight() );
        }
        else
        {
            rateOrInvariant = theRate;
        }
    }
    
    double
    uniMolMassExtrap::
    getRate( const cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rWrappedContext ) const
    {
        // Are we generating unary or binary reactions?
        if ( pMassive )
        {
            return fnd::bindingRate( rateOrInvariant,
                                     rWrappedContext.getPlexWeight(),
                                     pMassive->getWeight() );
        }
        else
        {
            return rateOrInvariant;
        }
    }
}
