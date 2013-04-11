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


#ifndef WRITEOUTPUTGRAPH_HH
#define WRITEOUTPUTGRAPH_HH

#include <fstream>
#include "fnd/reactionNetworkDescription.hh"

std::string
getEscapedString(const std::string& originalName)
{
    std::string theCopy( originalName );

    std::replace(theCopy.begin(),
                 theCopy.end(),
                 '-', '_');
    
    return theCopy;
}

template <typename speciesT, 
          typename reactionT>
void writeNetworkToDotFile(const fnd::ReactionNetworkDescription<speciesT, reactionT>& reactionNetwork, 
                           const std::string& fileName)
{

    typedef fnd::ReactionNetworkDescription<speciesT, reactionT> ReactionNetwork;

    std::ofstream outputFile;
    outputFile.open( fileName.c_str() );
    outputFile << "digraph \n{\n";

    const char QUOTE_CHAR='"';

    for( typename ReactionNetwork::SpeciesCatalog::const_iterator rxnIter = reactionNetwork.getSpeciesCatalog().begin();
         rxnIter != reactionNetwork.getSpeciesCatalog().end();
         ++rxnIter)
    {
        outputFile << getEscapedString( *rxnIter->first ) << ';' << std::endl;
    }


    for( typename ReactionNetwork::ReactionList::const_iterator allRxnsIter = reactionNetwork.getReactionList().begin();
         allRxnsIter != reactionNetwork.getReactionList().end();
         ++allRxnsIter)
    {
        for( typename ReactionNetwork::ReactionType::multMap::const_iterator reactantIter = (*allRxnsIter)->getReactants().begin();
             reactantIter != (*allRxnsIter)->getReactants().end();
             ++reactantIter)
        {
            for( typename ReactionNetwork::ReactionType::multMap::const_iterator productIter = (*allRxnsIter)->getProducts().begin();
                 productIter != (*allRxnsIter)->getProducts().end();
                 ++productIter)
            {
                outputFile << QUOTE_CHAR << getEscapedString( (*reactantIter).first->getName()) << QUOTE_CHAR << " -> " << QUOTE_CHAR <<getEscapedString((*productIter).first->getName()) << QUOTE_CHAR << ';' << std::endl;;
            }
        }
    }

    outputFile << "}" << std::endl;
    outputFile.close();
}

#endif
