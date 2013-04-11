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

#ifndef DIMER_DECOMPRXNGEN_H
#define DIMER_DECOMPRXNGEN_H

#include "plex/plexUnit.hh"
#include "dimer/decomposeExtrap.hh"

namespace dimer
{
    /*! \ingroup decompGroup
      \brief Reaction generator decompositions. */
    class decompRxnGen :
        public fnd::rxnGen<cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily> >
    {
        utl::autoVector<mzr::mzrReaction>* pFamily;
        mzr::mzrUnit& rMzrUnit;
        plx::plexUnit& rPlexUnit;
        decomposeExtrapolator* pExtrap;
        
    public:
        // Note that this reaction generator memory manages the rate extrapolator.
        decompRxnGen( utl::autoVector<mzr::mzrReaction>* pDecompFamily,
                      mzr::mzrUnit& refMzrUnit,
                      plx::plexUnit& refPlexUnit,
                      decomposeExtrapolator* pExtrapolator ) :
            pFamily( pDecompFamily ),
            rMzrUnit( refMzrUnit ),
            rPlexUnit( refPlexUnit ),
            pExtrap( pExtrapolator )
        {}
        
        ~decompRxnGen( void )
        {
            delete pExtrap;
        }
        
        void
        respond( const fnd::featureStimulus<cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily> >& rStimulus );
        
    };
}

#endif // DIMER_DECOMPRXNGEN_H
