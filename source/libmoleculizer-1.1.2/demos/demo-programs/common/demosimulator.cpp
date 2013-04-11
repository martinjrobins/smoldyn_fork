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
#include  "utl/defs.hh"
#include "utl/utility.hh"
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
using std::cout;
using std::cerr;
using std::endl;


void SimpleSimulator::createModelFromFile( const std::string& modelFile, std::map<std::string, int>& model )
{
    model.clear();

    std::ifstream inputfile;
    inputfile.open( modelFile.c_str() );

    std::string current_line;
    while ( std::getline( inputfile, current_line ) )
    {
        std::vector<std::string> tokenVector;

        utl::tokenize( current_line, tokenVector, " " );
        if ( tokenVector.size() == 0 ) continue;


        if ( tokenVector.size() != 2 )
        {
            if ( tokenVector.size() == 1 && tokenVector[0] == "" ) continue;

            std::cerr << "Error, bad line '" << current_line << "'.\n";
            int num = 1;

            for(unsigned int tokenVectorNdx = 0;
                tokenVectorNdx != tokenVector.size();
                ++tokenVectorNdx)
            {
                std::cerr << num++ << ":\t" << tokenVector[tokenVectorNdx] << '\n';
            }
            
            assert( tokenVector.size() == 2 );

        }

        std::string name = tokenVector[0];
        std::string numAsString = tokenVector[1];
        int population;

        utl::from_string( population, numAsString );
        model.insert( std::make_pair( name, population ) );
    }
}

SimpleSimulator::SimpleSimulator()
    :
    ptrSpeciesReactionGenerator(new mzr::moleculizer ),
    rulesLoaded( false )
    
{
    srand( time( NULL ) );
}

SimpleSimulator::~SimpleSimulator()
{
    delete ptrSpeciesReactionGenerator;
}





// SimpleSimulator::SimpleSimulator( std::string rulesfile,
//                                   std::string modelfile )
// {
//     srand( time( NULL ) );
//     attachRuleFile( rulesfile );

//     std::cout << "SS: State after attaching rules, before attaching Model:" << std::endl;
//     ptrSpeciesReactionGenerator->printAll();

//     std::cout << "---------(END)" << std::endl;

//     attachModelFile( modelfile );

//     std::cout << "---------(BEGIN)" << std::endl;

//     std::cout << "SS: State after attaching rules, model" << std::endl;
//     ptrSpeciesReactionGenerator->printAll();

//     std::cout << "---------(END)" << std::endl;
// }


void 
SimpleSimulator::loadRules( std::string rulesFile )
{
    // This attaches the rules to moleculizer.
    
    if ( rulesLoaded )
    {
        releaseRules();
    }

    ptrSpeciesReactionGenerator->loadXmlFileName( rulesFile );
}

void 
SimpleSimulator::releaseRules()
{
    delete ptrSpeciesReactionGenerator;
    ptrSpeciesReactionGenerator = new mzr::moleculizer;
}


void 
SimpleSimulator::loadModel( std::string modelfile )
{
    // This function takes the contentents of model file and puts it into 
    theModel.clear();

    std::map<std::string, int> theNewModel;
    createModelFromFile( modelfile, theNewModel );

    if ( !assertModelValidity( theNewModel ) )
    {
        std::cerr << "Error, the model is not valid.  Crashing." << std::endl;
        exit( 1 );
    }

    theModel.swap( theNewModel );
}


void SimpleSimulator::attachRuleFile( std::string rulesfile )
{
    if ( ptrSpeciesReactionGenerator->getModelHasBeenLoaded() )
    {
        std::cerr << "Modelfile has already been loaded.  Error." << std::endl;
        exit( 1 );
    }

    ptrSpeciesReactionGenerator->loadXmlFileName( rulesfile );
}


void SimpleSimulator::attachModelFile( std::string modelfile )
{

    theModel.clear();

    std::map<std::string, int> theNewModel;
    createModelFromFile( modelfile, theNewModel );

    if ( !assertModelValidity( theNewModel ) )
    {
        std::cerr << "Error, the model is not valid.  Crashing." << std::endl;
        exit( 1 );
    }

    theModel.swap( theNewModel );
}


bool SimpleSimulator::assertModelValidity( const std::map<std::string, int>& model )
{
    if ( model.size() == 0 )
    {
        cerr << "Error.  No species are present in the model." << endl;
        return false;
    }

    for( std::map<std::string, int>::const_iterator modelIter = model.begin();
         modelIter != model.end();
         ++modelIter)
    {
        // Find out if this is a legal name.
        try
        {
            ptrSpeciesReactionGenerator->getSpeciesWithUniqueID( modelIter->first );
        }
        catch ( mzr::IllegalNameXcpt )
        {
            std::cerr << "Illegal name: " << modelIter->first << std::endl;
            return false;
        }
        catch ( ... )
        {
            return false;
        }
    }
    
    return true;
}

void SimpleSimulator::initialize()
{
    

    for( std::map<std::string, int>::const_iterator modelIter = theModel.begin();
         modelIter != theModel.end();
         ++modelIter)
    {
        if ( modelIter->second > 0 )
        {
            // First we assume that the name present is defined in the rules file...
            if ( ptrSpeciesReactionGenerator->nameIsUserName( modelIter->first ) )
            {
                std::string mangledName( ptrSpeciesReactionGenerator->convertUserNameToSpeciesID( modelIter->first ) );
                ptrSpeciesReactionGenerator->incrementNetworkBySpeciesTag( mangledName );
            }
            else
            {
                ptrSpeciesReactionGenerator->incrementNetworkBySpeciesTag( modelIter->first );
            }
        }
    }
}


void SimpleSimulator::executeReaction( const mzr::moleculizer::ReactionType* ptrRxn )
{
    std::cout << "Executing: " << ptrRxn->getName() << std::endl;

    for( mzr::moleculizer::ReactionType::multMap::const_iterator subIter = ptrRxn->getReactants().begin();
         subIter != ptrRxn->getReactants().end();
         ++subIter)
    {
        std::cout << '-' << subIter->second << ' ' << subIter->first->getName() << '\n';

        std::string name = subIter->first->getName();
        theModel[ name ] -= subIter->second;
    }

    for( mzr::moleculizer::ReactionType::multMap::const_iterator prodIter = ptrRxn->getProducts().begin();
         prodIter != ptrRxn->getProducts().end();
         ++prodIter)
    {
        std::cout << '+' << prodIter->second << ' ' << prodIter->first->getName() << '\n';

        std::string name = prodIter->first->getName();

        if ( theModel.find( name ) == theModel.end() )
        {
            std::cout << "Creating new species '" << name << "'.";
            theModel[ name ] = prodIter->second;
        }
        else
        {
            theModel[ name ] += prodIter->second;
        }
    }

    std::cout << std::endl;

}


void SimpleSimulator::printState() const
{
}
