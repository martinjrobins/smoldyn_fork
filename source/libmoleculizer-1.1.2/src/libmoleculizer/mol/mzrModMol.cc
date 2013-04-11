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

#include "mol/mzrModMol.hh"
#include "mol/unkModSiteXcpt.hh"
#include "mol/molEltName.hh"
#include "plex/mzrPlexFamily.hh"
#include "plex/plexEltName.hh"
#include <libxml++/libxml++.h>

namespace bnd
{
    mzrModMol::
    mzrModMol( const std::string& rName,
               const std::vector<mzrBndSite>& rSites,
               double molecularWeight,
               const std::map<std::string, const cpx::modification*>& rDefaultModMap ) :
        cpx::modMol<mzrMol> ( mzrMol( rName,
                                      rSites ),
                              molecularWeight,
                              rDefaultModMap )
    {}
    
    int
    mzrModMol::
    mustGetModSiteNdx( const std::string& rModSiteName,
                       xmlpp::Node* pRequestingNode ) const
        throw( utl::xcpt )
    {
        int ndx = -1;
        
        if ( ! getModSiteNdx( rModSiteName,
                              ndx ) )
        {
            throw unkModSiteXcpt( pRequestingNode,
                                  rModSiteName,
                                  this );
        }
        return ndx;
    }
    
    std::string
    mzrModMol::
    genInstanceName( int molInstanceNdx ) const
    {
        std::ostringstream oss;
        oss << "mod-mol_"
            << molInstanceNdx;
        return oss.str();
    }
    
    std::string
    mzrModMol::
    getInformativeModificationName() const
    {
        
        // TODO non-critical.  Either write or remove.
        std::string name;
        //       for(unsigned int i = 0; i != modSiteNames.size(); ++i)
        //       {
        //           name += modSiteNames[i] +
        //       }
        
        return name;
    }
    
    xmlpp::Element*
    mzrModMol::
    insertInstanceState( xmlpp::Element* pInstanceStatesElt,
                         int molInstanceNdx,
                         cpx::molParam param ) const
    {
        // Insert mod-mol-ref/mod-map elements as the description of the
        // modMolState pointed to by param.
        //
        // To save space and be consistent with the input document format, we'd
        // like to omit modification sites that have their default modification
        // (ususally something like "none").
        
        xmlpp::Element* pModMolInstanceRefElt
            = pInstanceStatesElt->add_child( plx::eltName::modMolInstanceRef );
        
        pModMolInstanceRefElt
            ->set_attribute( plx::eltName::modMolInstanceRef_nameAttr,
                             genInstanceName( molInstanceNdx ) );
        
        // Insert the modification-map element.
        xmlpp::Element* pModMapElt = pModMolInstanceRefElt
            ->add_child( eltName::modMap );
        
        // Get the state pointed to by param.
        const cpx::modMolState& rState = externState( param );
        
        // Get the default state for comparison with the specified state.
        const cpx::modMolState& rDefaultState = * ( getDefaultState() );
        
        // Insert a mod-site-ref/mod-ref pair for each modification site that
        // is not in its default state.
        //
        // This seems unnecessarily complicated, involving indexes of modifications,
        // or alternatively, marching in parallel through the two vectors of
        // modification pointers and the vector of modification site names;
        // like a ternary version of std::for_each.
        for ( int modSiteNdx = 0;
              modSiteNdx < modSiteCount();
              ++modSiteNdx )
        {
            // Get pointers to the (interned) actual and default modifications
            // of modification site at modNdx.
            const cpx::modification* pActualModification = rState[modSiteNdx];
            const cpx::modification* pDefaultModification = rDefaultState[modSiteNdx];
            
            if ( pActualModification != pDefaultModification )
            {
                // Insert mod-site-ref element.
                xmlpp::Element* pModSiteRefElt
                    = pModMapElt->add_child( eltName::modSiteRef );
                
                // Add the modification site name attribute.
                pModSiteRefElt->set_attribute( eltName::modSiteRef_nameAttr,
                                               modSiteNames[modSiteNdx] );
                
                // Add the mod-ref element, giving the name of the modification.
                xmlpp::Element* pModRefElt
                    = pModSiteRefElt->add_child( eltName::modRef );
                
                pModRefElt->set_attribute( eltName::modRef_nameAttr,
                                           pActualModification->getName() );
            }
        }
        return pModMolInstanceRefElt;
    }
    
    // Class for inserting a binding site on a mzrModMol.
    class insertSite :
        public std::unary_function<mzrBndSite, void>
    {
        xmlpp::Element* pMolElt;
    public:
        insertSite( xmlpp::Element* pMolElement ) :
            pMolElt( pMolElement )
        {}
        
        void
        operator()( const mzrBndSite& rSite ) const throw( std::exception )
        {
            rSite.insertElt( pMolElt );
        }
    };
    
    // Class for inserting the non-default
    class insertNonDefault : public
    std::unary_function<std::pair<const cpx::modMolState, std::vector<cpx::siteParam> >,
                        void>
    {
        xmlpp::Element* pModMolElt;
        const mzrModMol* pMol;
        const cpx::modMolState* pDfltState;
        const std::vector<cpx::siteParam>& rDefaultShapes;
    public:
        insertNonDefault( xmlpp::Element* pModMolElement,
                          const mzrModMol* pModMol,
                          const cpx::modMolState* pDefaultState,
                          const std::vector<cpx::siteParam>& rDefaultSiteParams ) :
            pModMolElt( pModMolElement ),
            pMol( pModMol ),
            pDfltState( pDefaultState ),
            rDefaultShapes( rDefaultSiteParams )
        {}
        
        void
        operator()( const argument_type& rAlloMapEntry ) const throw( std::exception )
        {
            const std::vector<cpx::siteParam>& rEntryShapes
                = rAlloMapEntry.second;
            
            // We don't want to list states that are not truly allosteric
            // in the sense of having some non-default site shape.
            if ( rEntryShapes != rDefaultShapes )
            {
                // Insert the allosteric-state element.
                //
                // Is this element necessary or useful?  It is a wrapper for the
                // different kinds of state descriptions that different kinds of mols
                // will have.
                xmlpp::Element* pAllostericStateElt
                    = pModMolElt->add_child( eltName::allostericState );
                
                // Insert the mod-map element, whose content describes the
                // modification state.
                xmlpp::Element* pModMapElt
                    = pAllostericStateElt->add_child( eltName::modMap );
                
                // For each modification site that is not in its default modification
                // state, emit a modSiteRef/modRef pair, giving the actual
                // modification.
                for ( int modSiteNdx = 0;
                      modSiteNdx < pMol->modSiteCount();
                      ++modSiteNdx )
                {
                    const cpx::modMolState* pEntryState = & ( rAlloMapEntry.first );
                    
                    const cpx::modification* pActualMod = ( *pEntryState )[modSiteNdx];
                    const cpx::modification* pDefaultMod = ( *pDfltState )[modSiteNdx];
                    
                    // Is the modification not the default modification for this site?
                    if ( pActualMod != pDefaultMod )
                    {
                        xmlpp::Element* pModSiteRefElt
                            = pModMapElt->add_child( eltName::modSiteRef );
                        
                        pModSiteRefElt
                            ->set_attribute( eltName::modSiteRef_nameAttr,
                                             pMol->modSiteNames[modSiteNdx] );
                        
                        xmlpp::Element* pModRefElt
                            = pModSiteRefElt->add_child( eltName::modRef );
                        
                        pModRefElt->set_attribute( eltName::modRef_nameAttr,
                                                   pActualMod->getName() );
                    }
                }
                
                // Insert the site-shape-map element, which gives the
                // (non-default)shapes of the binding sites when in this allosteric
                // state.
                xmlpp::Element* pSiteShapeMapElt
                    = pAllostericStateElt->add_child( eltName::siteShapeMap );
                
                // For each binding site that is not in its default shape,
                // emit a bindingSiteRef/siteShapeRef pair, giving the actual
                // shape of the binding site.
                for ( int siteNdx = 0;
                      siteNdx < pMol->getSiteCount();
                      ++siteNdx )
                {
                    
                    const cpx::siteShape* pDefaultSiteShape = rDefaultShapes[siteNdx];
                    const cpx::siteShape* pActualSiteShape = rEntryShapes[siteNdx];
                    
                    if ( pDefaultSiteShape != pActualSiteShape )
                    {
                        xmlpp::Element* pBindingSiteRefElt
                            = pSiteShapeMapElt->add_child( eltName::bindingSiteRef );
                        
                        const mzrModMol& rMol = *pMol;
                        pBindingSiteRefElt
                            ->set_attribute( eltName::bindingSiteRef_nameAttr,
                                             rMol[siteNdx].getName() );
                        
                        xmlpp::Element* pSiteShapeRefElt
                            = pBindingSiteRefElt->add_child( eltName::siteShapeRef );
                        
                        pSiteShapeRefElt
                            ->set_attribute( eltName::siteShapeRef_nameAttr,
                                             pActualSiteShape->getName() );
                    }
                }
            }
        }
    };
    
    xmlpp::Element*
    mzrModMol::insertElt( xmlpp::Element* pMolsElt ) const
        throw( utl::xcpt )
    {
        // We need the default state to compute the molecular weight and to
        // get the list of default modifications.
        const cpx::modMolState* pDefaultState = getDefaultState();
        
        // Insert the head element for this modMol.
        xmlpp::Element* pModMolElt
            = pMolsElt->add_child( eltName::modMol );
        
        pModMolElt->set_attribute( eltName::modMol_nameAttr,
                                   getName() );
        
        // Insert the weight element.
        xmlpp::Element* pWeightElt
            = pModMolElt->add_child( eltName::weight );
        
        // Add the mol weight in attribute.
        double molWeight = pDefaultState->getMolWeight();
        pWeightElt->set_attribute( eltName::weight_daltonsAttr,
                                   utl::stringify<double> ( molWeight ) );
        
        // Cause all the binding sites to insert themselves.
        //
        // Distressingly, mzrModMol gets "begin" and "end" two different ways:
        // as a basicMol and as a feature.
        std::for_each( cpx::basicMol<mzrBndSite>::begin(),
                       cpx::basicMol<mzrBndSite>::end(),
                       insertSite( pModMolElt ) );
        
        // Run through all the modification sites, giving the name and default
        // modification of each.  This could be done with binary for_each.
        for ( int modSiteNdx = 0;
              modSiteNdx < modSiteCount();
              ++modSiteNdx )
        {
            // Insert element for giving the name of the modification site.
            xmlpp::Element* pModSiteElt
                = pModMolElt->add_child( eltName::modSite );
            
            pModSiteElt->set_attribute( eltName::modSite_nameAttr,
                                        modSiteNames[modSiteNdx] );
            
            // Insert element for default modification.
            xmlpp::Element* pDefaultModRefElt
                = pModSiteElt->add_child( eltName::defaultModRef );
            
            const cpx::modification* pDefaultMod = ( *pDefaultState )[modSiteNdx];
            pDefaultModRefElt->set_attribute( eltName::defaultModRef_nameAttr,
                                              pDefaultMod->getName() );
        }
        
        // Run through all the registered states of this mol, displaying the
        // "allosteric" ones; i.e. all except the default state.  For each
        // (non-default) state, we only want to see the modification sites that
        // do not have their default modification.
        std::vector<cpx::siteParam> defaultSiteParams = getDefaultSiteParams();
        std::for_each( alloMap.begin(),
                       alloMap.end(),
                       insertNonDefault( pModMolElt,
                                         this,
                                         pDefaultState,
                                         defaultSiteParams ) );
        
        return pModMolElt;
    }
}
