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

#include "mol/mzrMol.hh"
#include "mol/unkSiteXcpt.hh"
#include "plex/mzrPlexFamily.hh"

namespace bnd
{
    mzrMol::
    mzrMol( const std::string& rName,
            const std::vector<mzrBndSite>& rSites ) :
        cpx::basicMol<bnd::mzrBndSite> ( rName,
                                         rSites )
    {}
    
    int
    mzrMol::
    mustFindSite( const std::string& rSiteName,
                  xmlpp::Node* pRequestingNode ) const
        throw( utl::xcpt )
    {
        int siteNdx = -1;
        
        if ( ! findSite( rSiteName,
                         siteNdx ) )
            throw( unkSiteXcpt( rSiteName,
                                pRequestingNode ) );
        
        return siteNdx;
    }
    
    mzrBndSite*
    mzrMol::
    mustGetSite( const std::string& rSiteName,
                 xmlpp::Node* pRequestingNode )
        throw( utl::xcpt )
    {
        mzrBndSite* pSite = getSite( rSiteName );
        
        if ( ! pSite )
            throw( unkSiteXcpt( rSiteName,
                                pRequestingNode ) );
        
        return pSite;
    }
    
    std::string
    mzrMol::
    genInstanceName( int molInstanceNdx ) const
    {
        std::ostringstream oss;
        oss << "mzr-mol_"
            << molInstanceNdx;
        return oss.str();
    }
}
