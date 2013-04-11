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

#include "utl/dom.hh"
#include "mzr/moleculizer.hh"
#include "mzr/mzrUnit.hh"
#include "mol/molUnit.hh"
#include "mol/molEltName.hh"
#include "mol/mzrModMol.hh"
#include "mol/smallMol.hh"
#include "mol/parseModMap.hh"
#include "mol/parseModSite.hh"
#include "mol/parseMod.hh"
#include "mol/parseBndSite.hh"
#include "mol/dupModNameXcpt.hh"
#include "mol/unkSiteXcpt.hh"
#include "mol/unkSiteShapeXcpt.hh"
#include <libxml++/libxml++.h>

namespace bnd
{
    class installModification :
        public std::unary_function<xmlpp::Node*, void>
    {
        molUnit& rMolUnit;
        
    public:
        installModification( molUnit& refMolUnit ) :
            rMolUnit( refMolUnit )
        {}
        
        void
        operator()( xmlpp::Node* pModNode ) const
        {
            parseModification modParser;
            cpx::modification* pModification
                = modParser( pModNode );
            
            //       if(! rMolUnit.addMod(pModification))
            // 	   throw dupModNameXcpt(pModification->getName(),
            // 		                pModNode);
            
            rMolUnit.mustAddMod( pModification );
            
        }
    };
    
    class processAllostericSite :
        public std::unary_function<xmlpp::Node*, void>
    {
        molUnit& rMolUnit;
        std::vector<cpx::siteParam>& rParams;
        mzrModMol* pMol;
    public:
        processAllostericSite( molUnit& refMolUnit,
                               mzrModMol* pModMol,
                               std::vector<cpx::siteParam>& rSiteParams ) :
            rMolUnit( refMolUnit ),
            rParams( rSiteParams ),
            pMol( pModMol )
        {}
        
        void
        operator()( xmlpp::Node* pBindingSiteRefNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pBindingSiteRefElt
                = utl::dom::mustBeElementPtr( pBindingSiteRefNode );
            
            // Get the name of the binding site.
            std::string bindingSiteName
                = utl::dom::mustGetAttrString
                ( pBindingSiteRefElt,
                  eltName::bindingSiteRef_nameAttr );
            
            // Convert the binding site's name into the binding site's index.
            //
            // We need both the index and the binding site itself.
            int bindingSiteNdx;
            if ( ! pMol->findSite( bindingSiteName,
                                   bindingSiteNdx ) )
                throw unkSiteXcpt( bindingSiteName,
                                   pBindingSiteRefElt );
            
            // Get the siteShapeRef element, which gives the allosteric shape
            // of the binding site.
            xmlpp::Element* pSiteShapeRefElt
                = utl::dom::mustGetUniqueChild( pBindingSiteRefElt,
                                                eltName::siteShapeRef );
            
            // Get the name of the site shape.
            std::string siteShapeName
                = utl::dom::mustGetAttrString
                ( pSiteShapeRefElt,
                  eltName::siteShapeRef_nameAttr );
            
            // Get pointer to the site shape.
            //
            // (We need the binding site itself in order to get the site
            // shape, given its name.)
            mzrModMol& rMol = *pMol;
            const mzrBndSite& rBindingSite = rMol[bindingSiteNdx];
            
            cpx::siteParam pSiteShape
                = rBindingSite.getShape( siteShapeName );
            if ( ! pSiteShape )
                throw unkSiteShapeXcpt( pBindingSiteRefNode,
                                        rBindingSite,
                                        pMol,
                                        siteShapeName );
            
            // Replace the default site shape of the binding site
            // with the specified site shape in the vector of site shapes.
            rParams[bindingSiteNdx] = pSiteShape;
        }
    };
    
    class installModMolAlloState :
        public std::unary_function<xmlpp::Node*, void>
    {
        molUnit& rMolUnit;
        mzrModMol* pMol;
    public:
        installModMolAlloState( molUnit& refMolUnit,
                                mzrModMol* pModMol ) :
            rMolUnit( refMolUnit ),
            pMol( pModMol )
        {}
        
        void
        operator()( xmlpp::Node* pAlloStateNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pAlloStateElt
                = utl::dom::mustBeElementPtr( pAlloStateNode );
            
            // Get the map from modificaton sites to modifications that
            // describes this allosteric state.  This operation is very
            // similar to parsing the description of the modification
            // sites and their default modifications.
            //
            // It looks like the mod-map element could be eliminated.
            xmlpp::Element* pModMapElt
                = utl::dom::mustGetUniqueChild( pAlloStateElt,
                                                eltName::modMap );
            xmlpp::Node::NodeList modSiteRefs
                = pModMapElt->get_children( eltName::modSiteRef );
            std::map<std::string, const cpx::modification*> modMap;
            transform( modSiteRefs.begin(),
                       modSiteRefs.end(),
                       std::inserter( modMap,
                                      modMap.begin() ),
                       parseModMap( rMolUnit ) );
            // Intern the modification map to get a state of the modMol.
            cpx::molParam pState
                = pMol->internModMap( modMap );
            
            // Get the site shape map, which gives the shapes of the
            // mol's binding sites when in the above modification state.
            //
            // It looks like the site-shape-map element could be eliminated.
            xmlpp::Element* pSiteShapeMapElt
                = utl::dom::mustGetUniqueChild( pAlloStateElt,
                                                eltName::siteShapeMap );
            xmlpp::Node::NodeList bindingSiteRefs
                = pSiteShapeMapElt->get_children( eltName::bindingSiteRef );
            
            // Get the vector of site shapes currently associated with this
            // mol state.  (In all liklihood, this vector is actually generated
            // and installed in the mol's "allostery database" by this call to
            // alloMol::allostery.
            std::vector<cpx::siteParam>& rSiteShapes
                = pMol->allostery( pState );
            
            // Process the bindingSiteRefs, making modifications to the
            // defaultShapes, producing the allosteric vector of shapes.
            // Since this vector of siteParams is "hot," doing the replacements
            // through this
            std::for_each( bindingSiteRefs.begin(),
                           bindingSiteRefs.end(),
                           processAllostericSite( rMolUnit,
                                                  pMol,
                                                  rSiteShapes ) );
        }
    };
    
    class installModMol :
        public std::unary_function<xmlpp::Node*, void>
    {
        
        molUnit& rMolUnit;
        
    public:
        installModMol( molUnit& refMolUnit ) :
            rMolUnit( refMolUnit )
        {}
        
        void
        operator()( xmlpp::Node* pModMolNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pModMolElt
                = utl::dom::mustBeElementPtr( pModMolNode );
            
            std::string modMolName
                = utl::dom::mustGetAttrString( pModMolElt,
                                               eltName::modMol_nameAttr );
            
            // Get the weight element.
            xmlpp::Element* pWeightElt
                = utl::dom::mustGetUniqueChild( pModMolElt,
                                                eltName::weight );
            
            // Get the weight.
            double weight
                = utl::dom::mustGetAttrPosDouble( pWeightElt,
                                                  eltName::weight_daltonsAttr );
            
            // Process the binding sites.
            xmlpp::Node::NodeList bindingSiteNodes
                = pModMolElt->get_children( eltName::bindingSite );
            std::vector<mzrBndSite> bindingSites;
            std::transform( bindingSiteNodes.begin(),
                            bindingSiteNodes.end(),
                            std::back_inserter( bindingSites ),
                            parseMzrBndSite() );
            
            // TODO: DEBUG_TRY
            // This is just a trial thing here....
            //      std::sort( bindingSites.begin(),
            //                 bindingSites.end());
            
            // Process the modification sites.
            xmlpp::Node::NodeList modSiteNodes
                = pModMolElt->get_children( eltName::modSite );
            std::map<std::string, const cpx::modification*> modMap;
            std::transform( modSiteNodes.begin(),
                            modSiteNodes.end(),
                            std::inserter( modMap,
                                           modMap.begin() ),
                            parseModSite( rMolUnit ) );
            
            // Construct the mol.  This has to be done before processing allosteric
            // states, because the mol's "allostery database" has to exist.
            mzrModMol* pModMol
                = new mzrModMol( modMolName,
                                 bindingSites,
                                 weight,
                                 modMap );
            
            // Install the mol in plexUnit's catalog.
            rMolUnit.mustAddMol( pModMol,
                                 pModMolNode );
            
            // Process the allosteric states.
            xmlpp::Node::NodeList alloStateNodes
                = pModMolElt->get_children( eltName::allostericState );
            std::for_each( alloStateNodes.begin(),
                           alloStateNodes.end(),
                           installModMolAlloState( rMolUnit,
                                                   pModMol ) );
        }
    };
    
    class installSmallMol :
        public std::unary_function<xmlpp::Node*, void>
    {
        molUnit& rMolUnit;
    public:
        installSmallMol( molUnit& refMolUnit ) :
            rMolUnit( refMolUnit )
        {
        }
        
        void
        operator()( xmlpp::Node* pSmallMolNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pSmallMolElt
                = utl::dom::mustBeElementPtr( pSmallMolNode );
            
            std::string smallMolName
                = utl::dom::mustGetAttrString( pSmallMolElt,
                                               eltName::smallMol_nameAttr );
            
            // Get the weight element.
            xmlpp::Element* pWeightElt
                = utl::dom::mustGetUniqueChild( pSmallMolElt,
                                                eltName::weight );
            
            // Get the molecular weight.
            double weight
                = utl::dom::mustGetAttrPosDouble( pWeightElt,
                                                  eltName::weight_daltonsAttr );
            
            // Construct the smallMol.
            smallMol* pSmallMol
                = new smallMol( smallMolName,
                                weight );
            
            // Install the new smallMol in the catalog.
            rMolUnit.mustAddMol( pSmallMol,
                                 pSmallMolNode );
        }
    };
    
    void
    molUnit::parseDomInput( xmlpp::Element* pRootElement,
                            xmlpp::Element* pModelElement,
                            xmlpp::Element* pStreamElt )
        throw( std::exception )
    {
        // Get the modifications element.
        xmlpp::Element* pModifications
            = utl::dom::mustGetUniqueChild( pModelElement,
                                            eltName::modifications );
        
        // Did some tests on the modification nodes to answer the following
        // question: do attributes show up in get_children results?
        //
        // This seems possible, since a list of arbitrary Node*'s is returned.  But
        // only elements result from Node::add_child, so libxml++ may regard
        // Elements as the only legitimate "children."
        //
        // So far, it looks like the only reason for not returning a list of
        // elements is that they don't want to have a new type for it.  NodeList is
        // also used to return results from xpath queries, and therefore must be
        // able to contain Node::Attribute*'s.
        
        // Install the modifications.  For now, these are going into a static
        // catalog in the modMixin class.  (Note that "installModification" does not
        // require "pTheMolculizer" as an argument, a very very bad sign.)  This
        // catalog should be an ordinary member of the mol unit, and units should be
        // allocated by new DURING MODULE LOADING, rather than in .so
        // initialization, so that different moleculizer processes will get
        // different module objects.
        
        xmlpp::Node::NodeList mods
            = pModifications->get_children( eltName::modification );
        std::for_each( mods.begin(),
                       mods.end(),
                       installModification( *this ) );
        
        // Get the mols element.
        xmlpp::Element* pMols
            = utl::dom::mustGetUniqueChild( pModelElement,
                                            eltName::mols );
        
        // Install the small-mols, which represent small molecules that
        // participate in binding (e.g. ATP) and therefore need the binding
        // machinery that mols have.
        xmlpp::Node::NodeList smallMols
            = pMols->get_children( eltName::smallMol );
        std::for_each( smallMols.begin(),
                       smallMols.end(),
                       installSmallMol( *this ) );
        
        // Install the mod-mols.  Again note that, due to the evil static
        // catalogs, etc. the argument list to "installModMol" is way too short.
        xmlpp::Node::NodeList modMols
            = pMols->get_children( eltName::modMol );
        std::for_each( modMols.begin(),
                       modMols.end(),
                       installModMol( *this ) );
    }

    cpx::modification*
    parseModification::operator()(xmlpp::Node* pModificationNode) const
    {
	xmlpp::Element* pModElt
	    = utl::dom::mustBeElementPtr( pModificationNode );
            
	std::string name
	    = utl::dom::mustGetAttrString( pModElt,
					   eltName::modification_nameAttr );
            
	xmlpp::Element* pWeightDeltaElt
	    = utl::dom::mustGetUniqueChild( pModElt,
					    eltName::weightDelta );
            
	double weightDelta = utl::dom::mustGetAttrDouble
	    ( pWeightDeltaElt,
	      eltName::weightDelta_daltonsAttr );
            
	return new cpx::modification( name,
				      weightDelta );
    }

    std::pair<std::string, const cpx::modification*>
    parseModMap::operator()( xmlpp::Node* pModSiteRefNode ) const
	throw( utl::xcpt )
    {
	xmlpp::Element* pModSiteRefElt
	    = utl::dom::mustBeElementPtr( pModSiteRefNode );
            
	// Get the mod site name.
	std::string modSiteName
	    = utl::dom::mustGetAttrString( pModSiteRefElt,
					   eltName::modSiteRef_nameAttr );
            
	// Get the mod-ref element that tells what modification is at this
	// modification site.
	xmlpp::Element* pModRefElt
	    = utl::dom::mustGetUniqueChild( pModSiteRefElt,
					    eltName::modRef );
            
	// Get the modification name.
	std::string modName
	    = utl::dom::mustGetAttrString( pModRefElt,
					   eltName::modRef_nameAttr );
            
	// Look up the modification in the BAD BAD BAD static catalog.
	//
	// Once again, testing for success here indicates lack of trust
	// in validation.
	const cpx::modification* pMod = rMolUnit.getMod( modName );
	if ( 0 == pMod ) throw unkModXcpt( modName,
					   pModRefElt );
            
	return std::make_pair( modSiteName,
			       pMod );
    }




    std::pair<std::string, const cpx::modification*>
    parseModSite::operator()( xmlpp::Node* pModSiteNode ) const
	throw( utl::xcpt )
    {
	// Make sure the node is an element, possibly unnecessarily.
	xmlpp::Element* pModSiteElt
	    = dynamic_cast<xmlpp::Element*>( pModSiteNode );
	if ( 0 == pModSiteElt ) throw utl::dom::badElementCastXcpt( pModSiteNode );
            
	// Get the mod site name.
	std::string modSiteName
	    = utl::dom::mustGetAttrString( pModSiteElt,
					   eltName::modSite_nameAttr );
            
	// Get the default modification element.
	xmlpp::Element* pDefaultModRefElt
	    = utl::dom::mustGetUniqueChild( pModSiteElt,
					    eltName::defaultModRef );
            
	// Get the default modification name.
	std::string defaultModName
	    = utl::dom::mustGetAttrString( pDefaultModRefElt,
					   eltName::defaultModRef_nameAttr );
            
	// Look up the default modification.
	//
	// Note that this is yet another BAD BAD BAD static catalog use.
	//
	// Testing that the lookup succeeded indicates lack of trust in
	// validation.
	const cpx::modification* pDefaultMod
	    = rMolUnit.getMod( defaultModName );
            
	if ( 0 == pDefaultMod ) throw unkModXcpt( defaultModName,
						  pDefaultModRefElt );
            
	return std::make_pair( modSiteName,
			       pDefaultMod );
    }



        mzrBndSite
        parseMzrBndSite::operator()( xmlpp::Node* pBindingSiteNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pBindingSiteElt
                = utl::dom::mustBeElementPtr( pBindingSiteNode );
            
            std::string name
                = utl::dom::mustGetAttrString( pBindingSiteElt,
                                               eltName::bindingSite_nameAttr );
            
            
            std::set<std::string> siteShapeNames;
            xmlpp::Node::NodeList siteShapeNodes
                = pBindingSiteElt->get_children( eltName::siteShape );
            std::transform( siteShapeNodes.begin(),
                            siteShapeNodes.end(),
                            std::inserter( siteShapeNames,
                                           siteShapeNames.begin() ),
                            parseSiteShapeName() );
            
            xmlpp::Element* pDefaultShapeRefElt
                = utl::dom::mustGetUniqueChild( pBindingSiteElt,
                                                eltName::defaultShapeRef );
            
            std::string defaultShapeName
                = utl::dom::mustGetAttrString( pDefaultShapeRefElt,
                                               eltName::defaultShapeRef_nameAttr );
            
            return mzrBndSite( name,
                               siteShapeNames,
                               defaultShapeName );
        }



}
