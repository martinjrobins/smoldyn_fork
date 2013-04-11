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

#ifndef OMNISTRUCTUREQUERY_H
#define OMNISTRUCTUREQUERY_H

#include "cpx/plexIso.hh"
#include "cpx/ftrSpec.hh"

namespace cpx
{
    template<class plexT>
    class omniStructureQueryArg
    {
    public:
        const plexT& rPlex;
        const plexIso& rInjection;
        
        omniStructureQueryArg( const plexT& rStructurePlex,
                               const plexIso& rInjectionIso ) :
            rPlex( rStructurePlex ),
            rInjection( rInjectionIso )
        {}
    };
    
    // Examines matching plex to see if specified free sites in the
    // omni are also free sites in the matching plex.
    template<class plexT>
    class omniFreeSiteQuery :
        public fnd::query<omniStructureQueryArg<plexT> >
    {
        siteSpec freeSiteSpec;
        
    public:
        omniFreeSiteQuery( const siteSpec& rFreeSiteSpec ) :
            freeSiteSpec( rFreeSiteSpec )
        {}
        
        ~omniFreeSiteQuery( void )
        {}
        
        bool
        operator()( const omniStructureQueryArg<plexT>& rArg ) const;
    };

    // Examines matching plex to see if specified bound sites in the
    // omni are also bound sites in the matching plex.
    template<class plexT>
    class omniBoundSiteQuery :
        public fnd::query<omniStructureQueryArg<plexT> >
    {
        siteSpec boundSiteSpec;
        
    public:
        omniBoundSiteQuery( const siteSpec& rBoundSiteSpec ) :
            boundSiteSpec( rBoundSiteSpec )
        {}
        
        ~omniBoundSiteQuery( void )
        {}
        
        bool
        operator()( const omniStructureQueryArg<plexT>& rArg ) const;
    };



}

#include "cpx/omniStructureQueryImpl.hh"

#endif // OMNISTRUCTUREQUERY_H
