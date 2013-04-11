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
**   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2008
**
** Modifing Authors:
**
*/


#ifndef __LIBMZR_C_INTERFACE_H__
#define __LIBMZR_C_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct moleculizer_handle
    {
        void* mzrObject;
        char* error_message;
        int error_state;
        
    } moleculizer;
    
    typedef struct species_type
    {
      char* name; /* Is this enough? */
      double* mass;  /* These are in daltons */
      double* radius; 
      double* diffusionCoeff;
        
    } species;
    
    typedef struct reaction_type
    {
        char* name;

        int numberReactants;
        species** reactantVector;
        
        int numberProducts;
        species** productVector;
        
        double* rate;
        
    } reaction;
    

    const char* getErrorMessage(moleculizer* handle);
    int getErrorState( moleculizer* handle);
    void setErrorMessage( moleculizer* handle, const char* error_message);
    void clearErrorState( moleculizer* handle);
    

/*************************************************
** 
** Allocators/Memory Management
**
*************************************************/

    /* These fuctions create a new moleculizer object and release it after being used.
       A call to createNewMoleculizerObject should always be paired with a call to 
       freeMoleculizerObject */
    moleculizer* createNewMoleculizerObject();
    void freeMoleculizerObject( moleculizer* handle);


/*************************************************
** 
** Setup
**
*************************************************/

    int setRateExtrapolation( moleculizer* handle, int extrapolation);


/*************************************************
** 
** Functions for working with input.
**
*************************************************/
  
    int loadXMLRulesFile(moleculizer* handle, char* fileName);
    int loadXMLRulesString( moleculizer* handle, char* file);

    int loadCommonRulesFile(moleculizer* handle, char* fileName);
    int loadCommonRulesString( moleculizer* handle, char* file);


    // int getErrorMessage()


/*************************************************
** 
** Functions for working with output
**
*************************************************/

    int writeDotFile( moleculizer* handle, char* fileName);


/*************************************************
** 
** Functions for expanding the network
**
*************************************************/

    int setGenerateDepth(moleculizer* handle, int generateDepth);
    int getGenerateDepth(moleculizer* handle, int* pGenerateDepth);

    int expandNetwork( moleculizer* handle);
    int getBoundedNetwork( moleculizer* handle, long maxNumSpecies, long maxNumReactions, species*** pSpeciesArray, int* pNumSpec, reaction*** pReactionArrry, int* pNumRxns);
    int incrementSpecies( moleculizer* handle, char* speciesTag);

    int expandSpeciesByTag( moleculizer* handle, char* theTag);
    int expandSpeciesByID( moleculizer* handle, char* theID);
//     int expandSpecies( moleculizer* handle, species* mzrSpecies);
//     int expandReaction(moleculizer* handle, reaction* mzrReaction);

/*************************************************
** 
** Functions for viewing state
**
*************************************************/

    int getNumberOfSpecies(moleculizer* handle);
    int getNumberOfReactions(moleculizer* handle);
    int getDeltaSpecies( moleculizer* handle, species*** pSpeciesArray, int* pNum);
    int getDeltaReactions( moleculizer* handle, reaction*** pReactionArray, int* pNum);
    int clearDeltaState( moleculizer* handle);
    int getAllSpecies(moleculizer* handle, species*** pSpeciesArray, int* numberSpecies);
    int getAllReactions(moleculizer* handle, reaction*** pReactionArray, int* numberReactions);

    /* These functions are for use with smoldyn's mzroutput code it uses in startup */
    int getNumModificationDefs( moleculizer* handle);
    int getNumMolDefs( moleculizer* handle);
    int getNumReactionRules( moleculizer* handle);
    int getNumDimerDecompReactionRules( moleculizer* handle);
    int getNumOmniGenReactionRules( moleculizer* handle);
    int getNumUniMolGenReactionRules( moleculizer* handle);
    int getNumSpeciesStreams( moleculizer* handle);
    
/*************************************************
** 
** Functions for working with reactions
**
*************************************************/
    /* These two functions can be used with a Species Key (it's canonical string representation) to 
       determine what reactions, if any they participate in. These functions return pointers to reaction 

       arrays, which contain pointers to reactions and species.  Ownership is transfered to the user, 
       who must free them manually. */

    int getReactionsBetween(moleculizer* handle, char* speciesName1, char* speciesName2, reaction*** ptrReactionPtrArray, int* numReactions);
    int getUnaryReactions(moleculizer* handle, char* speciesName, reaction*** ptrReactionPtrArray, int* numReactions);


/*************************************************
** 
** Functions for working with species
**
*************************************************/

    /* This function returns all species that have not been expanded yet. */
    int getAllExteriorSpecies(moleculizer* handle, species*** pSpeciesArray, int* numberSpecies);
    int getMolCountInTaggedSpecies(moleculizer* handle, const char* molName, const char* speciesTag);
		  int getSpeciesSymmetryDegree(moleculizer* handle, const char* speciesTag, int* automorphism_size);

/*************************************************
** 
** Functions for working with species streams
**
*************************************************/

    int getAllStreamSpecies(moleculizer* handle, char* streamName, species*** pSpeciesArray, int* numberSpecies);
    void getAllSpeciesStreams( moleculizer* handle, char*** speciesStreamArray, int* numberSpeciesStreams);
    int checkSpeciesTagIsInSpeciesStream( moleculizer* handle, const char* speciesID, char* speciesStream);
    int checkSpeciesIDIsInSpeciesStream( moleculizer* handle, const  char* speciesID, char* speciesStream);


/*************************************************
 **
 ** Functions for working with names
 **
 ************************************************/

    int convertTaggedNameToUniqueID( moleculizer* handle, char* speciesTag, char* speciesID, unsigned int idSize);  
    int convertUniqueIDToTaggedName( moleculizer* handle, char* speciesID, char* speciesTag, unsigned int tagSize);
    int convertUserNameToTaggedName(moleculizer* handle, char* theUserName, char* correspondingTag, unsigned int bufferSize);
    int convertUserNameToUniqueID(moleculizer* handle, char* theUserName, char* correspondingSpeciesID, unsigned int bufferSize);

    /* This function converts an explicit name, unique ID, or tagged name to a tagged name */
    int convertSomeNameToTaggedName( moleculizer* handle, char* theName, char* taggedNameBuffer, unsigned int bufferSize);

    int getExplicitSpeciesList(moleculizer* handle, char*** theExplicitSpeciesNames, unsigned int* numSpecies);


/*************************************************
** 
** Functions for memory management
**
*************************************************/

    /* These functions free species and reaction arrays, such as those provided by the getReactionsBetween
       and getUnaryReactions functions.  Usually, a call to freeReaction( reactionArray, *numReactions)
       is sufficient to properly release all this information. */

    void freeReactionArray( reaction** pRxnArray, unsigned int numArrayElements);
    void freeSpeciesArray( species** pSpeciesArray, unsigned int numArrayElements);
    void freeCharPtrArray( char** pCharArray, unsigned int numCharPtrElements);
    void freeReaction( reaction* pRxn );
    void freeSpecies( species* pSpecies );


#ifdef __cplusplus
}
#endif

#endif
