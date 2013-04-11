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

#ifndef UTL_XCPT_H
#define UTL_XCPT_H

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

#define DEFINE_MZR_EXCEPTION( xcptName )    \
    class xcptName : public utl::xcpt       \
    {                                       \
    public:                                 \
        xcptName()                          \
            :                               \
            utl::xcpt("Internal Exception: xcptName.")  \
        {}                                              \
    };                                                  \
    


#define DEFINE_STANDARD_MSG_EXCEPTION_CLASS( xcptName, message) \
    class xcptName : public utl::xcpt                           \
    {                                                           \
    public:                                                     \
        xcptName()                                              \
            :                                                   \
            utl::xcpt( message )                                \
        {}                                                      \
    };

namespace utl
{
    class xcpt :
        public std::exception
    {
    protected:
        std::string message;
        
    public:
        xcpt( const std::string& rMessage ) :
            message( rMessage )
        {}
        
        xcpt( const char* pMessage ) :
            message( pMessage )
        {}
        
        ~xcpt( void ) throw()
        {}
        
        const std::string&
        getMessage( void ) const throw()
        {
            return message;
        }
        
        // Implementation of std::exception::what.
        const char*
        what( void ) const throw()
        {
            return message.c_str();
        }
        
        void
        warn( void )
        {
            std::cerr << mkWarnMsg()
                      << getMessage()
                      << std::endl;
        }
        
        void
        wailAndBail( void )
        {
            std::cerr << mkMsg()
                      << getMessage()
                      << std::endl;
            exit( 1 );
        }
        
        // But this doesn't include the application name, and it doesn't
        // look good for getting it in where this message is called for.
        //
        // Most of those cases should be handled with wailAndBail, rather than
        // throw, since they are all essentially debugging loose-ends.
        static std::string
        mkMsg( void )
        {
            return std::string( "(CRITICAL) " );
        }
        
        static std::string
        mkWarnMsg( void )
        {
            return std::string( "(WARNING) " );
        }
    };
    
    class FatalXcpt : public xcpt
    {
        static std::string
        mkMsg(const std::string& message)
        {
            std::ostringstream oss;
            oss << xcpt::mkMsg()
                << "Fatal Exception: " 
                << message;
            return oss.str();
        }
        
    public:
        FatalXcpt( const std::string& errorMsg)
            :
            xcpt( mkMsg( errorMsg) )
        {}
    };
    
    class NotImplementedXcpt : public xcpt
    {
        static std::string
        mkMsg( const std::string& functionName )
        {
            std::ostringstream oss;
            oss << "Error: function '"
                << functionName
                << "' has not yet been implemented.";
            return oss.str();
        }
        
    public:
        NotImplementedXcpt( const std::string& funcName )
            :
            xcpt( mkMsg( funcName ) )
        {}
    };
    
    class UndefinedBehaviorXcpt : public xcpt
    {
    public:
        std::string
        mkMsg( const std::string& functionName )
        {
            std::ostringstream oss;
            oss << "Error in function '"
                << functionName
                << "'.  Undefined behavior.";
            return oss.str();
        }
        
        std::string
        mkMsg( const std::string& functionName, const std::string& msg )
        {
            std::ostringstream oss;
            oss << "Error in function '"
                << functionName
                << "'.  Undefined behavior with msg='"
                << msg
                << "'";
            return oss.str();
        }
        
        UndefinedBehaviorXcpt( const std::string& functionName )
            :
            utl::xcpt( mkMsg( functionName ) )
        {}
        
        
        UndefinedBehaviorXcpt( const std::string& functionName, const std::string& msg )
            :
            utl::xcpt( mkMsg( functionName, msg ) )
        {}
        
    };
    
}

#endif // UTL_XCPT_H
