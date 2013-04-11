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

#ifndef FTR_UNIMOLEXTRAP_H
#define FTR_UNIMOLEXTRAP_H

#include "cpx/cxMol.hh"

namespace ftr
{
    class uniMolExtrapolator
    {
    public:
        virtual
        ~uniMolExtrapolator( void )
        {}
        
        virtual
        double
        getRate( const cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rContext ) const = 0;
    };
    
    class uniMolNoExtrap :
        public uniMolExtrapolator
    {
        double rate;
    public:
        uniMolNoExtrap( double theRate ) :
            rate( theRate )
        {}
        
        double
        getRate( const cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rContext ) const
        {
            return rate;
        }
    };
    
    class uniMolMassExtrap :
        public uniMolExtrapolator
    {
        // Pointer to "massive" part of auxiliary reactant species; null if there
        // is no auxiliary reactant species.
        const fnd::massive* pMassive;
        
        // The rate if the reaction is unary (0 == pMassive) but the binding
        // invariant if the reaction is binary.
        double rateOrInvariant;
        
    public:
        // For creating unary reactions.
        uniMolMassExtrap( double theRate ) :
            pMassive( 0 ),
            rateOrInvariant( theRate )
        {}
        
        // For creating binary reactions.
        uniMolMassExtrap( double theRate,
                          const bnd::mzrModMol* pEnablingMol,
                          const fnd::massive* pMassiveAuxiliarySpecies );
        
        double
        getRate( const cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rContext ) const;
    };
}

#endif //  FTR_UNIMOLEXTRAP_H
