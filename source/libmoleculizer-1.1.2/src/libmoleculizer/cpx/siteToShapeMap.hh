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

#ifndef CPX_SITETOSHAPEMAP_H
#define CPX_SITETOSHAPEMAP_H

#include <map>
#include "utl/xcpt.hh"
#include "cpx/ftrSpec.hh"
#include "cpx/siteShape.hh"
#include "cpx/plexIso.hh"

namespace cpx
{
    template<class omniPlexT> class subPlexSpec;
    
    // This is used to describe the shapes of the binding sites in a particular
    // plexSpecies.  It is also used to record the allosteric variations in site
    // shapes connected with an omniplex or with a particular plex structure
    // (see plexFamily).
    //
    // The "setSiteShape(s)" methods support Moleculizer's (unwholesome and
    // really completely unsatisfactory) way of bringing about a number of
    // allosteric changes by simply sequentially overlaying their effects on
    // binding site shapes.
    class siteToShapeMap :
        public std::map<siteSpec, const siteShape*>
    {
    public:
        
        // Returns null if the siteSpec is unmapped.
        const siteShape*
        getSiteShape( const siteSpec& rPlexSiteSpec ) const;
        
        // Throws an exception if the siteSpec is unmapped.
        const siteShape*
        mustGetSiteShape( const siteSpec& rPlexSiteSpec ) const
            throw( utl::xcpt );
        
        // Forces insertion of the one given site-shape pair.
        void
        setSiteShape( const siteSpec& rPlexSiteSpec,
                      const siteShape* pSiteShape );
        
        // Same as above, but does the insertion through the
        // given subPlexSpec.
        template<class omniPlexT>
        void
        setSiteShape( const siteSpec& rPlexSiteSpec,
                      const siteShape* pSiteShape,
                      const subPlexSpec<omniPlexT>& rSubPlexSpec );
        
        // Forces insertion of all the site-to-shape entries from the
        // given rSiteToShapeMap.
        void
        setSiteShapes( const siteToShapeMap& rSiteToShapeMap );
        
        // Forces insertion of all the site-to-shape entries from the given
        // rSiteToShapeMap, but pushes all of the site specifications
        // through the given injection.
        template<class omniPlexT>
        void
        setSiteShapes( const siteToShapeMap& rSiteToShapeMap,
                       const subPlexSpec<omniPlexT>& rSubPlexSpec );
    };
}

#include "cpx/siteToShapeMapImpl.hh"

#endif // CPX_SITETOSHAPEMAP_H
