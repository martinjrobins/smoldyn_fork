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


#ifndef __MZREXCEPTION_HH
#define __MZREXCEPTION_HH

#include "utl/xcpt.hh"
#include "utl/dom.hh"

namespace mzr
{
    class ModelNotLoadedXcpt : public utl::xcpt
    {
    public:
        static std::string
        mkMsg(const std::string& functionName);
        ModelNotLoadedXcpt(const std::string& funcName);
    };

    class BadRulesDefinitionXcpt : public utl::xcpt
    {
    public:
        static std::string
        mkMsg();
        
        BadRulesDefinitionXcpt();
    };
    

    class IllegalNameXcpt : public utl::xcpt
    {
    public:
        static std::string
        mkMsg( const std::string& illegalName );
        IllegalNameXcpt( const std::string& illegalName );
    };
    
    class illegalSpeciesNameXcpt
        : public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rSpeciesName,
               const std::string& rMessage = "" );
    public:
        illegalSpeciesNameXcpt( const std::string& rSpeciesName, const std::string& rMessage );
    };
    
    class dumpableNotSpeciesStreamXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rDumpableName );
        
    public:
        dumpableNotSpeciesStreamXcpt( const std::string& rDumpableName ) :
            utl::xcpt( mkMsg( rDumpableName ) )
        {}
    };
    
    
    class unkDumpableXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rDumpableName,
               xmlpp::Node* pOffendingNode = 0 );
        
    public:
        unkDumpableXcpt( const std::string& rDumpableName,
                         xmlpp::Node* pOffendingNode = 0 ) :
            utl::xcpt( mkMsg( rDumpableName,
                              pOffendingNode ) )
        {}
    };
    
    class dupDumpableNameXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rDumpableName,
               xmlpp::Node* pOffendingNode = 0 );
        
    public:
        dupDumpableNameXcpt( const std::string& rDumpableName,
                             xmlpp::Node* pOffendingNode = 0 ) :
            utl::xcpt( mkMsg( rDumpableName,
                              pOffendingNode ) )
        {}
    };
    
    
    class unhandledModelContentXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( xmlpp::Node* pOffendingModelContentNode );

    public:
        unhandledModelContentXcpt( xmlpp::Node* pOffendingModelContentNode ) :
            utl::xcpt( mkMsg( pOffendingModelContentNode ) )
        {}
    };
    
    class unhandledExplicitSpeciesContentXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( xmlpp::Node* pBadExplicitSpeciesContentNode );

    public:
        unhandledExplicitSpeciesContentXcpt( xmlpp::Node* pBadExplicitSpeciesContentNode ) :
            utl::xcpt( mkMsg( pBadExplicitSpeciesContentNode ) )
        {}
    };
    
    class unhandledSpeciesStreamsContentXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( xmlpp::Node* pBadSpeciesStreamsContentNode );

    public:
        unhandledSpeciesStreamsContentXcpt( xmlpp::Node* pBadSpeciesStreamsContentNode ) :
            utl::xcpt( mkMsg( pBadSpeciesStreamsContentNode ) )
        {}
    };
    
    class unhandledEventsContentXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( xmlpp::Node* pOffendingEventsContentNode );
        
    public:
        unhandledEventsContentXcpt( xmlpp::Node* pOffendingEventsContentNode ) :
            utl::xcpt( mkMsg( pOffendingEventsContentNode ) )
        {}
    };
    
    class unhandledReactionGenXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( xmlpp::Node* pBadReactionGenNode );

    public:
        unhandledReactionGenXcpt( xmlpp::Node* pBadReactionGenNode ) :
            utl::xcpt( mkMsg( pBadReactionGenNode ) )
        {
        }
    };
    
    class unkSpeciesXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rSpeciesName,
               xmlpp::Node* pOffendingNode = 0 );
        
    public:
        unkSpeciesXcpt( const std::string& rSpeciesName,
                        xmlpp::Node* pOffendingNode = 0 ) :
            utl::xcpt( mkMsg( rSpeciesName,
                              pOffendingNode ) )
        {}
    };
    
    class unknownUserNameXcpt
        :
        public utl::xcpt
    {
        static std::string
        mkMsg(const std::string& unknownUserName);
        
    public:
        unknownUserNameXcpt( const std::string& unkName)
            :
            utl::xcpt( mkMsg( unkName ))
        {}
        
    };
    
    class dupSpeciesNameXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rSpeciesName,
               xmlpp::Node* pRequestingNode = 0 );
        
    public:
        dupSpeciesNameXcpt( const std::string& rSpeciesName,
                            xmlpp::Node* pRequestingNode = 0 ) :
            utl::xcpt( mkMsg( rSpeciesName,
                              pRequestingNode ) )
        {}
    };
    
    
    class stopEventInPastXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( double now,
               double badEventTime );
        
    public:
        stopEventInPastXcpt( double now,
                             double badEventTime ) :
            utl::xcpt( mkMsg( now,
                              badEventTime ) )
        {}
    };
    
    class unkStatStreamXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rBadStreamName,
               xmlpp::Node* pOffendingNode = 0 );
        
    public:
        unkStatStreamXcpt( const std::string& rBadStreamName,
                           xmlpp::Node* pOffendingNode = 0 ) :
            utl::xcpt( mkMsg( rBadStreamName,
                              pOffendingNode ) )
        {}
    };
    
    class missingExtrapolationParameter :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rSpeciesName,
               const std::string& missingParamName );
        
    public:
        missingExtrapolationParameter( const std::string& rSpeciesName,
                                       const std::string& missingParamName )
            :
            utl::xcpt( mkMsg( rSpeciesName, missingParamName ) )
        {}
    };

    class badPreconditionXcpt : public utl::xcpt
    {
    public:
        badPreconditionXcpt( const std::string& msg)
            :
            utl::xcpt( msg )
        {}
    };
    
    class mzrPythonXcpt : public utl::xcpt
    {
    public:
	mzrPythonXcpt( const std::string& msg, const std::string& pythonException)
	    : 
	    utl::xcpt( msg ), 
            python_error( pythonException)
	{}

        ~mzrPythonXcpt() throw() {}

        std::string python_error;
        int line_number;

        std::string getPythonMessage() const;
        std::string getLongMessage() const;
    };

    class MoleculizerModuleMissingXcpt : public utl::xcpt
    {
    public:
        MoleculizerModuleMissingXcpt( const std::string& msg)
            :
            utl::xcpt( msg )
        {
        }
    };

    class SpeciesIsNotPlexType : public utl::xcpt
    {
						static std::string
						mkMsg(const std::string& specName);
    public:
						SpeciesIsNotPlexType( const std::string& specName)
								:
								utl::xcpt(mkMsg(specName))
						{}
    };


}

#endif
