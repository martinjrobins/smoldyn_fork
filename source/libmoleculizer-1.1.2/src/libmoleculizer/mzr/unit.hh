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

#ifndef UNIT_H
#define UNIT_H

#include <set>
#include <string>

#include "utl/dom.hh"

namespace mzr
{
    // This is how a unit indicates what it can parse and parses what it can
    // from a moleculizer-input document.
    class inputCapabilities
    {
        // Names of elements in input files that are handled by a unit.
        //
        // These are connected with the places in the input file where elements may
        // appear that depend on the presence of a particular unit.
        std::set<std::string> modelContentNames;
        std::set<std::string> reactionGenNames;
        std::set<std::string> explicitSpeciesContentNames;
        std::set<std::string> speciesStreamsContentNames;
        std::set<std::string> eventsContentNames;
        
    public:
        
        // This allows the parser to just add up the inputCapabilities's of the
        // modules, to form its own inputCapabilities.
        void
        addCap( const inputCapabilities& rCapToAdd )
        {
            modelContentNames.insert( rCapToAdd.modelContentNames.begin(),
                                      rCapToAdd.modelContentNames.end() );
            
            reactionGenNames.insert( rCapToAdd.reactionGenNames.begin(),
                                     rCapToAdd.reactionGenNames.end() );
            
            explicitSpeciesContentNames.insert( rCapToAdd.explicitSpeciesContentNames.begin(),
                                                rCapToAdd.explicitSpeciesContentNames.end() );
            
            speciesStreamsContentNames.insert( rCapToAdd.speciesStreamsContentNames.begin(),
                                               rCapToAdd.speciesStreamsContentNames.end() );
            
            eventsContentNames.insert( rCapToAdd.eventsContentNames.begin(),
                                       rCapToAdd.eventsContentNames.end() );
        }
        
        // To build the above sets in unit constructors.
        //
        // Each is connected with a place in the input file schema where elements
        // may appear that depend on the presence of a particular unit.
        void
        addModelContentName( const std::string& rEltName )
        {
            modelContentNames.insert( rEltName );
        }
        
        void
        addExplicitSpeciesContentName( const std::string& rEltName )
        {
            explicitSpeciesContentNames.insert( rEltName );
        }
        
        void
        addSpeciesStreamsContentName( const std::string& rEltName )
        {
            speciesStreamsContentNames.insert( rEltName );
        }
        
        void
        addEventsContentName( const std::string& rEltName )
        {
            eventsContentNames.insert( rEltName );
        }
        
        void
        addReactionGenName( const std::string& rEltName )
        {
            reactionGenNames.insert( rEltName );
        }
        
        bool
        handlesModelContentElt( xmlpp::Element* pElt ) const;

        
        
        // To test if this unit handles a particular reaction generator element.
        bool
        handlesReactionGensContent( xmlpp::Element* pElt ) const;

        
        // To test if this unit handles a particular explictSpecies content element.
        bool
        handlesExplictSpeciesContent( xmlpp::Element* pElt ) const;

        
        // To test if this unit handles a particular speciesStreams content element.
        bool
        handlesSpeciesStreamsContent( xmlpp::Element* pElt ) const;

        
        // To test if this unit handles a particular events content element.
        bool
        handlesEventsContent( xmlpp::Element* pElt ) const;

    };
    
    class moleculizer;
    
    class unit
    {
    public:
        
        // For debugging only, at present.  Possibly useful for other things?
        std::string name;
        
        moleculizer& rMolzer;
        
        unit( const std::string& rName,
              moleculizer& rMoleculizer )
            :
            name( rName ),
            rMolzer( rMoleculizer )
        {}
        
        virtual ~unit( void )
        {}
        
        // Keeps track of the elements that are handled by this unit,
        // so that elements don't "fall through the cracks."
        inputCapabilities inputCap;
        
        // The actual input parsing done by this unit.
        virtual void
        parseDomInput( xmlpp::Element* pRootElt,
                       xmlpp::Element* pModelElt,
                       xmlpp::Element* pStreamElt ) throw( std::exception ) = 0;
        
        // This allows finalization steps after parsing but before running.
        // In this phase, plexUnit runs notifications on parsed (but never created)
        // plexSpecies
        // after all parsing is complete, so that these already-defined
        // plexSpecies can be safely created just by using update.  (This is
        // still due to the clunkiness of update.)
        virtual void
        prepareToRun( xmlpp::Element* pRootElt,
                      xmlpp::Element* pModelElt,
                      xmlpp::Element* pStreamElt ) throw( std::exception )
        {
            // The plex unit does a lot; the mzrUnit does a little.  So far,
            // nobody else does anything.
        }
        
        // How a unit contributes its part of a state dump.
        virtual void
        insertStateElts( xmlpp::Element* pRootElt ) throw( std::exception ) = 0;
    };
}

#endif
