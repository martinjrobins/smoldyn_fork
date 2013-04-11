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

#ifndef CPX_MODMOL_H
#define CPX_MODMOL_H

#include "cpx/alloMol.hh"
#include "cpx/modMolMixin.hh"
#include "cpx/modMolState.hh"
#include "cpx/stateMol.hh"

namespace cpx
{
    template<class baseMolT>
    class modMol :
        public alloMol<stateMol<baseMolT, modMolState> >,
        public modMolMixin
    {
    public:
        typedef stateMol<baseMolT, modMolState> stateMolType;
        
        // Use molUnit::getModMap to convert a
        // map<string, string> into a map<string, const modification*>
        // as a preliminary to using this constructor.
        modMol( const baseMolT& rBaseMolT,
                double molecularWeight,
                const std::map<std::string, const modification*>& rModMap );
        
        // Still using the default state to get the molecular weight.
        //
        // Use molUnit::getModMap to convert a
        // map<string, string> into a map<string, const modification*>
        // as a preliminary to using this function.
        const modMolState*
        internModMap( const std::map<std::string, const modification*>& rModMap );
        
        // Used to generate instance names for mod-mols in complexes in
        // state dump.
        std::string
        genInstanceName( int molInstanceNdx ) const;
    };
}

#include "cpx/modMolImpl.hh"

#endif // CPX_MODMOL_H
