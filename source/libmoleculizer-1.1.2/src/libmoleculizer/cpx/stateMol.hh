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

#ifndef CPX_STATEMOL_H
#define CPX_STATEMOL_H

namespace cpx
{
    template<class baseMolT, class stateT>
    class stateMol :
        public baseMolT
    {
    protected:
        // The default state of the mol.  This is used as a starting point
        // for constructing other states of the mol.
        //
        // Note that this isn't set in the constructor; descendant classes
        // must set this.
        const stateT* pDefaultState;
        
    public:
        typedef baseMolT baseMolType;
        typedef stateT stateType;
        
        stateMol( const baseMolT& rBaseMol ) :
            baseMolT( rBaseMol ),
            pDefaultState( 0 )
        {}
        
        // Returns the default state as a reference to the actual state class.
        // One can can then use the default state as the starting point
        // for making other states of the mol.
        const stateT*
        getDefaultState( void ) const
        {
            return pDefaultState;
        }
        
        // Note that this is/should be virtual in baseMolT, as in basicMol.
        molParam
        getDefaultParam( void ) const
        {
            return pDefaultState;
        }
    };
}

#endif // CPX_STATEMOL_H
