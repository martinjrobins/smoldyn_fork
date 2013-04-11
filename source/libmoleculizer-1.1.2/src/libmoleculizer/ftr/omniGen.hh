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

#ifndef OMNIGEN_H
#define OMNIGEN_H

#include "mol/smallMol.hh"
#include "cpx/cxOmni.hh"
#include "ftr/omniExtrap.hh"

namespace ftr
{
    class smallMolExchange
    {
    public:
        cpx::molSpec exchangedMolSpec;
        bnd::smallMol* pReplacementMol;
        
        smallMolExchange( void ) :
            exchangedMolSpec( -1 ),
            pReplacementMol( 0 )
        {}
        
        smallMolExchange( cpx::molSpec xchngMolSpec,
                          bnd::smallMol* pRplcmntMol ) :
            exchangedMolSpec( xchngMolSpec ),
            pReplacementMol( pRplcmntMol )
        {}
    };
    
    class modificationExchange
    {
    public:
        cpx::molSpec modMolSpec;
        int modSiteNdx;
        const cpx::modification* pReplacementMod;
        
        modificationExchange() :
            modMolSpec( -1 ),
            modSiteNdx( -1 ),
            pReplacementMod( 0 )
        {}
        
        modificationExchange( cpx::molSpec molSpec,
                              int modSite,
                              const cpx::modification* pMod ) :
            modMolSpec( molSpec ),
            modSiteNdx( modSite ),
            pReplacementMod( pMod )
        {}
    };
    
    class omniRxnGen :
        public fnd::rxnGen<cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex> >
    {
        // To intern reactions for memory management.
        mzr::mzrUnit& rMzrUnit;
        
        // To recognize the new complex.
        plx::plexUnit& rPlexUnit;
        
        // Exchanges of small-mol components.
        const std::vector<smallMolExchange> smallMolExchanges;
        
        // Exchanges of modifications.
        const std::vector<modificationExchange> modificationExchanges;
        
        // Additional reactant; null if there is no additional reactant.
        mzr::mzrSpecies* pAdditionalReactant;
        
        // Additional product; null if there is no additional product.
        mzr::mzrSpecies* pAdditionalProduct;
        
        // Reaction family to receive generated reactions.
        utl::autoVector<mzr::mzrReaction>* pFamily;
        
        // Rate extrapolator, which sometimes is unary and sometimes is binary.
        // This omniRxnGen is responsible for memory management of its
        // extrapolator.
        const omniExtrapolator* pExtrapolator;
        
    public:
        omniRxnGen( mzr::mzrUnit& refMzrUnit,
                    plx::plexUnit& refPlexUnit,
                    const std::vector<smallMolExchange>& rSMExchanges,
                    const std::vector<modificationExchange>& rModExchanges,
                    mzr::mzrSpecies* pAuxReactant,
                    mzr::mzrSpecies* pAuxProduct,
                    utl::autoVector<mzr::mzrReaction>* pReactionFamily,
                    const omniExtrapolator* pOmniExtrapolator ) :
            rMzrUnit( refMzrUnit ),
            rPlexUnit( refPlexUnit ),
            smallMolExchanges( rSMExchanges ),
            modificationExchanges( rModExchanges ),
            pAdditionalReactant( pAuxReactant ),
            pAdditionalProduct( pAuxProduct ),
            pFamily( pReactionFamily ),
            pExtrapolator( pOmniExtrapolator )
        {}
        
        ~omniRxnGen( void )
        {
            delete pExtrapolator;
        }
        
        void
        respond( const fnd::featureStimulus<cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex> >& rStimulus );
    };
}

#endif //  OMNIGEN_H
