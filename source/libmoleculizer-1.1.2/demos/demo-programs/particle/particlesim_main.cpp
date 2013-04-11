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

#include <string>
#include <iostream>
#include "utl/arg.hh"
#include "mzr/moleculizer.hh"
#include "demoparticlesimulator.hpp"

using namespace std;

void
processCommandLineArgs( int argc, char* argv[], std::string& theFileName, std::string& modelFile, int& numberIters );

void displayHelpAndExitProgram();

int main( int argc, char* argv[] )
{

    try
    {
        int numberIters = 500;
        std::string modelfile;
        std::string rulesfile;
        processCommandLineArgs( argc, argv, rulesfile, modelfile, numberIters );

        SimpleParticleSimulator theSim( rulesfile, modelfile );

        for ( int ii = 0; ii != numberIters; ++ii )
        {
            theSim.singleStep();
        }
        
        theSim.printAll();

        return 0;
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}

void processCommandLineArgs( int argc, char* argv[], std::string& rulesFile, std::string& modelFile, int& numberIters )
{

    bool rulesFound( false );
    bool modelFound( false );
    // Skip the command name
    argc--;
    argv++;

    if ( argc == 0 )
    {
        displayHelpAndExitProgram();
    }

    while ( 0 < argc )
    {
        std::string arg( *argv );
        argv++;
        argc--;

        if ( arg == "--help" )
        {
            displayHelpAndExitProgram();
        }

        if ( arg == "--rules" )
        {
            rulesFile = utl::mustGetArg( argc, argv );
            rulesFound = true;
        }

        if ( arg == "--model" )
        {
            modelFile = utl::mustGetArg( argc, argv );
            modelFound = true;
        }

        if ( arg == "-n" )
        {
            std::string numberString = utl::mustGetArg( argc, argv );
            utl::from_string( numberIters, numberString );
        }
    }

    if ( !modelFound || !rulesFound )
    {
        std::cerr << "Error, both a --rules and a --model parameter must be provided." << endl;
        exit( 1 );
    }

}

void displayHelpAndExitProgram()
{

    cout << "Usage: particlesim_demo --model modelfile --rules rulesfile" << endl;

    cout << "This is a demonstration program that demonstrates how libmoleculizer can be used as " << endl;
    cout << "a component in a particle-based simulator." << endl;

    cout << "Moleculizer should have come with associated documentation.  Please read it for more details." << endl;
    cout << "\tNathan Addy <addy@molsci.org>\n\tSeptember 18, 2008." << endl;

    exit( 0 );
}
