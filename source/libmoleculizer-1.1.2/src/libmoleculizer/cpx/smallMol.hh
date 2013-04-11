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

#ifndef CPX_SMALLMOL_H
#define CPX_SMALLMOL_H

#include "cpx/stateMol.hh"

namespace cpx
{
    template<class baseMolT>
    class smallMol :
        public stateMol<baseMolT, molState>
    {
        molState theState;
        
    public:
        typedef baseMolT baseMolType;
        typedef typename baseMolT::bindingSiteType bindingSiteType;
        
        // Rather than construct the binding site (whose exact type isn't known
        // here) for the mol, now we make the mol from the binding site.
        //
        // The binding site should have the same name as the mol, and it should
        // have one shape, also with the same name as the mol.
        smallMol( const baseMolType& rBaseMol,
                  double molecularWeight ) :
            stateMol<baseMolT, molState> ( rBaseMol ),
            theState( molecularWeight )
        {
            this->pDefaultState = &theState;
        }
        
        bindingSiteType*
        getUniqueSite( void )
        {
            return & ( this->sites[0] );
        }
    };
}

#endif // CPX_SMALLMOL_H
