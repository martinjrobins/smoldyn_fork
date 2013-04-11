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

#ifndef MZRUNIT_H
#define MZRUNIT_H

#include "utl/defs.hh"
#include "utl/dom.hh"

#include "fnd/query.hh"
#include "fnd/dumpable.hh"
#include "mzr/unit.hh"
#include "mzr/mzrException.hh"
#include "mzr/moleculizer.hh"

#include "mzr/molarFactor.hh"
#include "mzr/mzrSpecies.hh"
#include "mzr/mzrReaction.hh"

#include "mzr/mzrEltName.hh"


namespace mzr
{
    // This unit is vaguely special in that, as a shared object, it contains
    // the code for the application class, moleculizer.  As a parser,
    // it "cleans up" general stuff that is basic to moleculizer's operation,
    // rather than added in other units.
    class mzrUnit :
        public unit
    {
        
        // These are all the species that can be dumped to output.
        utl::catalog<mzrSpecies> speciesByName;
        
        // All the species that are not deleted by the recognizer and
        // therefore need management here.
        //
        // 29May03 I will probably want to farm these species out to
        // their units.  For the time being, these are probably just stochastirator
        // species.  Any chance of just keeping keeping, say stochastirator
        // species in a vector, to automate their deletion more gracefully?
        //
        // Each different kind of species would need its own list, and each
        // different kind of species would have to have a default constructor
        // for this kind of thing to work.
        utl::autoVector<mzrSpecies> userSpecies;
        
        // Memory management of reaction families, which hold all the automatically
        // generated reactions.
        utl::autoVector<utl::autoVector<mzrReaction> > reactionFamilies;
        
        // All dumpables.
        utl::autoCatalog<fnd::dumpable<fnd::basicDumpable::dumpArg> > dumpables;
        
        // Memory management for queries of all kinds.  The basicQuery
        // base class serves no purpose other than memory management here.
        utl::autoVector<fnd::baseQuery> queries;
        
        // This new command-line option supplants generateOption and generateOk,
        // in that making the depth negative should turn off reaction generation
        // after intial setup.
        int generateDepth;
        
    public:
        
        // Iterators into this are used in constructor of reaction to ensure that
        // each reaction is sensitized to each global state variable.
        std::vector<fnd::sensitivityList<mzrReaction>*> globalVars;
        
        mzrUnit( moleculizer& rMoleculizer );
        
        // Accessors for generation depth command-line argument.
        void
        setGenerateDepth( int depth )
        {
            generateDepth = depth;
            mzrReaction::setGenerateDepth( depth );
            mzrSpecies::setGenerateDepth( depth );
        }
        
        int
        getGenerateDepth( void )
        {
            return generateDepth;
        }
        
        // This doesn't register the species for deletion, so it can be used
        // for explicit plexSpecies, which are deleted by their plexFamilies.
        bool
        addSpecies( const std::string& rSpeciesName,
                    mzrSpecies* pSpecies )
        {
            
            // I should "record" it but not insist on it being recorded here. NJA
            rMolzer.recordSpecies( pSpecies );
            
            // We also add into moleculizer a map of user-names to their generated names.
            rMolzer.recordUserNameToSpeciesIDPair( rSpeciesName, pSpecies->getName() );
            
            return speciesByName.addEntry( rSpeciesName, ( mzrSpecies* ) pSpecies );
            
        }
        
        void
        mustAddSpecies( const std::string& rSpeciesName,
                        mzrSpecies* pSpecies,
                        xmlpp::Node* pRequestingNode = 0 )
            throw( utl::xcpt );
        
        
        
        /////////////////////////////////////////////////////////
        // For adding a species that will be memory-managed by mzrUnit.
        // (An example would be an explicit stochSpecies, but that's from
        // another module.)
        bool
        addUserSpecies( const std::string& rSpeciesName,
                        mzrSpecies* pSpecies )
        {
            bool result = addSpecies( rSpeciesName,
                                      pSpecies );
            
            if ( result ) userSpecies.push_back( pSpecies );
            
            return result;
        }
        
        mzrSpecies*
        findSpecies( const std::string& rSpeciesName ) const
        {
            return speciesByName.findEntry( rSpeciesName );
        }
        
        mzrSpecies*
        mustFindSpecies( const std::string& rSpeciesName,
                         xmlpp::Node* pRequestingNode = 0 ) const
            throw( utl::xcpt );
        
        
        // Memory management and traversal of reactions that are not automatically
        // generated.
        utl::autoVector<mzrReaction> userReactions;
        
        void
        addUserReaction( mzrReaction* pReaction )
        {
            userReactions.push_back( pReaction );
            rMolzer.recordReaction( pReaction );
        }
        
        // Memory management and traversal of reactions that are automatically
        // generated.
        //
        // It occurs to me that the only reason for these to exist is that the
        // reactions in a family are generated by a particular reaction generator.
        // It might therefore make sense to include in reactionFamily a reference
        // back to its generator.  One could even link generators back to user
        // input for diagnostics, etc.
        void
        addReactionFamily( utl::autoVector<mzrReaction>* pReactionFamily )
        {
            reactionFamilies.addEntry( pReactionFamily );
        }
        
        bool
        addDumpable( fnd::dumpable<fnd::basicDumpable::dumpArg>* pDumpable )
        {
            return dumpables.addEntry( pDumpable->getName(),
                                       pDumpable );
        }
        
        void
        getListOfDumpables( std::vector<std::string>& streamVec) const
        {
            for(utl::autoCatalog<fnd::dumpable<fnd::basicDumpable::dumpArg> >::const_iterator dumpablesIter = dumpables.begin();
                dumpablesIter != dumpables.end();
                ++dumpablesIter)
            {
                streamVec.push_back( dumpablesIter->first );
            }
        }

        int 
        getNumberOfDumpables() const
        {
            return dumpables.size();
        }
        
        // Throws an exception if there is already a dumpable
        // whose name duplicates that of the given dumpable.
        void
        mustAddDumpable( fnd::dumpable<fnd::basicDumpable::dumpArg>* pDumpable,
                         xmlpp::Node* pRequestingNode = 0 )
            throw( utl::xcpt );
        
        fnd::dumpable<fnd::basicDumpable::dumpArg>*
        findDumpable( const std::string& rDumpableName ) const
        {
            return dumpables.findEntry( rDumpableName );
        }
        
        fnd::dumpable<fnd::basicDumpable::dumpArg>*
        mustFindDumpable( const std::string& rDumpableName,
                          xmlpp::Node* pRequestingNode = 0 ) const
            throw( utl::xcpt );
        
        void
        addQuery( fnd::baseQuery* pQuery )
        {
            queries.push_back( pQuery );
        }
        
        virtual void
        parseDomInput( xmlpp::Element* pRootElt,
                       xmlpp::Element* pModelElt,
                       xmlpp::Element* pStreamElt ) throw( std::exception );
        
        // Just emits header lines in all the dumpables, schedules tabDumpEvents
        // for the first time, after which they schedule themselves.
        void
        prepareToRun( xmlpp::Element* pRootElt,
                      xmlpp::Element* pModelElt,
                      xmlpp::Element* pStreamElt ) throw( std::exception );
        
        
        // In addition to the above, sets the current (i.e. initial)
        // simulation time to the time at which state was dumped.
        void
        prepareToContinue( xmlpp::Element* pRootElt,
                           xmlpp::Element* pModelElt,
                           xmlpp::Element* pStreamsElt,
                           std::map<std::string, std::string>& rTagToName,
                           xmlpp::Element* pTaggedSpeciesElement )
            throw( std::exception );
        
        void
        insertStateElts( xmlpp::Element* pRootElt ) throw( std::exception );
        
        
    };
}

#endif // MZRUNIT_H
