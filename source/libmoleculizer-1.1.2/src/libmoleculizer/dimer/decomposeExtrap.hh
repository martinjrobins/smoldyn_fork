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

#ifndef DIMER_DECOMPOSEEXTRAP_H
#define DIMER_DECOMPOSEEXTRAP_H

#include "cpx/siteShape.hh"
#include "plex/mzrPlexSpecies.hh"
#include "plex/mzrPlexFamily.hh"

namespace dimer
{
    // Base class of rate extrapolators for decomposition reactions.
    class decomposeExtrapolator
    {
    public:
        virtual
        ~decomposeExtrapolator( void )
        {}
        
        virtual void
        setRate( cpx::siteParam leftParam,
                 cpx::siteParam rightParam,
                 double rate ) = 0;
        
        virtual double
        getRate( const cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rContext ) const
            throw( utl::xcpt ) = 0;
    };
    
    // Standard decomposition extrapolator, which doesn't really
    // do anything: the same rate applies regardless of the context.
    class decomposeNoExtrap :
        public decomposeExtrapolator
    {
        typedef
        std::map<std::pair<cpx::siteParam, cpx::siteParam>, double>
        rateMapType;
        
        rateMapType rateMap;
        
    public:
        // Both for inserting default rates and for writing allosteric
        // rates over default rates.
        //
        // Storing the rate with the key pair in only one order; this means that
        // search has to look for both orders.
        void
        setRate( cpx::siteParam leftParam,
                 cpx::siteParam rightParam,
                 double rate );
        
        // Retrieve decomposition rate for binding in a new complex species.
        double
        getRate( const cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rContext ) const
            throw( utl::xcpt );
    };
    
}

#endif // DIMER_DECOMPOSEEXTRAP_H
