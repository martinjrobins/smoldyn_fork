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

#ifndef PLEX_MZROMNISTRUCTUREQUERIES_H
#define PLEX_MZROMNISTRUCTUREQUERIES_H

#include "fnd/query.hh"
#include "cpx/omniStructureQuery.hh"
#include "plex/mzrPlex.hh"

namespace plx
{
    // Query on the structure of a plex that putatively matches
    // an omniPlex.  Only one type of structural query exists now:
    // is a site that is free on the omniplex actually free in the
    // complex where the omniplex is found. (omniFreeSiteQuery below).
    typedef
    fnd::query<cpx::omniStructureQueryArg<mzrPlex> >
    mzrOmniStructureQuery;
    
    // The only kind of structural query used by omniplexes at this time.
    typedef
    cpx::omniFreeSiteQuery<mzrPlex>
    mzrOmniFreeSiteQuery;

    typedef
    cpx::omniBoundSiteQuery<mzrPlex>
    mzrOmniBoundSiteQuery;
    
    // Conjunction of structural queries.  Each omniPlex has one of these
    // to "gate" putatively matching plexSpecies.
    typedef
    fnd::andQueries<mzrOmniStructureQuery>
    mzrOmniStructureQueries;
}

#endif // PLEX_MZROMNISTRUCTUREQUERIES_H
