/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
**
**        This file is part of Libmoleculizer
**
**        Copyright (C) 2001-2009 The Molecular Sciences Institute.
**
**::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
**
** Moleculizer is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** Moleculizer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Moleculizer; if not, write to the Free Software Foundation
** Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307,  USA
**
** END HEADER
**
** Original Author:
**   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2009
**
** Modifing Authors:
**
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mzr/libmzr_c_interface.h"

void showUsageAndExit()
{

    printf("Usage: c_interface_demo -f <FILE> [-n NumIters] [ -s MAXSPECIES ] [ -r MAXRXNS] [-w OUTPUTFILE] \n\n");

    printf("This is a demonstration program that demonstrates how libmoleculizer can be used in \n");
    printf("a program written in c for expanding whole reaction networks and displaying some information.\n\n");
    
    printf("Libmoleculizer should have come with associated documentation.  Please read it for more details.\n");
    printf("\tNathan Addy <addy@molsci.org>\n\tJanuary 7, 2009.\n");

    exit(0);
}

int loadModelFile( moleculizer* handle, int argc, char* argv[]);
int getNumberOfIterations( int argc, char* argv[]);
int getVerbose( int argc, char* argv[]);
int getMaxNumSpecies( int argc, char* argv[]);
int getMaxNumReactions( int argc, char* argv[]);

int getWriteToFile( int argc, char* argv[], char* name, int size);

void showAllSpecies( moleculizer* handle);
void showAllReactions( moleculizer* handle);

void printReactionPtr(reaction* pRxn);

int main(int argc, char* argv[] )
{
    srand(time(NULL));

    int verbose = 0;

    char outputFile[256];
    int doOutput = getWriteToFile( argc, argv, outputFile, 256);
    
    if (argc == 1)
    {
        showUsageAndExit();
    }

    moleculizer* pMoleculizer = createNewMoleculizerObject();

    setRateExtrapolation( pMoleculizer, 1 );
    int result = loadModelFile( pMoleculizer, argc, argv);

    switch(result)
    {
    case 0:
        printf("Model loaded successfully.\n");
        break;
    case 1:
        printf("Unknown error on load.  Aborting\n");
        return 1;
    case 2:
        printf("Document unparsable.  Aborting.\n");
        return 1;
    case 3:
        printf("Moleculizer has already loaded rules. Ignoring and continuing.\n");
        return 1;
    }

  int numIter = getNumberOfIterations(argc, argv);
  int maxSpecies = getMaxNumSpecies( argc, argv);
  int maxRxns = getMaxNumReactions( argc, argv);

  if (numIter > 0 && (maxSpecies > 0 || maxRxns > 0) )
  {
      // ANCHOR
      printf("Error, if either (max species and/or max rxns) can be set or numIters. ");
      showUsageAndExit();
      
  }

  verbose = getVerbose(argc, argv);
  
  if(numIter == -1 )
  {
      int numRxns;
      int numSpec;

      species** speciesArray;
      reaction** rxnArray;
      
      int errorCode = \
          getBoundedNetwork( pMoleculizer, maxSpecies, maxRxns, &speciesArray, &numSpec, &rxnArray, &numRxns );

      if (errorCode)
      {
          printf("Unknown erorr.  Check error messages for description.  Exiting...\n");
          exit(1);
      }
      else
      {
          numIter = 10;
          printf("Network generated to %d species and %d reactions.\n", numSpec, numRxns);
      }
  }
  else
  {
      int iter = 0;

      for( iter = 0; iter != numIter; ++iter)
      {
          species** theSpecies;
          int numSpecies = 0;

          getAllExteriorSpecies( pMoleculizer, &theSpecies, &numSpecies);

          if (numSpecies == 0)
          {
              printf("Entire network has been generated on the %dth iteration.\n", iter);
              freeSpeciesArray( theSpecies, numSpecies);
              break;
          }
          
          /* Get a random number in the range [0, numSpecies) */
          int speciesNumber = rand() % numSpecies;

          printf("Iteration %d: Expanding %s\n", iter + 1, theSpecies[speciesNumber]->name);
          incrementSpecies( pMoleculizer, theSpecies[speciesNumber]->name);

          freeSpeciesArray( theSpecies, numSpecies);
      }

  }

  if (numIter == -1)
  {
      printf("Expanded entire network.\n");
  }
  else
  {
      printf("Expanded %d iterations.", numIter);
  }

  printf("\n##########################################\n");

  printf("There are %d species and %d reactions.\n", 
         getNumberOfSpecies(pMoleculizer),
         getNumberOfReactions(pMoleculizer) );


  if ( verbose )
  {
      showAllSpecies( pMoleculizer );
      showAllReactions( pMoleculizer);
  }

  if (doOutput)
  {
      writeDotFile(pMoleculizer, outputFile);
  }
           
  freeMoleculizerObject( pMoleculizer );
  return 0;

}


int loadModelFile( moleculizer* handle, int argc, char* argv[])
{
    int argNum = 0;

    for(argNum = 0; argNum != argc; ++argNum)
    {
        if (strcmp( argv[argNum], "-f") == 0 && argNum + 1 <= argc - 1)
        {
            int code = loadXMLRulesFile( handle, argv[ argNum + 1 ] );
            return code;
        }
    }

    /* No file was found. */
    printf("No mzr file was supplied to the program.\n");
    showUsageAndExit();
}

int getNumberOfIterations(int argc, char* argv[])
{
    int argNum = 0;
    
    for(argNum = 0; argNum != argc; ++argNum)
    {
        if (strcmp(argv[argNum], "-n") == 0 && argNum + 1 <= argc - 1)
        {
            int numIters = atoi( argv[ argNum + 1] );
            return numIters;
        }
    }

    return -1;
}


void showAllSpecies( moleculizer* handle)
{
    species** speciesArray;
    int numberOfSpecies;

    printf("##### Species #############################\n");
    getAllSpecies( handle, &speciesArray, &numberOfSpecies);

    int index;
    for(index = 0; index != numberOfSpecies; ++index)
    {
        printf("%d:\t%s\n", index + 1, speciesArray[index]->name);
    }

    freeSpeciesArray(speciesArray, numberOfSpecies);

    printf("##########################################\n");
}

void showAllReactions( moleculizer* handle)
{
    reaction** reactionArray;
    int numberOfReactions;

    printf("##### Reactions ##########################\n");

    getAllReactions(handle, &reactionArray, &numberOfReactions);

    int index;
    for (index = 0; index != numberOfReactions; ++index)
    {
        printReactionPtr(reactionArray[index]);
    }

    freeReactionArray(reactionArray, numberOfReactions);

    printf("##########################################\n");

    return;
}

int getVerbose( int argc, char* argv[])
{
    int argNum = 0;

    for(argNum = 0; argNum != argc; ++argNum)
    {
        if (strcmp( argv[ argNum], "-v") == 0 || strcmp(argv[ argNum], "--verbose") == 0)
        {
            return 1;
        }
    }

    return 0;
}

void printReactionPtr(reaction* pRxn)
{
    int numSubstrates = 0;
    int numProducts = 0;

    int num = 0;

    for(num = 0; num != pRxn->numberReactants; ++num)
    {
        printf( "%s ", pRxn->reactantVector[num]->name );

        if (num + 2 == pRxn->numberReactants)
        {
            printf(" + ");
        }
    }

    printf("-> ");

    for(num = 0; num != pRxn->numberProducts; ++num)
    {
        printf( "%s ", pRxn->productVector[num]->name );

        if (num + 2 == pRxn->numberProducts )
        {
            printf(" + ");
        }
    }

    printf("\n");
}


int getWriteToFile( int argc, char* argv[], char* name, int size)
{
    int num = 0;
    
    for(num = 0; num != argc; ++num)
    {
        if (strcmp( argv[num], "-w" ) == 0 && num + 1 <= argc - 1)
        {
            if (strlen( argv[num + 1] ) >= size -1 )
            {
                return 0;
            }
            else
            {
                strcpy( name, argv[num + 1]);
                printf("Writing to file %s\n", argv[num + 1]);
                return 1;
            }
        }
    }
    
    return 0;
}


int getMaxNumSpecies( int argc, char* argv[])
{
    int argNum = 0;
    
    for(argNum = 0; argNum != argc; ++argNum)
    {
        if (strcmp(argv[argNum], "-s") == 0 && argNum + 1 <= argc - 1)
        {
            int maxSpec = atoi( argv[ argNum + 1] );
            return maxSpec;
        }
    }

    return -1;
}

int getMaxNumReactions( int argc, char* argv[])
{
    int argNum = 0;
    
    for(argNum = 0; argNum != argc; ++argNum)
    {
        if (strcmp(argv[argNum], "-r") == 0 && argNum + 1 <= argc - 1)
        {
            int maxRxns = atoi( argv[ argNum + 1] );
            return maxRxns;
        }
    }

    return -1;
}
