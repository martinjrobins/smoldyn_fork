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

#include "demosimulator.hpp"
#include "mzr/mzrSpecies.hh"
#include "mzr/mzrReaction.hh"
using namespace mzr;

class SimpleParticleSimulator : public SimpleSimulator
{
public:
    SimpleParticleSimulator( std::string rulesfile, std::string modelfile )
    {

        loadRules( rulesfile );
        loadModel( modelfile );

        initialize();
    }

    void singleStep();
    virtual void initialize()
    {
        SimpleSimulator::initialize();
    }

    void printAll() const
    {
        std::cout << "Particle Simulator State" << std::endl;
        printAllSpecies();
        printAllRxns();
    }

    void printAllSpecies() const
    {

        for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = ptrSpeciesReactionGenerator->getSpeciesCatalog().begin();
             specIter != ptrSpeciesReactionGenerator->getSpeciesCatalog().end();
             ++specIter)
        {
            printSpec( specIter->second );
        }
    }
    
    void printAllRxns() const
    {

        for( mzr::moleculizer::ReactionList::const_iterator rxnIter = ptrSpeciesReactionGenerator->getReactionList().begin();
             rxnIter != ptrSpeciesReactionGenerator->getReactionList().end();
             ++rxnIter)
        {
            printRxn( *rxnIter );
        }
    }


    void printRxn(const mzr::mzrReaction* mzrReact) const;
    void printSpec(const mzr::mzrSpecies* speciesName) const;
    
private:
    void doSingleUnaryReaction();
    void displayNewSpeciesMsg(const mzr::mzrSpecies* mzrSpec) const;
    int numReactions;
};
