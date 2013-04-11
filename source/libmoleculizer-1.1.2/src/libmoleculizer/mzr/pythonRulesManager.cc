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

#include "pythonRulesManager.hh"
#include "mzrException.hh"

#include <iostream>

namespace mzr
{
    PythonRulesManager::PythonRulesManager()
        :
        _isInitialized( false ),
        paramPythonFunctionName( new char[256] ),
        modPythonFunctionName( new char[256] ),
        molPythonFunctionName( new char[256] ),
        alloPlexPythonFunctionName( new char[256] ),
        alloOmniPythonFunctionName( new char[256] ),
        dimerGenPythonFunctionName( new char[256] ),
        omniGenPythonFunctionName( new char[256] ),
        uniMolGenPythonFunctionName( new char[256] ),
        speciesStreamPythonFunctionName(new char[256] ),
        explicitSpeciesPythonFunctionName(new char[256] ),
        getFileStringFunctionName( new char[256] ),
        addWholeRulesFileFunctionName( new char[256] ),
        addWholeRulesStringFunctionName( new char[256] ),
        singleStringTuple( new char[256] ), // This parameter describes that the arguments to follow amount to a single string.
        mzrFileConverterClassInst( NULL )
    {
        strcpy( paramPythonFunctionName, "addParameterStatement" );
        strcpy( modPythonFunctionName, "addModificationStatement" );
        strcpy( molPythonFunctionName, "addMolStatement" );
        strcpy( alloPlexPythonFunctionName, "addAllostericPlexStatement" );
        strcpy( alloOmniPythonFunctionName, "addOmniGenStatement" );
        strcpy( dimerGenPythonFunctionName, "addDimerizationGenStatement" );
        strcpy( omniGenPythonFunctionName, "addOmniGenStatement" );
        strcpy( uniMolGenPythonFunctionName, "addUniMolGenStatement" );
        strcpy( speciesStreamPythonFunctionName, "addSpeciesStreamStatement" );
        strcpy( explicitSpeciesPythonFunctionName, "addExplicitSpeciesStatement" );
        strcpy( getFileStringFunctionName, "writeToString" );
        strcpy( addWholeRulesFileFunctionName, "addWholeRulesFile" );
        strcpy( addWholeRulesStringFunctionName, "addWholeRulesString" );
        strcpy( singleStringTuple, "s" ); // This parameter describes that the arguments to follow amount to a single string

    }

    PythonRulesManager::~PythonRulesManager()
    {
        // This is pretty bad.  For now, I am basically commenting out
        // everything.  It doesn't matter too much, as libmoleculizer
        // is alive for the entire lifetime of Smoldyn, but otherwise
        // I get complaints from OSX malloc about non-aligning
        // pointers being freed, which I don't really understand.
        
        if (isInitialized() )
        {
            // This releases the memory of the moleculizer object back to
            // python.
            // //Py_DECREF( mzrFileConverterClassInst );

            // This finalizes and finishes the python interpreter.
            // Py_Finalize();
        }
        
        // Delete the rest of our local memory.
        delete paramPythonFunctionName;
        delete modPythonFunctionName;
        delete molPythonFunctionName;
        delete alloPlexPythonFunctionName;
        delete alloOmniPythonFunctionName;
        delete dimerGenPythonFunctionName;
        delete omniGenPythonFunctionName;
        delete uniMolGenPythonFunctionName;
        delete speciesStreamPythonFunctionName;
        delete explicitSpeciesPythonFunctionName;
        delete getFileStringFunctionName;
        delete addWholeRulesFileFunctionName;
        delete addWholeRulesStringFunctionName;
        delete singleStringTuple;
        // delete mzrFileConverterClassInst;
    }

    void 
    PythonRulesManager::initialize()
    {
        if (this->isInitialized()) {
            throw utl::xcpt("Error in libmoleculizer startup.  Trying to initialize Python twice."); }

        PyObject *pName, *pModule, *pDict, *pFunc;
        PyObject *pSysName, *pSysModule;

        PyObject *pErrorHandlerClass;
        PyObject *pErrorHandler;

        Py_Initialize();
        
        pName = PyString_FromString( "moleculizer" );
        pModule = PyImport_Import( pName );

        if (pModule == NULL || PyErr_Occurred())
        {
            throw MoleculizerModuleMissingXcpt("Error in libmoleculizer: The Moleculizer module could not be loaded.  Check your PYTHONPATH.");
        }

        // Setup the Error Handling stuff
        pDict = PyModule_GetDict( pModule );

        pErrorHandlerClass = PyDict_GetItemString( pDict, "StdoutCatcher" );
        if (pModule == NULL || PyErr_Occurred())
        {
            throw MoleculizerModuleMissingXcpt("Error in libmoleculizer: In the Moleculizer python module the class StdoutCatcher could not be found.  Are you using libmoleculizer with an old version of the Moleculizer python libraries?");
        }

        pErrorHandler = PyObject_CallObject( pErrorHandlerClass, NULL);
        this->getErrorFunction = PyObject_GetAttrString(pErrorHandler, "getLastLine");
        
        if (pErrorHandler == NULL || PyErr_Occurred())
        {
            throw MoleculizerModuleMissingXcpt("Error in libmoleculizer: An object of type StdoutCatcher could not be created.");
        }
        int ret = PySys_SetObject("stderr", pErrorHandler);


        pFunc = PyDict_GetItemString( pDict, "MoleculizerRulesFile" );
        if (pFunc==NULL || PyErr_Occurred())
        {
            std::string error_string = capturePythonErrorMessage();
            throw mzrPythonXcpt("In the Moleculizer python module the class MoleculizerRulesFile could not be found.", error_string);
        }

        this->mzrFileConverterClassInst = PyObject_CallObject(pFunc, NULL);

        if (mzrFileConverterClassInst == NULL || PyErr_Occurred()) 
        {
            std::string error_string = capturePythonErrorMessage();
            throw mzrPythonXcpt("Could not create the python class moleculizer.MoleculizerRulesFile.  Cannot create PythonRulesManager.", error_string);
        }

        this->_isInitialized = true;
        return;
    }

    std::string
    PythonRulesManager::getXmlString() const
    {
        if( !isInitialized() ) 
            throw mzrPythonXcpt("You are trying to get the mzr-xml string for a rules-file that has not yet been entered (PythonRulesManager is not yet initialized), which is almost certainly not what you want.", "N/A");
                                                        
        PyObject* fileAsString = PyObject_CallMethod(mzrFileConverterClassInst, getFileStringFunctionName, NULL);

        if (!fileAsString)
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error, getXmlString did not work.", error_message);
        }

        std::string the_file( PyString_AsString(fileAsString) );
        //Py_DECREF( fileAsString );

        return the_file;
    }

    // 
    // std::string 
    // PythonRulesManager::addRulesFile( file_name ) 
    // 
    // Pass in the name of a file, and this function will return the
    // string of the xml file generated by reading and processing it.
    std::string 
    PythonRulesManager::addRulesFile( const std::string& rulesFile)
    // This function returns a xml-mzr string of a rules file passed in by string, by calling 
    // moleculizer.MoleculizerRulesFile.addWholeRulesFile( self, rulesFile), which clears its memory
    // and loads the contents of ruleFile into memory.  The results of the python class' getXmlString, 
    // which returns a string of the compiled mzr-xml file are returned.  
    {
        if( !isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod( mzrFileConverterClassInst, addWholeRulesFileFunctionName, singleStringTuple, rulesFile.c_str());
      
        if (result) 
        {
            //Py_DECREF( result );
            return getXmlString();
        }
        else 
        {
            // In this case there was an error.  Because this was an
            // error in the conversion of the human readable rules
            // file into XML, this exception will have theree methods:
            // one for getting the python exception string, one for
            // getting the line number, and one for getting the line
            // in the file that caused the problem.

            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a rules file in PythonRulesManager.", error_message);

        }

    }

    std::string
    PythonRulesManager::addRulesString( const std::string& rulesString)
    {
        if( !isInitialized() ) initialize();


        PyObject* result = PyObject_CallMethod( mzrFileConverterClassInst, addWholeRulesStringFunctionName, singleStringTuple, rulesString.c_str());
         if (result) 
        {
            //Py_DECREF( result );
            return getXmlString();
        }
        else 
        {
            std::string pyErrorMessage(capturePythonErrorMessage());
            throw mzrPythonXcpt("Error Interpreting/Translating Common Rules.", pyErrorMessage);
        }
    }


    void PythonRulesManager::addParameterStatement(const std::string& paramLine)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, paramPythonFunctionName, singleStringTuple, paramLine.c_str());
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a parameter statement in PythonRulesManager.", error_message);
        }

    }

    void PythonRulesManager::addModificationStatement(const std::string& modLine)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, modPythonFunctionName, singleStringTuple, modLine.c_str());
        if (result) 
        {
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a Modification statement in PythonRulesManager.", error_message);
        }

    }

    void PythonRulesManager::addMolsStatement(const std::string& molsLIne)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, molPythonFunctionName, singleStringTuple, molsLIne.c_str());
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a Molecules statement in PythonRulesManager.", error_message);
        }
    }

    void PythonRulesManager::addAllostericPlexStatement(const std::string& alloPlexLine)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, alloPlexPythonFunctionName, singleStringTuple, alloPlexLine.c_str());
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a Explicit-Allostery statement in PythonRulesManager.", error_message);
        }
    }

    void PythonRulesManager::addAllostericOmniStatement(const std::string& alloOmniLine)
    {

        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, alloOmniPythonFunctionName, singleStringTuple, alloOmniLine.c_str());
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a Allosteric-Classes statement in PythonRulesManager.", error_message);
        }
    }

    void PythonRulesManager::addDimerizationGenStatement(const std::string& dimerGenLine)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, dimerGenPythonFunctionName, singleStringTuple, dimerGenLine.c_str());
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a Association-Reactions statement in PythonRulesManager.", error_message);
        }
    }

    void PythonRulesManager::addOmniGenStatement(const std::string& omniGenLine)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, omniGenPythonFunctionName, singleStringTuple, omniGenLine.c_str());
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a Transformation-Reactions statement in PythonRulesManager.", error_message);
        }
    }

    void PythonRulesManager::addUniMolGenStatement(const std::string& uniMolGenLine)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, uniMolGenPythonFunctionName, singleStringTuple, uniMolGenLine.c_str());
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a parameter statement in PythonRulesManager.", error_message);
        }
    }

    void PythonRulesManager::addSpeciesStreamStatement(const std::string& speciesStreamLine)
    {
        if (!isInitialized() ) initialize();

        PyObject* result = PyObject_CallMethod(mzrFileConverterClassInst, speciesStreamPythonFunctionName, singleStringTuple, speciesStreamLine.c_str() );
        if (result) 
        {
            //Py_DECREF( result );
            return;
        }
        else 
        {
            std::string error_message(capturePythonErrorMessage());
            throw mzrPythonXcpt( "Error trying to add a Species-Classes statement in PythonRulesManager.", error_message);
        }
    }

    std::string PythonRulesManager::capturePythonErrorMessage() const
    {
        // Call the getErrorFunction to get the string, convert to a
        // std::string and return.
        PyObject* objResult = NULL;
        char* error_message; 

        PyErr_Print();

        objResult = PyObject_CallObject(getErrorFunction, NULL);

        if (objResult == NULL || PyErr_Occurred() )
        {
            std::cerr << "Error occurred calling get last line." << std::endl;
        }

        if (!PyString_Check(objResult))
        {
            std::cerr << "Did not return anything" << std::endl;
        }
        
        error_message = PyString_AsString(objResult);
        return std::string(error_message);
    }


}
