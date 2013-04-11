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


#include <string>
#include <iostream>
#include <iterator>
#include "utl/arg.hh"
#include "mzr/unitsMgr.hh"
#include "mzr/moleculizer.hh"
#include "plex/mzrPlexFamily.hh"

using namespace std;

enum RunMode { Full = 0,
               BoundedRun = 1,
               Iterations = 2 };


struct inputArgsStruct
{
    bool fileDefined;
    bool fileIsXml;
    bool fileIsRules;
    std::string fileName;

    bool hasOutputFile;
    std::string outputFile;

    int maxSpecies;
    int maxRxns;
    int numberIters;

    bool verbose;
    bool quiet;

    int runMode;


    inputArgsStruct()
        :
        fileDefined( false ),
        fileIsXml( false ),
        fileIsRules(false ),
        fileName( "" ),
        hasOutputFile( false ),
        outputFile( "" ),
        maxSpecies( -1 ),
        maxRxns( -1 ),
        verbose( false ),
        quiet( false ),
        runMode( Full )
    {}
};


void
processCommandLineArgs( int argc, char* argv[], inputArgsStruct& inputArgs);

void
displayHelpAndExitProgram();

bool getUninitializedSpecies( const mzr::moleculizer& moleculizerRef, std::string& speciesName);

void printAllSpeciesStreams(mzr::moleculizer& theMolzer);

void runFullRunMoleculizer( mzr::moleculizer& mzr, const inputArgsStruct& inputArgsStruct);
void runIterationsMoleculizer( mzr::moleculizer& mzr, const inputArgsStruct& inputArgsStruct);
void runBoundedRunMoleculizer( mzr::moleculizer& mzr, const inputArgsStruct& inputArgsStruct);


void printStreamByName( mzr::moleculizer& refMolzer, const std::string& streamName);
void printStreamByTag( mzr::moleculizer& refMolzer, const std::string& streamName);

void printAllSpeciesByName(mzr::moleculizer& theMolzer);
void printAllSpeciesByID(mzr::moleculizer& theMolzer, std::string str = "");
void printAllReactions( mzr::moleculizer& refMolzer);
void printAllPlexFamilies( mzr::moleculizer& theMolzer, std::string str = "");

mzr::moleculizer::CachePosition
createBoundedNetwork(mzr::moleculizer& refMolzer, int maxSpec, int maxRxns);

void createFullNetwork(mzr::moleculizer& refMolzer);
void doNIterations(mzr::moleculizer& refMolzer, int number);

void ensureConsistentInputArgs( const inputArgsStruct& inputArgs)
{
    return;
}

// Nasty little global variable.
mzr::moleculizer::CachePosition pos;

int main(int argc, char* argv[])
{

  inputArgsStruct inputArgs;
  processCommandLineArgs(argc, argv, inputArgs);
  ensureConsistentInputArgs( inputArgs );

  mzr::moleculizer theMoleculizer;
  
  if (inputArgs.fileIsXml)
  {
      theMoleculizer.loadXmlFileName( inputArgs.fileName );
  }
  else if (inputArgs.fileIsRules)
  {
      theMoleculizer.loadCommonRulesFileName( inputArgs.fileName );
  }
  else
  {
      std::cerr << "Error, no filename was supplied." << std::endl;
  }


  if ( ! inputArgs.quiet )
  {
      std::cout << "There are initially " << theMoleculizer.getTotalNumberSpecies() << " species and " 
                << theMoleculizer.getTotalNumberReactions() << " reactions in " << theMoleculizer.getNumberOfPlexFamilies() << " plex families." << std::endl;

      if( inputArgs.verbose )
      {
          std::cout << "########################################" << std::endl;
          printAllSpeciesByID(theMoleculizer);
          std::cout << "########################################" << std::endl;
          printAllPlexFamilies( theMoleculizer );
          std::cout << "########################################" << std::endl;
      }

  }

  


  if (inputArgs.runMode == Full)
  {
      runFullRunMoleculizer( theMoleculizer, inputArgs);
  }
  else if (inputArgs.runMode == Iterations )
  {
      runIterationsMoleculizer( theMoleculizer, inputArgs);
  }
  else if (inputArgs.runMode == BoundedRun )
  {
      runBoundedRunMoleculizer( theMoleculizer, inputArgs);
  }
  else
  {
      std::cerr << "Unknown run mode in input arguments." <<std::endl;
      throw std::exception();
  }


  if (!inputArgs.quiet)
  {
      std::cout << "################################################" << '\n';
      std::cout << "After processing: \n" ;

      if (inputArgs.runMode == Full || inputArgs.runMode == Iterations)
      {
          std::cout << "There are " 
                    << theMoleculizer.getTotalNumberReactions() << " reactions and " 
                    << theMoleculizer.getTotalNumberSpecies() << " species in " 
                    << theMoleculizer.getNumberOfPlexFamilies() << " families in the network." << std::endl;
      }
      else
      {
          int numRxns = std::distance(theMoleculizer.theDeltaReactionList.begin(), pos.second);
          int numSpecies = std::distance(theMoleculizer.theDeltaSpeciesList.begin(), pos.first);
          std::cout << "There are " 
                    << numSpecies << " species and " 
                    << numRxns<< " reactions in the bounded network.  " << '\n'
                    << "Should be bounded by " << inputArgs.maxSpecies << " species and "
                    << inputArgs.maxRxns << " reactions."
                    << std::endl;
      }

      if (inputArgs.verbose)
      {
          std::cout << "################################################" << '\n';
          printAllSpeciesStreams(theMoleculizer);
          std::cout << "################################################" << '\n';
          printAllSpeciesByName(theMoleculizer);
          std::cout << "################################################" << '\n';
          printAllSpeciesByID(theMoleculizer);
          std::cout << "################################################" << '\n';
          printAllReactions(theMoleculizer);
          std::cout << "################################################" << '\n';
      }
  }

  if ( inputArgs.hasOutputFile )
  {
        if (inputArgs.runMode != BoundedRun)
        {
            theMoleculizer.writeOutputFile(inputArgs.outputFile, true);
        }
        else
        {
            theMoleculizer.writeOutputFile(inputArgs.outputFile, true, pos);
        }
  }

  return 0;
  
}

void printAllSpeciesStreams(mzr::moleculizer& refMolzer)
{
    std::vector<std::string> theStreams;
    refMolzer.getSpeciesStreams( theStreams );
    std::cout << " There are " << theStreams.size() << " streams." << std::endl;

    for( std::vector<std::string>::const_iterator strIter = theStreams.begin();
         strIter != theStreams.end();
         ++strIter)
    {
        printStreamByName( refMolzer, *strIter);
        std::cout << "################################################" << '\n';
        printStreamByTag( refMolzer, *strIter);
    }
}

void printAllReactions( mzr::moleculizer& refMolzer)
{
    for( std::list<mzr::mzrReaction*>::const_iterator iter = refMolzer.getReactionList().begin();
         iter != refMolzer.getReactionList().end();
         ++iter)
    {
        std::cout << (*iter)->getName()  << '\t' << (*iter)->getRate() << std::endl;
    }
}

void printStreamByName( mzr::moleculizer& refMolzer, const std::string& streamName)
{
    std::cout << "'" << streamName << "'\n[\n" ;

    std::vector<const mzr::mzrSpecies*> theSpecies;

    refMolzer.getSpeciesInSpeciesStream( streamName, theSpecies);

    for( std::vector<const mzr::mzrSpecies*>::const_iterator specIter = theSpecies.begin();
         specIter != theSpecies.end();
         ++specIter)
    {
        std::cout << streamName << "@@" << (*specIter)->getName() << '\n';
    }

    std::cout << ']' << std::endl;
}

void printStreamByTag( mzr::moleculizer& refMolzer, const std::string& streamName)
{
    std::cout << "'" << streamName << "'\n[\n" ;

    std::vector<const mzr::mzrSpecies*> theSpecies;

    refMolzer.getSpeciesInSpeciesStream( streamName, theSpecies);

    for( std::vector<const mzr::mzrSpecies*>::const_iterator specIter = theSpecies.begin();
         specIter != theSpecies.end();
         ++specIter)
    {
        std::cout << streamName << "@@" << (*specIter)->getTag() << '\n';
    }

    std::cout << ']' << std::endl;
}

bool getUninitializedSpecies( const mzr::moleculizer& moleculizerRef, std::string& speciesName)
{
    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = moleculizerRef.getSpeciesCatalog().begin();
         specIter != moleculizerRef.getSpeciesCatalog().end();
         ++specIter)
    {
        if(!specIter->second->hasNotified())
        {
            speciesName = *(specIter->first);
            return true;
        }
    }

    return false;
}



void processCommandLineArgs( int argc, char* argv[], inputArgsStruct& theInputArgs)
{

    bool file( false );

    // Skip the command name
    argc--;
    argv++;

    if ( argc == 0 )
    {
        displayHelpAndExitProgram();
    }

    while ( 0 < argc )
    {
        // Must have exactly one of -x/--xml or -f/--rulesfile
        // -v/--verbose is verbose output
        // -d/--dump  dump intermediate file (for rules->xml conversion)
        // -q/--quiet quiet mode
        // -o/--output write to an output file
        // -s/--maxspecies Bound the generated network by s species
        // -r/--maxreactions  Bound the generated network by r reactions
        // -n Perform number of iterations on the network
        
        std::string arg( *argv );
        argv++;
        argc--;

        if ( arg == "--help" )
        {
            displayHelpAndExitProgram();
        }

        if ( arg == "-x" || arg == "--xml")
        {

            std::string xmlFileName;
            xmlFileName = utl::mustGetArg( argc, argv );

            if(theInputArgs.fileDefined)
            {
                std::cerr << "A rules file can only be added once..." << std::endl;
                if (theInputArgs.fileIsXml)
                {
                    std::cerr << "XML rules file " << theInputArgs.fileName << " already defined." << std::endl;
                }
                else if (theInputArgs.fileIsRules )
                {
                    std::cerr << "MZR rules file " << theInputArgs.fileName << " already defined." << std::endl;
                }
                else
                {
                    std::cerr << "Inconsistant internal data. Unknown error." << std::endl;
                }
                
                throw std::exception();
                
            }
           
            theInputArgs.fileDefined = true;
            theInputArgs.fileIsXml = true;
            theInputArgs.fileName = xmlFileName;
        }



        if ( arg == "-f" || arg == "--rulesfile")
        {
            std::string rulesFileName;
            rulesFileName = utl::mustGetArg( argc, argv );


            if(theInputArgs.fileDefined)
            {
                std::cerr << "A rules file can only be added once..." << std::endl;
                if (theInputArgs.fileIsXml)
                {
                    std::cerr << "XML rules file " << theInputArgs.fileName << " already defined." << std::endl;
                }
                else if (theInputArgs.fileIsRules )
                {
                    std::cerr << "MZR rules file " << theInputArgs.fileName << " already defined." << std::endl;
                }
                else
                {
                    std::cerr << "Inconsistant internal data. Unknown error." << std::endl;
                }
                
                throw std::exception();
                
            }

            theInputArgs.fileDefined = true;
            theInputArgs.fileIsRules = true;
            theInputArgs.fileName = rulesFileName;
        }

        if( arg == "-n" || arg == "--numiters")
        {
            std::string numAsString = utl::mustGetArg( argc, argv);
            theInputArgs.numberIters = utl::argMustBeNNInt( numAsString );
            theInputArgs.runMode = Iterations;
        }

        if( arg == "-v" || arg == "--verbose")
        {
            theInputArgs.verbose = true;
            theInputArgs.quiet = false;
        }
        if(arg == "-q" || arg == "--quiet")
        {
            theInputArgs.verbose = false;
            theInputArgs.quiet = true;
        }
        if(arg == "-o")
        {
            theInputArgs.hasOutputFile = true;
            theInputArgs.outputFile = utl::mustGetArg( argc, argv );
        }
        if(arg == "-s" || arg == "--maxspecies" )
        {

            std::string numAsString = utl::mustGetArg( argc, argv);
            int maxSpec = utl::argMustBeNNInt( numAsString );
            
            theInputArgs.maxSpecies = maxSpec;
            theInputArgs.runMode = BoundedRun;
        }
        if(arg == "-r" || arg == "--maxreactions" )
        {
            std::string numAsString = utl::mustGetArg( argc, argv);
            int maxRxns = utl::argMustBeNNInt( numAsString );

            theInputArgs.maxRxns = maxRxns;
            theInputArgs.runMode = BoundedRun;
        }
    }

}




void printAllSpeciesByName(mzr::moleculizer& theMolzer)
{
    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = theMolzer.theSpeciesListCatalog.begin();
         specIter != theMolzer.theSpeciesListCatalog.end();
         ++specIter)
    {
        std::cout << "ALL@@" << *specIter->first << std::endl;
    }
}

void printAllSpeciesByID(mzr::moleculizer& theMolzer, std::string str)
{
    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = theMolzer.theSpeciesListCatalog.begin();
         specIter != theMolzer.theSpeciesListCatalog.end();
         ++specIter)
    {
        std::cout << str << theMolzer.convertSpeciesTagToSpeciesID( *specIter->first ) << std::endl;
    }
}

void printAllPlexFamilies( mzr::moleculizer& theMolzer, std::string str)
{
    for( std::multimap<int, plx::mzrPlexFamily*>::const_iterator citer= theMolzer.pUserUnits->pPlexUnit->recognize.plexHasher.begin();
         citer != theMolzer.pUserUnits->pPlexUnit->recognize.plexHasher.end();
         ++citer)
        {
            std::cout << citer->second->getPlexFamilyName() << std::endl;
        }
}

void displayHelpAndExitProgram()
{

    cout << "Usage: network_expander -f <FILE>" << endl;

    cout << "This is a demonstration program that demonstrates how libmoleculizer can be used as " << endl;
    cout << "a component for expanding whole reaction networks and displaying some information." << endl;

    cout << "on them.";

    cout << "Libmoleculizer should have come with associated documentation.  Please read it for more details." << endl;
    cout << "\tNathan Addy <addy@molsci.org>\n\tSeptember 18, 2008." << endl;

    exit( 0 );
}


mzr::moleculizer::CachePosition
createBoundedNetwork(mzr::moleculizer& refMolzer, int maxSpec, int maxRxns)
{
    return refMolzer.generateCompleteNetwork( maxSpec, maxRxns);
}

void createFullNetwork(mzr::moleculizer& refMolzer)
{
    std::cout << "Generating complete network... " << std::endl;
    refMolzer.generateCompleteNetwork();
    std::cout << "Done!" << std::endl;
}

void doNIterations(mzr::moleculizer& refMolzer, int number)
{

    std::cout << "Expanding network for " << number << " iterations. " << std::endl;


      for ( int iterNdx = 0; iterNdx != number; ++iterNdx)
      {
          std::cout << "----------------------------------------" << std::endl;
          std::cout << "Iteration " << iterNdx + 1 << "/" << number << ": " << std::endl;

          std::string tag;
          int numExpanded = 0;
          if( getUninitializedSpecies(refMolzer, tag) )
          {
              ++numExpanded;
              std::string uniqueId = refMolzer.convertSpeciesTagToSpeciesID( tag );
              std::cout << numExpanded << " Expanding (" << tag << " -> " << uniqueId << " ) " << std::endl;
              refMolzer.incrementNetworkBySpeciesTag( tag );

              std::cout << "(numSpec, numRxns ) = " << "(" << refMolzer.getTotalNumberSpecies() << ", " << refMolzer.getTotalNumberReactions() << ")" << std::endl;

              std::cout << "-----------" << std::endl;


          }
          else
          {
              std::cout << "All species incremented." << std::endl;
              std::cout << "Breaking on iteration " << iterNdx << std::endl;
              break;
          }
      }

}


void runFullRunMoleculizer( mzr::moleculizer& mzr, const inputArgsStruct& inputArgsStruct)
{

    std::cout << "Expanding whole network..." << std::endl;
    createFullNetwork( mzr );
}


void runIterationsMoleculizer( mzr::moleculizer& mzr, const inputArgsStruct& inputArgsStruct)
{
    doNIterations( mzr, inputArgsStruct.numberIters );
}

void runBoundedRunMoleculizer( mzr::moleculizer& mzr, const inputArgsStruct& inputArgsStruct)
{

    if(!inputArgsStruct.quiet)
    {
        std::cout << "Creating bounded network with (maxSpec, maxRxn) = ( " ;

        if ( inputArgsStruct.maxSpecies > 0 ) std::cout << inputArgsStruct.maxSpecies;
        else std::cout << "inf";
    
        std::cout << ", ";

        if (inputArgsStruct.maxRxns > 0 ) std::cout << inputArgsStruct.maxRxns;
        else std::cout << "inf";

        std::cout << ") " << std::endl;
    }

    pos = createBoundedNetwork( mzr, inputArgsStruct.maxSpecies, inputArgsStruct.maxRxns );
}
