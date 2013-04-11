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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2008
//
// Modifing Authors:
//
//

#ifndef RXNNETWORKCATALOG_HH
#define RXNNETWORKCATALOG_HH

#include "utl/defs.hh"
#include "fnd/fndXcpt.hh"
#include "fnd/basicReaction.hh"
#include "fnd/basicSpecies.hh"
#include <set>

namespace fnd
{
    
    // Both speciesType and reactionType must have a public function
    // void expandReactionNetwork() - that is, derived from
    // fnd::reactionNetworkComponent.

    template <typename speciesT,
              typename reactionT>
    class ReactionNetworkDescription
    {
    public:
        DECLARE_TYPE( speciesT, SpeciesType );
        DECLARE_TYPE( reactionT, ReactionType );
        
        DECLARE_TYPE( String, SpeciesName );
        DECLARE_TYPE( SpeciesName, SpeciesTag );
        DECLARE_TYPE( SpeciesName, SpeciesID );
        DECLARE_TYPE( const SpeciesTag*, SpeciesHandle);

        typedef std::map<SpeciesHandle, SpeciesTypePtr, utl::aux::compareByPtrValue<SpeciesTag> > SpeciesCatalog;

        typedef std::set<std::string> Registry;

        typedef typename SpeciesCatalog::iterator SpeciesCatalogIter;
        typedef typename SpeciesCatalog::const_iterator SpeciesCatalogCIter;

        typedef std::list<SpeciesTypePtr> SpeciesList;
        typedef typename SpeciesList::iterator SpeciesListIter;
        typedef typename SpeciesList::const_iterator SpeciesListCIter;

        typedef std::list<ReactionTypePtr> ReactionList;
        typedef typename ReactionList::iterator ReactionListIter;
        typedef typename ReactionList::const_iterator ReactionListCIter;

        typedef std::pair<SpeciesListIter, ReactionListIter> CachePosition;
        
        typedef std::multimap<SpeciesTypePtr, ReactionTypePtr> ParticipatingSpeciesRxnMap;
        typedef std::map< SpeciesHandle, const SpeciesID*, utl::aux::compareByPtrValue<SpeciesTag> > SpeciesNameMap;

        // This is a map of SpeciesTag* -> Species*
        SpeciesCatalog theSpeciesListCatalog;
        SpeciesNameMap speciesTagToSpeciesIDChart;
        SpeciesNameMap speciesIDToSpeciesTagChart;

        std::map<std::string, ReactionTypePtr> reactionChart;

        ReactionList theCompleteReactionList;

        ReactionList unaryReactionList;
        ReactionList binaryReactionList;
        
        SpeciesList  theDeltaSpeciesList;
        ReactionList theDeltaReactionList;
        
        ReactionList zeroSubstrateRxns;
        ParticipatingSpeciesRxnMap singleSubstrateRxns;
        ParticipatingSpeciesRxnMap doubleSubstrateRxns;
        ParticipatingSpeciesRxnMap autoDimerizationRxns;

    public:

        ReactionNetworkDescription();
        virtual ~ReactionNetworkDescription();

        ///////////////////////////////////////////////////////////////////////////
        // Species Location API
        // 
        // The following functions allow for looking up species via their tagged 
        // names.  
        ///////////////////////////////////////////////////////////////////////////
        
        SpeciesTypePtr findSpecies( const SpeciesTag& specTag ) throw( fnd::NoSuchSpeciesXcpt );
        SpeciesTypeCptr findSpecies( const SpeciesTag& specTag ) const throw( fnd::NoSuchSpeciesXcpt );
        bool checkSpeciesIsKnown( const std::string& speciesName ) const;


        bool
        findReactionWithSubstrates( SpeciesTypeCptr A,
                                    std::vector<ReactionTypeCptr>& reactionVector);

        
        bool
        findReactionWithSubstrates( SpeciesTypeCptr A,
                                    SpeciesTypeCptr B,
                                    std::vector<ReactionTypeCptr>& reactionVector);

        const ReactionList& getReactionList() const;

        SpeciesCatalog& getSpeciesCatalog();
        const SpeciesCatalog& getSpeciesCatalog() const;

        unsigned int getTotalNumberSpecies() const;
        unsigned int getTotalNumberReactions() const;


        ///////////////////////////////////////////////////////////////////////////
        //  Delta species and reactions API
        //
        //  The following functions are designed to work on delta species and 
        //  reactions.  These functions are the way that you determine what portions
        //  of the network have been generated since the last time resetCurrentState
        //  was called
        ///////////////////////////////////////////////////////////////////////////

        unsigned int getNumberDeltaSpecies() const;
        unsigned int getNumberDeltaReactions() const;

        const SpeciesList& getDeltaSpeciesList() const;
        const ReactionList& getDeltaReactionList() const;

        void resetCurrentState();
        
        ///////////////////////////////////////////////////////////////////////////
        //  New species and reaction recording API
        // 
        //  The following functions are used for recording new species and reactions.
        //  The functions beginning "record" that return bools return true if the 
        //  species/reaction is new and false otherwise.  The mustRecord functions 
        //  expect to succeed.  They throw an exception otherwise. 
        virtual bool recordSpecies( SpeciesTypePtr pSpecies );
        
        // This function works in exactly the same manner as record species.  However
        // whether it succeeds or fails, the name under which the species is/has been 
        // registered is placed in the refName parameter.
        virtual bool recordSpecies( SpeciesTypePtr pSpecies, SpeciesTag& refName );
        virtual bool recordReaction( ReactionTypePtr pRxn );

        void mustRecordSpecies( SpeciesTypePtr pSpecies ) throw( utl::xcpt );
        void mustRecordSpecies( SpeciesTypePtr pSpecies, SpeciesTag& refName ) throw( utl::xcpt );
        void mustRecordReaction( ReactionTypePtr pRxn ) throw( utl::xcpt );

        void incrementNetworkBySpeciesTag( const SpeciesTag& rName ) throw( utl::xcpt );
        
        SpeciesID convertSpeciesTagToSpeciesID( const SpeciesTag& rTag ) const throw( utl::xcpt );
        SpeciesTag convertSpeciesIDToSpeciesTag( const SpeciesID& rID) const throw( utl::xcpt );

        Registry SpeciesRegistry;
        Registry ReactionRegistry;
    };

}

#include "reactionNetworkDescriptionImpl.hh"

#endif // RXNNETWORKCATALOG_HH

