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

#ifndef CPX_ALLOMOL_H
#define CPX_ALLOMOL_H

#include <vector>
#include <map>
#include "utl/xcpt.hh"
#include "cpx/siteShape.hh"
#include "cpx/molState.hh"

namespace cpx
{
    template<class stateMolT>
    class alloMol :
        public stateMolT
    {
    protected:
        
        // Mapping from state to the shapes of the binding sites when
        // the mol is in the state, conveying the allosteric properties of
        // the mol.
        //
        // Note the implication here that mol states (stateT) must be suitable as
        // map keys.
        typedef typename std::vector<const siteShape*> shapeVector;
        typedef typename std::map<typename alloMol::stateType, shapeVector> alloMapType;
        typedef typename alloMapType::value_type alloMapValue;
        typedef typename alloMapType::iterator alloIterator;
        typedef typename alloMapType::const_iterator constAlloIterator;
        alloMapType alloMap;
        
        // Core of public externState and allostery member functions.
        //
        // Down-casts generic state pointer to pointer to stateT, throwing an
        // exception if not possible.  Checks that state is in the allostery
        // map, verifying that pStateToExternalize was produced by internState.
        constAlloIterator
        externalize( molParam pStateToExternalize ) const
            throw( typename utl::xcpt );
        
        alloIterator
        externalize( molParam pStateToExternalize )
            throw( typename utl::xcpt );
        
    public:
        
        typedef stateMolT stateMolType;
        
        alloMol( const stateMolType& rStateMol ) :
            stateMolType( rStateMol )
        {}
        
        // Adds the new state to the mol's database of states, and returns a
        // generic pointer to the interned state.
        const typename alloMol::stateType*
        internState( const typename alloMol::stateType& rStateToIntern );
        
        // Takes a generic pointer to an interned state and returns the actual
        // interned state.  Throws an exception if the generic pointer does not
        // "down cast" to a stateT*.  Throws an exception if the stateT* does not
        // point to an entry in the mol's database (indicating that pStateToExtern
        // was not returned from some earlier call to internState.)
        const typename alloMol::stateType&
        externState( molParam pState ) const
            throw( typename utl::xcpt )
        {
            return externalize( pState )->first;
        }
        
        // Note that this is/should be virtual in stateMolT, as it is in basicMol.
        //
        // Gives the shapes of the binding sites when the mol is in the state
        // given by the generic state pointer, which should have been produced
        // earlier by internState (or an exception will be thrown.)
        std::vector<siteParam>&
        allostery( molParam pState )
            throw( typename utl::xcpt )
        {
            return externalize( pState )->second;
        }
    };
}

#include "cpx/alloMolImpl.hh"

#endif // CPX_ALLOMOL_H
