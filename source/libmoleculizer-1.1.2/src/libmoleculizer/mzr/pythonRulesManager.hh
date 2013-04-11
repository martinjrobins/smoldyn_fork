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
//

#ifndef __PYTHON_RULES_MANAGER_HH
#define __PYTHON_RULES_MANAGER_HH

#include <Python.h>
#include <string>

namespace mzr
{

  class PythonRulesManager
  {
  public:
    PythonRulesManager();
    ~PythonRulesManager();

    std::string 
    getXmlString() const;

    std::string 
    addRulesFile( const std::string& rulesFile);
    
    std::string 
    addRulesString( const std::string& rulesString);

    void addParameterStatement(const std::string& paramLine);
    void addModificationStatement(const std::string& modLine);
    void addMolsStatement(const std::string& molsLIne);
    void addAllostericPlexStatement(const std::string& alloPlexLine);
    void addAllostericOmniStatement(const std::string& alloOmniLine);
    void addDimerizationGenStatement(const std::string& dimerGenLine);
    void addOmniGenStatement(const std::string& omniGenLine);
    void addUniMolGenStatement(const std::string& uniMolGenLine);
    void addSpeciesStreamStatement(const std::string& speciesStreamLine);

    bool isInitialized() const
    {
      return _isInitialized;
    }

  protected:
    void initialize();
      std::string capturePythonErrorMessage() const;

  private:


    bool _isInitialized;

      char* paramPythonFunctionName;
      char* modPythonFunctionName;
      char* molPythonFunctionName;
      char* alloPlexPythonFunctionName;
      char* alloOmniPythonFunctionName;
      char* dimerGenPythonFunctionName;
      char* omniGenPythonFunctionName;
      char* uniMolGenPythonFunctionName;
      char* speciesStreamPythonFunctionName;
      char* explicitSpeciesPythonFunctionName;
      char* getFileStringFunctionName;
      char* addWholeRulesFileFunctionName;
      char* addWholeRulesStringFunctionName;
      char* singleStringTuple;
      
      PyObject* mzrFileConverterClassInst;
      PyObject* getErrorFunction;
  };
}


#endif
