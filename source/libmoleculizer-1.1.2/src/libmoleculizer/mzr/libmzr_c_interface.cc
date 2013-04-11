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

#include "libmzr_c_interface.h"

#include "utl/xcpt.hh"
#include "utl/writeOutputGraph.hh"
#include "moleculizer.hh"
#include "mzr/spatialExtrapolationFunctions.hh"
#include "unitsMgr.hh"
#include "mol/molUnit.hh"
#include "mzr/mzrSpeciesDumpable.hh"
#include "mzr/pythonRulesManager.hh"

#include <libxml++/libxml++.h>
#include <iterator>
#include <string.h>
#include <stdio.h>


// 
// Declarations for functions locally defined.
// 
// 

mzr::moleculizer* 
convertCMzrPtrToMzrPtr(moleculizer* cMzrPtr);

int 
calculateSumOfMultMap( const mzr::mzrReaction::multMap& speciesMap);

species* 
createNewCSpeciesFromMzrSpecies( moleculizer* cMzrPtr, const mzr::mzrSpecies* pMzrSpecies);

reaction* 
createNewCRxnFromMzrReaction( moleculizer* cMzrPtr, const mzr::mzrReaction* pMzrReaction);

void 
createCSpeciesArrayFromSpeciesMap(moleculizer* cMzrPtr, 
                                  const mzr::mzrReaction::multMap& speciesMap, 
                                  species*** speciesList, int& numberInList);


//
// The interface presented in the header file -- the c-interface.
//
// 

moleculizer* createNewMoleculizerObject()
{
    // Creates a new moleculizer object (the c-struct) that wraps a mzr::moleculizer c++ object.
    // The pointer is owned by whoever calls this function and must be free'd using the 
    // freeMoleculizerObject function.
    try
    {
        moleculizer_handle* newHandle = new moleculizer_handle;

        newHandle->mzrObject = (void*) new mzr::moleculizer;
        newHandle->error_message = NULL;
        newHandle->error_state = 0;

        return newHandle;
    }
    catch(...)
    {
        return NULL;
    }
}

void setErrorMessage( moleculizer* handle, const char* error_message)
{
    handle->error_state = 1;
    handle->error_message = new char[strlen(error_message) + 1];
    strcpy( handle->error_message, error_message);
    return;
}

void clearErrorState( moleculizer* handle)
{
    delete [] handle->error_message;
    handle->error_message = NULL;
    handle->error_state = 0;
    return;
}

int getErrorState(moleculizer* handle)
{
    return handle->error_state;
}

const char* getErrorMessage(moleculizer* handle)
{
    return (const char*) handle->error_message;
}

void freeMoleculizerObject( moleculizer* handle)
{
    // This function frees the c-style moleculizer handle.
    delete convertCMzrPtrToMzrPtr(handle);
    delete [] handle->error_message;
    delete handle;
}

int setRateExtrapolation( moleculizer* handle, int extrapolation)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            MODEL_ALREADY_LOADED = 2};

    try
    {
        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );
        underlyingMoleculizerObject->setRateExtrapolation( extrapolation );
    }
    catch(utl::modelAlreadyLoadedXcpt e)
    {
        e.warn();
        return MODEL_ALREADY_LOADED;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
    
    return SUCCESS;
}


int loadCommonRulesFile(moleculizer* handle, char* fileName)
{
    // This function takes a string to a file containing an xml rules 
    // description for the system and loads it into mzr::moleculizer.
    // See documentation for a specification of model files as well as
    // examples.
    
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1, 
                            DOCUMENT_UNPARSABLE = 2,
                            RULES_ALREADY_LOADED = 3,
                            FILE_NOT_FOUND = 4};
    
    try
    {
        convertCMzrPtrToMzrPtr( handle )->loadCommonRulesFileName( std::string(fileName) );
        return SUCCESS;
    }
    catch(mzr::mzrPythonXcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getLongMessage().c_str());
        return UNKNOWN_ERROR;
    }
    catch(utl::dom::noDocumentParsedXcpt xcpt)
    {
        // In any case, we put the message here somewhere.
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return DOCUMENT_UNPARSABLE;
    }
    catch(utl::modelAlreadyLoadedXcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return RULES_ALREADY_LOADED;
    }
    catch(utl::dom::xcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return DOCUMENT_UNPARSABLE;
    }
    catch(utl::xcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return UNKNOWN_ERROR;
    }
    catch(xmlpp::internal_error x)
    {
        // Is this correct?  
        setErrorMessage(handle, "File not found.");
        return FILE_NOT_FOUND;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
    
}

// This function takes a string containing the contents of a human
// readable rules definition file and loads it into the provided
// moleculizer object.
int loadCommonRulesString( moleculizer* handle, char* rulesCstring)
{

    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR=1,
                            DOCUMENT_UNPARSABLE = 2,
                            RULES_ALREADY_LOADED = 3,
                            MISSING_MOLECULIZER_MODULE = 4};
    
    std::string rules( rulesCstring );
    try
    {
        convertCMzrPtrToMzrPtr( handle )->loadCommonRulesString( rules );
        return SUCCESS;
    }
    catch(mzr::mzrPythonXcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getLongMessage().c_str());
        return UNKNOWN_ERROR;
    }
    catch(mzr::MoleculizerModuleMissingXcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return MISSING_MOLECULIZER_MODULE;
    }
    catch(utl::dom::noDocumentParsedXcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return DOCUMENT_UNPARSABLE;
    }
    catch(utl::modelAlreadyLoadedXcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return RULES_ALREADY_LOADED;
    }
    catch(utl::xcpt xcpt)
    {
        setErrorMessage(handle, xcpt.getMessage().c_str());
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        setErrorMessage(handle, "Unknown Error");

        return UNKNOWN_ERROR;
    }
    
}

int loadXMLRulesFile(moleculizer* handle, char* fileName)
{
    // This function takes a string to a file containing an xml rules 
    // description for the system and loads it into mzr::moleculizer.
    // See documentation for a specification of model files as well as
    // examples.
    
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1, 
                            DOCUMENT_UNPARSABLE = 2,
                            RULES_ALREADY_LOADED = 3,
                            FILE_NOT_FOUND = 4};
    
    try
    {
        convertCMzrPtrToMzrPtr( handle )->loadXmlFileName( std::string(fileName) );
        return SUCCESS;
    }
    catch(utl::dom::noDocumentParsedXcpt xcpt)
    {
        xcpt.warn();
        return DOCUMENT_UNPARSABLE;
    }
    catch(utl::modelAlreadyLoadedXcpt xcpt)
    {
        xcpt.warn();
        return RULES_ALREADY_LOADED;
    }
    catch(utl::dom::xcpt)
    {
        return DOCUMENT_UNPARSABLE;
    }
    catch(utl::xcpt xcpt)
    {
        xcpt.warn();
        return UNKNOWN_ERROR;
    }
    catch(xmlpp::internal_error x)
    {
        return FILE_NOT_FOUND;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
    
}

int loadXMLRulesString( moleculizer* handle, char* rulesCstring)
{
    // This function takes a string containing a rules definition of the file and loads 
    // it into the provided moleculizer object.
    
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR=1,
                            DOCUMENT_UNPARSABLE = 2,
                            RULES_ALREADY_LOADED = 3};
    
    std::string rules( rulesCstring );
    try
    {
        convertCMzrPtrToMzrPtr( handle )->loadXmlString( rules );
        return SUCCESS;
    }
    catch(utl::dom::noDocumentParsedXcpt xcpt)
    {
        xcpt.warn();
        return DOCUMENT_UNPARSABLE;
    }
    catch(utl::modelAlreadyLoadedXcpt xcpt)
    {
        xcpt.warn();
        return RULES_ALREADY_LOADED;
    }
    catch(utl::xcpt xcpt)
    {
        xcpt.warn();
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
    
}

int setGenerateDepth(moleculizer* handle, int generateDepth)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1};
    try
    {
        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );
        underlyingMoleculizerObject->setGenerateDepth(generateDepth);
    }
    catch(utl::xcpt x)
    {
        x.warn();
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
    
    return SUCCESS;
}

int getGenerateDepth(moleculizer* handle, int* pGenerateDepth)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };
    try
    {
        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );
        int generationDepth = underlyingMoleculizerObject->getGenerationDepth();
        (*pGenerateDepth) = generationDepth;
    }
    catch(utl::xcpt x)
    {
        x.warn();
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }

    return SUCCESS;
}

int expandNetwork( moleculizer* handle)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            NO_MODEL_LOADED_ERROR = 2};

    try
    {
        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );
        underlyingMoleculizerObject->generateCompleteNetwork();
    }
    catch(mzr::ModelNotLoadedXcpt x)
    {
        x.what();
        return NO_MODEL_LOADED_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }

    return SUCCESS;
}

int getBoundedNetwork( moleculizer* handle, long maxNumSpecies, long maxNumReactions, species*** pSpeciesArray, int* pNumSpec, reaction*** pReactionArray, int* pNumRxns)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1};

    try
    {
        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );
        mzr::moleculizer::CachePosition theMaxNetwork = underlyingMoleculizerObject->generateCompleteNetwork(maxNumSpecies, maxNumReactions);

        typedef reaction* reactionPtr;
        typedef species* speciesPtr;

        int numSpecies = std::distance( underlyingMoleculizerObject->theDeltaSpeciesList.begin(), theMaxNetwork.first);
        int numRxns = std::distance( underlyingMoleculizerObject->theDeltaReactionList.begin(), theMaxNetwork.second);

        species** specArray = new speciesPtr[ numSpecies ];
        reaction** rxnArray = new reactionPtr[ numRxns ];

        int specNdx = 0;
        int rxnNdx = 0;
        for( mzr::moleculizer::SpeciesListIter specIter = underlyingMoleculizerObject->theDeltaSpeciesList.begin();
             specIter != theMaxNetwork.first;
             ++specIter)
        {

            specArray[specNdx] = createNewCSpeciesFromMzrSpecies( handle, *specIter) ;
            ++specNdx;
        }

        for( mzr::moleculizer::ReactionListIter rxnIter = underlyingMoleculizerObject->theDeltaReactionList.begin();
             rxnIter != theMaxNetwork.second;
             ++rxnIter)
        {
            rxnArray[rxnNdx] = createNewCRxnFromMzrReaction( handle, *rxnIter);
            ++rxnNdx;
        }

        // Assuming everything has gotten here, copy everything over.
        *pSpeciesArray = specArray;
        *pReactionArray = rxnArray;

        *pNumSpec = numSpecies;
        *pNumRxns = numRxns;

        return SUCCESS;
    }
    catch(utl::xcpt x)
    {
        x.warn();
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
}


int incrementSpecies( moleculizer* handle, char* speciesTag)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            NON_EXISTANT_SPECIES_NAME = 2};
    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
        std::string strSpeciesName( speciesTag );
        moleculizerPtr->incrementNetworkBySpeciesTag( speciesTag );
    }
    catch( fnd::NoSuchSpeciesXcpt x )
    {
        x.warn();
        return NON_EXISTANT_SPECIES_NAME;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }

    return SUCCESS;
}


int getNumberOfSpecies(moleculizer* handle)
{
    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
        return moleculizerPtr->getTotalNumberSpecies();
    }
    catch(...)
    {
        return -1;
    }
}


int getNumberOfReactions(moleculizer* handle)
{
    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
        int numberOfReactions = moleculizerPtr->getTotalNumberReactions();

        return numberOfReactions;
    }
    catch(...)
    {
        return -1;
    }
}


int getDeltaSpecies( moleculizer* handle, species*** pSpeciesArray, int* pNum)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };

    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

        *pNum = moleculizerPtr->getNumberDeltaSpecies();
        *pSpeciesArray = new species* [ *pNum ];

        int index = 0;
        
        for( mzr::moleculizer::SpeciesList::const_iterator deltaSpecIter = moleculizerPtr->getDeltaSpeciesList().begin();
             deltaSpecIter != moleculizerPtr->getDeltaSpeciesList().end();
             ++deltaSpecIter)
        {
            (*pSpeciesArray)[index++] = createNewCSpeciesFromMzrSpecies( handle, *deltaSpecIter);
        }

        return SUCCESS;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
}

int getDeltaReactions( moleculizer* handle, reaction*** pReactionArray, int* pNum)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };
    
    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

        *pNum = moleculizerPtr->getNumberDeltaReactions();
    
        *pReactionArray = new reaction* [*pNum];

        int index = 0;
        
        for(mzr::moleculizer::ReactionList::const_iterator deltaRxnIter = moleculizerPtr->getDeltaReactionList().begin();
            deltaRxnIter != moleculizerPtr->getDeltaReactionList().end();
            ++deltaRxnIter)
        {
            (*pReactionArray)[index++] = createNewCRxnFromMzrReaction( handle, *deltaRxnIter );
        }

        return SUCCESS;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }

}

int clearDeltaState( moleculizer* handle)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };

    try
    {
        convertCMzrPtrToMzrPtr( handle )->resetCurrentState();
        return SUCCESS;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }

}

int convertTaggedNameToUniqueID( moleculizer* handle, char* speciesTag, char* speciesID, unsigned int idSize)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            TAG_NOT_FOUND = 2,
                            INSUFFICIENT_MEMORY = 3 };

    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
        std::string theTaggedName( speciesTag );
        
        std::string theUniqueID = moleculizerPtr->convertSpeciesTagToSpeciesID( theTaggedName );

        if (theUniqueID.size() + 1 > idSize) return INSUFFICIENT_MEMORY;
        
        strcpy( speciesID, theUniqueID.c_str() );

        return SUCCESS;
    }
    catch(fnd::NoSuchSpeciesXcpt x)
    {
        return TAG_NOT_FOUND;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }

}

int convertUniqueIDToTaggedName( moleculizer* handle, char* speciesID, char* speciesTag, unsigned int tagSize)
{

    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1, 
                            SPECIES_ID_UNKNOWN = 2,
                            NOT_ENOUGH_BUFFER_MEMORY = 3 };

    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
        std::string theSpeciesID( speciesID );
        
        std::string theTaggedName = moleculizerPtr->convertSpeciesIDToSpeciesTag( theSpeciesID );

        if (theTaggedName.size() + 1 > tagSize) return NOT_ENOUGH_BUFFER_MEMORY;
        
        strcpy( speciesTag, theTaggedName.c_str() );

        return SUCCESS;
    }
    catch(fnd::NoSuchSpeciesXcpt x)
    {
        return SPECIES_ID_UNKNOWN;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
}


int convertUserNameToTaggedName(moleculizer* handle, char* theUserName, char* correspondingTag, unsigned int bufferSize)
{
    
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            NONEXISTANT_USER_NAME = 2,
                            NEED_A_BIGGER_BUFFER = 3};
    
    try
    {
        std::string userName( theUserName );
        std::string speciesKey = convertCMzrPtrToMzrPtr(handle)->convertUserNameToTaggedName( userName );
        
        if (speciesKey.size() + 1 > bufferSize) return NEED_A_BIGGER_BUFFER;
        strcpy(correspondingTag, speciesKey.c_str() );
        
        return SUCCESS;
        
    }
    catch( mzr::unknownUserNameXcpt e)
    {
        e.warn();
        return NONEXISTANT_USER_NAME;
    }
    catch(utl::xcpt e)
    {
        e.warn();
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
}

int convertSomeNameToTaggedName( moleculizer* handle, char* theName, char* taggedNameBuffer, unsigned int bufferSize)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            NONEXISTANT_NAME = 2,
                            NEED_A_BIGGER_BUFFER = 3};

    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
        std::string randomName( theName );

        std::string taggedName = moleculizerPtr->convertSomeNameToTaggedName( randomName );

        if (bufferSize < taggedName.size() + 1) return NEED_A_BIGGER_BUFFER;
        
        strcpy( taggedNameBuffer, taggedName.c_str());
        return SUCCESS;

    }
    catch(...)
    {
        return NONEXISTANT_NAME;
    }


}

int convertUserNameToUniqueID(moleculizer* handle, char* theUserName, char* correspondingSpeciesID, unsigned int bufferSize)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            NONEXISTANT_USER_NAME = 2,
                            NEED_A_BIGGER_BUFFER = 3};
    
    try
    {
        mzr::moleculizer* pMoleculizer = convertCMzrPtrToMzrPtr(handle);

        std::string userName( theUserName );
        std::string speciesID = pMoleculizer->convertUserNameToSpeciesID( userName );

        if (speciesID.size() + 1 > bufferSize) return NEED_A_BIGGER_BUFFER;
        
        strcpy(correspondingSpeciesID, speciesID.c_str() );
        return SUCCESS;
        
    }
    catch( mzr::unknownUserNameXcpt e)
    {
        e.warn();
        return NONEXISTANT_USER_NAME;
    }
    catch(utl::xcpt e)
    {
        e.warn();
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }

}

int getExplicitSpeciesList(moleculizer* handle, char*** explicitNamesArray, unsigned int* numSpecies)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };

    try
    {

        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

        std::vector<std::string> explicitSpeciesNamesVect;
        moleculizerPtr->getUserNames(explicitSpeciesNamesVect);


        typedef char* charPtr;

        *explicitNamesArray = new charPtr[ explicitSpeciesNamesVect.size() ];
        *numSpecies = explicitSpeciesNamesVect.size();
        
        for( unsigned int nameNdx = 0; nameNdx != explicitSpeciesNamesVect.size(); ++nameNdx)
        {
            (*explicitNamesArray)[nameNdx] = new char[ explicitSpeciesNamesVect[nameNdx].size() + 1];
            strcpy( (*explicitNamesArray)[nameNdx], explicitSpeciesNamesVect[nameNdx].c_str());
        }

        return SUCCESS;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
}


int getReactionsBetween(moleculizer* handle, char* cStrSpeciesName1, char* cStrSpeciesName2, reaction*** ptrReactionPtrArray, int* numReactions)
{
    // This function takes two species names, and provides a moleculizer* and a pointer to an array of
    // reaction*'s, and uses moleculizer to determine if there are any reactions between the two species.
    // If so, they are placed in reactionPtrArray and their number is placed in numReactions.
    
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            ILLEGAL_SPECIES_NAME = 2};
    
    std::string speciesName1( cStrSpeciesName1 );
    std::string speciesName2( cStrSpeciesName2 );
    
    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
    
    const mzr::mzrSpecies* species1;
    const mzr::mzrSpecies* species2;
    
    try
    {
        species1 = moleculizerPtr->getSpeciesWithUniqueID( speciesName1 );
        species2 = moleculizerPtr->getSpeciesWithUniqueID( speciesName2 );
    }
    catch(mzr::IllegalNameXcpt e)
    {
        return ILLEGAL_SPECIES_NAME; // Species Name is illegal.
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
    
    std::vector<const mzr::mzrReaction*> reactionsBetweenContainer;
    reactionsBetweenContainer.reserve(4);
    
    moleculizerPtr->findReactionWithSubstrates( species1, species2, reactionsBetweenContainer);
    
    if( reactionsBetweenContainer.empty() )
    {
        *ptrReactionPtrArray = NULL;
        *numReactions = 0;
        return SUCCESS;
    }
    
    typedef reaction* reactionPtr;
    *ptrReactionPtrArray = new reactionPtr[ reactionsBetweenContainer.size() ];
    int size = reactionsBetweenContainer.size();
    *numReactions = size;
    
    for(unsigned int rxnIndex = 0; rxnIndex != reactionsBetweenContainer.size(); ++rxnIndex)
    {
        (*ptrReactionPtrArray)[ rxnIndex ] = createNewCRxnFromMzrReaction( handle, reactionsBetweenContainer[rxnIndex]);
    }
    
    return SUCCESS;
}

int getUnaryReactions(moleculizer* handle, char* speciesName, reaction*** ptrReactionPtrArray, int* numReactions)
{
    // Same as getBinaryReactions, but with only one species.
    
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            ILLEGAL_SPECIES_NAME = 2};
    
    try
    {
        mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
        const mzr::mzrSpecies* pSpecies;
        
        pSpecies = moleculizerPtr->getSpeciesWithUniqueID( std::string(speciesName) );
        
        // Just a guess so as to preserve speed when a few reactions are returned;
        std::vector<const mzr::mzrReaction*> unaryReactionContainer;
        unaryReactionContainer.reserve(10);
        
        moleculizerPtr->findReactionWithSubstrates( pSpecies, unaryReactionContainer);
        
        if ( unaryReactionContainer.empty() )
        {
            *ptrReactionPtrArray = NULL;
            *numReactions = 0;
            return SUCCESS;
        }
        
        *numReactions = unaryReactionContainer.size();
        
        typedef reaction* reactionPtr;
        *ptrReactionPtrArray = new reactionPtr[ unaryReactionContainer.size() ];
        
        for(unsigned int rxnIndex = 0; rxnIndex != unaryReactionContainer.size(); ++rxnIndex)
        {
            (*ptrReactionPtrArray)[rxnIndex] = createNewCRxnFromMzrReaction( handle, unaryReactionContainer[ rxnIndex ] );
        }
        
        return SUCCESS;
    }
    catch( mzr::IllegalNameXcpt )
    {
        return ILLEGAL_SPECIES_NAME;
    }
    catch(utl::xcpt e)
    {
        e.warn();
        return UNKNOWN_ERROR;
    }
    catch(...)
    {
        return UNKNOWN_ERROR;
    }
}

 
int getAllStreamSpecies(moleculizer* handle, char* cStrStreamName, species*** pSpeciesArray, int* numberSpecies)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1,
                            STREAM_DOES_NOT_EXIST = 3 };

    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

    std::string streamName( cStrStreamName );
    std::vector<const mzr::mzrSpecies*> speciesVector;

    moleculizerPtr->getSpeciesInSpeciesStream( streamName, speciesVector );

    *numberSpecies = moleculizerPtr->getTotalNumberSpecies();
    *pSpeciesArray = new species* [ moleculizerPtr->getTotalNumberSpecies() ];

    int index = 0;
    for(unsigned int ndx = 0; ndx != speciesVector.size(); ++ndx)
    {
        (*pSpeciesArray)[index] = createNewCSpeciesFromMzrSpecies( handle, speciesVector[ndx]);
    }
    
    return SUCCESS;
}

int getAllSpecies(moleculizer* handle, species*** pSpeciesArray, int* numberSpecies)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };

    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

    *numberSpecies = moleculizerPtr->getTotalNumberSpecies();
    *pSpeciesArray = new species* [ *numberSpecies ];

    int index = 0;
    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = moleculizerPtr->getSpeciesCatalog().begin();
         specIter != moleculizerPtr->getSpeciesCatalog().end();
         ++specIter)
    {
        (*pSpeciesArray)[index++] = createNewCSpeciesFromMzrSpecies( handle, specIter->second );
    }

    return SUCCESS;
}

int getAllReactions(moleculizer* handle, reaction*** pReactionArray, int* numberReactions)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };

    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

    *numberReactions = moleculizerPtr->getTotalNumberReactions();
    *pReactionArray = new reaction* [ *numberReactions ];

    int index = 0;

    for( mzr::moleculizer::ReactionList::const_iterator rxnIter = moleculizerPtr->getReactionList().begin();
         rxnIter != moleculizerPtr->getReactionList().end();
         ++rxnIter)
    {
        (*pReactionArray)[index++] = createNewCRxnFromMzrReaction( handle, *rxnIter );
    }

    return SUCCESS;
}

void freeReactionArray( reaction** pRxnArray, unsigned int numElements)
{
    for( unsigned int num = 0; num != numElements; ++num)
    {
        // This will free numElements in the array (all of them);
        freeReaction( pRxnArray[num] );
    }
    
    // This will delete the allocated memory for the pointers in the array themselves.
    delete [] pRxnArray;
}

void freeCharPtrArray( char** pCharArray, unsigned int numElements)
{
    for( unsigned int num = 0; num != numElements; ++num)
    {
        delete pCharArray[num];
    }

    delete [] pCharArray;
}

void freeSpeciesArray( species** speciesArray, unsigned int numElements)
{
    for(unsigned int num = 0; num != numElements; ++num)
    {
        // Releases the memory each of the pointers points to.
        freeSpecies( speciesArray[num] );
    }
    
    // Releases the pointers themselves.
    delete [] speciesArray;
}

void freeReaction( reaction* pRxn)
{
    // This frees up all the memory associated with each of the pointers.
    freeSpeciesArray( pRxn->reactantVector, pRxn->numberReactants);
    freeSpeciesArray( pRxn->productVector, pRxn->numberProducts);
    
    delete [] pRxn->name;
    delete pRxn->rate;
}

void freeSpecies( species* pSpecies)
{
    // I am told this is correct under all cases.  
    delete [] pSpecies->name;
    
    delete pSpecies->mass;
    delete pSpecies->radius;
    delete pSpecies->diffusionCoeff;
}


void getAllSpeciesStreams( moleculizer* handle, char*** speciesStreamArray, int* numberSpeciesStreams)
{
    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
    std::vector<std::string> speciesStreams;

    moleculizerPtr->getSpeciesStreams( speciesStreams );

    *numberSpeciesStreams = speciesStreams.size();

    typedef char* charPtr;
    *speciesStreamArray = new charPtr[ speciesStreams.size() ];
    for(unsigned int ndx = 0; ndx != speciesStreams.size(); ++ndx)
    {
        (*speciesStreamArray)[ ndx ] = new char[ speciesStreams[ndx].size() + 1];
        strcpy( (*speciesStreamArray)[ ndx ], speciesStreams[ndx].c_str() );
    }

    return;
}

int getAllExteriorSpecies(moleculizer* handle, species*** pSpeciesArray, int* numberSpecies)
{
    enum LOCAL_ERROR_TYPE { SUCCESS = 0,
                            UNKNOWN_ERROR = 1 };

    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

    std::vector<const mzr::mzrSpecies*> uninitializedSpecies;

    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = moleculizerPtr->getSpeciesCatalog().begin();
         specIter != moleculizerPtr->getSpeciesCatalog().end();
         ++specIter)
    {
        if (!specIter->second->hasNotified())
        {
            uninitializedSpecies.push_back( specIter->second );
        }
    }

    *numberSpecies = uninitializedSpecies.size();
    *pSpeciesArray = new species* [ *numberSpecies ];

    int index = 0;

    for(std::vector<const mzr::mzrSpecies*>::const_iterator specIter = uninitializedSpecies.begin();
        specIter != uninitializedSpecies.end();
        ++specIter)
    {
        (*pSpeciesArray)[index++] = createNewCSpeciesFromMzrSpecies( handle, *specIter );
    }

    return SUCCESS;
}

int getSpeciesSymmetryDegree(moleculizer* handle, const char* speciesTag, int* automorphism_size)
{
		// This function returns the size of the the automorphism group of
		// the species.  This can/will be used to adjust reaction rates to
		// their correct values.

		enum LOCAL_ERROR_TYPE { SUCCESS = 0,
																										UNKNOWN_ERROR = 1 };

		try
		{
				mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );

				std::string specName(speciesTag);
				mzr::mzrSpecies* pMzrSpecies = moleculizerPtr->findSpecies(specName);
				(*automorphism_size) = moleculizerPtr->getSizeOfAutomorphismGroup(*pMzrSpecies);

				return SUCCESS;
		}
		catch(...)
		{
				return UNKNOWN_ERROR;
		}
}

int getMolCountInTaggedSpecies(moleculizer* handle, const char* molName, const char* speciesTag)
{
    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
    try
    {
	int count = moleculizerPtr->getMolCountInTaggedSpecies( molName, speciesTag);
	return count;
    }
    catch(...)
    {
	return -1;
    }
}

int writeDotFile( moleculizer* handle, char* fileName)
{
    mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
    writeNetworkToDotFile( *moleculizerPtr, fileName);

    return 1;
}

int getNumModificationDefs( moleculizer* handle)
{
   mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
   return moleculizerPtr->getNumberOfDefinedModifications();
}

int getNumMolDefs( moleculizer* handle)
{
   mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
   return moleculizerPtr->getNumberOfDefinedMols();
}

int getNumReactionRules( moleculizer* handle)
{
   mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
   return moleculizerPtr->getNumberOfDefinedRules();
}

int getNumDimerDecompReactionRules( moleculizer* handle)
{
   mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
   return moleculizerPtr->getNumberOfDimerReactionRules();
}

int getNumOmniGenReactionRules( moleculizer* handle)
{
   mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
   return moleculizerPtr->getNumberOfOmniGenReactionRules();
}

int getNumUniMolGenReactionRules( moleculizer* handle)
{
   mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
   return moleculizerPtr->getNumberOfUniMolReactionRules();
}

int getNumSpeciesStreams( moleculizer* handle)
{
   mzr::moleculizer* moleculizerPtr = convertCMzrPtrToMzrPtr( handle );
   return moleculizerPtr->getNumberOfSpeciesStreams();
}

////////////////////////////////////////////////////////////////////////////////////////////////

inline 
mzr::moleculizer* convertCMzrPtrToMzrPtr(moleculizer* cMzrPtr)
{
    return (mzr::moleculizer*) cMzrPtr->mzrObject;
}


int calculateSumOfMultMap( const mzr::mzrReaction::multMap& speciesMap)
{
    int size = 0;
    for(mzr::mzrReaction::multMap::const_iterator iter = speciesMap.begin();
        iter != speciesMap.end();
        ++iter)
    {
        size += iter->second;
    }

    return size;
}


/* The moleculizer here is so that we can look up species SpecificData such as the radius and whatnot. */
species* createNewCSpeciesFromMzrSpecies( moleculizer* cMzrPtr, const mzr::mzrSpecies* pMzrSpecies)
{
    species* newSpecies = new species;
    
    newSpecies->name = NULL;
    newSpecies->mass = NULL;
    newSpecies->radius = NULL;
    newSpecies->diffusionCoeff = NULL;
    
    // COPY OVER THE NAME.
    std::string speciesKey( pMzrSpecies->getTag() );            // Create a string with the name.
    newSpecies->name = new char[speciesKey.size() + 1];          // Create a new char buffer in newSpecies->name.
    strcpy( newSpecies->name, speciesKey.c_str() );              // Copy the string into the char buffer.
    
    // Copy over the mass
    newSpecies->mass = new double;                    // Allocate new mem for a double in mass.
    *(newSpecies->mass) = pMzrSpecies->getWeight();   // Put the weight there.
    
    // Set the radius and the diffusion coefficient
    newSpecies->radius = new double;      
    *newSpecies->radius = mzr::extrapolateMolecularRadius( pMzrSpecies );

    newSpecies->diffusionCoeff = new double; 
    *newSpecies->diffusionCoeff = mzr::getDiffusionCoeffForSpecies( pMzrSpecies );
    
    // Return the pointer to the newly created and instantiated newSpecies.
    return newSpecies;
}


reaction* createNewCRxnFromMzrReaction( moleculizer* cMzrPtr, const mzr::mzrReaction* pMzrReaction)
{

    static int number = 0;

    std::string reactionName( pMzrReaction->getName() );
    
    reaction* theNewRxn = new reaction;

    // This is for Smoldyn, which doesn't care for super long names...
    // assert( reactionName.size() + 1 < 256 );


    if (reactionName.size() + 1 >= 256)
    {
	theNewRxn->name = new char[256];
	int ndx;
	for(ndx = 0; ndx != 250; ++ndx)
	{
	    theNewRxn->name[ndx] = reactionName[ndx];
	}
	for(ndx = 250; ndx != 256; ++ndx)
	{
	    theNewRxn->name[ndx] = '\0';
	}

	std::ostringstream oss;
	oss << number++;

	strcat( theNewRxn->name, oss.str().c_str() );
	
    }
    else
    {
	theNewRxn->name = new char[ reactionName.size() + 1] ;
	strcpy( theNewRxn->name, reactionName.c_str() );
    }



    theNewRxn->rate = new double;
    
    *(theNewRxn->rate) = pMzrReaction->getRate();
    
    createCSpeciesArrayFromSpeciesMap( cMzrPtr, pMzrReaction->getReactants(),&theNewRxn->reactantVector, theNewRxn->numberReactants); 
    createCSpeciesArrayFromSpeciesMap( cMzrPtr, pMzrReaction->getProducts(), &theNewRxn->productVector, theNewRxn->numberProducts);
    
    return theNewRxn;
}


void createCSpeciesArrayFromSpeciesMap(moleculizer* cMzrPtr, 
                                       const mzr::mzrReaction::multMap& speciesMap, 
                                       species*** ptrSpeciesPtrArray, int& numberInList)
{
    int numberSpeciesInMap = calculateSumOfMultMap(speciesMap);
    
    typedef species* SpeciesPtr;
    *ptrSpeciesPtrArray = new SpeciesPtr[numberSpeciesInMap] ;
    
    int counter = 0;

    for( mzr::mzrReaction::multMap::const_iterator specIter = speciesMap.begin();
         specIter != speciesMap.end();
         ++specIter)
    {
        int specCounter = 0;
        while(specCounter++ < specIter->second)
        {
            // This is the index-th species*
            (*ptrSpeciesPtrArray)[counter++] = createNewCSpeciesFromMzrSpecies(cMzrPtr, specIter->first);
        }
    }
    
    numberInList = numberSpeciesInMap;
    
    return;
}


int checkSpeciesTagIsInSpeciesStream( moleculizer* handle, const char* speciesTag, char* speciesStream)
{
    try
    {
        std::string speciesTagStr( speciesTag );
        std::string speciesStreamStr( speciesStream );

        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );

        if ( underlyingMoleculizerObject->speciesWithTagIsInSpeciesStream(speciesTagStr, speciesStreamStr) )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    catch(...)
    {
        return -1;
    }
}


int checkSpeciesIDIsInSpeciesStream( moleculizer* handle, char* speciesID, char* speciesStream)
{

   try
    {
        std::string speciesIDStr( speciesID );
        std::string speciesStreamStr( speciesStream );

        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );

        if ( underlyingMoleculizerObject->speciesWithUniqueIDIsInSpeciesStream(speciesIDStr, speciesStreamStr) )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    catch(...)
    {
        return -1;
    }
}


int expandSpeciesByTag( moleculizer* handle, char* theTag) {
    try {
        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );
        underlyingMoleculizerObject->findSpecies(theTag)->expandReactionNetwork();
        return 0;
    }
    catch(...) {
        return 1;
    }
}


int expandSpeciesByID( moleculizer* handle, char* theID) {
    try {
        mzr::moleculizer* underlyingMoleculizerObject = convertCMzrPtrToMzrPtr( handle );
        underlyingMoleculizerObject->getSpeciesWithUniqueID(theID)->expandReactionNetwork();
        return 0;
    }
    catch(...) {
        return 1;
    }
}

