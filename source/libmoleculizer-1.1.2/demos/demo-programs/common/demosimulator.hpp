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


#include "mzr/moleculizer.hh"
#include <string>
#include <map>

class SimpleSimulator
{
public:
    SimpleSimulator();
    // SimpleSimulator( std::string rulesfile, std::string modelfile );
    virtual ~SimpleSimulator();

    void loadRules(std::string rulesFile);
    void releaseRules();
    void loadModel(std::string modelFile);

    virtual void singleStep() = 0;
    void printState() const;

    int getNumSpecies() const
    {
        return ptrSpeciesReactionGenerator->getTotalNumberSpecies();
    }

    int getNumRxns() const
    {
        return ptrSpeciesReactionGenerator->getTotalNumberReactions();
    }

public:
    typedef std::pair<std::string, int> modelPairType;

protected:
    mzr::moleculizer* ptrSpeciesReactionGenerator;
    bool rulesLoaded;
    
    std::map<std::string, int> theModel;

    virtual void initialize();

    void executeReaction( const mzr::moleculizer::ReactionType* ptrRxn );

private:
    void attachRuleFile( std::string rulesfile );
    void attachModelFile( std::string modelfile );

    bool assertModelValidity( const std::map<std::string, int>& model );


    void createModelFromFile( const std::string& modelFile, std::map<std::string, int>& model );
};




