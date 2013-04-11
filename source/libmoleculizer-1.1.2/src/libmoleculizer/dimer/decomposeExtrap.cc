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

#include "dimer/decomposeExtrap.hh"
#include "dimer/dimerXcpt.hh"

namespace dimer
{
    void
    decomposeNoExtrap::
    setRate( cpx::siteParam leftParam,
             cpx::siteParam rightParam,
             double rate )
    {
        std::pair<rateMapType::iterator, bool> insertResult
            = rateMap.insert( std::make_pair( std::make_pair( leftParam,
                                                              rightParam ),
                                              rate ) );
        if ( ! insertResult.second )
        {
            insertResult.first->second = rate;
        }
    }
    
    double
    decomposeNoExtrap::
    getRate( const cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rContext ) const
        throw( utl::xcpt )
    {
        // Get the shapes (bnd::siteParam) of the sites in the binding.
        std::pair<cpx::siteParam, cpx::siteParam> siteParams
            = rContext.getSiteParams();
        
        // Since the rates were stored with the key pair in only one
        // order, we have to check for both orders when looking it up.
        rateMapType::const_iterator iEntry
            = rateMap.find( siteParams );
        
        if ( iEntry == rateMap.end() )
        {
            iEntry = rateMap.find( std::make_pair( siteParams.second,
                                                   siteParams.first ) );
            if ( iEntry == rateMap.end() )
                throw missingDecomposeRateXcpt( siteParams.first->getName(),
                                                siteParams.second->getName() );
        }
        
        return iEntry->second;
    }
}
