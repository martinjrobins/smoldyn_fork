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

#ifndef PARSEPLEX_H
#define PARSEPLEX_H

#include "utl/dom.hh"
#include "mol/molUnit.hh"
#include "plex/mzrPlexQueries.hh"
#include "plex/parserPlex.hh"
#include "plex/plexUnit.hh"

namespace plx
{
    // Parses a mol-instance, adding it to the plex currently being parsed.
    class parseMolInstance :
        public std::unary_function<xmlpp::Node*, void>
    {
        bnd::molUnit& rMolUnit;
        parserPlex& rParsedPlex;
    public:
        parseMolInstance( bnd::molUnit& refMolUnit,
                          parserPlex& refParsedPlex ) :
            rMolUnit( refMolUnit ),
            rParsedPlex( refParsedPlex )
        {}
        
        void
        operator()( xmlpp::Node* pMolInstNode ) const
            throw( utl::xcpt );
    };
    
    // Parses a mol-instance-ref node, two of which with the same function
    // appear in a plex binding.
    class parseBindingPartner :
        public std::unary_function<xmlpp::Node*, cpx::siteSpec>
    {
        parserPlex& rParsedPlex;
        std::set<cpx::siteSpec>& rBoundSites;
    public:
        parseBindingPartner( parserPlex& refParsedPlex,
                             std::set<cpx::siteSpec>& refBoundSites ) :
            rParsedPlex( refParsedPlex ),
            rBoundSites( refBoundSites )
        {}
        
        cpx::siteSpec
        operator()( xmlpp::Node* pMolInstRefNode ) const
            throw( utl::xcpt );
    };
    
    
    // Parses a plex binding, adding it to the plex currently being parsed.
    class parseBinding :
        public std::unary_function<xmlpp::Node*, void>
    {
        parserPlex& rParsedPlex;
        std::set<cpx::siteSpec>& rBoundSites;
    public:
        parseBinding( parserPlex& refParsedPlex,
                      std::set<cpx::siteSpec>& refBoundSites ) :
            rParsedPlex( refParsedPlex ),
            rBoundSites( refBoundSites )
        {}
        
        void
        operator()( xmlpp::Node* pBindingNode ) const
            throw( utl::xcpt );
    };
    
    // Parses a plexElement, producing a parserPlex. Note that this does not
    // recognize the plex (locate its isomorphism class.)
    class parsePlex :
        public std::unary_function<xmlpp::Element*, void>
    {
        bnd::molUnit& rMolUnit;
        parserPlex& rParsedPlex;
    public:
        parsePlex( bnd::molUnit& refMolUnit,
                   parserPlex& refParsedPlex ) :
            rMolUnit( refMolUnit ),
            rParsedPlex( refParsedPlex )
        {}
        
        void
         operator()( xmlpp::Element* pPlexElt ) const
            throw( utl::xcpt );
    };
    
    // Parses a plex and recognizes it, returning the plexFamily (isomorphism
    // class) to which it belongs. The recognized plexFamily is not initialized
    // in the usual way, and the mapping from mol instance names to (paradigm)
    // mol indexes is not constructed.
    //
    // This is for preliminary scan of omniplexes.
    mzrPlexFamily*
    unifyPlexNode( xmlpp::Node* pPlexNode,
                   bnd::molUnit& rMolUnit,
                   plexUnit& rPlexUnit,
                   parserPlex& rParsedPlex )
    throw( utl::xcpt );
    
    // Parses a plex and recognizes it, returning the plexFamily (isomorphism
    // class) to which it belongs.  Also constructs
    // mapping from mol instance names to (paradigm) mols; this mapping is
    // installed in rParserPlex.
    //
    // Recognition is done in the usual way, so this is intended for routine
    // parsing of plexes.
    mzrPlexFamily*
    recognizePlexElt( xmlpp::Element* pPlexElt,
                      parserPlex& rParserPlex,
                      bnd::molUnit& rMolUnit,
                      plexUnit& rPlexUnit );
    
    // Parses an instance-states node, such as might occur in an allosteric-plex
    // or allosteric-omni construction, into a given overall state query.
    //
    // This routine is the point at which new parsers for new kinds of mol state
    // queries for new kinds of mols should be installed.  For now, we have only
    // mod-mols.
    void
    parseInstanceStateQueries( xmlpp::Node* pInstanceStatesNode,
                               mzrPlexQueries* pQuery,
                               const parserPlex& rParserPlex,
                               bnd::molUnit& rMolUnit,
                               mzr::mzrUnit& rMzrUnit )
        throw( utl::xcpt );
    
    //   void
    //   parseInstanceStateQueries(xmlpp::Node* pInstanceStatesNode,
    // 			    cpx::andPlexQueries<bnd::mzrMol, mzrPlex, mzrPlexSpecies, mzrPlexFamily>* pQuery,
    // 			    const parserPlex& rParserPlex,
    // 			    bnd::molUnit& rMolUnit,
    // 			    plexUnit& rPlexUnit)
    //     throw(utl::xcpt);
    
    // Parses an instance-states node, such as might occur in a plex-species
    // construction.  For the mol instances in the plex whose states are
    // described, this results in a new molParam, which is installed into the
    // given vector of molParams, which gives a molParam for each mol instance,
    // indexed as they are in the given parserPlex.
    //
    // (One cannot just return the vector here: sometimes there are no
    // instance states queries, and the vector of default molParams must be
    // constructed anyway.)
    //
    // One uses this vector to construct an actual plexSpecies, given the plex's
    // structural class (plexFamily).
    //
    // This routine is the point at which new parsers for new kinds of mol
    // states for new kinds of mols should be installed, similar to the above
    // code for mol state queries; again, there are only mod-mols at this time.
    void
    parseInstanceStates( xmlpp::Node* pInstanceStatesNode,
                         std::vector<cpx::molParam>& rMolParams,
                         const parserPlex& rParserPlex,
                         const mzrPlexFamily* pPlexFamily,
                         bnd::molUnit& rMolUnit )
        throw( utl::xcpt );
    
    // Parses a plex and instance states for that plex, generating a parserPlex
    // and a query.
    class parsePlexClass :
        public std::unary_function<xmlpp::Node*, mzrPlexFamily*>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        
        // Output variables.
        parserPlex& rPlex;
        cpx::andPlexQueries<mzrPlexSpecies,
                            mzrOmniPlex>* pQuery;
        
    public:
        parsePlexClass( mzr::mzrUnit& refMzrUnit,
                        bnd::molUnit& refMolUnit,
                        plexUnit& refPlexUnit,
                        parserPlex& refParsedPlex,
                        cpx::andPlexQueries<mzrPlexSpecies, mzrOmniPlex>* pParsedQueries ) :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit ),
            rPlex( refParsedPlex ),
            pQuery( pParsedQueries )
        {}
        
        mzrPlexFamily*
        operator()( xmlpp::Node* pParentNode ) const
            throw( utl::xcpt );
    };
    
    // Parses a plex species, installing it in its plexFamily.
    //
    // This is the core of both parseExplicitPlexSpecies, which adds naming of
    // the species, and parseTaggedPlexSpecies, which adds parsing of elements
    // that tell whether the species had been updated at the time of the state
    // dump.
    mzrPlexSpecies*
    parsePlexSpecies( xmlpp::Element* pParentElement,
                      mzr::mzrUnit& rMzrUnit,
                      bnd::molUnit& rMolUnit,
                      plexUnit& rPlexUnit )
    throw( utl::xcpt );
    
    
    // Parses an explicit plex species, adding it to mzrUnit's catalog
    // of named species.
    //
    // This has to happen after all the omniplexes have been recognized, all the
    // allosteric modifications associated with plexes and omniplexes installed,
    // and all the plexes have been connected to their features.
    class parseExplicitPlexSpecies :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        
    public:
        parseExplicitPlexSpecies( mzr::mzrUnit& refMzrUnit,
                                  bnd::molUnit& refMolUnit,
                                  plexUnit& refPlexUnit ) :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit )
        {}
        
        mzrPlexSpecies*
        operator()( xmlpp::Node* pPlexSpeciesNode ) const
            throw( utl::xcpt );
    };
    
    // Parses a tagged plex species, as appears in state dumps.
    class parseTaggedPlexSpecies :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        std::vector<mzrPlexSpecies*>& rUpdated;
        
    public:
        parseTaggedPlexSpecies( mzr::mzrUnit& refMzrUnit,
                                bnd::molUnit& refMolUnit,
                                plexUnit& refPlexUnit,
                                std::vector<mzrPlexSpecies*>& rUpdatedSpecies ) :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit ),
            rUpdated( rUpdatedSpecies )
        {}
        
        mzrPlexSpecies*
        operator()( xmlpp::Node* pPlexSpeciesNode ) const
            throw( utl::xcpt );
    };
    
    // Parses an allosteric-sites element, making insertions
    // into a siteToShapeMap.  This map could be the one in
    // an omniPlex, or it could be one destined for insertion
    // into a plexFamily's alloStateList.
    //
    // Should this be an ordinary function (do I ever use it in
    // a template algo)?
    class parseAllostericSites :
        public std::unary_function<xmlpp::Node*, void>
    {
        const parserPlex& rParserPlex;
        cpx::siteToShapeMap& rSiteToShapeMap;
        
    public:
        parseAllostericSites( const parserPlex& refParserPlex,
                              cpx::siteToShapeMap& refSiteToShapeMap ) :
            rParserPlex( refParserPlex ),
            rSiteToShapeMap( refSiteToShapeMap )
        {}
        
        void
        operator()( xmlpp::Element* pAlloSitesElt ) const
            throw( utl::xcpt );
    };
    
    // Parses an allosteric-plex element, as appears in input.
    class parseAllostericPlex :
        public std::unary_function<xmlpp::Node*, void>
    {
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        mzr::mzrUnit& rMzrUnit;
        
    public:
        parseAllostericPlex( bnd::molUnit& refMolUnit,
                             plexUnit& refPlexUnit,
                             mzr::mzrUnit& refMzrUnit ) :
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit ),
            rMzrUnit( refMzrUnit )
        {}
        
        void
        operator()( xmlpp::Node* pAlloPlexNode ) const
            throw( utl::xcpt );
    };
    
    // Parses a plex-species-stream, as appears in input.
    //
    // This adds a dumpable to the plexFamily.  The dumpable has as its filter
    // the query parsed from the contained plexClass.  It also has a name, and
    // it's installed in a catalog in mzrUnit, so it can be found for inclusion
    // into dump files ("dump-stream" elements).
    class parsePlexSpeciesStream :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        
    public:
        parsePlexSpeciesStream( mzr::mzrUnit& refMzrUnit,
                                bnd::molUnit& refMolUnit,
                                plexUnit& refPlexUnit ) :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit )
        {}
        
        void
        operator()( xmlpp::Node* pPlexSpeciesStreamNode ) const
            throw( utl::xcpt );
    };

    class createMonomericPlexesFromMols
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;

    public:
        createMonomericPlexesFromMols(mzr::mzrUnit& refMzrUnit,
                                      bnd::molUnit& refMolUnit,
                                      plexUnit& refPlexUnit ) :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit )
        {}

        void operator()( const utl::autoCatalog<bnd::mzrMol>::value_type& vt);
    };
}

#endif // PARSEPLEX_H
