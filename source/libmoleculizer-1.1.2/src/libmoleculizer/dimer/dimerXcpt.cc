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


#include "dimerXcpt.hh"

namespace dimer
{
    
    // I think tons of useful information is actually available through
    // the binding context, but not emitted here.
    std::string
    missingDecomposeRateXcpt::
    mkMsg( const std::string& rFirstSiteShapeName,
           const std::string& rSecondSiteShapeName )
    {
        std::ostringstream msgStream;
        msgStream << "decomposeNoExtrap extrapolator encountered "
                  << "site shapes '"
                  << rFirstSiteShapeName
                  << "' and '"
                  << rSecondSiteShapeName
                  << "' for which no nominal rate has been given.";
        return msgStream.str();
    }
    
    
    // There is lots of useful information in the cxSite's that could be emitted
    // for the user; e.g. the mols, sites, and site shapes involved.
    std::string
    missingDimerizeInvariantXcpt::
    mkMsg( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxLeft,
           const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxRight )
    {
        std::ostringstream msgStream;
        msgStream << "dimerizeMassExtrap extrapolator encountered "
                  << "site shapes for which no nominal rate has been given.";
        return msgStream.str();
    }
    
    
    // There is lots more useful information in the sites that could be emitted
    // here; e.g. the names of the sites and shapes.
    std::string
    missingDimerizeRateXcpt::
    mkMsg( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxLeft,
           const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& cxRight )
    {
        std::ostringstream msgStream;
        msgStream << "dimerizeNoExtrap extrapolator encountered "
                  << "site shapes for which no nominal rate has been given.";
        return msgStream.str();
    }
    
}
