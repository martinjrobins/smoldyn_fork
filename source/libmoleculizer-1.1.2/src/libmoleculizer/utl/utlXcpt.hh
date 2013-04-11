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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2008
//   * Accumulating Larry's original exceptions into one file.

#ifndef __UTLXCPT_HH
#define __UTLXCPT_HH


// This file accumulates the general utl::xcpt's that larry wrote.

#include "xcpt.hh"

namespace utl
{
    
    class badNNDoubleArgXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rTheBadArg );
        
    public:
        badNNDoubleArgXcpt( const std::string& rTheBadArg ) :
            utl::xcpt( mkMsg( rTheBadArg ) )
        {}
    };
    
    class badNNIntArgXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rTheBadArgument );
        
    public:
        badNNIntArgXcpt( const std::string& rTheBadArgument ) :
            utl::xcpt( mkMsg( rTheBadArgument ) )
        {}
    };
    
    class badPosIntArgXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rTheBadArgument );
        
    public:
        badPosIntArgXcpt( const std::string& rTheBadArgument ) :
            utl::xcpt( mkMsg( rTheBadArgument ) )
        {}
    };
    
    class insuffArgsXcpt :
        public utl::xcpt
    {
        static std::string
        mkCountsMsg( int actualArgCount,
                     int minimumArgCount );
        
        static std::string
        mkGeneralMsg( void );
        
        insuffArgsXcpt( const std::string& rMsg );
        
    public:
        static
        insuffArgsXcpt
        general( void );
        
        static
        insuffArgsXcpt
        counts( int actualArgCount,
                int minimumArgCount );
    };
    
    class unkArgXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rTheUnrecognizedArg );
        
    public:
        unkArgXcpt( const std::string& rTheUnrecognizedArg ) :
            utl::xcpt( mkMsg( rTheUnrecognizedArg ) )
        {}
    };
    
    class modelAlreadyLoadedXcpt : public utl::xcpt
    {
        
    public:
        modelAlreadyLoadedXcpt()
            :
            utl::xcpt( "Model already loaded." )
        {}
    };
    
    class badFileNameXcpt :
        public xcpt
    {
    public:
        badFileNameXcpt()
            :
            xcpt( "Error: No filename specified.  Proper usage is \"-f FILENAME\"." )
        {}
    };
    
}


#endif
