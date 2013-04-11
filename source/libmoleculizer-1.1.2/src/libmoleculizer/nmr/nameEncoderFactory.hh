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

#ifndef NMRMANGLERFACTORY_HH
#define NMRMANGLERFACTORY_HH

#include "nmrExceptions.hh"
#include "nameAssembler.hh"
#include "complexSpeciesEncoderNames.hh"
#include "nameAssemblers.hh"
#include "complexFormNameAssembler.hh"


namespace nmr
{
    
    DECLARE_CLASS( nmrUnit );
    DECLARE_CLASS( NameEncoderFactory );
    class NameEncoderFactory
    {
    public:
        
        NameEncoderFactory( nmrUnit* pNmrUnit )
            :
            theNmrUnit( pNmrUnit )
        {}
        
        NameAssembler*
        create( const std::string& manglerName ) throw( NoSuchNameEncoderXcpt )
        {
            //       if( manglerName == manglernames::basicManglerName )
            //         {
            //           return new basicNameAssembler<molT>;
            //         }
            //       else if ( manglerName == manglernames::detailedManglerName )
            //         {
            //           return new readableNameAssembler<molT>;
            
            //         }
            //       else if ( manglerName == manglernames::compactManglerName )
            //         {
            //           return new MangledNameAssembler<molT>;
            //         }
            //       else
            //         {
            //           throw NoSuchNameManglerXcpt( manglerName );
            //         }
            
            if ( manglerName == manglernames::compactEncoderName )
            {
														return new MangledNameAssembler( theNmrUnit );
														// return new MangledNameAssembler;
            }
            else if (manglerName == manglernames::complexFormEncoderName )
            {
														return new complexFormNameAssembler( theNmrUnit );
														// return new complexFormNameAssembler;
            }
            else
            {
                throw NoSuchNameEncoderXcpt( manglerName );
            }
            
        }
        
    protected:
        nmrUnit* theNmrUnit;
        
    };
    
    
}


#endif
