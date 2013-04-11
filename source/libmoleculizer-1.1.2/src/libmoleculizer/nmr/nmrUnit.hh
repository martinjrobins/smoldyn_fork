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

#ifndef NMRUNIT_HH
#define NMRUNIT_HH

#include "plex/plexUnit.hh"
#include "nmrEltName.hh"
#include "nameAssembler.hh"
#include "nameEncoderFactory.hh"
#include "plex/mzrPlexSpecies.hh"
#include "mzr/unit.hh"
#include "mol/molUnit.hh"

namespace plx
{
    DECLARE_CLASS( plexUnit );
}

namespace mzr
{
    DECLARE_CLASS( mzrSpecies );
}

namespace nmr
{
    class nmrUnit : public mzr::unit
    {
    public:
        nmrUnit( mzr::moleculizer& rMoleculizer )
            :
            unit( "nmr",
                  rMoleculizer ),
            pMzrUnit( NULL ),
            pMolUnit( NULL ),
            pPlexUnit( NULL ),
            ptrNameEncoderFactory( new NameEncoderFactory( this ) ),
            ptrNameAssembler( NULL )
        {
            // setDefaultNameEncoder( manglernames::compactEncoderName );
            setDefaultNameEncoder( manglernames::complexFormEncoderName );
        }
        
        ~nmrUnit()
        {
            // Don't delete any pointers to Units as they are memory managed elsewhere.
            delete ptrNameEncoderFactory;
            delete ptrNameAssembler;
        }
        
        mzr::mzrSpecies*
        constructSpeciesFromName( const std::string& speciesName );
        
        
        const NameAssembler*
        getNameEncoder() const throw( MissingNameEncoderXcpt );
        
        void
        setDefaultNameEncoder( const std::string& nameEncoderName ) throw( NoSuchNameEncoderXcpt );
        
        void
        setMzrUnit( mzr::mzrUnit* ptrMzrUnit )
        {
            pMzrUnit = ptrMzrUnit;
        }
        
        void
        setPlexUnit( plx::plexUnit* ptrPlexUnit )
        {
            pPlexUnit = ptrPlexUnit;
        }
        
        void setMolUnit( bnd::molUnit* ptrMolUnit )
        {
            pMolUnit = ptrMolUnit;
        }
        
        void
        parseDomInput( xmlpp::Element* pRootElt,
                       xmlpp::Element* pModelElt,
                       xmlpp::Element* pStreamElt ) throw( std::exception );
        
        void insertStateElts( xmlpp::Element* pRootElt ) throw( std::exception );
        
        mzr::mzrUnit* pMzrUnit;
        bnd::molUnit* pMolUnit;
        plx::plexUnit* pPlexUnit;
        
        // Protect this next function.
        //         plx::mzrPlexSpecies*
        //         makePlexFromName(const std::string& mangledName) const;
        
        NameEncoderFactory* ptrNameEncoderFactory;
        NameAssembler* ptrNameAssembler;
    };
}


#endif

