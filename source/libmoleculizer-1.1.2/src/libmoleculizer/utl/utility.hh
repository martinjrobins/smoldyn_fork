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

#ifndef UTILITY_HH
#define UTILITY_HH

#include <string>
#include <vector>
#include <sstream>
#include "utlXcpt.hh"

namespace utl
{
    std::string
    getFileName( int argc,
                 char* argv[] );
    
    void tokenize( const std::string& str,
                   std::vector<std::string>& tokens,
                   const std::string& deliminator = " " );
    
    template <class T>
    bool from_string( T& t,
                      const std::string& s )
    {
        std::istringstream iss( s );
        return !( iss >> t ).fail();
    }
    
    template <class ForwardIter,
              class OutputIter,
              class UnaryPred>
    OutputIter copy_if( ForwardIter begin,
                        ForwardIter end,
                        OutputIter dest,
                        UnaryPred f )
    {
        while ( begin!=end )
        {
            if ( f( *begin ) )
            {
                *dest=*begin;
                ++dest;
            }
            ++begin;
        }
        return dest;
    }
    
    
    template <typename WriteableType>
    std::string
    stringify( const WriteableType& rThingToStringify )
    {
        typename std::ostringstream oss;
        oss << rThingToStringify;
        return oss.str();
    }
    
    bool
    stringIsInt( const std::string& rString,
                 int& rInt );
    
    bool
    stringIsDouble( const std::string& rString,
                    double& rDouble );

    namespace aux
    {
        template <typename T>
        class compareByPtrValue
        {
        public:
            bool operator()( const T* const a, const T* const b ) const
            {
                return *a < *b;
            }
        };
        
        template <typename ListCatalogT>
        class doDeleteStringPtrs
            : public std::unary_function<typename ListCatalogT::value_type, void>
        {
        public:
            void operator()( const typename doDeleteStringPtrs::argument_type& refPairWithString )
            {
                delete refPairWithString.first;
            }
        };
        
        template <typename ListCatalogT>
        class findEntryWithName
            : public std::unary_function<typename ListCatalogT::value_type, bool>
        {
        public:
            findEntryWithName( const std::string& nameToFind )
                :
                nameMatchTarget( nameToFind )
            {}
            
            bool operator()( const typename findEntryWithName::argument_type& potentialMatch )
            {
                return ( *potentialMatch.first == nameMatchTarget );
            }
            
        private:
            const std::string& nameMatchTarget;
        };
    }

    
    
}

#endif
