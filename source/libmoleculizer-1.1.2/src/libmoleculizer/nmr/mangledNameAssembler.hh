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


#ifndef __MANGLEDNAMEASSEMBLER_HH
#define __MANGLEDNAMEASSEMBLER_HH

#include "utl/defs.hh"
#include "utl/utility.hh"
#include "nmr/nmrExceptions.hh"

#include "nmr/nameAssembler.hh"
#include "nmr/complexSpecies.hh"
#include "nmr/complexOutputState.hh"

namespace nmr
{
    
    // TODO/9 Fill in detailed description of the mangling method used by MangledNameAssembler.
    
    DECLARE_CLASS( MangledNameAssembler );
    class MangledNameAssembler : public NameAssembler
    {
    public:
        
        DECLARE_TYPE( std::vector<std::string>, strVect );
        DECLARE_TYPE( std::vector<std::string>::iterator, strVectIter );
        DECLARE_TYPE( std::vector<std::string>::const_iterator, cstrVectIter );
        
        MangledNameAssembler( nmrUnit* theNmrUnit )
            :
            NameAssembler( "MangledNameAssembler", theNmrUnit )
        {
            try
            {
                assertEncodeDecodeAccuracy( this );
            }
            catch ( encodeDecodeInconsistencyXcpt xcpt )
            {
                xcpt.warn();
                std::cerr << "Continuing..." << std::endl;
            }
        }
        
        std::string
        createNameFromOutputState( const ComplexOutputState& aCOS ) const;
        
        ComplexOutputState
        createOutputStateFromName( const std::string& name ) const throw( nmr::UnparsableNameXcpt );
        
    protected:
        
        // These functions are used for construct
        std::string constructMangledMolList( ComplexOutputStateCref aComplexOutputState ) const;
        std::string constructMangledBindingList( ComplexOutputStateCref aComplexOutputState ) const;
        std::string constructMangledModificationList( ComplexOutputStateCref aComplexOutputState ) const;
        
        std::string processBindingString( const std::string& aString ) const;
        std::string processModificationToken( const std::string& aModString ) const;
        
        std::string getEncodedLength( const std::string& stringInQuestion ) const;
        
        
    protected:
        // These functions are used to decode the encoded names.
        void parseMangledMolString( const std::string& molString, std::vector<ComplexOutputState::MolTokenStr>& molTokenVector ) const;
        void parseMangledBindingString( const std::string& bindingString, std::vector<ComplexOutputState::BindingTokenStr>& bindingTokenVector ) const;
        void parseMangledModificationString( const std::string& modString, std::vector<ComplexOutputState::ModificationTokenStr>& modificationTokenVector ) const;

    };

}

#endif
