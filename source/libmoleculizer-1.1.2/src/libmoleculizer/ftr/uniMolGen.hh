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

#ifndef UNIMOLGEN_H
#define UNIMOLGEN_H

#include "cpx/cxMol.hh"
#include "mzr/mzrUnit.hh"
#include "plex/plexUnit.hh"
#include "ftr/uniMolExtrap.hh"

namespace ftr
{
    class molModExchange
    {
    public:
        int modSiteNdx;
        const cpx::modification* pReplacementMod;
        
        molModExchange() :
            modSiteNdx( -1 ),
            pReplacementMod( 0 )
        {}
        
        molModExchange( int modSite,
                        const cpx::modification* pMod ) :
            modSiteNdx( modSite ),
            pReplacementMod( pMod )
        {}
    };
    
    class uniMolRxnGen :
        public fnd::rxnGen<cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily> >
    {
        // To intern reactions for memory management.
        mzr::mzrUnit& rMzrUnit;
        
        // To recognize the new complex.
        plx::plexUnit& rPlexUnit;
        
        // The mod-mol triggering reaction generation.
        bnd::mzrModMol* pEnablingMol;
        
        // Queries on the mol's state that must be passed to enable
        // reaction generation.
        cpx::andMolStateQueries* pMolQueries;
        
        // Exchanges of modifications.
        const std::vector<molModExchange> molModExchanges;
        
        // Additional massive reactant; null if there is no additional reactant.
        mzr::mzrSpecies* pAdditionalReactant;
        
        // Additional product; null if there is no additional product.
        mzr::mzrSpecies* pAdditionalProduct;
        
        // Reaction family to receive generated reactions.
        utl::autoVector<mzr::mzrReaction>* pFamily;
        
        // Rate extrapolator, which sometimes is unary and sometimes is binary.
        // This omniRxnGen is responsible for memory management of its
        // extrapolator.
        const uniMolExtrapolator* pExtrapolator;
        
    public:
        uniMolRxnGen( mzr::mzrUnit& refMzrUnit,
                      plx::plexUnit& refPlexUnit,
                      bnd::mzrModMol* pEnablingModMol,
                      cpx::andMolStateQueries* pAndMolStateQueries,
                      const std::vector<molModExchange>& rModExchanges,
                      mzr::mzrSpecies* pAuxReactant,
                      mzr::mzrSpecies* pAuxProduct,
                      utl::autoVector<mzr::mzrReaction>* pReactionFamily,
                      const uniMolExtrapolator* pUniMolExtrapolator ) :
            rMzrUnit( refMzrUnit ),
            rPlexUnit( refPlexUnit ),
            pEnablingMol( pEnablingModMol ),
            pMolQueries( pAndMolStateQueries ),
            molModExchanges( rModExchanges ),
            pAdditionalReactant( pAuxReactant ),
            pAdditionalProduct( pAuxProduct ),
            pFamily( pReactionFamily ),
            pExtrapolator( pUniMolExtrapolator )
        {}
        
        ~uniMolRxnGen( void )
        {
            delete pExtrapolator;
        }
        
        void
        respond( const fnd::featureStimulus<cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily> >& rStimulus );
    };
}

#endif // UNIMOLGEN_H
