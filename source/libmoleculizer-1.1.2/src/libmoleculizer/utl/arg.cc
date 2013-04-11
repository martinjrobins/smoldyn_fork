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
//   Larry Lok, Research Fellow, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//

#include "utl/utility.hh"
#include "utl/utlXcpt.hh"
#include "utl/arg.hh"

namespace utl
{
    std::string
    mustGetArg( int& rArgc,
                char**& rArgv )
        throw( xcpt )
    {
        if ( rArgc <= 0 )
        {
            throw insuffArgsXcpt::general();
        }
        
        std::string argString( *rArgv );
        ++rArgv;
        --rArgc;
        
        return argString;
    }
    
    int
    argMustBePosInt( const std::string& rArgString )
        throw( xcpt )
    {
        int argValue = -1;
        
        if ( !( stringIsInt( rArgString,
                             argValue )
                && ( 0 < argValue ) ) )
        {
            throw badPosIntArgXcpt( rArgString );
        }
        
        return argValue;
    }
    
    int
    argMustBeNNInt( const std::string& rArgString )
        throw( xcpt )
    {
        int argValue = -1;
        
        if ( !( stringIsInt( rArgString,
                             argValue )
                && ( 0 <= argValue ) ) )
        {
            throw badNNIntArgXcpt( rArgString );
        }
        
        return argValue;
    }
    
    double
    argMustBeNNDouble( const std::string& rArgString )
        throw( xcpt )
    {
        double argValue = -1.0;
        
        if ( !( stringIsDouble( rArgString,
                                argValue )
                && ( 0.0 <= argValue ) ) )
        {
            throw badNNDoubleArgXcpt( rArgString );
        }
        
        return argValue;
    }
}
