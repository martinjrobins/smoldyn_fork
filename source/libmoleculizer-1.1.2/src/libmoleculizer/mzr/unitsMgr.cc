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

#include "mzr/unitsMgr.hh"

#include "mzr/mzrUnit.hh"
#include "mol/molUnit.hh"
#include "dimer/dimerUnit.hh"
#include "plex/plexUnit.hh"
#include "stoch/stochUnit.hh"
#include "ftr/ftrUnit.hh"
#include "nmr/nmrUnit.hh"
#include "mzr/mzrSpeciesDumpable.hh"
#include <libxml++/libxml++.h>

namespace mzr
{
    unitsMgr::
    unitsMgr( moleculizer& rMoleculizer ) :
        pNmrUnit( new nmr::nmrUnit( rMoleculizer ) ),
        pMzrUnit( new mzr::mzrUnit( rMoleculizer ) ),
        pMolUnit( new bnd::molUnit( rMoleculizer ) ),
        pPlexUnit( new plx::plexUnit( rMoleculizer,
                                      *pMzrUnit,
                                      *pMolUnit,
                                      *pNmrUnit ) ),
        pStochUnit( new stoch::stochUnit( rMoleculizer,
                                          *pMzrUnit ) ),
        pDimerUnit( new dimer::dimerUnit( rMoleculizer,
                                          *pMzrUnit,
                                          *pMolUnit,
                                          *pPlexUnit ) ),
        pFtrUnit( new ftr::ftrUnit( rMoleculizer,
                                    *pMzrUnit,
                                    *pMolUnit,
                                    *pPlexUnit ) )
    {
        
        pNmrUnit->setMzrUnit( pMzrUnit );
        pNmrUnit->setMolUnit( pMolUnit );
        pNmrUnit->setPlexUnit( pPlexUnit );
        
        // Note that these need to be in output order, which is slightly
        // different from linkage order: mzrUnit "plays cleanup" by parsing
        // after other units are done.
        
        // dimerUnit has to come before plexUnit, since the plex parser checks
        // for impossible bindings.
        //
        // ftrUnit has to come after plexUnit, since it uses omniPlexes in its
        // reaction generators.
        addEntry( pNmrUnit );
        addEntry( pStochUnit );
        addEntry( pMolUnit );
        addEntry( pDimerUnit );
        addEntry( pPlexUnit );
        addEntry( pFtrUnit );
        addEntry( pMzrUnit );
        
        
    }
    
    // Class for constructing overall input capabilities of moleculizer
    // from input capabilities of each unit.
    class addCapToUnion :
        public std::unary_function<const unit*, void>
    {
        inputCapabilities& rUnionCaps;
    public:
        addCapToUnion( inputCapabilities& rUnionCapabilities ) :
            rUnionCaps( rUnionCapabilities )
        {}
        
        void
        operator()( const unit* pUnit ) const
        {
            rUnionCaps.addCap( pUnit->inputCap );
        }
    };
    
    void
    unitsMgr::
    unionInputCaps( inputCapabilities& rUnion )
    {
        for_each( begin(),
                  end(),
                  addCapToUnion( rUnion ) );
    }
}
