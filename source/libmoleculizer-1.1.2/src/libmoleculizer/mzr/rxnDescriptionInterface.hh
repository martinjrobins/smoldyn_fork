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

#ifndef RXNDESCRIPTIONINTERFACE_HH
#define RXNDESCRIPTIONINTERFACE_HHo

#include "utl/autoCatalog.hh"
#include <vector>
#include <list>

namespace mzr
{
    class mzrReaction;
    class mzsSpecies;
}

struct ReactionNetworkDescription
{
    // Doesn't memory manage anything
    
    bool
    recordSpecies( mzrSpecies* pSpecies )
    {}
    
    bool
    recordReaction( mzrReaction* pReaction )
    {
        std::string reactionName = pReaction->getCanoncalName();
        return reactionCatalog->addEntry( reactionName, pReaction );
    }
    
    utl::catalog<mzr::mzrSpecies>* pSpeciesCatalog;
    std::list<mzr::mzrSpecies*>* pSpeciesList;
    utl::catalog<mzr::mzrReaction>* pReactionCatalog;
    std::list<mzr::mzrReaction*>* pReactionList;
    
    ReactionNetoworkDescription( utl::catalog<mzr::mzrSpecies>* aSpeciesCatalog,
                                 std::list<mzr::mzrSpecies*>* aSpeciesList,
                                 utl::catalog<mzr::mzrReaction>* aReactionCatalog,
                                 std::list<mzr::mzrReaction*>* aReactionList )
        :
        pSpeciesCatalog( aSpeciesCatalog ),
        pSpeciesList( aSpeciesList ),
        pReactionCatalog( aReactionCatalog ),
        pReactionList( aReactionList )
    {}
    
};


#endif
