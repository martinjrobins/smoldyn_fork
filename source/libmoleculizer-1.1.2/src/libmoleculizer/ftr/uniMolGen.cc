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
#include "cpx/cxMol.hh"
#include "ftr/uniMolGen.hh"
#include "mzr/mzrSpeciesDumpable.hh"

namespace ftr
{
    class exchangeMods :
        public std::unary_function<molModExchange, void>
    {
        cpx::modMolState& rState;
        
    public:
        exchangeMods( cpx::modMolState& rTargetState ) :
            rState( rTargetState )
        {}
        
        void
        operator()( const molModExchange& rExchange ) const
        {
            rState[rExchange.modSiteNdx] = rExchange.pReplacementMod;
        }
    };
    
    void
    uniMolRxnGen::
    respond( const fnd::featureStimulus<cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily> >& rStimulus )
    {
        const cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rContext
            = rStimulus.getContext();
        
        // Check if the mol's state passes the tests for reaction generation.
        cpx::andMolStateQueries& rMolQueries = *pMolQueries;
        cpx::molParam enablingParam = rContext.getMolParam();
        if ( rMolQueries( enablingParam ) )
        {
            // Construct the reaction and intern it for memory management.
            mzr::mzrReaction* pReaction
                = new mzr::mzrReaction( rMzrUnit.globalVars.begin(),
                                        rMzrUnit.globalVars.end() );
            pFamily->addEntry( pReaction );
            
            // Record within the reaction that 'this' is its creator.  This is used for the
            // reaction to globally look up paramater information associated with the rxnGen.
            pReaction->setOriginatingRxnGen( this );
            
            
            // Add the triggering complex as a reactant of multiplicity 1.
            pReaction->addReactant( rContext.getSpecies(),
                                    1 );
            
            // If an auxiliary reactant was specified, add it with
            // multiplicity 1.
            if ( pAdditionalReactant )
            {
                pReaction->addReactant( pAdditionalReactant,
                                        1 );
            }
            
            // If an auxiliary product was specified, add it with multiplicity
            // 1.
            if ( pAdditionalProduct )
            {
                pReaction->addProduct( pAdditionalProduct,
                                       1 );
            }
            
            // Set the rate of the reaction.
            pReaction->setRate( pExtrapolator->getRate( rContext ) );
            
            // Construct the primary product species.
            //
            // Start by constructing the state of the enabling mol
            // after modification exchange.
            cpx::modMolState enablingState
                = pEnablingMol->externState( enablingParam );
            std::for_each( molModExchanges.begin(),
                           molModExchanges.end(),
                           exchangeMods( enablingState ) );
            
            // Put the post-modification state of the mol into the product
            // species's molParams.
            std::vector<cpx::molParam> productMolParams
                = rContext.getMolParams();
            productMolParams[rContext.getMolSpec()]
                = pEnablingMol->internState( enablingState );
            
            // Determine the primary product species.
            plx::mzrPlexFamily& rPlexFamily
                = rContext.getPlexFamily();
            plx::mzrPlexSpecies* pProductSpecies
                = rPlexFamily.makeMember( productMolParams );
            
            // Add the primary product species to the reaction, with
            // multiplicity 1.
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
}
