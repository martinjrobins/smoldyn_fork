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


#include <libxml++/libxml++.h>
#include "mangledNameAssembler.hh"
#include "complexSpeciesOutputMinimizer.hh"

#include "utl/utility.hh"
#include "nmr/nmrUnit.hh"

#include <utility>
#include "mzr/mzrSpeciesDumpable.hh"


namespace nmr
{
    
    ComplexOutputState
    MangledNameAssembler::createOutputStateFromName( const std::string& name ) const throw( nmr::UnparsableNameXcpt )
    {
        try
        {
            std::vector<std::string> tokens;
            utl::tokenize( name, tokens, "___" );
            
            if ( tokens.size() != 3 ) throw nmr::UnparsableNameXcpt( name );
            
            
            std::string theMolString( tokens.at( 0 ) );
            std::string theBindingString( tokens.at( 1 ) );
            std::string theModificationString( tokens.at( 2 ) );
            
            std::vector<ComplexOutputState::MolTokenStr> molTokens;
            std::vector<ComplexOutputState::BindingTokenStr> bindingTokens;
            std::vector<ComplexOutputState::ModificationTokenStr> modificationTokens;
            
            parseMangledMolString( theMolString, molTokens );
            parseMangledBindingString( theBindingString, bindingTokens );
            parseMangledModificationString( theModificationString, modificationTokens );
            
            ComplexOutputState newOutputState;
            newOutputState.theMolTokens = molTokens;
            newOutputState.theBindingTokens = bindingTokens;
            newOutputState.theModificationTokens = modificationTokens;
            
            return newOutputState;
        }
        catch ( ... )
        {
            throw nmr::UnparsableNameXcpt( name );
        }
        
    }
    
    
    std::string
    MangledNameAssembler::createNameFromOutputState( ComplexOutputStateCref aComplexSpeciesOutputState ) const
    {
        std::string aComplexSpeciesName( "" );
        
        aComplexSpeciesName += "___";
        aComplexSpeciesName += constructMangledMolList( aComplexSpeciesOutputState );
        aComplexSpeciesName += "___";
        aComplexSpeciesName += constructMangledBindingList( aComplexSpeciesOutputState );
        aComplexSpeciesName += "___";
        aComplexSpeciesName += constructMangledModificationList( aComplexSpeciesOutputState );
        
        //         if (aComplexSpeciesName != "_________")
        //         {
        //             theNmrUnit.getSpeciesFromName( aComplexSpeciesName );
        //         }
        
        return aComplexSpeciesName;
    }
    
    
    std::string
    MangledNameAssembler::constructMangledMolList( ComplexOutputStateCref aComplexOutputState ) const
    {
        std::string mangledMolName( "" );
        
        std::vector<std::string>::const_iterator molIterator;
        for ( molIterator = aComplexOutputState.theMolTokens.begin();
              molIterator != aComplexOutputState.theMolTokens.end();
              ++molIterator )
        {
            std::string tmpName( *molIterator );
            tmpName=getEncodedLength( tmpName ) +tmpName;
            mangledMolName+=tmpName;
        }
        
        return mangledMolName;
    }
    

    std::string
    MangledNameAssembler::constructMangledBindingList( ComplexOutputStateCref aComplexOutputState ) const
    {
        std::string mangledBindingList( "" );
        
        std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string> > >::const_iterator bndIterator;
        for ( bndIterator = aComplexOutputState.theBindingTokens.begin();
              bndIterator != aComplexOutputState.theBindingTokens.end();
              ++bndIterator )
        {
            std::string bindingStr;
            std::string first( bndIterator->first.first );
            std::string second( bndIterator->first.second );
            std::string third( bndIterator->second.first );
            std::string fourth( bndIterator->second.second );
            
            bindingStr = processBindingString( first ) + processBindingString( second ) + processBindingString( third ) + processBindingString( fourth );
            mangledBindingList += bindingStr;
        }
        
        return mangledBindingList;
    }
    
    
    std::string
    MangledNameAssembler::constructMangledModificationList( ComplexOutputStateCref aComplexOutputState ) const
    {
        std::string mangledModString( "" );
        
        typedef std::pair<std::string, std::pair<std::string, std::string> > Modification;
        typedef std::vector<Modification> ModificationList;
        
        for ( ModificationList::const_iterator i = aComplexOutputState.theModificationTokens.begin();
              i!=aComplexOutputState.theModificationTokens.end();
              ++i )
        {
            std::string currentModificationString;
            std::string first = i->first;
            std::string second = i->second.first;
            std::string third = i->second.second;
            
            currentModificationString = processModificationToken( first ) + processModificationToken( second ) + processModificationToken( third );
            mangledModString += currentModificationString;
        }
        
        return mangledModString;
    }
    
    
    
    std::string
    MangledNameAssembler::getEncodedLength( const std::string& stringInQuestion ) const
    {
        int len=stringInQuestion.size();
        if ( len<10 )
        {
            //if len is between 1 and 9 inclusive then it is only one charector long
            return utl::stringify( len );
        }
        else
        {
            std::string tmp;
            tmp="_" + utl::stringify( len ) + "_";
            return tmp;
        }
    }
    
    
    void
    MangledNameAssembler::parseMangledMolString( const std::string& molString, std::vector<ComplexOutputState::MolTokenStr>& molTokenVector ) const
    {
        std::string::size_type index = 0;
        while ( true )
        {
            if ( index == molString.size() ) return;
            
            std::string::size_type lengthOfMolNameToken( 0 );
            
            // Index here is going to point to the LengthToken
            if ( molString[index] == '_' )
            {
                index+=1;
                std::string::size_type numberEnd = molString.find( "_", index );
                utl::from_string<std::string::size_type> ( lengthOfMolNameToken, std::string( molString, index, numberEnd - index ) );
                index = numberEnd + 1;
            }
            else
            {
                utl::from_string<std::string::size_type> ( lengthOfMolNameToken, std::string( molString, index, 1 ) );
                index += 1;
            }
            
            // Post conditions:
            // 1.  Index should point to the first character of the name
            // 2.  length should be the appropriate length of the name.
            
            molTokenVector.push_back( std::string( molString, index, lengthOfMolNameToken ) );
            index += lengthOfMolNameToken;
        }
        
        return;
    }
    
    
    void
    MangledNameAssembler::parseMangledBindingString( const std::string& bindingString,
                                                     std::vector<ComplexOutputState::BindingTokenStr>& bindingTokenVector ) const
    {
        
        std::vector<std::string> tmpVector;
        std::string::size_type currentBindingLength;
        
        std::string::size_type index = 0;
        
        while ( true )
        {
            if ( index == bindingString.size() ) break;
            
            if ( bindingString[index] == '_' )
            {
                // We go into special mode....
                index += 1;
                
                if ( bindingString[index] == '_' )
                {
                    // Not implemented.
                    throw utl::NotImplementedXcpt( "Error when trying to parse the bindingString '" + bindingString + "'.  Double '__' is a valid binding string, but has not yet been implemented by the maintainer (those lazy shits)" );
                }
                else
                {
                    // like 52_2100
                    utl::from_string<std::string::size_type> ( currentBindingLength,
                                                               std::string( bindingString, index, 1 ) );
                    index += 1;
                    
                }
                
            }
            else
            {
                // Implicit binding length.
                index += 0;
                currentBindingLength = 1;
            }
            
            // Post conditions:
            // 1. Index should point to the first character of the name
            // 2. currentBindingLength should be the length of the next index number.
            
            std::string number( bindingString, index, currentBindingLength );
            tmpVector.push_back( number );
            
            index += currentBindingLength;
        }
        
        // Checking for an badly formed binding string
        // Error condition.
        if ( tmpVector.size() % 4 != 0 )
        {
            throw badBindingNameXcpt( bindingString );
        }
        
        // Package the sequence of integers into a sequence of bindingTokens
        for ( unsigned int ii = 0;
              ii != tmpVector.size() / 4;
              ++ii )
        {
            bindingTokenVector.push_back( std::make_pair( std::make_pair( tmpVector[4*ii], tmpVector[4*ii + 1] ),
                                                          std::make_pair( tmpVector[4*ii+2], tmpVector[4*ii+3] ) ) );
        }
        
        return;
        
    }
    
    
    void
    MangledNameAssembler::parseMangledModificationString( const std::string& modString,
                                                          std::vector<ComplexOutputState::ModificationTokenStr>& modificationTokenVector ) const
    {
        // A mod string should be ( string, (string, string) )
        
        std::vector<std::string> tmpVector;
        std::string::size_type index( 0 );
        std::string::size_type numberOfCharactersInNextToken;
        
        while ( true )
        {
            if ( index == modString.size() ) break;
            
            // According to processModificationToken, each token begins with a _
            if ( modString[index] != '_' ) throw badModificationNameXcpt( modString );
            
            ++index;
            
            if ( modString[index] == '_' )
            {
                // We have a multi-charecter length to read.
                
                index += 1;
                
                std::string::size_type endOfLengthString = modString.find( "_", index );
                utl::from_string<std::string::size_type> ( numberOfCharactersInNextToken,
                                                           std::string( modString,
                                                                        index,
                                                                        endOfLengthString - index ) );
                index = endOfLengthString + 1;
            }
            else
            {
                utl::from_string<std::string::size_type> ( numberOfCharactersInNextToken,
                                                           std::string( modString,
                                                                        index,
                                                                        1 ) );
                index += 1;
            }
            
            tmpVector.push_back( std::string( modString,
                                              index,
                                              numberOfCharactersInNextToken ) );
            
            index += numberOfCharactersInNextToken;
            
        }
        
        if ( tmpVector.size() % 3 != 0 )
        {
            std::cerr << "##############################" << std::endl;
            std::cerr << "Error in parsing modificaton string '" << modString << "'." << std::endl;
            std::cerr << "Which was parsed as: " << std::endl;
            for ( unsigned int ii = 0;
                  ii != tmpVector.size();
                  ++ii )
            {
                std::cerr << ii << ":\t\"" << tmpVector[ii] << "\"" << std::endl;
            }
            throw badModificationNameXcpt( modString );
        }
        
        for ( unsigned int ii = 0;
              ii != tmpVector.size() / 3;
              ++ii )
        {
            modificationTokenVector.push_back( std::make_pair( tmpVector[3*ii],
                                                               std::make_pair( tmpVector[3*ii + 1], tmpVector[3*ii + 2] ) ) );
        }
        
        
    }
    
    
    std::string
    MangledNameAssembler::processBindingString( const std::string& aString ) const
    {
        if ( aString.empty() )
        {
            throw GeneralNmrXcpt( "Error.  MangledNameAssembler::processBindingString failed.\nThis function cannot be called on a null string." );
        }
        
        
        std::string returnVal;
        
        if ( aString.size() ==1 )
        {
            returnVal = aString;
        }
        else if ( aString.size() <10 )
        {
            returnVal = "_" + utl::stringify( aString.size() ) + aString;
        }
        else
        {
            returnVal = "__"+utl::stringify( aString.size() ) + "_" + aString;
        }
        
        return returnVal;
    }
    
    
    std::string
    MangledNameAssembler::processModificationToken( const std::string& aModString ) const
    {
        std::string returnVal;
        
        if ( aModString.size() <10 )
        {
            returnVal =  "_" + utl::stringify( aModString.size() ) + aModString;
        }
        else
        {
            returnVal = "__" + utl::stringify( aModString.size() ) +  "_" + aModString;
        }
        return returnVal;
    }
}
