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

#ifndef FTRUNIT_H
#define FTRUNIT_H

#include "mzr/mzrUnit.hh"
#include "mol/molUnit.hh"
#include "plex/plexUnit.hh"
#include "ftr/ftrEltName.hh"

namespace ftr
{
    class ftrUnit : public mzr::unit
    {
    public:
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plx::plexUnit& rPlexUnit;
        
        ftrUnit( mzr::moleculizer& rMoleculizer,
                 mzr::mzrUnit& refMzrUnit,
                 bnd::molUnit& refMolUnit,
                 plx::plexUnit& refPlexUnit ) :
            mzr::unit( "ftr",
                       rMoleculizer ),
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit ),
            numOmniGens( 0 ),
            numUniMolGens( 0 )
        {
            // Register reaction generator names.  This is used by the parser
            // to verify that all elements appearing in the input file are "claimed"
            // by some module.  Why did I think that was a good idea?  Now, "open
            // schema" i.e. pay no attention to extraneous matter while parsing.
            // seems more rational to me.
            inputCap.addReactionGenName( eltName::omniGen );
            inputCap.addReactionGenName( eltName::uniMolGen );
            
            // Register the enabling complexes for omniRxnGen generators
            // as omniplexes, for processing by the plexUnit.
            const std::string slash( "/" );
            std::ostringstream omniGensXpath;
            omniGensXpath << mzr::eltName::model
                          << slash
                          << mzr::eltName::reactionGens
                          << slash
                          << eltName::omniGen
                          << slash
                          << eltName::enablingOmniplex;
            rPlexUnit.addOmniXpath( omniGensXpath.str() );
        }
        
        // The input parsing routine for this unit.
        void
        parseDomInput( xmlpp::Element* pRootElement,
                       xmlpp::Element* pModelElement,
                       xmlpp::Element* pStreamElt )
            throw( std::exception );
        
        // The state output routine for this unit.
        void
        insertStateElts( xmlpp::Element* pUnitStatesElt ) throw( std::exception );


        int getNumberOfOmniGenRules() const
        {
            return numOmniGens;
        }

        int getNumberOfUniMolGenRules() const
        {
            return numUniMolGens;
        }

    protected:
        int numOmniGens;
        int numUniMolGens;
    };
}

#endif // FTRUNIT_H
