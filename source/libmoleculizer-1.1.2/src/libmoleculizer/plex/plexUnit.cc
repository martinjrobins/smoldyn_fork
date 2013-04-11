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


#include "plex/plexExceptions.hh"
#include "plex/plexUnit.hh"
#include "plex/dupNodeOmniXcpt.hh"
#include "plex/noOmniForNodeXcpt.hh"
#include <libxml++/libxml++.h>
#include "mzr/mzrSpeciesDumpable.hh"

namespace plx
{
    plexUnit::
    plexUnit( mzr::moleculizer& rMoleculizer,
              mzr::mzrUnit& refMzrUnit,
              bnd::molUnit& refMolUnit,
              nmr::nmrUnit& refNmrUnit ) :
        mzr::unit( "plex",
                   rMoleculizer ),
        rMzrUnit( refMzrUnit ),
        rMolUnit( refMolUnit ),
        rNmrUnit( refNmrUnit ),
        recognize( *this, rNmrUnit )
    {
        // Model elements for which plex unit is responsible.
        inputCap.addModelContentName( eltName::allostericPlexes );
        inputCap.addModelContentName( eltName::allostericOmnis );
        
        // Explicit species for which plex unit is responsible.
        inputCap.addExplicitSpeciesContentName( eltName::plexSpecies );
        
        // Species streams for which plex unit is responsible.
        inputCap.addSpeciesStreamsContentName
            ( eltName::plexSpeciesStream );
        inputCap.addSpeciesStreamsContentName
            ( eltName::omniSpeciesStream );
        
        // Not responsible for any reaction generators.
        // Not responsible for any species.
        
        // The plexUnit may as well register its own omniplex Xpaths.
        const std::string slash( "/" );
        
        std::ostringstream alloOmnisXpath;
        alloOmnisXpath << mzr::eltName::model
                       << slash
                       << eltName::allostericOmnis
                       << slash
                       << eltName::allostericOmni;
        addOmniXpath( alloOmnisXpath.str() );
        
        std::ostringstream omniSpeciesStreamXpath;
        omniSpeciesStreamXpath << mzr::eltName::streams
                               << slash
                               << mzr::eltName::speciesStreams
                               << slash
                               << eltName::omniSpeciesStream;
        addOmniXpath( omniSpeciesStreamXpath.str() );
    }
    
    // Installs a new binding feature for the given pair of structural
    // sites. Any plex that binds the two given structural bindings
    // together will need use the bindingFeature associated to the pair
    // of structural sites to inform the family of decompositions of
    // the structural binding.
    fnd::feature<cpx::cxBinding<mzrPlexSpecies, mzrPlexFamily> >*
    plexUnit::addBindingFeature( bnd::mzrMol* pLeftMol,
                                 int leftMolSiteSpec,
                                 bnd::mzrMol* pRightMol,
                                 int rightMolSiteSpec )
    {
        cpx::structuralSite<bnd::mzrMol> leftSite( pLeftMol,
                                                   leftMolSiteSpec );
        
        cpx::structuralSite<bnd::mzrMol> rightSite( pRightMol,
                                                    rightMolSiteSpec );
        
        cpx::structuralBinding<bnd::mzrMol> sBinding( leftSite,
                                                      rightSite );
        
        fnd::feature<cpx::cxBinding<mzrPlexSpecies, mzrPlexFamily> >
            bindingFtr;
        
        // I note (13Jul05) that I don't take exception to failure of insertion.
        cpx::knownBindings<bnd::mzrMol, fnd::feature<cpx::cxBinding<mzrPlexSpecies, mzrPlexFamily> > >::iterator
            iEntry = bindingFeatures.insert( std::make_pair( sBinding,
                                                             bindingFtr ) ).first;
        
        return & ( iEntry->second );
    }
    
    // I expect this routine to be used in the constructor for plexFamily,
    // to construct the plexFamily's feature map for bindings.
    //
    // This routine looks for the feature "in both directions."
    fnd::feature<cpx::cxBinding<mzrPlexSpecies, mzrPlexFamily> >*
    plexUnit::findBindingFeature( bnd::mzrMol* pLeftMol,
                                  int leftMolSiteSpec,
                                  bnd::mzrMol* pRightMol,
                                  int rightMolSiteSpec )
    {
        cpx::structuralSite<bnd::mzrMol> leftSite( pLeftMol,
                                                   leftMolSiteSpec );
        
        cpx::structuralSite<bnd::mzrMol> rightSite( pRightMol,
                                                    rightMolSiteSpec );
        
        cpx::structuralBinding<bnd::mzrMol> sBinding( leftSite,
                                                      rightSite );
        
        cpx::knownBindings<bnd::mzrMol, fnd::feature<cpx::cxBinding<mzrPlexSpecies, mzrPlexFamily> > >::iterator
            iEntry = bindingFeatures.find( sBinding );
        
        // May have to try the sites in the opposite order.
        if ( iEntry == bindingFeatures.end() )
        {
            cpx::structuralBinding<bnd::mzrMol> oppBinding( rightSite,
                                                            leftSite );
            iEntry = bindingFeatures.find( oppBinding );
        }
        
        return iEntry == bindingFeatures.end()
            ? 0
            : & ( iEntry->second );
    }
    
    void
    plexUnit::
    addOmniPlex( mzrOmniPlex* pOmniPlex,
                 xmlpp::Node* pParentNode )
        throw( utl::xcpt )
    {
        // Many omniplexes may have the same plexFamily, so
        // we should pay no attention if this insertion fails.
        omniPlexFamilies.insert( pOmniPlex->getFamily() );
        
        // If there is already an entry for the node, then something
        // is internally exceptional.
        if ( !( nodeToOmni.insert( std::make_pair( pParentNode,
                                                   pOmniPlex ) ).second ) )
            throw dupNodeOmniXcpt( pParentNode );
    }
    
    mzrOmniPlex*
    plexUnit::
    getOmniForNode( xmlpp::Node* pParentNode ) const
    {
        std::map<xmlpp::Node*, mzrOmniPlex*>::const_iterator
            iEntry
            = nodeToOmni.find( pParentNode );
        
        return ( nodeToOmni.end() == iEntry )
            ? 0
            : iEntry->second;
    }
    
    mzrOmniPlex*
    plexUnit::
    mustGetOmniForNode( xmlpp::Node* pParentNode ) const
        throw( utl::xcpt )
    {
        mzrOmniPlex* pOmni
            = getOmniForNode( pParentNode );
        
        if ( ! pOmni ) throw noOmniForNodeXcpt( pParentNode );
        
        return pOmni;
    }
    
    // For dumping state in XML.
    void
    plexUnit::
    insertStateElts( xmlpp::Element* pUnitStatesElt )
        throw( std::exception )
    {
        pUnitStatesElt->add_child("plex-unit-state");

        //         // Get the model element.
        //         xmlpp::Element* pModelElt
        //         = utl::dom::mustGetUniqueChild (pRootElt,
        //                                         mzr::eltName::model);
        
        //         // Ensure that the tagged-species element is here.
        //         xmlpp::Element* pTaggedSpeciesElt
        //             = utl::dom::mustGetUniqueChild (pModelElt,
        //                                             mzr::eltName::taggedSpecies);
        
        //         // Insert tagged-plex-species nodes.
        //          recognize.insertSpecies (pTaggedSpeciesElt,
        //                                   rMzrUnit.getMolarFactor().getFactor() );
    }
    
    
    mzrPlexSpecies*
    plexUnit::constructNewPlexSpeciesFromComplexOutputState( nmr::ComplexOutputStateCref aCOS ) throw( NonConstructableComplexOutputStateXcpt )
    {
        try
        {
            plx::mzrPlex* pMzrPlex = new plx::mzrPlex;
            
            for ( std::vector<std::string>::const_iterator iter = aCOS.theMolTokens.begin();
                  iter != aCOS.theMolTokens.end();
                  ++iter )
            {
                try
                {
                    bnd::mzrMol* ptrMzrMol = rMolUnit.mustFindMol( *iter );
                    pMzrPlex->mols.push_back( ptrMzrMol );
                }
                catch ( utl::xcpt e )
                {
                    e.wailAndBail();
                }
            }
            
            for ( std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string> > >::const_iterator iter = aCOS.theBindingTokens.begin();
                  iter != aCOS.theBindingTokens.end();
                  ++iter )
            {
                int first_molIndex, first_bndIndex, second_molIndex, second_bndIndex;
                
                utl::from_string( first_molIndex, iter->first.first );
                utl::from_string( first_bndIndex, iter->first.second );
                utl::from_string( second_molIndex, iter->second.first );
                utl::from_string( second_bndIndex, iter->second.second );
                
                cpx::siteSpec firstBinding( first_molIndex, first_bndIndex );
                cpx::siteSpec secondBinding( second_molIndex, second_bndIndex );
                
                pMzrPlex->bindings.push_back( cpx::binding( firstBinding, secondBinding ) );
            }
            
            cpx::plexIso originalToResultIsomorphism;
            plx::mzrPlexFamily* pProductFamily
                = recognize( *pMzrPlex, originalToResultIsomorphism );
            
            // const plx::mzrPlex& refParadigm = pProductFamily->getParadigm();
            
            std::vector<cpx::molParam> theParams = pProductFamily->makeDefaultMolParams();
            
            // For each of the modifications we have here...
            for ( std::vector<std::pair< std::string, std::pair<std::string, std::string> > >::const_iterator iter = aCOS.theModificationTokens.begin();
                  iter != aCOS.theModificationTokens.end();
                  ++iter )
            {
                // Whenever we read a new modification with a new mol index, it means that mol is a
                // mod-mol and we should prepare to
                
                int molIndex;
                utl::from_string( molIndex, iter->first );
                
                // DEBUG TODO this is just a trial of something that SHOULD NOT WORK.
                // int mappedMolIndex = originalToResultIsomorphism.forward.applyToMolSpec( molIndex );
                int mappedMolIndex = molIndex;
                
                // Get the modMol that must be in that spot (it must be a mod mol because it has a modification...)
                bnd::mzrModMol* pMzrMol = dynamic_cast<bnd::mzrModMol*>( pMzrPlex->mols[mappedMolIndex] );
                if ( !pMzrMol ) throw utl::FatalXcpt("Unknown Exception in plexUnit::constructNewPlexSpeciesFromComplexOutputState (plkkkjfdj)");
                
                const cpx::modMolState* pModMolState = dynamic_cast<const cpx::modMolState*>( theParams[originalToResultIsomorphism.forward.applyToMolSpec( molIndex )] );
                pMzrMol->internState( *pModMolState );
                
                cpx::modMolState molState = pMzrMol->externState( theParams[originalToResultIsomorphism.forward.applyToMolSpec( molIndex )] );
                int modificationIndex = ( *pMzrMol ).modSiteNameToNdx[iter->second.first];
                
                // const std::string& modificationSiteName = ( *pMzrMol ).modSiteNames[ modificationIndex ];
                
                molState[modificationIndex] = rMolUnit.mustGetMod( iter->second.second );
                theParams[originalToResultIsomorphism.forward.applyToMolSpec( molIndex )] = pMzrMol->internState( molState );
            }
            
            mzrPlexSpecies* defaultSpecies = pProductFamily->makeMember( pProductFamily->makeDefaultMolParams() );
            mzrPlexSpecies* createdSpecies = pProductFamily->makeMember( theParams );
            
            rMolzer.recordSpecies( defaultSpecies );
            rMolzer.recordSpecies( createdSpecies );
            
            return createdSpecies;
        }
        catch ( std::exception e )
        {
            std::cerr << e.what() << std::endl;
            throw plx::NonConstructableComplexOutputStateXcpt();
        }
        catch ( ... )
        {
            throw plx::NonConstructableComplexOutputStateXcpt();
        }
    }
}

