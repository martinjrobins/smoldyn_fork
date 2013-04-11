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

#include "basicNameAssembler.hh"

#include <string>
#include <vector>

namespace nmr
{
    
    std::string
    basicNameAssembler::createNameFromOutputState( ComplexOutputStateCref aCOS ) const
    {
        std::string name( "" );
        for ( std::vector<std::string>::const_iterator i = aCOS.theMolTokens.begin();
              i != aCOS.theMolTokens.end();
              ++i )
        {
            name += *i + ", ";
        }
        
        name = name.substr( 0 , name.length() - 2 );
        name += " -- ";
        
        for ( std::vector< ComplexOutputState::BindingTokenStr >::const_iterator i = aCOS.theBindingTokens.begin();
              i != aCOS.theBindingTokens.end();
              ++i )
        {
            name += ( *i ).first.first + ( *i ).first.second + ( *i ).second.first + ( *i ).second.second + ", ";
        }
        
        if ( aCOS.theBindingTokens.size() )
        {
            name = name.substr( 0 , name.length() - 2 );
        }
        
        name += " -- ";
        
        
        for ( std::vector< ComplexOutputState::ModificationTokenStr >::const_iterator iter = aCOS.theModificationTokens.begin();
              iter != aCOS.theModificationTokens.end();
              ++iter )
        {
            name += "( " + ( *iter ).first + ", " + ( *iter ).second.first + ", " + ( *iter ).second.second + "), ";
        }
        
        return name;
    }
    
    ComplexOutputState
    basicNameAssembler::createOutputStateFromName( const std::string& aMangledName ) const throw( utl::NotImplementedXcpt )
    {
        // TODO/3 Write this function( basicNameAssembler::createOutputStateFromName).
        return NameAssembler::createOutputStateFromName( aMangledName );
    }
    
}
