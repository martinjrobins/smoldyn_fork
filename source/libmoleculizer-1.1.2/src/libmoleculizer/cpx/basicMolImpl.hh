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

#ifndef CPX_BASICMOLIMPL_H
#define CPX_BASICMOLIMPL_H

namespace cpx
{
    class duplicateSiteNameXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rMolName,
               const std::string& rDuplicateSiteName )
        {
            std::ostringstream msgStream;
            msgStream << "The mol "
                      << rMolName
                      << " already has a binding site named "
                      << rDuplicateSiteName
                      << ".";
            return msgStream.str();
        }
    public:
        duplicateSiteNameXcpt( const std::string& rMolName,
                               const std::string& rDuplicateSiteName ) :
            utl::xcpt( mkMsg( rMolName,
                              rDuplicateSiteName ) )
        {}
    };
    
    template<class bndSiteT>
    basicMol<bndSiteT>::
    basicMol( const typename std::string& rName,
              const typename std::vector<bndSiteT>& rSites )
        throw( typename utl::xcpt ) :
        std::vector<bndSiteT> ( rSites ),
        name( rName ),
        defaultShapes( getDefaultSiteParams() )
    {
        int siteNdx = this->size();
        basicMol& rMe = *this;
        while ( 0 < siteNdx-- )
        {
            const typename std::string& rSiteName = rMe[siteNdx].getName();
            
            typename std::pair<indexIterator, bool> insertResult
                = siteNameToNdx.insert( indexValueType( rSiteName,
                                                        siteNdx ) );
            if ( ! insertResult.second )
                throw duplicateSiteNameXcpt( name,
                                             rSiteName );
        }
    }
    
    template<class bndSiteT>
    basicMol<bndSiteT>::
    basicMol( const basicMol& rOriginal ) :
        std::vector<bndSiteT> ( rOriginal ),
        name( rOriginal.getName() ),
        siteNameToNdx( rOriginal.siteNameToNdx ),
        defaultShapes( getDefaultSiteParams() )
    {}
    
    template<class bndSiteT>
    std::string
    basicMol<bndSiteT>::
    genInstanceName( int molInstanceNdx ) const
    {
        std::ostringstream oss;
        oss << "basic-mol_"
            << molInstanceNdx;
        return oss.str();
    }
    
    template<class bndSiteT>
    bool
    basicMol<bndSiteT>::
    findSite( const typename std::string& rName,
              int& rSiteNdx ) const
    {
        constIndexIterator iEntry
            = siteNameToNdx.find( rName );
        if ( siteNameToNdx.end() == iEntry )
        {
            return false;
        }
        else
        {
            rSiteNdx = iEntry->second;
            return true;
        }
    }
    
    template<class bndSiteT>
    bndSiteT*
    basicMol<bndSiteT>::
    getSite( const typename std::string& rName )
    {
        int siteNdx = -1;
        basicMol& rMe = *this;
        return findSite( rName, siteNdx )
            ? & ( rMe[siteNdx] )
            : 0;
    }
    
    template<class bndSiteT>
    std::vector<siteParam>
    basicMol<bndSiteT>::
    getDefaultSiteParams( void ) const
    {
        std::vector<siteParam> result( this->size() );
        std::transform( this->begin(),
                        this->end(),
                        result.begin(),
                        std::mem_fun_ref( &bndSiteT::getDefaultShape ) );
        return result;
    }
}

#endif // CPX_BASICMOLIMPL_H
