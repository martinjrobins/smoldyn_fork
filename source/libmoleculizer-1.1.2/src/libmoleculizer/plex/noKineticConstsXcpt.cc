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

#include <sstream>
#include "plex/noKineticConstsXcpt.hh"

namespace plx
{
    std::string
    noKineticConstsXcpt::
    mkShapesOnlyMsg( const std::string& rLeftSiteShapeName,
                     const std::string& rRightSiteShapeName )
    {
        std::ostringstream msgStream;
        msgStream << "Attempt to form binding of site shapes "
                  << rLeftSiteShapeName
                  << " and "
                  << rRightSiteShapeName
                  << " for which no on/off rates have been given.";
        return msgStream.str();
    }
    
    std::string
    noKineticConstsXcpt::
    mkMolsAndSitesMsg( const std::string& rLeftMolName,
                       const std::string& rLeftSiteName,
                       const std::string& rRightMolName,
                       const std::string& rRightSiteName )
    {
        std::ostringstream msgStream;
        msgStream << "Plex contains binding between site "
                  << rLeftSiteName
                  << " on mol "
                  << rLeftMolName
                  << " and site "
                  << rRightSiteName
                  << " on mol "
                  << rRightMolName
                  << " for which no on/off rates have been given.";
        return msgStream.str();
    }
    
    // Thrown in dimerizeRxnGen.cc, makeBinaryReactions.
    std::string
    noKineticConstsXcpt::
    mkFullMsg( const std::string& rLeftMolName,
               const std::string& rLeftSiteName,
               const std::string& rLeftSiteShapeName,
               const std::string& rRightMolName,
               const std::string& rRightSiteName,
               const std::string& rRightSiteShapeName )
    {
        std::ostringstream msgStream;
        msgStream << "Attempt to generate binding reaction between site "
                  << rLeftSiteName
                  << " on mol "
                  << rLeftMolName
                  << " (in shape "
                  << rLeftSiteShapeName
                  << ") with site "
                  << rRightSiteName
                  << " on mol "
                  << rRightMolName
                  << " (in shape "
                  << rRightSiteShapeName
                  << ") for which no on/off rates have been given.";
        return msgStream.str();
    }
}
