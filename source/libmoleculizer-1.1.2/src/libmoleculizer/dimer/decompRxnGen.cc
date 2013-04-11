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

#include "mzr/moleculizer.hh"
#include "mzr/mzrReaction.hh"
#include "cpx/cxBinding.hh"
#include "mol/mzrBndSite.hh"
#include "plex/plexUnit.hh"
#include "dimer/decompRxnGen.hh"
#include "dimer/dimerUnit.hh"
#include "mzr/mzrSpeciesDumpable.hh"

namespace dimer
{
    void
    decompRxnGen::
    respond( const fnd::featureStimulus<cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily> >& rStimulus )
    {
        const cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rNewContext
            = rStimulus.getContext();
        
        // Create the new reaction, and install it in the family of
        // decomposition reactions for memory management.
        mzr::mzrReaction* pReaction
            = new mzr::mzrReaction( rMzrUnit.globalVars.begin(),
                                    rMzrUnit.globalVars.end() );
        
        // Record within the reaction that 'this' is its creator.  This is used for the
        // reaction to globally look up paramater information associated with the rxnGen.
        pReaction->setOriginatingRxnGen( this );
        
        pFamily->addEntry( pReaction );
        
        // Add the substrate and sensitize to it.
        pReaction->addReactant( rNewContext.getSpecies(),
                                1 );
        
        // Extrapolate the rate of the reaction.
        pReaction->setRate( pExtrap->getRate( rNewContext ) );
        
        // Ingredients for the mandatory result species.
        std::vector<cpx::molParam> mndtryResultParams;
        plx::mzrPlexFamily* pMndtryResultFamily;
        
        const plx::mzrPlex& rWholePlex
            = rNewContext.getPlexFamily().getParadigm();
        
        // The index of the binding that is decomposing.
        cpx::bindingSpec breakingBindingNdx
            = rNewContext.getBindingSpec();
        
        // Make a copy of the chosen plexSpecies's paradigm, but omitting
        // the binding that is to be broken.  At the same time, we'll get
        // the molNdx's of the ends of the binding that is to be broken.
        plx::mzrPlex brokenPlex;
        brokenPlex.mols = rWholePlex.mols;
        
        // Need the two mols on the ends of the specified binding as
        // "seeds" for computing the (1 or 2) connected components of the
        // brokenPlex.
        int leftMolNdx = -1;
        int rightMolNdx = -1;
        // Copy over all except the specified binding.
        for ( int bindingNdx = 0;
              bindingNdx < ( int ) rWholePlex.bindings.size();
              bindingNdx++ )
        {
            const cpx::binding& rBinding
                = rWholePlex.bindings[bindingNdx];
            
            if ( bindingNdx == breakingBindingNdx )
            {
                leftMolNdx = rBinding.leftSite().molNdx();
                rightMolNdx = rBinding.rightSite().molNdx();
            }
            else
            {
                brokenPlex.bindings.push_back( rBinding );
            }
        }
        
        // Get the connected component of the left (mandatory) half of the
        // broken plex.  Note that this way of finding connected components
        // is quite general; actually more than we need, and most of the
        // work (reversing the edge->vertex map) gets repeated here.
        plx::mzrPlex leftComponent;
        cpx::plexIso leftIso( brokenPlex.mols.size(),
                              brokenPlex.bindings.size() );
        brokenPlex.makeTrackedComponent( leftMolNdx,
                                         leftComponent,
                                         leftIso );
        
        // Find the species of the left component, along with an isomorphism
        // to the species's paradigm.
        cpx::plexIso toLeftParadigm;
        pMndtryResultFamily = rPlexUnit.recognize( leftComponent,
                                                   toLeftParadigm );
        
        // Construct the parameters for the left component.
        //
        // First, construct the vector of molParams by permuting the
        // molParams from the original plex.
        for ( int leftParaMolNdx = 0;
              leftParaMolNdx < ( int ) leftComponent.mols.size();
              leftParaMolNdx++ )
        {
            // Bring along the old mol params, using the two tracking maps.
            int molNdx = toLeftParadigm.backward.molMap[leftParaMolNdx];
            int preImgNdx = leftIso.backward.molMap[molNdx];
            mndtryResultParams.push_back
                ( rNewContext.getMolParams()[preImgNdx] );
        }
        
        // Construct the mandatory result species.
        plx::mzrPlexSpecies* pMndtryResult
            = pMndtryResultFamily->getMember( mndtryResultParams );
        
        // Add it as a product of multiplicity one.
        pReaction->addProduct( pMndtryResult,
                               1 );
        
        // Continue reaction generation at one depth lower.
        int notificationDepth
            = rStimulus.getNotificationDepth() - 1;
        if ( 0 <= notificationDepth )
        {
            pMndtryResult->ensureNotified( notificationDepth );
        }
        
        // Now start looking at the other result component, if any.
        bool resultIsConnected
            = ( leftComponent.mols.size() == brokenPlex.mols.size() );
        if ( ! resultIsConnected )
        {
            // Ingredients for the optional result species, which only is
            // needed if the molecule decomposes into two parts.
            std::vector<cpx::molParam> optResultParams;
            plx::mzrPlexFamily* pOptResultFamily;
            
            // Extract the other connected component.  Note that we really
            // might want to do both of these at the same time.
            plx::mzrPlex rightComponent;
            cpx::plexIso rightIso( brokenPlex.mols.size(),
                                   brokenPlex.bindings.size() );
            brokenPlex.makeTrackedComponent( rightMolNdx,
                                             rightComponent,
                                             rightIso );
            
            // Intern the right connected component.
            cpx::plexIso toRightParadigm;
            pOptResultFamily = rPlexUnit.recognize( rightComponent,
                                                    toRightParadigm );
            
            // Construct the parameters for the right component.
            //
            // First, construct the vector of molParams by permuting the
            // molParams from the original plex.
            for ( int rightParaMolNdx = 0;
                  rightParaMolNdx < ( int ) rightComponent.mols.size();
                  rightParaMolNdx++ )
            {
                // Bring along the old mol params, using the two tracking maps.
                int molNdx = toRightParadigm.backward.molMap[rightParaMolNdx];
                int preImgNdx = rightIso.backward.molMap[molNdx];
                
                optResultParams.push_back
                    ( rNewContext.getMolParams()[preImgNdx] );
            }
            
            // Construct the optional result species.
            plx::mzrPlexSpecies* pOptResult
                = pOptResultFamily->getMember( optResultParams );
            
            // Add it as a product of multiplicity 1.
            pReaction->addProduct( pOptResult,
                                   1 );
            
            // Record all species and reactions that have been created
            // by this reaction generation here
            // A decomposition reaction produces on rxn and two species.
            // **IMPORTANT**
            // ** The code to follow should not be moved to a point prior to
            // completion construction of the complete reacction.
            
            // Record the result species and reactions generated here
            // with moleculizer's catalog of all species and reactions.
            rMzrUnit.rMolzer.recordSpecies( pMndtryResult );
            rMzrUnit.rMolzer.recordSpecies( pOptResult );
            rMzrUnit.rMolzer.recordReaction( pReaction );
            
            // Continue reaction generation at one depth lower.
            if ( 0 <= notificationDepth )
            {
                pOptResult->ensureNotified( notificationDepth );
            }
            
        }
    }
}
