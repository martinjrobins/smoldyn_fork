//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//        This file is part of Libmoleculizer
//
//        Copyright (C) 2010 Nathan Addy
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
//   Nathan Addy, Scientific Programmer, Lawrence Berkeley National Lab, 2010
//
// Modifing Authors:
//
//

#ifndef __COMPLEXFORMNAMEASSEMBLER_HH
#define __COMPLEXFORMNAMEASSEMBLER_HH

#include "utl/defs.hh"
#include "utl/utility.hh"
#include "nmr/nmrExceptions.hh"
#include "nmr/nameAssembler.hh"

#include "nmr/complexSpecies.hh"
#include "nmr/complexOutputState.hh"
#include "nmr/complexSpeciesOutputMinimizer.hh"

namespace nmr
{

    DECLARE_CLASS( complexFormNameAssembler );

    // This class produces names for a ComplexOutputStateCref in a
    // "Complex Form".  This is analagous to the forms used in the
    // BNGL language.  

    class complexFormNameAssembler : public NameAssembler
    {
    public:

        complexFormNameAssembler( nmrUnit* theNmrUnit )
            :
            NameAssembler( "ComplexFormNameAssembler", theNmrUnit )
        {}

        ~complexFormNameAssembler(){}

        std::string createNameFromOutputState( ComplexOutputStateCref aCOS) const;

    private:
        bool getMoleculeComponents( ComplexOutputStateCref aCOS, unsigned int mol_ndx, \
                                    std::vector<int>& binding_ndxs, std::vector<int>& modification_ndxs) const;

        bool isMoleculeSimple( ComplexOutputStateCref aCOS, unsigned int mol_ndx) const;


    };
}

#endif 
