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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2009
//
// Modifing Authors:
//
//

#include <iostream>

namespace fnd
{
    template <typename speciesT, typename reactionT>
    typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypePtr
    ReactionNetworkDescription<speciesT, reactionT>::findSpecies( const typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTag& name ) 
        throw( fnd::NoSuchSpeciesXcpt )
    {
        SpeciesCatalogIter theIter = theSpeciesListCatalog.find( &name );
        if ( theIter != theSpeciesListCatalog.end() )
        {
            return theIter->second;
        }
        else
        {
            throw fnd::NoSuchSpeciesXcpt( name );
        }
            
    }



    template <typename speciesT, typename reactionT>
    typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypeCptr
    ReactionNetworkDescription<speciesT, reactionT>::findSpecies( const typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTag& name ) const 
        throw( fnd::NoSuchSpeciesXcpt )
    {
        SpeciesCatalogCIter theIter = theSpeciesListCatalog.find( &name );
        
        if ( theIter != theSpeciesListCatalog.end() )
        {
            return theIter->second;
        }
        else
        {
            throw fnd::NoSuchSpeciesXcpt( name );
        }
    }


    template <typename speciesT, typename reactionT>
    bool
    ReactionNetworkDescription<speciesT, reactionT>::findReactionWithSubstrates( typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypeCptr A,
                                                                                 std::vector<typename ReactionNetworkDescription<speciesT, reactionT>::ReactionTypeCptr>& reactionVector)
    {
        // Because we are not clearing the reaction vector (the goal for this is so that 
        // users can easily collect together many different reactions from different sources or whatever.
        typename std::vector<ReactionTypeCptr>::const_iterator originalEnd = reactionVector.end();

        if (!A->hasNotified()) 
        {
            // Hurm.  This seems uncool to me, but I don't really know to get around it otherwise.  
            const_cast<SpeciesTypePtr>(A)->expandReactionNetwork();
        }

        typename ParticipatingSpeciesRxnMap::const_iterator iter = singleSubstrateRxns.find( const_cast<SpeciesTypePtr>(A) );
            
        while ( iter->first == A )
        {
            reactionVector.push_back( iter->second );
            ++iter;
        }
            
        return ( reactionVector.end() != originalEnd );
    }


    template <typename speciesT, typename reactionT>
    bool
    ReactionNetworkDescription<speciesT, reactionT>::findReactionWithSubstrates( typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypeCptr A,
                                                                                 typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypeCptr B,
                                                                                 std::vector<ReactionNetworkDescription<speciesT, reactionT>::ReactionTypeCptr>& reactionVector)
    {
        
        typename std::vector<ReactionTypeCptr>::const_iterator originalEnd = reactionVector.end();

        // This feels wrong (although semantically, so right), and probably means things 
        // should be refactored.
        if( ! A->hasNotified() )
        {
            const_cast<SpeciesTypePtr>(A)->expandReactionNetwork();
        }

        if (!B->hasNotified() )
        {
            const_cast<SpeciesTypePtr>(B)->expandReactionNetwork();
        }
            
            
        if ( A != B )
        {
            
            std::pair< typename ParticipatingSpeciesRxnMap::iterator, typename ParticipatingSpeciesRxnMap::iterator> rangeIterA =  doubleSubstrateRxns.equal_range( const_cast<SpeciesTypePtr>(A) );
            std::pair< typename ParticipatingSpeciesRxnMap::iterator, typename ParticipatingSpeciesRxnMap::iterator> rangeIterB =  doubleSubstrateRxns.equal_range( const_cast<SpeciesTypePtr>(B) );

            std::set<ReactionTypePtr> rxnsWithASubstrate;
            std::set<ReactionTypePtr> rxnsWithBSubstrate;

            for( typename ParticipatingSpeciesRxnMap::iterator iterA = rangeIterA.first; iterA != rangeIterA.second; ++iterA)
            {
                rxnsWithASubstrate.insert( iterA->second );
            }

            for( typename ParticipatingSpeciesRxnMap::iterator iterB = rangeIterB.first; iterB != rangeIterB.second; ++iterB)
            {
                rxnsWithBSubstrate.insert( iterB->second );
            }

            std::set_intersection( rxnsWithASubstrate.begin(), rxnsWithASubstrate.end(),
                                   rxnsWithBSubstrate.begin(), rxnsWithBSubstrate.end(),
                                   std::back_inserter( reactionVector) );
                                  
        }
        else
        {
            SpeciesTypePtr APrime = const_cast<SpeciesTypePtr>(A);
            
            std::pair< typename ParticipatingSpeciesRxnMap::iterator, typename ParticipatingSpeciesRxnMap::iterator> rangeIter = \
                autoDimerizationRxns.equal_range( APrime );

            for( typename ParticipatingSpeciesRxnMap::iterator iter = rangeIter.first; iter != rangeIter.second; ++iter)
            {
                reactionVector.push_back( iter->second);
            }
        }
    
        return ( reactionVector.end() != originalEnd );
    }


    template <typename speciesT, typename reactionT>
    bool ReactionNetworkDescription<speciesT, reactionT>::checkSpeciesIsKnown( const std::string& speciesName ) const
    {
        return !( theSpeciesListCatalog.find( &speciesName ) == theSpeciesListCatalog.end() );
        
    }

//////////////////////////////////////////////////
    template <typename speciesT, typename reactionT>
    typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesCatalog&
    ReactionNetworkDescription<speciesT, reactionT>::getSpeciesCatalog()
    {
        return theSpeciesListCatalog;
    }
        
        
    template <typename speciesT, typename reactionT>
    const typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesCatalog&
    ReactionNetworkDescription<speciesT, reactionT>::getSpeciesCatalog() const
    {
        return theSpeciesListCatalog;
    }
        
    template <typename speciesT, typename reactionT>
    const typename ReactionNetworkDescription<speciesT, reactionT>::ReactionList&
    ReactionNetworkDescription<speciesT, reactionT>::getReactionList() const
    {
        return theCompleteReactionList;
    }

    template <typename speciesT, typename reactionT>
    const typename ReactionNetworkDescription<speciesT, reactionT>::ReactionList&
    ReactionNetworkDescription<speciesT, reactionT>::getDeltaReactionList() const
    {
        return theDeltaReactionList;
    }


    template <typename speciesT, typename reactionT>
    const typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesList&
    ReactionNetworkDescription<speciesT, reactionT>::getDeltaSpeciesList() const
    {
        return theDeltaSpeciesList;
    }
        
    // These two functions are the interface that reaction generators use
    // to record their species.  They add the (speciesName, speciesPtr)
    // and (reactionName, reactionPtr) entries to the catalog respectively.
    // If the object being recorded is new, we also record it as a hit in the
    // total number of species/reactions as well as the delta number of species/
    // reactions.

    
    template <typename speciesT, typename reactionT>
    bool
    ReactionNetworkDescription<speciesT, reactionT>::recordSpecies( typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypePtr pSpecies )
    {

        // BUGFIX for E3.  Hoho.
        // Get the name and check to see if it's been recorded yet.  
        std::string specName = pSpecies->getName();
        Registry::iterator specPosition;
        std::pair<Registry::iterator, bool> ret;
        ret = SpeciesRegistry.insert(specName);
        if (!ret.second) return false;
        // End BUGFIX

        SpeciesHandle speciesHandle( new SpeciesTag( pSpecies->getTag() ) );

        if ( theSpeciesListCatalog.find( speciesHandle ) == theSpeciesListCatalog.end() )
        {

            SpeciesIDPtr speciesIDPtr( new SpeciesID( pSpecies->getName() ) );

            theSpeciesListCatalog.insert( std::make_pair( speciesHandle, pSpecies ) );
            speciesTagToSpeciesIDChart.insert( std::make_pair( speciesHandle, speciesIDPtr ) );
            speciesIDToSpeciesTagChart.insert( std::make_pair( speciesIDPtr, speciesHandle ) );

            theDeltaSpeciesList.push_back( pSpecies );
            return true;
        }

        delete speciesHandle;
        return false;
    }
        

    template <typename speciesT, typename reactionT>
    bool
    ReactionNetworkDescription<speciesT, reactionT>::recordSpecies( typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypePtr pSpecies, 
                                                                    typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesID& name )
    {
        SpeciesHandle speciesHandle( new SpeciesID( pSpecies->getTag() ) );
            
        // Put the speciesID into the name.
        name = *speciesHandle;
            
        if ( theSpeciesListCatalog.find( speciesHandle ) == theSpeciesListCatalog.end() )
        {
            SpeciesIDPtr speciesIDPtr( new SpeciesID( pSpecies->getName() ) );
            
            theSpeciesListCatalog.insert( std::make_pair( speciesHandle, pSpecies ) );
            speciesTagToSpeciesIDChart.insert( std::make_pair( speciesHandle, speciesIDPtr ) );
            speciesIDToSpeciesTagChart.insert( std::make_pair( speciesIDPtr, speciesHandle ) );

            theDeltaSpeciesList.push_back( pSpecies );
            return true;
        }

        delete speciesHandle;
        return false;
    }
        

    template <typename speciesT, typename reactionT>
    bool
    ReactionNetworkDescription<speciesT, reactionT>::recordReaction( typename ReactionNetworkDescription<speciesT, reactionT>::ReactionTypePtr pRxn )
    {
        // Get the name and check to see if it's been recorded yet.  
        std::string rxnName = pRxn->getName();
        Registry::iterator rxnPos;
        std::pair<Registry::iterator, bool> ret;
        ret = ReactionRegistry.insert(rxnName);

        if (!ret.second)
        {
            if (pRxn->getArity() == 1)
            {
                double originalRxnRate = reactionChart[rxnName]->getRate();
                double rxnRate = pRxn->getRate();
                reactionChart[rxnName]->setRate( originalRxnRate + rxnRate);
            }
            return false;
        }

        int rxnArity( pRxn->getArity() );
        if ( rxnArity > 2 )
        {
            throw utl::FatalXcpt("Error in reactionNetworkCatalog::recordReaction.  Reaction passed in is non-standard");
        }
            
        theCompleteReactionList.push_back( pRxn );
        theDeltaReactionList.push_back( pRxn );
        reactionChart[rxnName] = pRxn;

        switch ( rxnArity )
        {
        case 0:
            // Register in the map of creation reactions
            zeroSubstrateRxns.push_back( pRxn );
            break;
                
        case 1:
            // Register in a multi map of 1->? reactions
            SpeciesTypePtr pOnlySubstrate;
            pOnlySubstrate = pRxn->getReactants().begin()->first;
            singleSubstrateRxns.insert( std::make_pair( pOnlySubstrate, pRxn ) );
            break;

        case 2:
            if ( pRxn->getReactants().begin()->second == 1)
            {

                SpeciesTypePtr theSpeciesPtr( NULL );

                // In this case, the reaction is of type A + B -> ? where A != B
                for( typename fnd::basicReaction<SpeciesType>::multMap::const_iterator reactantIter = pRxn->getReactants().begin();
                     reactantIter != pRxn->getReactants().end();
                     ++reactantIter)
                {
                    theSpeciesPtr = reactantIter->first;
                    doubleSubstrateRxns.insert( std::make_pair( theSpeciesPtr, pRxn ) );
                }
            }
            else
            {
                // Rxn is of type A + A -> ?.
                autoDimerizationRxns.insert( std::make_pair( pRxn->getReactants().begin()->first, pRxn) );
            }
            break;

        default:
            throw utl::FatalXcpt("Error in reactionNetworkCatalog::recordReaction.  Reaction is non-standard. (jfjhnnnnnfhd348)"  );
            break;
        }
            
        return true;
    }
        
    // These are not used by anyone at the moment.  It might be better to use them instead
    // of the plain old recordSpecies/recordReaction functions, but who knows.

    template <typename speciesT, typename reactionT>
    void 
    ReactionNetworkDescription<speciesT, reactionT>::mustRecordSpecies( typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypePtr pSpecies ) throw( utl::xcpt )
    {
        if ( !recordSpecies( pSpecies ) )
        {
            throw DuplicatedCatalogEntryXcpt( pSpecies->getName() );
        }
    }

    template <typename speciesT, typename reactionT>
    void 
    ReactionNetworkDescription<speciesT, reactionT>::mustRecordSpecies( typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTypePtr pSpecies, 
                                                                        typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTag& refName ) throw( utl::xcpt )
    {
        if ( !recordSpecies( pSpecies, refName ) )
        {
            throw DuplicatedCatalogEntryXcpt( pSpecies->getName() );
        }
    }
        


    template <typename speciesT, typename reactionT>
    void
    ReactionNetworkDescription<speciesT, reactionT>::mustRecordReaction( typename ReactionNetworkDescription<speciesT, reactionT>::ReactionTypePtr pRxn ) throw( utl::xcpt )
    {
        if ( recordReactions( pRxn ) )
        {
            return;
        }
        else
        {
            throw DuplicatedCatalogEntryXcpt( pRxn->getName() );
        }
            
    }
        

    template <typename speciesT, typename reactionT>
    unsigned int
    ReactionNetworkDescription<speciesT, reactionT>::getTotalNumberSpecies() const
    {
        return theSpeciesListCatalog.size();
    }


    template <typename speciesT, typename reactionT>        
    unsigned int
    ReactionNetworkDescription<speciesT, reactionT>::getTotalNumberReactions() const
    {
        return theCompleteReactionList.size();
    }
        
    // The getDeltaNumber(Species|Reactions) functions return the number
    // of those objects created since the last time the recordCurrentState
    // funsource ction was source called.

    template <typename speciesT, typename reactionT>
    unsigned int
    ReactionNetworkDescription<speciesT, reactionT>::getNumberDeltaReactions() const
    {
        return theDeltaReactionList.size();
    }
        

    template <typename speciesT, typename reactionT>
    unsigned int
    ReactionNetworkDescription<speciesT, reactionT>::getNumberDeltaSpecies() const
    {
        return theDeltaSpeciesList.size();
    }
        

    template <typename speciesT, typename reactionT>
    void 
    ReactionNetworkDescription<speciesT, reactionT>::resetCurrentState()
    {
        theDeltaSpeciesList.clear();
        theDeltaReactionList.clear();
    }
        

    template <typename speciesT, typename reactionT>
    void 
    ReactionNetworkDescription<speciesT, reactionT>::incrementNetworkBySpeciesTag( const typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTag& rName ) throw( utl::xcpt )
    {
        SpeciesCatalogCIter iter = theSpeciesListCatalog.find( &rName );
            
        if ( iter != theSpeciesListCatalog.end() )
        {
            iter->second->expandReactionNetwork();
        }
        else
        {
            throw NoSuchSpeciesXcpt( rName );
        }
    }


    template <typename speciesT, typename reactionT>
    ReactionNetworkDescription<speciesT, reactionT>::ReactionNetworkDescription()
        :
        theDeltaSpeciesList(),
        theDeltaReactionList()
    {}
        

     template <typename speciesT, typename reactionT>
    ReactionNetworkDescription<speciesT, reactionT>::~ReactionNetworkDescription()
    {
        // We don't memory manage any SpeciesType* or ReactionType*, but we do memory
        // manage the string* in theSpeciesListCatalog.
            

        for( typename SpeciesNameMap::iterator iter = speciesTagToSpeciesIDChart.begin();
             iter != speciesTagToSpeciesIDChart.end();
             ++iter)
        {
            delete iter->first;
            delete iter->second;
        }
    }



    template <typename speciesT, typename reactionT>
    typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesID 
    ReactionNetworkDescription<speciesT, reactionT>::convertSpeciesTagToSpeciesID( const typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTag& rTag ) const 
        throw( utl::xcpt )
    {
        typename SpeciesNameMap::const_iterator iter = speciesTagToSpeciesIDChart.find( &rTag );
        if( iter == speciesTagToSpeciesIDChart.end() ) throw NoSuchSpeciesXcpt( rTag );
        
        return (*iter->second);
    }

    template <typename speciesT, typename reactionT>
    typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesTag 
    ReactionNetworkDescription<speciesT, reactionT>::convertSpeciesIDToSpeciesTag( const typename ReactionNetworkDescription<speciesT, reactionT>::SpeciesID& rID) const 
        throw( utl::xcpt )
    {

        typename SpeciesNameMap::const_iterator iter = speciesIDToSpeciesTagChart.find( &rID );
        if( iter == speciesIDToSpeciesTagChart.end() ) throw NoSuchSpeciesXcpt( rID );
        
        return (*iter->second);
    }

}
