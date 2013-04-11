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

#ifndef FNDEXCEPTIONS_HH
#define FNDEXCEPTIONS_HH

#include "utl/defs.hh"
#include "utl/xcpt.hh"
#include "utl/dom.hh"

namespace fnd
{
    
    class DuplicatedCatalogEntryXcpt : public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& ElementName )
        {
            std::ostringstream msgStream;
            msgStream << "Internal Error: "
                      << "Object "
                      << "'"
                      << ElementName
                      << "' was already present in the ReactionNetworkDescription.";
            return msgStream.str();
        }
        
    public:
        DuplicatedCatalogEntryXcpt( const std::string& refObjectName )
            :
            utl::xcpt( mkMsg( refObjectName ) )
        {}
    };
    
    class NoSuchSpeciesXcpt : public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& speciesName )
        {
            std::ostringstream msgStream;
            msgStream << "Internal Error: Species '"
                      << speciesName
                      << "' was not found in ReactionNetworkDescription.";
            return msgStream.str();
        }
        
    public:
        NoSuchSpeciesXcpt( const std::string& speciesName )
            :
            xcpt( mkMsg( speciesName ) )
        {}
        
    };
    
    class NoSuchReactionXcpt : public utl::xcpt
    {
        std::string
        makeMessage( const std::string& reactionName )
        {
            std::ostringstream msgStream;
            msgStream << "Internal Error: Reaction '"
                      << reactionName
                      << "' was not found in ReactionNetworkDescription.";
            return msgStream.str();
        }
        
    public:
        ~NoSuchReactionXcpt() throw() {}
        
        
        NoSuchReactionXcpt( const std::string& reactionName )
            :
            xcpt( "" ),
            theReactionName( reactionName )
        {
            message = this->makeMessage( reactionName );
        }
        
    private:
        std::string theReactionName;
    };
    
    
    class badDumpFileXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rBadFileName );
        
    public:
        badDumpFileXcpt( const std::string& rBadFileName ) :
            utl::xcpt( mkMsg( rBadFileName ) )
        {}
    };
    
    class speciesNotMassiveXcpt :
        public utl::xcpt
    {
        std::string
        mkMsg( xmlpp::Node* pOffendingNode = 0 );
    public:
        speciesNotMassiveXcpt( xmlpp::Node* pOffendingNode = 0 );
    };
    
    
}



#endif
