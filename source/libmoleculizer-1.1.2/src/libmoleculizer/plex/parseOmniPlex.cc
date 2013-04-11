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

#include <libxml++/libxml++.h>
#include "mzr/mzrSpeciesDumpable.hh"
#include "plex/parseOmniPlex.hh"
#include "plex/mzrPlexFamily.hh"
#include "plex/parsePlex.hh"
#include "plex/mzrPlexQueries.hh"
#include "plex/mzrOmniStructureQueries.hh"

namespace plx
{
    class parseUnboundSiteQuery :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzrOmniStructureQueries* pQueries;
        const parserPlex& rParserPlex;
        plexUnit& rPlexUnit;
        
    public:
        parseUnboundSiteQuery( mzrOmniStructureQueries* pStructureQueries,
                               const parserPlex& rParsedPlex,
                               plexUnit& refPlexUnit ) :
            pQueries( pStructureQueries ),
            rParserPlex( rParsedPlex ),
            rPlexUnit( refPlexUnit )
        {}
        
        void
        operator()( xmlpp::Node* pInstanceRefNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pInstanceRefElt
                = utl::dom::mustBeElementPtr( pInstanceRefNode );
            
            // Parse the instance name.
            std::string instanceName
                = utl::dom::mustGetAttrString( pInstanceRefElt,
                                               eltName::instanceRef_nameAttr );
            
            // Convert the instance name to a mol index.
            int molNdx = rParserPlex.mustGetMolNdxByName( pInstanceRefElt,
                                                          instanceName );
            
            xmlpp::Element* pSiteRefElt
                = utl::dom::mustGetUniqueChild( pInstanceRefElt,
                                                eltName::siteRef );
            
            // Parse the name of the site that is supposed to be free.
            std::string siteName
                = utl::dom::mustGetAttrString( pSiteRefElt,
                                               eltName::siteRef_nameAttr );
            
            // Ask the mol to convert the site name into a site index.
            const bnd::mzrMol* pMol = rParserPlex.mols[molNdx];
            int siteNdx = pMol->mustFindSite( siteName,
                                              pSiteRefElt );
            
            // Construct the query, and add it to the plexUnit for memory
            // management.
            mzrOmniFreeSiteQuery* pFreeSiteQuery
                = new mzrOmniFreeSiteQuery( cpx::siteSpec( molNdx,
                                                           siteNdx ) );
            rPlexUnit.addStructureQuery( pFreeSiteQuery );
            
            // Add the free site query to the overall structural query.
            pQueries->addQuery( pFreeSiteQuery );
        }
    };

    class parseBoundSiteQuery :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzrOmniStructureQueries* pQueries;
        const parserPlex& rParserPlex;
        plexUnit& rPlexUnit;
        
    public:
        parseBoundSiteQuery( mzrOmniStructureQueries* pStructureQueries,
                               const parserPlex& rParsedPlex,
                               plexUnit& refPlexUnit ) :
            pQueries( pStructureQueries ),
            rParserPlex( rParsedPlex ),
            rPlexUnit( refPlexUnit )
        {}
        
        void
        operator()( xmlpp::Node* pInstanceRefNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pInstanceRefElt
                = utl::dom::mustBeElementPtr( pInstanceRefNode );
            
            // Parse the instance name.
            std::string instanceName
                = utl::dom::mustGetAttrString( pInstanceRefElt,
                                               eltName::instanceRef_nameAttr );
            
            // Convert the instance name to a mol index.
            int molNdx = rParserPlex.mustGetMolNdxByName( pInstanceRefElt,
                                                          instanceName );
            
            xmlpp::Element* pSiteRefElt
                = utl::dom::mustGetUniqueChild( pInstanceRefElt,
                                                eltName::siteRef );
            
            // Parse the name of the site that is supposed to be bound.
            std::string siteName
                = utl::dom::mustGetAttrString( pSiteRefElt,
                                               eltName::siteRef_nameAttr );
            
            // Ask the mol to convert the site name into a site index.
            const bnd::mzrMol* pMol = rParserPlex.mols[molNdx];
            int siteNdx = pMol->mustFindSite( siteName,
                                              pSiteRefElt );
            
            // Construct the query, and add it to the plexUnit for memory
            // management.
            mzrOmniBoundSiteQuery* pBoundSiteQuery
                = new mzrOmniBoundSiteQuery( cpx::siteSpec( molNdx,
                                                            siteNdx ) );
            rPlexUnit.addStructureQuery( pBoundSiteQuery );
            
            // Add the bound site query to the overall structural query.
            pQueries->addQuery( pBoundSiteQuery );
        }
    };




    
    void
    parseOmniPlex::
    operator()( xmlpp::Node* pParentNode ) const
        throw( utl::xcpt )
    {
        // Unify the plex; i.e. find its plexFamily in the database, or create it,
        // but don't initialize the plexFamily (connectToFeatures).  plexFamilies
        // can't be connected to their features until after all omniPlexes have
        // been parsed in this way.
        xmlpp::Element* pPlexElt
            = utl::dom::mustGetUniqueChild( pParentNode,
                                            eltName::plex );
        parserPlex parsedPlex;
        mzrPlexFamily* pFamily
            = unifyPlexNode( pPlexElt,
                             rMolUnit,
                             rPlexUnit,
                             parsedPlex );
        
        // Parse the instance states, getting a query.
        mzrPlexQueries* pAndPlexQueries
            = new mzrPlexQueries();
        
        xmlpp::Element* pInstanceStatesElt
            = utl::dom::getOptionalChild( pParentNode,
                                          eltName::instanceStates );
        if ( pInstanceStatesElt )
        {
            parseInstanceStateQueries( pInstanceStatesElt,
                                       pAndPlexQueries,
                                       parsedPlex,
                                       rMolUnit,
                                       rMzrUnit );
        }
        
        // Parse optional structure queries.
        //
        // The only structure query for the time being is a test if a particular
        // (free) site on the omni is free in the plex where the omni is found.
        mzrOmniStructureQueries* pStructureQueries
            = new mzrOmniStructureQueries();
        rPlexUnit.addStructureQuery( pStructureQueries );
        
        xmlpp::Element* pUnboundSitesElt
            = utl::dom::getOptionalChild( pParentNode,
                                          eltName::unboundSites );

        xmlpp::Element* pBoundSitesElt
            = utl::dom::getOptionalChild( pParentNode,
                                          eltName::boundSites );

        if ( pUnboundSitesElt )
        {
            xmlpp::Node::NodeList instanceRefNodes
                = pUnboundSitesElt->get_children( eltName::instanceRef );
            
            std::for_each( instanceRefNodes.begin(),
                           instanceRefNodes.end(),
                           parseUnboundSiteQuery( pStructureQueries,
                                                  parsedPlex,
                                                  rPlexUnit ) );
        }

        if ( pBoundSitesElt )
        {
            xmlpp::Node::NodeList instanceRefNodes
                = pBoundSitesElt->get_children( eltName::instanceRef );
            
            std::for_each( instanceRefNodes.begin(),
                           instanceRefNodes.end(),
                           parseBoundSiteQuery( pStructureQueries,
                                                parsedPlex,
                                                rPlexUnit ) );
        }
        
        // Construct the mzrOmniPlex (which also adds it to its plexFamily.)
        //
        // This is so that new plexFamily's can run down the list
        // of omniPlexes in each structural class that the recognizer
        // found in the new plexFamily.  The new plexFamily checks the
        // structural query of each of these omniPlexes, connecting itself
        // to those whose tests it passes.
        mzrOmniPlex* pOmni
            = new mzrOmniPlex( pFamily,
                               pStructureQueries,
                               pAndPlexQueries );
        
        // Register the family has having omniplexes.
        //
        // This is so recognizer will check for its presence in
        // new plexes.
        rPlexUnit.addOmniPlex( pOmni,
                               pParentNode );
    }
    
    // This could also return the plexFamily with no additional work.
    mzrOmniPlex*
    findOmni( xmlpp::Node* pParentNode,
              bnd::molUnit& rMolUnit,
              plexUnit& rPlexUnit,
              parserPlex& rParsedPlex )
        throw( utl::xcpt )
    {
        // Get the plexFamily of the omniPlex under pParentNode.
        xmlpp::Element* pPlexElt
            = utl::dom::mustGetUniqueChild( pParentNode,
                                            eltName::plex );
        
        // Here, the mzrPlexFamily is returned, but discarded.
        unifyPlexNode( pPlexElt,
                       rMolUnit,
                       rPlexUnit,
                       rParsedPlex );
        
        // With the reorganization of omniPlexes, this lookup is now separate
        // from all the above.  Previously, the plexFamily we just found did this.
        return rPlexUnit.mustGetOmniForNode( pParentNode );
    }
    
    mzrOmniPlex*
    parseAllostericOmni::
    operator()( xmlpp::Node* pParentNode ) const
        throw( utl::xcpt )
    {
        // Find omniPlex parsed earlier.
        parserPlex parsedPlex;
        mzrOmniPlex* pOmni
            = findOmni( pParentNode,
                        rMolUnit,
                        rPlexUnit,
                        parsedPlex );
        
        // Parse allosteric sites.
        //
        // The allosteric modifications are installed in the omni's
        // siteToShapeMap.
        parseAllostericSites alloSitesParser( parsedPlex,
                                              pOmni->getSiteToShapeMap() );
        xmlpp::Element* pAlloSitesElt
            = utl::dom::mustGetUniqueChild( pParentNode,
                                            eltName::allostericSites );
        alloSitesParser( pAlloSitesElt );
        
        return pOmni;
    }
    
    void
    parseOmniSpeciesStream::
    operator() (xmlpp::Node* pOmniSpeciesStreamNode) const
        throw (utl::xcpt)
    {
        xmlpp::Element* pOmniSpeciesStreamElt
            = utl::dom::mustBeElementPtr (pOmniSpeciesStreamNode);
        
        // Get the name of the species stream.
        std::string streamName
            = utl::dom::mustGetAttrString
            (pOmniSpeciesStreamElt,
             eltName::omniSpeciesStream_nameAttr);
        
        // Find omniPlex parsed earlier.
        parserPlex parsedPlex;
        mzrOmniPlex* pOmni
            = findOmni (pOmniSpeciesStreamNode,
                        rMolUnit,
                        rPlexUnit,
                        parsedPlex);
        
        // Construct species dumpable to attach to omniplex.
        // Add it to mzrUnit for memory management and lookup.
        mzr::multiSpeciesDumpable<mzrPlexSpecies>* pDumpable
            = new mzr::multiSpeciesDumpable<mzrPlexSpecies> (streamName);
        
        rMzrUnit.addDumpable (pDumpable);
        
        // Attach dumpable to omniplex, where it will be told of all
        // new species satisfying the omniplex's queries.
        pOmni->getSubPlexFeature()->setDumpable(pDumpable);
    }

    void 
    createMonomericPlexesFromMols::operator()( const utl::autoCatalog<bnd::mzrMol>::value_type& vt)
    {

        // FIX ME!!!
        // Write me....

        // This will be modeled after parseExplicitPlexSpecies
        
        // This function calls parsePlexSpecies to return a pointer
        // parsePlexSpecies first calls recognize plex element and 
        // recognizePlexElt calls plexParser on the element and then does it stuff

        parserPlex thePlex;

        std::string molName( vt.first);
        molName = "the-" + molName;

        thePlex.addMolByName( molName, vt.second );

        // This does unification as well as connection to features. 
        cpx::plexIso recognitionIsos;
        mzrPlexFamily* pPlexFamily = rPlexUnit.recognize( thePlex,
                                                          recognitionIsos );

        std::vector<cpx::molParam> molParams
            = pPlexFamily->makeDefaultMolParams();

        mzrPlexSpecies* pSpecies = pPlexFamily->getMember( molParams );

        rMzrUnit.addSpecies( vt.first,
                             pSpecies );

        return;
        

    }


}
