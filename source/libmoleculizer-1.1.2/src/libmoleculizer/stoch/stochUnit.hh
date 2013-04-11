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

#ifndef STOCHUNIT_H
#define STOCHUNIT_H

/*! \defgroup stochGroup The stochastirator unit.
  \ingroup unitsGroup
  \brief Small-molecules, stoichiometric reactions. */

/*! \file stochUnit.hh
  \ingroup stochGroup
  \brief Defines unit for small molecules and stoichiometric reactions. */

#include "utl/defs.hh"
#include "mzr/unit.hh"
#include "stoch/unkStochSpeciesXcpt.hh"
#include "stoch/stochSpecies.hh"
#include "stoch/stochEltName.hh"

namespace stoch
{
    /*! \ingroup stochGroup
      \brief Unit for small molecules and stoichiometric reactions. */
    class stochUnit :
        public mzr::unit
    {
        utl::autoCatalog<stochSpecies> stochSpeciesByName;
        std::vector<mzr::mzrReaction*> noSubstrateArrows;
        
    public:
        
        bool
        addStochSpecies( const std::string& rSpeciesName,
                         stochSpecies* pStochSpecies );
        
        void
        mustAddStochSpecies( const std::string& rSpeciesName,
                             stochSpecies* pStochSpecies,
                             xmlpp::Node* pRequestingNode = 0 )
            throw( utl::xcpt );
        
        stochSpecies*
        findStochSpecies( const std::string& rSpeciesName );
        
        stochSpecies*
        mustGetStochSpecies( xmlpp::Node* pRequestingNode,
                             const std::string& rSpeciesName )
        throw( unkStochSpeciesXcpt );
        
        void
        addNoSubstrateArrow( mzr::mzrReaction* pArrow );
        
        mzr::mzrUnit& rMzrUnit;
        
        stochUnit( mzr::moleculizer& rMoleculizer,
                   mzr::mzrUnit& refMzrUnit ) :
            mzr::unit( "stoch",
                       rMoleculizer ),
            rMzrUnit( refMzrUnit )
        {
            // Unit isn't responsible for any model elements or
            // reaction generators.
            
            // Add responsibility for stoch species.
            inputCap.addExplicitSpeciesContentName( eltName::stochSpecies );
            
            // Not responsible for any species streams or events.
        }
        
        void
        parseDomInput( xmlpp::Element* pRootElement,
                       xmlpp::Element* pModelElement,
                       xmlpp::Element* pStreamElt ) throw( std::exception );
        
        void
        prepareToRun( xmlpp::Element* pRootElt,
                      xmlpp::Element* pModelElt,
                      xmlpp::Element* pStreamElt ) throw( std::exception );
        
        void
        prepareToContinue( xmlpp::Element* pRootElt,
                           xmlpp::Element* pModelElt,
                           xmlpp::Element* pStreamsElt,
                           std::map<std::string, std::string>& rTagToName,
                           xmlpp::Element* pTaggedSpeciesElement )
            throw( std::exception );
        
        void
        insertStateElts( xmlpp::Element* pRootElt ) throw( std::exception );
    };
}

#endif // STOCHUNIT_H
