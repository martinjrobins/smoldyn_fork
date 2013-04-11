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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//

#ifndef __DIMERXCPT_HH
#define __DIMERXCPT_HH

#include "utl/defs.hh"
#include "cpx/cxBinding.hh"
#include "cpx/cxSite.hh"
#include "plex/mzrPlexSpecies.hh"
#include "plex/mzrPlexFamily.hh"

namespace dimer
{
    
    // Thrown by decomposeNoExtrap extrapolator when the extrapolator is asked
    // to construct a decomposition rate for site shape pairs that have not been
    // given a nominal rate.
    class missingDecomposeRateXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rFirstSiteShapeName,
               const std::string& rSecondSiteShapeName );
        
    public:
        missingDecomposeRateXcpt( const std::string& rFirstSiteShapeName,
                                  const std::string& rSecondSiteShapeName ) :
            utl::xcpt( mkMsg( rFirstSiteShapeName,
                              rSecondSiteShapeName ) )
        {}
    };
    
    // Thrown by dimerizeMassExtrap extrapolator when the extrapolator is asked
    // to construct rates for site shape pairs that have not been given a
    // nominal rate.
    class missingDimerizeInvariantXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxLeft,
               const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxRight );
        
    public:
        missingDimerizeInvariantXcpt( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxLeft,
                                      const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxRight )
            :
            utl::xcpt( mkMsg( cxLeft, cxRight ) )
        {}
    };
    
    
    // Thrown by dimerizeNoExtrap extrapolator when the extrapolator is asked to
    // construct rates for site shape pairs that have not been given a nominal
    // rate.
    class missingDimerizeRateXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxLeft,
               const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxRight );
        
    public:
        missingDimerizeRateXcpt( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxLeft,
                                 const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxRight )
            :
            utl::xcpt( mkMsg( cxLeft, cxRight ) )
        {}
    };
    
    
    
}

#endif
