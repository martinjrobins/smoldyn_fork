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

#ifndef CPX_OMNISTRUCTUREQUERYIMPL_H
#define CPX_OMNISTRUCTUREQUERYIMPL_H

#include "cpx/binding.hh"

namespace cpx
{
    class bindingContainsSite :
        public std::unary_function<binding, bool>
    {
        // The site being searched for among the bindings
        // AFTER it has been translated by the omni imbedding.
        const siteSpec& rQuerySiteSpec;
        
    public:
        bindingContainsSite( const siteSpec& rSiteSpec ) :
            rQuerySiteSpec( rSiteSpec )
        {}
        
        bool
        operator()( const binding& rBinding ) const
        {
            return ( rBinding.leftSite() == rQuerySiteSpec )
                || ( rBinding.rightSite() == rQuerySiteSpec );
        }
    };
    
    template<class plexT>
    bool
    omniFreeSiteQuery<plexT>::
    operator()( const omniStructureQueryArg<plexT>& rArg ) const
    {
        const plexT& rPlex = rArg.rPlex;
        const plexIso& rInjection = rArg.rInjection;
        
        // Translate the spec of the site we want to check into the indexing of
        // the given plex using the injection of the omniplex into the given plex
        // where it has been recognized.
        siteSpec translatedSiteSpec
            = rInjection.forward.applyToSiteSpec( freeSiteSpec );
        
        // Return true if no binding in the plex contains the translated site
        // spec; i.e. if the site specified by freeSiteSpec in the omni
        // is also free in the plex where the omni is embedded.
        return
            ( rPlex.bindings.end()
              == std::find_if( rPlex.bindings.begin(),
                               rPlex.bindings.end(),
                               bindingContainsSite( translatedSiteSpec ) ) );
        
    }


    template<class plexT>
    bool
    omniBoundSiteQuery<plexT>::
    operator()( const omniStructureQueryArg<plexT>& rArg ) const
    {
        const plexT& rPlex = rArg.rPlex;
        const plexIso& rInjection = rArg.rInjection;
        
        // Translate the spec of the site we want to check into the indexing of
        // the given plex using the injection of the omniplex into the given plex
        // where it has been recognized.
        siteSpec translatedSiteSpec
            = rInjection.forward.applyToSiteSpec( boundSiteSpec );
        
        // NJA: I don't understand this code exactly, and possibly
        // could be sped up.  This is a copy and paste job from the
        // code to omniFreeSiteQuery, changed so that if unbound
        // returns true, bound returns false and vice versa.
        return
            ( rPlex.bindings.end()
              != std::find_if( rPlex.bindings.begin(),
                               rPlex.bindings.end(),
                               bindingContainsSite( translatedSiteSpec ) ) );
        
    }





}

#endif // CPX_OMNISTRUCTUREQUERYIMPL_H
