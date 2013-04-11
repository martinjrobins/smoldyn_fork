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

#include "dimer/dimerXcpt.hh"
#include "dimer/dimerizeExtrap.hh"


namespace dimer
{
    void
    dimerizeNoExtrap::
    setRate( cpx::siteParam leftParam,
             cpx::siteParam rightParam,
             double rate )
    {
        // For the time being, I'm storing the pair in only one order,
        // then searching for both orders when the pair is looked up.
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
    dimerizeNoExtrap::
    getRate( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rLeftContext,
             const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rRightContext ) const
    {
        // Now we have to check for both orders in the pair of site shape
        // pointers.
        rateMapType::const_iterator iEntry
            = rateMap.find( std::make_pair( rLeftContext.getSiteParam(),
                                            rRightContext.getSiteParam() ) );
        if ( iEntry == rateMap.end() )
        {
            iEntry = rateMap.find( std::make_pair( rRightContext.getSiteParam(),
                                                   rLeftContext.getSiteParam() ) );
            if ( iEntry == rateMap.end() )
                throw missingDimerizeRateXcpt( rLeftContext,
                                               rRightContext );
        }
        
        return iEntry->second;
    }
    
    void
    dimerizeMassExtrap::
    setRate( cpx::siteParam leftParam,
             cpx::siteParam rightParam,
             double rate )
    {
        double invariant = fnd::bindingInvariant( rate,
                                                  leftMass,
                                                  rightMass );
        
        std::pair<invMapType::iterator, bool> insertResult
            = invariantMap.insert( std::make_pair( std::make_pair( leftParam,
                                                                   rightParam ),
                                                   invariant ) );
        if ( ! insertResult.second )
        {
            insertResult.first->second = invariant;
        }
    }
    
    double
    dimerizeMassExtrap::
    getRate( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rLeftContext,
             const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rRightContext ) const
    {
        // Since the rates were stored with the key pair in only one
        // order, we have to check for both orders when looking it up.
        invMapType::const_iterator iEntry
            = invariantMap.find( std::make_pair( rLeftContext.getSiteParam(),
                                                 rRightContext.getSiteParam() ) );
        if ( iEntry == invariantMap.end() )
        {
            iEntry = invariantMap.find( std::make_pair( rRightContext.getSiteParam(),
                                                        rLeftContext.getSiteParam() ) );
            if ( iEntry == invariantMap.end() )
                throw missingDimerizeInvariantXcpt( rLeftContext,
                                                    rRightContext );
        }
        
        return fnd::bindingRate( iEntry->second,
                                 rLeftContext.getPlexWeight(),
                                 rRightContext.getPlexWeight() );
    }
}
