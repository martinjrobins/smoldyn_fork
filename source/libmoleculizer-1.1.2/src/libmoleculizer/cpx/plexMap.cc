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

#include "cpx/plexMap.hh"

namespace cpx
{
    siteSpec
    plexMap::
    applyToSiteSpec( const siteSpec& rSourceSiteSpec ) const
    {
        siteSpec targetSpec;
        targetSpec.setMolNdx( molMap[rSourceSiteSpec.molNdx()] );
        targetSpec.setSiteNdx( rSourceSiteSpec.siteNdx() );
        return targetSpec;
    }
    
    plexMap
    plexMap::
    makeIdentity( int molCount,
                  int bindingCount )
    {
        plexMap returnValue( molCount,
                             bindingCount );
        while ( 0 < molCount-- )
            returnValue.molMap[molCount] = molCount;
        
        while ( 0 < bindingCount-- )
            returnValue.bindingMap[bindingCount] = bindingCount;
        
        return returnValue;
    }
}
