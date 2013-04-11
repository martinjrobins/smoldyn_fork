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


#include "complexOutputState.hh"
#include <sstream>

namespace nmr
{
    bool
    ComplexOutputState::operator== ( ComplexOutputStateCref other ) const
    {
        return ( theMolTokens == other.theMolTokens &&
                 theBindingTokens == other.theBindingTokens &&
                 theModificationTokens == other.theModificationTokens );
    }
    
    bool
    ComplexOutputState::operator!= ( ComplexOutputStateCref other ) const
    {
        return !( *this == other );
    }
    
    void
    ComplexOutputState::addMolTokenToOutputState( MolTokenStrCref aMolToken )
    {
        theMolTokens.push_back( aMolToken );
    }
    
    void
    ComplexOutputState::addBindingTokenToOutputState( BindingTokenStrCref aBindingToken )
    {
        theBindingTokens.push_back( aBindingToken );
        
    }
    
    void
    ComplexOutputState::addModificationTokenToOutputState( ModificationTokenStrCref aModificationToken )
    {
        theModificationTokens.push_back( aModificationToken );
    }
    
    void
    ComplexOutputState::clear()
    {
        theMolTokens.clear();
        theBindingTokens.clear();
        theModificationTokens.clear();
    }
    
    std::string
    ComplexOutputState::repr() const
    {
        std::ostringstream oss;
        for( unsigned int ndx = 0; ndx != theMolTokens.size(); ++ndx)
        {
            oss << theMolTokens[ndx] << "-";
        }
        

        for( unsigned int ndx = 0; ndx != theBindingTokens.size(); ++ndx)
        {
            oss << '|' ;
            oss << theBindingTokens[ndx].first.first << "-";
            oss << theBindingTokens[ndx].first.second << "-";
            oss << theBindingTokens[ndx].second.first << "-";
            oss << theBindingTokens[ndx].second.second << '|' << '-';
        } 

        for( unsigned int ndx = 0; ndx != theModificationTokens.size(); ++ndx)
        {
            oss << '|';
            oss << theModificationTokens[ndx].first
                << '-'
                << theModificationTokens[ndx].second.first
                << '-'
                << theModificationTokens[ndx].second.second << '|';
        }
        
        return oss.str();
        
    }
}

std::ostream& operator<< ( std::ostream& os, const nmr::ComplexOutputState& cos )
{
    //   os << "Mols:\n";
    //   for(std::vector<nmr::ComplexOutputState::MolTokenStr>::const_iterator iter = cos.theMolTokens.begin();
    //       iter != cos.theMolTokens.end();
    //       ++iter)
    //     {
    //       os << *iter << ", ";
    
    //     }
    //   os << "\nBindings:\n";
    
    //   for(std::vector<nmr::ComplexOutputState::BindingTokenStr>::const_iterator iter = cos.theBindingTokens.begin();
    //       iter != cos.theBindingTokens.end();
    //       ++iter)
    //     {
    //       os << "((" << (*iter).first.first << ", " << (*iter).first.second << "), (" << (*iter).second.first << ", " << (*iter).second.second << ")), ";
    //     }
    
    //   os << "\nModifications:\n";
    
    //   for(std::vector<nmr::ComplexOutputState::ModificationTokenStr>::const_iterator iter = cos.theModificationTokens.begin();
    //       iter != cos.theModificationTokens.end();
    //       ++iter)
    //     {
    //       os << "( " << (*iter).first << ", ( " << (*iter).second.first << ", " << (*iter).second.second << ")), ";
    //     }
    //   if (cos.theModificationTokens.size() == 0) os << "*";
    
    //   os << "\n";
    //   return os;
    
    os << cos.repr();
    return os;
}
