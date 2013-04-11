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

#include "mzr/mzrUnit.hh"
#include "mol/mzrModMol.hh"
#include "plex/plexUnit.hh"
#include "ftr/omniGen.hh"
#include "ftr/exchangedModXcpt.hh"
#include "mzr/mzrSpeciesDumpable.hh"

namespace ftr
{
    class exchangeSmallMols :
        public std::unary_function<smallMolExchange, void>
    {
        const cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex>& rEnabling;
        
        std::vector<bnd::mzrMol*>& rMols;
        
    public:
        exchangeSmallMols( const cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex>& rEnablingOmni,
                           std::vector<bnd::mzrMol*>& rResultMols ) :
            rEnabling( rEnablingOmni ),
            rMols( rResultMols )
        {}
        
        void
        operator()( const smallMolExchange& rExchange ) const
        {
            // Get the index, in the actual reactant species, of the mol to be
            // exchanged.
            cpx::molSpec exchangedMolSpecTr
                = rEnabling.translateMolSpec( rExchange.exchangedMolSpec );
            
            // Debugging
            if (( exchangedMolSpecTr < 0 )
                || ((( int ) rMols.size() ) <= exchangedMolSpecTr ) )
            {
                std::cerr << "Bad mol spec found!"
                          << std::endl;
            }
            
            // Swap in the new small-mol.
            rMols[exchangedMolSpecTr] = rExchange.pReplacementMol;
        }
    };
    
    class exchangeSmallMolParams :
        public std::unary_function<smallMolExchange, void>
    {
        const cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex>& rEnabling;
        const cpx::plexIso& rResultToResultParadigm;
        std::vector<cpx::molParam>& rProductMolParams;
    public:
        exchangeSmallMolParams( const cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex>& rEnablingOmni,
                                const cpx::plexIso& refResultToResultParadigm,
                                std::vector<cpx::molParam>& refProductMolParams ) :
            rEnabling( rEnablingOmni ),
            rResultToResultParadigm( refResultToResultParadigm ),
            rProductMolParams( refProductMolParams )
        {}
        
        void
        operator()( const smallMolExchange& rExchange ) const
        {
            // Translate the molSpec of the exchanged mol into "result" indexing.
            cpx::molSpec exchangedMolSpecTr
                = rEnabling.translateMolSpec( rExchange.exchangedMolSpec );
            
            // Determine where the default state of the swapped-in
            // small-mol should go in "paradigm" indexing.
            cpx::molSpec paradigmSmallMolSpec
                = rResultToResultParadigm.forward.molMap[exchangedMolSpecTr];
            
            // Install the default state of the swapped in small-mol.
            rProductMolParams[paradigmSmallMolSpec]
                = rExchange.pReplacementMol->getDefaultParam();
        }
    };
    
    class exchangeModifications :
        public std::unary_function<modificationExchange, void>
    {
        const cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex>& rEnabling;
        const cpx::plexIso& rResultToResultParadigm;
        const std::vector<bnd::mzrMol*>& rResultParadigmMols;
        std::vector<cpx::molParam>& rProductMolParams;
    public:
        exchangeModifications( const cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex>& rEnablingOmni,
                               const cpx::plexIso& refResultToResultParadigm,
                               const std::vector<bnd::mzrMol*>& refResultParadigmMols,
                               std::vector<cpx::molParam>& refProductMolParams ) :
            rEnabling( rEnablingOmni ),
            rResultToResultParadigm( refResultToResultParadigm ),
            rResultParadigmMols( refResultParadigmMols ),
            rProductMolParams( refProductMolParams )
        {}
        
        void
        operator()( const modificationExchange& rExchange ) const
        {
            // Get the index, in the actual reactant species, of the mol in which
            // the modification exchange will happen.
            cpx::molSpec modMolSpecTr
                = rEnabling.translateMolSpec( rExchange.modMolSpec );
            
            // Determine what mol the modification will be swapped in.
            cpx::molSpec paradigmModMolSpec
                = rResultToResultParadigm.forward.molMap[modMolSpecTr];
            
            // Get the mol and make sure it's a mod-mol.
            bnd::mzrModMol* pModMol
                = dynamic_cast<bnd::mzrModMol*>( rResultParadigmMols[paradigmModMolSpec] );
            
            if ( ! pModMol )
                throw exchangedModXcpt( rResultParadigmMols[paradigmModMolSpec] );
            
            // Get the current modification state.
            cpx::modMolState exchangedMolState
                = pModMol->externState( rProductMolParams[paradigmModMolSpec] );
            
            // Substitute in the modification.
            exchangedMolState[rExchange.modSiteNdx] = rExchange.pReplacementMod;
            
            // Intern the new modMol state.
            const cpx::molParam nuMolParam
                = pModMol->internState( exchangedMolState );
            
            // Install the molParam into the vector of molParams for the primary
            // product species.
            rProductMolParams[paradigmModMolSpec] = nuMolParam;
        }
    };
    
    void
    omniRxnGen::
    respond( const fnd::featureStimulus<cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex> >& rStimulus )
    {
        const cpx::cxOmni<bnd::mzrMol, plx::mzrPlexSpecies, plx::mzrPlexFamily, plx::mzrOmniPlex>& rContext
            = rStimulus.getContext();
        
        
        // Construct the reaction and intern it for memory management.
        mzr::mzrReaction* pReaction
            = new mzr::mzrReaction( rMzrUnit.globalVars.begin(),
                                    rMzrUnit.globalVars.end() );
        
        // Record within the reaction that 'this' is its creator.  This is used for the
        // reaction to globally look up paramater information associated with the rxnGen.
        pReaction->setOriginatingRxnGen( this );
        
        pFamily->addEntry( pReaction );
        
        // Add the triggering complex as a reactant of multiplicity 1.
        pReaction->addReactant( rContext.getSpecies(),
                                1 );
        
        // If an auxiliary reactant was specified, add it with multiplicity 1.
        if ( pAdditionalReactant )
        {
            pReaction->addReactant( pAdditionalReactant,
                                    1 );
        }
        
        // If an auxiliary product was specified, add it with multiplicity 1.
        if ( pAdditionalProduct )
        {
            pReaction->addProduct( pAdditionalProduct,
                                   1 );
        }
        
        // Set the rate of the reaction.
        pReaction->setRate( pExtrapolator->getRate( rContext ) );
        
        // Now construct the primary product species, the modified complex.
        // This occurs in two steps, making the structural complex and making
        // the parameters for the mols in the complex.
        
        // First, we have to construct its vector of mols, which is obtained
        // from the primary reactant's vector of mols by performing the
        // small-mol exchanges, if any.
        plx::mzrPlex resultPlex( rContext.getPlexFamily().getParadigm() );
        
        std::for_each( smallMolExchanges.begin(),
                       smallMolExchanges.end(),
                       exchangeSmallMols( rContext,
                                          resultPlex.mols ) );
        
        // Recognize the product complex, retaining the recognition map so
        // that we can reorder the states of the mols appropriately.
        cpx::plexIso resultToResultParadigm;
        plx::mzrPlexFamily* pProductFamily
            = rPlexUnit.recognize( resultPlex,
                                   resultToResultParadigm );
        
        // Now make the parameters for the mols in the product complex.
        
        // We start with a copy of the triggering complex's mol parameters.
        const std::vector<cpx::molParam>& rEnablingMolParams
            = rContext.getMolParams();
        int paradigmMolNdx = rEnablingMolParams.size();
        std::vector<cpx::molParam> resultMolParams( paradigmMolNdx );
        while ( 0 < paradigmMolNdx-- )
        {
            // Get the index of the mol in the triggering reactant complex.
            int molNdx
                = resultToResultParadigm.backward.molMap[paradigmMolNdx];
            
            resultMolParams[paradigmMolNdx] = rEnablingMolParams[molNdx];
        }
        
        // Substitute the default parameters for all small-mols that were
        // substituted in.
        std::for_each( smallMolExchanges.begin(),
                       smallMolExchanges.end(),
                       exchangeSmallMolParams( rContext,
                                               resultToResultParadigm,
                                               resultMolParams ) );
        
        // Perform the modification exchanges.
        std::for_each( modificationExchanges.begin(),
                       modificationExchanges.end(),
                       exchangeModifications( rContext,
                                              resultToResultParadigm,
                                              pProductFamily->getParadigm().mols,
                                              resultMolParams ) );
        
        // Construct the primary product species.
        plx::mzrPlexSpecies* pProductSpecies
            = pProductFamily->getMember( resultMolParams );
        
        // Add the primary product species to the reaction, with multiplicity
        // 1.
        pReaction->addProduct( pProductSpecies,
                               1 );
        
        
        // Record all species and reactions that have been created
        // by this reaction generation here
        // **IMPORTANT**
        // ** The code to follow should not be moved to a point prior to
        // completion construction of the complete reacction.
        
        // Record the result species and reactions generated here
        // with moleculizer's catalog of all species and reactions.
        rMzrUnit.rMolzer.recordReaction( pReaction );
        rMzrUnit.rMolzer.recordSpecies( pProductSpecies );
        
        // Continue reaction generation at one depth lower.
        int generateDepth
            = rStimulus.getNotificationDepth() - 1;
        if ( 0 <= generateDepth )
        {
            pProductSpecies->ensureNotified( generateDepth );
        }
    }
}
 
