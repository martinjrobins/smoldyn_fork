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

#ifndef CPX_CXSITEIMPL_H
#define CPX_CXSITEIMPL_H

namespace cpx
{
    template<class plexSpeciesT, class plexFamilyT>
    cxSite<plexSpeciesT, plexFamilyT>::
    cxSite( typename cxSite::plexSpeciesType* pPlexSpecies,
            const siteSpec& rSpec ) :
        fnd::featureContext<typename cxSite::plexSpeciesType, siteSpec> ( pPlexSpecies,
                                                                          rSpec )
    {}
    
    template<class plexSpeciesT, class plexFamilyT>
    siteSpec
    cxSite<plexSpeciesT, plexFamilyT>::
    getSiteSpec( void ) const
    {
        return this->getSpec();
    }
    
    template<class plexSpeciesT, class plexFamilyT>
    int
    cxSite<plexSpeciesT, plexFamilyT>::
    getPop( void ) const
    {
        return this->getSpecies()->getPop();
    }
    
    template<class plexSpeciesT, class plexFamilyT>
    double
    cxSite<plexSpeciesT, plexFamilyT>::
    getPlexWeight( void ) const
    {
        return this->getSpecies()->getWeight();
    }
    
    template<class plexSpeciesT, class plexFamilyT>
    plexFamilyT&
    cxSite<plexSpeciesT, plexFamilyT>::
    getPlexFamily( void ) const
    {
        return this->getSpecies()->rFamily;
    }
    
    template<class plexSpeciesT, class plexFamilyT>
    const siteToShapeMap&
    cxSite<plexSpeciesT, plexFamilyT>::
    getSiteToShapeMap( void ) const
    {
        return this->getSpecies()->siteToShapeMap;
    }
    
    template<class plexSpeciesT, class plexFamilyT>
    const std::vector<molParam>&
    cxSite<plexSpeciesT, plexFamilyT>::
    getMolParams( void ) const
    {
        return this->getSpecies()->molParams;
    }
    
    template<class plexSpeciesT, class plexFamilyT>
    siteParam
    cxSite<plexSpeciesT, plexFamilyT>::
    getSiteParam( void ) const
    {
        siteToShapeMap::const_iterator iSpecParam
            = this->getSpecies()->siteParams.find( getSiteSpec() );
        
        return iSpecParam->second;
    }
}

#endif // CPX_CXSITEIMPL_H
