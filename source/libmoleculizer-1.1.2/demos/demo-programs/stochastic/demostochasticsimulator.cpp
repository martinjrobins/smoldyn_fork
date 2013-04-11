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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//


#include <iostream>
#include <algorithm>
#include "demostochasticsimulator.hpp"


SimpleStochasticSimulator::SimpleStochasticSimulator( std::string rulesfile, std::string modelfile )
        :
        SimpleSimulator()
{
    loadRules(rulesfile);
    loadModel(modelfile);

    std::cout << "Prior to initialization there are " 
              << getNumSpecies() << " species and " << getNumRxns() << " reactions" << std::endl;

    recordNewReactions();

    initialize();
    
    std::cout << "After initialization there are " 
              << getNumSpecies() << " species and " << getNumRxns() << " reactions" << std::endl;
}

void SimpleStochasticSimulator::recordNewReactions()
{
    std::copy( ptrSpeciesReactionGenerator->theDeltaReactionList.begin(),
               ptrSpeciesReactionGenerator->theDeltaReactionList.end(),
               std::back_inserter( reactions ) );

    if ( ptrSpeciesReactionGenerator->theDeltaReactionList.size() > 0 )
    {
        std::cout << "Adding to StochasticSimulator: " << std::endl;
    }

    for( mzr::moleculizer::ReactionList::const_iterator rxnIter = ptrSpeciesReactionGenerator->theDeltaReactionList.begin();
         rxnIter != ptrSpeciesReactionGenerator->theDeltaReactionList.end();
         ++rxnIter)
    {
        printRxn( *rxnIter );
    }

    assert( reactions.size() == ptrSpeciesReactionGenerator->theCompleteReactionList.size() );

    ptrSpeciesReactionGenerator->resetCurrentState();
}

mzr::mzrReaction* SimpleStochasticSimulator::calculateReactionToFire()
{

    // Get a list of potential reactions, in this case a list of reactions
    // that have all positive numbers of substrates.

    std::vector<mzr::moleculizer::ReactionTypePtr> potentialReactions;
    getReactionsWithPositivePropensity( potentialReactions );

    if ( potentialReactions.size() == 0 )
    {
        std::cout << "There are no reactions with positive propensity..." << std::endl;
        return NULL;
    }


    // Pick a random reaction.
    int randomIndex = rand() % potentialReactions.size();
    mzr::moleculizer::ReactionTypePtr randomReactionPtr = potentialReactions[ randomIndex ];

    executeReaction( randomReactionPtr );

    return randomReactionPtr;
}


void SimpleStochasticSimulator::getReactionsWithPositivePropensity( std::vector<mzr::mzrReaction*>& okReactions )
{
    okReactions.clear();

    for( std::vector<mzr::mzrReaction*>::iterator rxnIter = reactions.begin();
         rxnIter != reactions.end();
         ++rxnIter)
    {
        if ( reactionHasPositiveSubstrates( *rxnIter ) )
        {
            okReactions.push_back( *rxnIter );
        }
    }
}

bool SimpleStochasticSimulator::reactionHasPositiveSubstrates( const mzr::mzrReaction* rxnPtr )
{

    for( mzr::moleculizer::ReactionType::multMap::const_iterator iter = rxnPtr->getReactants().begin();
         iter != rxnPtr->getReactants().end();
         ++iter)
    {
        std::string substrateName( iter->first->getName() );

        if ( theModel.find( substrateName ) == theModel.end() )
        {
            return false;
        }

        if ( theModel[ substrateName ] < iter->second ) return false;
    }

    return true;

}

void SimpleStochasticSimulator::singleStep()
{
    mzr::mzrReaction* reaction = calculateReactionToFire();

    if ( reaction )
    {
        executeReaction( reaction );
    }

    recordNewReactions();

    return;
}


