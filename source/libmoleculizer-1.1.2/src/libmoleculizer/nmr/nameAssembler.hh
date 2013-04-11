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

#ifndef __NAMEASSEMBLER_HH
#define __NAMEASSEMBLER_HH

#include "utl/defs.hh"
#include "nmr/nmrExceptions.hh"

#include "nmr/complexSpecies.hh"
#include "nmr/complexOutputState.hh"
#include "nmr/complexSpeciesOutputMinimizer.hh"

namespace nmr
{
    
    DECLARE_CLASS( nmrUnit );
    DECLARE_CLASS( NameAssembler );
    class NameAssembler
    {
    public:
        
        // This is intended as a sanity check function that can be called during constructors of
        // different NameAssemblers.  It will throw an exception if startingComplexOutputState != endingComplexOutputState
        static void assertEncodeDecodeAccuracy( NameAssembler* ptrNameAssembler ) throw( encodeDecodeInconsistencyXcpt )
        {
            // TODO/7: In NameAssembler::assertEncodeDecodeAccuracey, create a much more
            // complicated ComplexOutputState for checking encoding/decoding equivelence.
            
            // Create a sufficiently complicated ComplexOutputState.
            // ComplexOutputState aComplexOutputState;
            
            //             // Encode the COS into a name, then decode the name back into a COS.
            //             ComplexOutputState decodedEncodingOS = ptrNameAssembler->createOutputStateFromName( ptrNameAssembler->createNameFromOutputState( aComplexOutputState ) );
            
            //             try
            //             {
            //                 // If they don't match, throw an exception.
            //                 if (decodedEncodingOS != aComplexOutputState)
            //                 {
            //                     throw encodeDecodeInconsistencyXcpt( ptrNameAssembler->getName() );
            //                 }
            //             }
            //             catch( utl::NotImplementedXcpt x)
            //             {
            //                 throw encodeDecodeInconsistencyXcpt( ptrNameAssembler->getName() );
            //             }
        }
        
    public:
        NameAssembler( const std::string& nameAssemblerName,
                       nmr::nmrUnit* pNmrUnit )
            :
            assemblerName( nameAssemblerName ),
            theNmrUnit( pNmrUnit )
        {}
        
        virtual ~NameAssembler()
        {}
        
        const std::string&
        getName() const
        {
            return assemblerName;
        }
        
        // TODO/5 Depreciate NameAssembler::createName in favor
        // of "generateNameFromComplexSpecies".
        std::string
        createName( ComplexSpeciesCref aComplexSpecies ) const
        {
            ComplexOutputState anOutputState;
            aComplexSpecies.constructOutputState( anOutputState );
            return createNameFromOutputState( anOutputState );
        }
        
        // TODO/5 Depreciate NameAssembler::createCanonicalName in favor
        // of "generateCanonicalNameFromComplexSpecies".
        std::string
        createCanonicalName( ComplexSpeciesCref aComplexSpecies ) const
        {
            ComplexOutputState debugOutputState;
            aComplexSpecies.constructOutputState(debugOutputState);
            ComplexSpeciesOutputMinimizer canonicalNameGenerator;
            ComplexOutputState minimalOutputState = canonicalNameGenerator.getMinimalOutputState( aComplexSpecies );
            std::string answer = createNameFromOutputState( minimalOutputState );
            return answer;
        }
        
        std::string
        generateNameFromComplexSpecies( ComplexSpeciesCref aComplexSpecies ) const
        {
            ComplexOutputState anOutputState;
            aComplexSpecies.constructOutputState( anOutputState );
            
            // I'm surprised this works....
            return createNameFromOutputState( anOutputState );
        }
        
        std::string
        generateCanonicalNameFromComplexSpecies( ComplexSpeciesCref aComplexSpecies ) const
        {
            ComplexSpeciesOutputMinimizer canonicalNameGenerator;
            ComplexOutputState minimalOutputState = canonicalNameGenerator.getMinimalOutputState( aComplexSpecies );
            
            // I'm surprised this works
            return createNameFromOutputState( minimalOutputState );
        }
        
        virtual std::string createNameFromOutputState( ComplexOutputStateCref aComplexOutputState ) const = 0;
        virtual ComplexOutputState createOutputStateFromName( const std::string& aMangledName ) const
            throw( nmr::UnparsableNameXcpt, utl::NotImplementedXcpt )
        {
            // For the time being, I am trying out making this throw something but be overridable.
            // If it isn't successful, this function should go back to being pure-virtual.
            throw utl::NotImplementedXcpt( "NameAssembler::createOutputStateFromName" );
        }
        
    protected:
        const std::string assemblerName;
        nmr::nmrUnit* theNmrUnit;
        
    };
    
}

#endif
