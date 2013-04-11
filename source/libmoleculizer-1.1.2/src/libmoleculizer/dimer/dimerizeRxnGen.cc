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

#include "mzr/mzrReaction.hh"
#include "fnd/pchem.hh"
#include "mzr/moleculizer.hh"
#include "mol/mzrMol.hh"
#include "cpx/cxSite.hh"
#include "plex/plexUnit.hh"
#include "dimer/dimerizeRxnGen.hh"
#include "dimer/dimerUnit.hh"
#include "mzr/mzrSpeciesDumpable.hh"

namespace dimer
{
    //! Auxiliary class for joining plexes in a dimerization.
    class offsetBinding :
        public std::unary_function<cpx::binding, cpx::binding>
    {
        int offset;
    public:
        offsetBinding( int molOffset ) :
            offset( molOffset )
        {}
        
        cpx::siteSpec
        offsetSite( const cpx::siteSpec& rSpec )
        {
            return cpx::siteSpec( rSpec.molNdx() + offset,
                                  rSpec.siteNdx() );
        }
        
        cpx::binding
        operator()( const cpx::binding& rBinding )
        {
            return cpx::binding( offsetSite( rBinding.leftSite() ),
                                 offsetSite( rBinding.rightSite() ) );
        }
    };
    
    void
    dimerizeRxnGenPair::
    makeBinaryReactions
    ( const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rLeftContext,
      const cpx::cxSite<plx::mzrPlexSpecies, plx::mzrPlexFamily>& rRightContext,
      int generateDepth ) const
    {
        
        // Construct the (only) new reaction and install it in the family
        // of dimerization reactions.
        mzr::mzrReaction* pReaction
            = new mzr::mzrReaction( rMzrUnit.globalVars.begin(),
                                    rMzrUnit.globalVars.end() );
        
        // Record within the reaction that 'this' is its creator.  This is used for the
        // reaction to globally look up paramater information associated with the rxnGen.
        pReaction->setOriginatingRxnGen(( fnd::coreRxnGen* ) this );
        
        pFamily->addEntry( pReaction );
        
        // Add the reactants.
        pReaction->addReactant( rLeftContext.getSpecies(),
                                1 );
        pReaction->addReactant( rRightContext.getSpecies(),
                                1 );
        
        // Extrapolate the rate for the new reaction.
        pReaction->setRate( pExtrap->getRate( rLeftContext,
                                              rRightContext ) );
        
        // Now start cooking up the product species.
        std::vector<cpx::molParam> resultMolParams;
        plx::mzrPlexFamily* pResultFamily;
        
        // We join two distinct virtual molecules together.
        // Begin constructing the "joined" plex by copying the
        // paradigm of the left isomorphism class.
        const plx::mzrPlex& rLeftParadigm
            = rLeftContext.getPlexFamily().getParadigm();
        plx::mzrPlex joined( rLeftParadigm );
        
        // Insert the rightIsoClass paradigm's mols at the end.
        const plx::mzrPlex& rRightParadigm
            = rRightContext.getPlexFamily().getParadigm();
        joined.mols.insert( joined.mols.end(),
                            rRightParadigm.mols.begin(),
                            rRightParadigm.mols.end() );
        
        // Insert the rest of the "joined" plex's sites and
        // bindings, offsetting the mol indices.
        //
        // First, make the function that will do the offsetting.
        int leftMolCount = rLeftParadigm.mols.size();
        offsetBinding offset( leftMolCount );
        
        // Now do the offsetting.
        transform
            ( rRightParadigm.bindings.begin(),
              rRightParadigm.bindings.end(),
              std::back_insert_iterator<std::vector<cpx::binding> > ( joined.bindings ),
              offset );
        
        // Make a binding that binds the sites corresponding to the two
        // original free binding sites.
        cpx::binding
            joint( rLeftContext.getSiteSpec(),
                   offset.offsetSite( rRightContext.getSiteSpec() ) );
        
        // Add the new binding to the joined plex.
        joined.bindings.push_back( joint );
        
        // Intern the joined plex.
        cpx::plexIso joinedToParadigm;
        pResultFamily = rPlexUnit.recognize( joined,
                                             joinedToParadigm );
        
        // Reorder the joinedPlex's parameters using recognition
        // permutation.
        for ( int paradigmNdx = 0;
              paradigmNdx < ( int ) joined.mols.size();
              paradigmNdx++ )
        {
            // Construct the joinedPlex's (mol) paramters by
            // virtually appending the left and right plexes' (mol)
            // parameters.
            //
            // This needs to be cleaned up somehow.
            int joinedNdx = joinedToParadigm.backward.molMap[paradigmNdx];
            if ( joinedNdx < leftMolCount )
            {
                resultMolParams.push_back
                    ( rLeftContext.getMolParams()[joinedNdx] );
            }
            else
            {
                resultMolParams.push_back( rRightContext.getMolParams()
                                           [joinedNdx - leftMolCount] );
            }
        }
        
        // Construct result species.
        plx::mzrPlexSpecies* pResult
            = pResultFamily->getMember( resultMolParams );
        
        // Add it as a product of multiplicity 1.
        pReaction->addProduct( pResult,
                               1 );
        
        // Record all species and reactions that have been created
        // by this reaction generation here
        // **IMPORTANT**
        // ** The code to follow should not be moved to a point prior to
        // completion construction of the complete reacction.
        
        // Record the result species and reactions generated here
        // with moleculizer's catalog of all species and reactions.
        rMzrUnit.rMolzer.recordReaction( pReaction );
        rMzrUnit.rMolzer.recordSpecies( pResult );
        
        
        // Continue reaction generation at one lower depth.
        int notificationDepth = generateDepth - 1;
        if ( 0 <= notificationDepth )
        {
            pResult->ensureNotified( notificationDepth );
        }
        
    }
}
