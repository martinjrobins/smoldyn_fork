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


#include "demoparticlesimulator.hpp"

void SimpleParticleSimulator::singleStep()
{
    doSingleUnaryReaction();

    std::vector<std::string> stringPtrVector;

    for( std::map<std::string, int>::const_iterator specIter = theModel.begin();
         specIter != theModel.end();
         ++specIter)
    {
        if ( specIter->second > 0 )
        {
            stringPtrVector.push_back( specIter->first );
        }

    }

    // Pick two particles at random.

    std::string speciesNameOne, speciesNameTwo;
    bool illegalpick = true;

    do
    {
        int firstIndex = rand() % stringPtrVector.size();
        int secondIndex = rand() % stringPtrVector.size();
        speciesNameOne = stringPtrVector[ firstIndex ];
        speciesNameTwo = stringPtrVector[ secondIndex ];

        if ( speciesNameOne == speciesNameTwo && theModel[speciesNameOne] >= 2 )
        {
            illegalpick = false;
        }
        else
        {
            illegalpick = false;
        }
    }
    while ( illegalpick );


    std::cout << "Collision between " << speciesNameOne << " and " << speciesNameTwo << std::endl;

    // Look up these species in the moleculizer list.
    mzr::moleculizer::SpeciesTypeCptr speciesPtrOne = ptrSpeciesReactionGenerator->getSpeciesWithUniqueID( speciesNameOne );
    mzr::moleculizer::SpeciesTypeCptr speciesPtrTwo = ptrSpeciesReactionGenerator->getSpeciesWithUniqueID( speciesNameTwo );
    
    std::vector<mzr::moleculizer::ReactionTypeCptr> reactionVector;
    ptrSpeciesReactionGenerator->findReactionWithSubstrates( speciesPtrOne,
                                                             speciesPtrTwo,
                                                             reactionVector );

    if ( reactionVector.size() == 0 )
    {
        std::cout << "\tNORES \t-- (No reactions between)." << std::endl << std::endl;
        return;
    }


    std::cout << "\t!!! reaction." << std::endl;
    executeReaction( reactionVector[ rand() % reactionVector.size()] );
}


void SimpleParticleSimulator::doSingleUnaryReaction()
{

    std::vector<std::string> stringPtrVector;


    for( std::map<std::string, int>::const_iterator specIter = theModel.begin();
         specIter != theModel.end();
         ++specIter)
    {
        if ( specIter->second > 0 )
        {
            stringPtrVector.push_back( specIter->first );
        }

    }

    std::string particleName = stringPtrVector[rand() % stringPtrVector.size()];

    std::cout << "Possible unary reaction: " << '\n'
    << '\t' << particleName << std::endl;

    mzr::moleculizer::SpeciesTypeCptr particlePtr = ptrSpeciesReactionGenerator->getSpeciesWithUniqueID( particleName );

    std::vector<mzr::moleculizer::ReactionTypeCptr> reactionVector;
    ptrSpeciesReactionGenerator->findReactionWithSubstrates( particlePtr,
                                                             reactionVector );

    if ( reactionVector.size() == 0 )
    {
        std::cout << "\tNORES \t-- (No unary reactions).\n" << std::endl;
        return;
    }

    std::cout << "\t!!! reaction." << std::endl;

    mzr::moleculizer::ReactionTypeCptr rxn = reactionVector[ rand() % reactionVector.size()];

    executeReaction( rxn );

}


void SimpleParticleSimulator::displayNewSpeciesMsg( const mzrSpecies* mzrSpec)  const
{
    std::cout << "$$$New species ' " << mzrSpec->getName() << "' created." << std::endl;
}


void SimpleParticleSimulator::printRxn( const mzr::mzrReaction* pMzrRxn) const
{
    std::cout << pMzrRxn->getName() << "\n\t"
              << "kA = " << pMzrRxn->getRate() << std::endl;
}


void SimpleParticleSimulator::printSpec( const mzr::mzrSpecies* pSpec) const
{
    std::cout << pSpec->getName() << "\n\t" 
              << "mass = " << pSpec->getWeight() << std::endl;

}
