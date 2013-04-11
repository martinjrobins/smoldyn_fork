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

#ifndef DOMBATCHJOB_H
#define DOMBATCHJOB_H

#include <exception>

namespace utl
{
    namespace dom
    {
        // Base class for batchAppClass template argument to domBatchJob
        // and twoDomJob templates.
        //
        // It's not clear that this virtual base class is worth the ink, since the
        // application class must also have a certain kind of constructor.
        class domBatchApp
        {
        public:
            virtual
            ~domBatchApp( void )
            {}
            
            virtual int
            run( void ) throw( std::exception ) = 0;
        };
        
        // Template batch app that parses one XML input file from standard input,
        // constructs the application from it, and runs it.
        template<class batchAppClass>
        class domBatchJob
        {
            int argCount;
            char** argVector;
            
        public:
            domBatchJob( int argc, char** argv ) :
                argCount( argc ),
                argVector( argv )
            {}
            
            int
            parseNrun( void );
        };
        
        // Template batch app that parses one XML input file from standard input,
        // and another named as the first argument.  The application is
        // constructed from the two parsed XML documents, and then run.
        template<class batchAppClass>
        class twoDomJob
        {
            int argCount;
            char** argVector;
            
        public:
            twoDomJob( int argc, char** argv ) :
                argCount( argc ),
                argVector( argv )
            {}
            
            int
            parseNrun( void );
        };
    }
}

#include "utl/domJobImpl.hh"

#endif // DOMBATCHJOB_H
