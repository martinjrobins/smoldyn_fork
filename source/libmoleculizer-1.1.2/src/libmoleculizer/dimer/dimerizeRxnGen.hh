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

#ifndef DIMER_DIMERIZERXNGEN_H
#define DIMER_DIMERIZERXNGEN_H

#include "fnd/binaryRxnGen.hh"
#include "mol/siteFeature.hh"
#include "plex/plexUnit.hh"
#include "dimer/dimerizeExtrap.hh"

namespace dimer
{
    class dimerUnit;
    
    class dimerizeRxnGenPair :
        public fnd::binaryRxnGenPair<bnd::siteFeature, bnd::siteFeature>
    {
        utl::autoVector<mzr::mzrReaction>* pFamily;
        mzr::mzrUnit& rMzrUnit;
        plx::plexUnit& rPlexUnit;
        dimerizeExtrapolator* pExtrap;
        
    public:
        
        // Note that this reaction generator memory manages the rate extrapolator.
        dimerizeRxnGenPair( bnd::siteFeature& rLeftSiteFeature,
                            bnd::siteFeature& rRightSiteFeature,
                            utl::autoVector<mzr::mzrReaction>* pDimerizeFamily,
                            mzr::mzrUnit& refMzrUnit,
                            plx::plexUnit& refPlexUnit,
                            dimerizeExtrapolator* pExtrapolator ) :
            fnd::binaryRxnGenPair<bnd::siteFeature, bnd::siteFeature> ( rLeftSiteFeature,
                                                                        rRightSiteFeature ),
            pFamily( pDimerizeFamily ),
            rMzrUnit( refMzrUnit ),
            rPlexUnit( refPlexUnit ),
            pExtrap( pExtrapolator )
        {}
        
        ~dimerizeRxnGenPair( void )
        {
            delete pExtrap;
        }
        
        void
        makeBinaryReactions
        ( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rLeftContext,
          const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rRightContext,
          int generateDepth ) const;
    };
}

#endif // DIMER_DIMERIZERXNGEN_H
