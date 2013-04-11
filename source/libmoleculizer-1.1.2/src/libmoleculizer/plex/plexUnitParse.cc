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
#include "mzr/respondReaction.hh"
#include "plex/mzrPlexFamily.hh"
#include "plex/parsePlex.hh"
#include "plex/parseOmniPlex.hh"
#include <libxml++/libxml++.h>
#include "mzr/mzrSpeciesDumpable.hh"

namespace plx
{
    namespace
    {
        // Class for accumulating list of all plex nodes for omniplexes.
        class addPathNodesToList :
            public std::unary_function<std::string, void>
        {
            xmlpp::Node::NodeList& rNodes;
            xmlpp::Element* pRootElt;
        public:
            addPathNodesToList( xmlpp::Node::NodeList& rNodeList,
                                xmlpp::Element* pRootElement ) :
                rNodes( rNodeList ),
                pRootElt( pRootElement )
            {}
            
            void
            operator()( const std::string& rXpath ) const
                throw()
            {
                // Get the list satisfying this xPath query.
                xmlpp::NodeSet xpathHits
                    = pRootElt->find( rXpath );
                
                // Insert these at the end of the list of all hits.
                rNodes.insert( rNodes.end(),
                               xpathHits.begin(),
                               xpathHits.end() );
            }
        };
    }
    
    void
    plexUnit::parseDomInput( xmlpp::Element* pRootElt,
                             xmlpp::Element* pModelElt,
                             xmlpp::Element* pStreamsElt )
        throw( utl::xcpt )
    {

        // Use Xpaths to omniplex nodes, registered by other modules,
        // to find all the omniplexes in the file.
        //
        // We have to have all the omniplexes in place before we recognize any
        // complexes in the conventional way, thereby creating plexFamilies.

        xmlpp::Node::NodeList omniPlexNodes;
        std::for_each( omniXpaths.begin(),
                       omniXpaths.end(),
                       addPathNodesToList( omniPlexNodes,
                                           pRootElt ) );

        // "Unify" omniplex families (recognize, but without the usual
        // initializations) and put them on the plexUnit's list of omniplexes.
        // After this is done, plexes and omniplexes can be recognized in the usual
        // way.
        std::for_each( omniPlexNodes.begin(),
                       omniPlexNodes.end(),
                       parseOmniPlex( rMzrUnit,
                                      rMolUnit,
                                      *this ) );

        // Since the omniplex families have been "unified" in, they won't
        // undergo normal initialization when recognized.  Hence, we
        // run through them all and connect them to their features.
        //
        // After this point, we should be ready to ready to recognize complexes
        // in the usual way, creating plexFamilies.
        std::for_each( omniPlexFamilies.begin(),
                       omniPlexFamilies.end(),
                       std::mem_fun( &mzrPlexFamily::connectToFeatures ) );

        // Allosteric omnis.
        xmlpp::Element* pAlloOmnisElt
            = utl::dom::mustGetUniqueChild( pModelElt, eltName::allostericOmnis );
        
        xmlpp::Node::NodeList alloOmniNodes = pAlloOmnisElt->get_children( eltName::allostericOmni );
        
        // Allosteric plexes.
        xmlpp::Element* pAlloPlexesElt
            = utl::dom::mustGetUniqueChild( pModelElt,
                                            eltName::allostericPlexes );

        xmlpp::Node::NodeList alloPlexNodes
            = pAlloPlexesElt->get_children( eltName::allostericPlex );
        
        // Parse allosteric omnis.
        //
        // This must be completed before any species of complexes are generated.
        std::for_each( alloOmniNodes.begin(),
                       alloOmniNodes.end(),
                       parseAllostericOmni( rMolUnit,
                                            *this ) );

        // Parse allosteric plexes.
        //
        // This must be done before any species of complexes are generated.
        std::for_each( alloPlexNodes.begin(),
                       alloPlexNodes.end(),
                       parseAllostericPlex( rMolUnit,
                                            *this,
                                            rMzrUnit ) );
        if (pStreamsElt)
        {

            // Species streams.
            xmlpp::Element* pSpeciesStreamsElt
                = utl::dom::mustGetUniqueChild(pStreamsElt,
                                               mzr::eltName::speciesStreams);
        
            xmlpp::Node::NodeList omniSpeciesStreamNodes
                = pSpeciesStreamsElt->get_children(eltName::omniSpeciesStream);

        
            xmlpp::Node::NodeList plexSpeciesStreamNodes
                = pSpeciesStreamsElt->get_children(eltName::plexSpeciesStream);

            // Attach dumpables to families of complexes.  This must be done before
            // any species of complexes are generated.
        
            // Parse query-based dumpables for omniplexes.
            std::for_each(omniSpeciesStreamNodes.begin(),
                          omniSpeciesStreamNodes.end(),
                          parseOmniSpeciesStream(rMzrUnit,
                                                 rMolUnit,
                                                 *this));
        
            // Parse query-based dumpables for plexes.
            //     std::for_each(plexSpeciesStreamNodes.begin(),
            // 		  plexSpeciesStreamNodes.end(),
            // 		  parsePlexSpeciesStream(rMzrUnit,
            // 					 rMolUnit,
            // 					 *this));

        }
        

        // Explicit plexSpecies.
        xmlpp::Element* pExplicitSpeciesElt
            = utl::dom::mustGetUniqueChild( pModelElt,
                                            mzr::eltName::explicitSpecies );

        xmlpp::Node::NodeList plexSpeciesNodes
            = pExplicitSpeciesElt->get_children( eltName::plexSpecies );

        // Parse explicit plexSpecies, generating species of complexes, but not
        // populating them.  Since this doesn't create the initial population,
        // notification isn't an issue.
        std::for_each( plexSpeciesNodes.begin(),
                       plexSpeciesNodes.end(),
                       parseExplicitPlexSpecies( rMzrUnit,
                                                 rMolUnit,
                                                 *this ) );

        // This is my own addition -- that each mol should its corresponding plex
        // created if not also notified.  
        

           std::for_each( rMolUnit.getMolsByName().begin(),
                          rMolUnit.getMolsByName().end(),
                          createMonomericPlexesFromMols( rMzrUnit, rMolUnit, *this));
        
    }

    
    namespace
    {
        // Class for creating the initial populations of explicit plex species.
        //
        // This is the first time that notification happens, at least for
        // plexSpecies.
        class createInitialPop : public
        std::unary_function<xmlpp::Node*, void>
        {
            mzr::moleculizer& rMolzer;
            mzr::mzrUnit& rMzrUnit;
        public:
            createInitialPop( mzr::moleculizer& rMoleculizer,
                              mzr::mzrUnit& rMzr ) :
                rMolzer( rMoleculizer ),
                rMzrUnit( rMzr )
            {}
            
            void
            operator()( xmlpp::Node* pPlexSpeciesNode ) const
                throw( utl::xcpt )
            {
                xmlpp::Element* pPlexSpeciesElt
                    = utl::dom::mustBeElementPtr( pPlexSpeciesNode );
                
                std::string speciesName
                    = utl::dom::mustGetAttrString( pPlexSpeciesElt,
                                                   eltName::plexSpecies_nameAttr );
                
                mzr::mzrSpecies* pSpecies
                    = rMzrUnit.mustFindSpecies( speciesName,
                                                pPlexSpeciesElt );
                
                // Here, we want to do the same thing as a createEvent,
                // but we don't pay any attention to generateDepth.
                
                mzr::moleculizer::SpeciesID newSpecID;
                rMolzer.recordSpecies( pSpecies, newSpecID );
                rMolzer.incrementNetworkBySpeciesTag( newSpecID );
            }
        };
    }
    
    void
    plexUnit::prepareToRun( xmlpp::Element* pRootElt,
                            xmlpp::Element* pModelElt,
                            xmlpp::Element* pStreamElt )
        throw( utl::xcpt )
    {
        // Create the initial population of all explicit plex species.
        xmlpp::Element* pExplicitSpeciesElt
            = utl::dom::mustGetUniqueChild( pModelElt,
                                            mzr::eltName::explicitSpecies );
        xmlpp::Node::NodeList plexSpeciesNodes
            = pExplicitSpeciesElt->get_children( eltName::plexSpecies );
        
        std::for_each( plexSpeciesNodes.begin(),
                       plexSpeciesNodes.end(),
                       createInitialPop( rMolzer,
                                         rMzrUnit ) );
    }
    
    namespace
    {
        class accumulateSpecies : public
        std::unary_function<std::multimap<int, mzrPlexFamily*>::value_type, void>
        {
            std::vector<mzrPlexSpecies*>& rAllSpecies;
        public:
            accumulateSpecies( std::vector<mzrPlexSpecies*>& rAllSpeciesVector ) :
                rAllSpecies( rAllSpeciesVector )
            {}
            
            void
            operator()( const argument_type& rEntry ) const
            {
                mzrPlexFamily* pPlexFamily = rEntry.second;
                pPlexFamily->accumulateSpecies( rAllSpecies );
            }
        };
        
        class zeroUpdateSpecies : public
        std::unary_function<mzrPlexSpecies*, void>
        {
            fnd::sensitivityList<mzr::mzrReaction>& rAffected;
            int depth;
            
        public:
            zeroUpdateSpecies( fnd::sensitivityList<mzr::mzrReaction>& rAffectedReactions,
                               int generateDepth ) :
                rAffected( rAffectedReactions ),
                depth( generateDepth )
            {}
            
            void
            operator()( mzrPlexSpecies* pPlexSpecies ) const
            {
                pPlexSpecies->expandReactionNetwork( depth );
            }
        };
    }
    
    
    class parseTaggedPlexSpeciesNpop :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        fnd::sensitivityList<mzr::mzrReaction>& rAffected;
        
    public:
        parseTaggedPlexSpeciesNpop
        ( mzr::mzrUnit& refMzrUnit,
          bnd::molUnit& refMolUnit,
          plexUnit& refPlexUnit,
          fnd::sensitivityList<mzr::mzrReaction>& rAffectedReactions ) :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit ),
            rAffected( rAffectedReactions )
        {}
        
        void
        operator()( xmlpp::Node* pTaggedPlexSpeciesNode ) const
            throw( utl::xcpt )
        {
            xmlpp::Element* pTaggedPlexSpeciesElt
                = utl::dom::mustBeElementPtr( pTaggedPlexSpeciesNode );
            
            // In this case, we expect at most one species to appear
            // in updatedSpecies; in the usual application of
            // parseTaggedPlexSpecies, many species are parsed,
            // and the ones that had been updated at the time of dump
            // appear in updatedSpecies.
            std::vector<mzrPlexSpecies*> updatedSpecies;
            parseTaggedPlexSpecies tpsParser( rMzrUnit,
                                              rMolUnit,
                                              rPlexUnit,
                                              updatedSpecies );
            mzrPlexSpecies* pSpecies
                = tpsParser( pTaggedPlexSpeciesElt );
            
            
            
            // All this re-reading of files as well as dumping need to be completely re-writted, 
            // really.  For the time being, I am commenting this out (specific population of 
            // different tagged plexes), as it serves no purpose anymore.
            
            // Get the population of the species at dump time.
            //             xmlpp::Element* pPopulationElt
            //                 = utl::dom::mustGetUniqueChild( pTaggedPlexSpeciesElt,
            //                                                 eltName::population );
            
            //             int dumpedPop
            //                 = utl::dom::mustGetAttrInt( pPopulationElt,
            //                                             eltName::population_countAttr );
            
            // Update the species with its dumped population.  This obviates
            // the "zeroUpdate" pass that has to be made in prepareToDump,
            // as well as the usual "prepareToRun" which updates the
            // explicit plex species with the populations provided in
            // moleculizer-input.
            xmlpp::Node::NodeList updatedNodes
                = pTaggedPlexSpeciesElt->get_children( eltName::updated );
            
            // Update the species if called for.
            //
            // The plexSpecies we just parsed above is the only one that could
            // appear in updatedSpecies, due to this special application of
            // parseTaggedPlexSpecies. In the usual application of
            // parseTaggedPlexSpecies, many species are parsed, and the ones that
            // had been updated at the time of dump appear in updatedSpecies.
            if ( 0 < updatedSpecies.size() )
            {
                pSpecies->expandReactionNetwork( rMzrUnit.getGenerateDepth() );
            }
        }
    };
    
    void
    plexUnit::prepareToContinue( xmlpp::Element* pRootElt,
                                 xmlpp::Element* pModelElt,
                                 xmlpp::Element* pStreamsElt,
                                 std::map<std::string, std::string>& rTagToName,
                                 xmlpp::Element* pTaggedSpeciesElement )
        throw( utl::xcpt )
    {
        // Parse the tagged-plex-species.
        xmlpp::Node::NodeList taggedPlexSpeciesNodes
            = pTaggedSpeciesElement->get_children( eltName::taggedPlexSpecies );
        
        // Going along with the now-well-established principle of stupid names.
        // The analogous routine for reading explicit plex-species is
        // "processPlexSpecies".
        //
        // In this version the populations from the dump are used as the
        // initial populations of the species.  All the plex species that
        // exist should be updated here, so that the "zeroUpdate" pass
        // is not necessary.
        fnd::sensitivityList<mzr::mzrReaction> affectedReactions;
        std::for_each( taggedPlexSpeciesNodes.begin(),
                       taggedPlexSpeciesNodes.end(),
                       parseTaggedPlexSpeciesNpop( rMzrUnit,
                                                   rMolUnit,
                                                   *this,
                                                   affectedReactions ) );
        
        // Reschedule the affected reactions.  The current simulation time
        // should have been set to the dump time by the mzrUnit.
        std::for_each( affectedReactions.begin(),
                       affectedReactions.end(),
                       mzr::respondReaction( rMolzer ) );
        
        // In this version, do NOT run prepareToRun here, as that
        // sets the populations of the explicit species and reschedules
        // reactions.  May want rearchitecture here.
    }
}
