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

#include "utl/defs.hh"
#include "utl/domXcpt.hh"
#include "mol/mzrMol.hh"
#include "plex/plexUnit.hh"
#include "plex/mzrPlex.hh"
#include "plex/mzrPlexSpecies.hh"
#include "mzr/mzrSpecies.hh"
#include "nmr/nmrExceptions.hh"
#include "nmr/nmrEltName.hh"
#include "nmr/nmrUnit.hh"

#include "mzr/mzrSpeciesDumpable.hh"
#include "plex/mzrPlexSpecies.hh"
#include <libxml++/libxml++.h>


namespace nmr
{
    const NameAssembler*
    nmrUnit::getNameEncoder() const
        throw( MissingNameEncoderXcpt )
    {
        if ( !ptrNameAssembler )
        {
            throw MissingNameEncoderXcpt();
        }
        
        return ptrNameAssembler;
    }
    
    mzr::mzrSpecies*
    nmrUnit::constructSpeciesFromName( const std::string& uniqueid )
    {

        // We shouldn't have to, but we really should check to make sure the unique id isn't already present.
        try
        {
            rMolzer.convertSpeciesIDToSpeciesTag( uniqueid );

            std::cout << "converted name " << uniqueid << " was found!" << std::endl;
        }
        catch(utl::xcpt e)
        {
            e.wailAndBail();
        }
        catch(...)
        {
            std::cout << "HELLLLPPP " << std::endl;
        }

        try
        {
            // 1.  The currently set nameEncoder has the responsibility of decoding the thing into a complexOutputState.
            ComplexOutputState aCOS = getNameEncoder()->createOutputStateFromName( uniqueid );
            
            // TODO: Ensure that this does  not register the constructed plexSpecies into the reactionCatalog.
            
            // Is this what we want to do?  Would it be better to install but not expand the newly
            // created species?
            plx::mzrPlexSpecies* newMzrSpecies = pPlexUnit->constructNewPlexSpeciesFromComplexOutputState( aCOS );
            
            return newMzrSpecies;
        }
        // I should separate the reasons things went bad here.
        catch ( ... )
        {
            throw nmr::IllegalNameXcpt( uniqueid );
        }
        
    }
    
    void
    nmrUnit::setDefaultNameEncoder( const std::string& nameEncoderName ) throw( NoSuchNameEncoderXcpt )
    {
        delete ptrNameAssembler;
        ptrNameAssembler = ptrNameEncoderFactory->create( nameEncoderName );
    }
    
    
    
    void
    nmrUnit::parseDomInput( xmlpp::Element* pRootElt,
                            xmlpp::Element* pModelElt,
                            xmlpp::Element* pStreamElt ) throw( std::exception )
    {
        //         // TODO: Debug nmrUnit::parseDomInput and make sure it all works.
        //         // For the moment, the one thing (the naming strategy that should be used) is contained
        //         // in an attribute entitled "naming-convention".  The value of that string should
        //         // be passed to nmrUnit::setDefaultNameEncoder.
        //         try
        //         {
        //             std::string strategy = utl::dom::mustGetAttrString (pModelElt, eltName::namingConvention);
        //             setDefaultNameEncoder (strategy);
        //         }
        //         catch (NoSuchNameEncoderXcpt xcpt)
        //         {
        //             xcpt.wailAndBail();
        //         }
        //         catch( utl::dom::missingAttrXcpt x)
        //         {
        //             x.warn();
        //         }
    }
    

    void nmrUnit::insertStateElts( xmlpp::Element* pUnitStatesElt )
        throw( std::exception )
    {

        pUnitStatesElt->add_child("dimer-unit-state");
        
        xmlpp::Element* pNamingConvention = pUnitStatesElt->add_child("naming-convention");
        pNamingConvention->set_attribute("type", getNameEncoder()->getName() );

    }
}
