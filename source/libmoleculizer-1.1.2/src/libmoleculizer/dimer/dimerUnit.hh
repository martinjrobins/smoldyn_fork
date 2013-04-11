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

#ifndef DIMER_DIMERUNIT_H
#define DIMER_DIMERUNIT_H

/*! \defgroup dimerGroup The dimer unit.
  \ingroup unitsGroup
  \brief Provides dimerization and decomposition reactions. */

/*! \file dimerUnit.hh
  \ingroup dimerGroup
  \brief Defines dimerUnit. */

#include "mzr/mzrUnit.hh"
#include "mol/molUnit.hh"
#include "dimer/dimerEltName.hh"
#include "dimer/dimerizeRxnGen.hh"
#include "dimer/decompRxnGen.hh"

namespace dimer
{
    
    /*! \ingroup dimerGroup
      \brief Provides dimerization and decomposition reactions.
      
      This unit provides the basic reactions for binding complexes
      together at free binding sites, and conversely, for breaking complexes
      apart at bindings.
      
      This unit also provides databases of rates constants connected
      with bindings and with %pairs of compatible binding sites.  These
      databases are available through static functions.  They are used by
      plexFamily routines in addition to being used in this unit, so
      that for the time being, this is an essentially mandatory unit. */
    
    class dimerUnit :
        public mzr::unit
    {
        
    public:
        
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plx::plexUnit& rPlexUnit;
        
        dimerUnit( mzr::moleculizer& rMoleculizer,
                   mzr::mzrUnit& refMzrUnit,
                   bnd::molUnit& refMolUnit,
                   plx::plexUnit& refPlexUnit ) :
            mzr::unit( "dimer",
                       rMoleculizer ),
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit ),
            numDimerDecomGens(0)
        {
            // The only thing this unit handles is dimerization/decomposition
            // reactions.
            inputCap.addReactionGenName( eltName::dimerizationGen );
        }
        
        void
        parseDomInput( xmlpp::Element* pRootElement,
                       xmlpp::Element* pModelElement,
                       xmlpp::Element* pStreamElt ) throw( std::exception );
        
        void
        insertStateElts( xmlpp::Element* pRootElt ) throw( std::exception );


        int getNumberOfDimerDecompRules() const;

    private:
        int numDimerDecomGens;
    };
}

#endif // DIMER_DIMERUNIT_H
