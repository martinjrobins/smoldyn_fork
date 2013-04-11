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

#ifndef FND_STOCHREACTION_H
#define FND_STOCHREACTION_H

#include <cmath>
#include "fnd/basicReaction.hh"
#include "fnd/physConst.hh"

namespace fnd
{
    // Adds propensity calculation to basic_reaction; useful in algorithms
    // deriving from Gillespie's originals.
    template<class speciesType>
    class gillspReaction :
        public basicReaction<speciesType>
    {
        class combinationsForReactant;
        
    public:
        gillspReaction( double reactionRate = 0.0 ) :
            basicReaction<speciesType> ( reactionRate )
        {}
        
        double
        propensity( double volume ) const;
    };
    
    
    /*! \brief Auxiliary function class for reaction propensity calculation.
      
      This multiplies the number of reactive combinations of molecules by factor
      coming from one substrate's population p and multiplicity m in the
      reaction: p(p - 1)(p - 2)...(p - m + 1).
      
      Note that this is NOT the actual number of combinations; rather it is the
      number of combinations times m!.  See note in mzrReaction::propensity.*/
    template<class speciesType>
    class gillspReaction<speciesType>::combinationsForReactant :
        public std::unary_function<typename gillspReaction<speciesType>::multMap::value_type, void>
    {
        double& rCombinations;
    public:
        combinationsForReactant( double& rCombinationCount ) :
            rCombinations( rCombinationCount )
        {}
        
        void
        operator()( const typename gillspReaction<speciesType>::multMap::value_type& rSpeciesMult ) const
        {
            int speciesPop = rSpeciesMult.first->getPop();
            
            int multiplicity = rSpeciesMult.second;
            
            // Note that if speciesPop drops to 0 during this operation, then
            // rCombinations becomes 0.
            while ( 0 < multiplicity-- ) rCombinations *= speciesPop--;
        }
    };
    
    template<class speciesType>
    double
    gillspReaction<speciesType>::
    propensity( double volume ) const
    {
        // Figure out the number of combinations of substrate molecules.
        //
        // Note that this will get the correct number of combinations (1) for a
        // "no-substrate" reaction.
        double combinations = 1.0;
        for_each( this->reactants.begin(),
                  this->reactants.end(),
                  combinationsForReactant( combinations ) );
        
        // Here the factors of m! that were multiplied into the number of
        // combinations compensate for using the ordinary determinisitc reaction
        // rate.
        //
        // Is this right for a "no-substrate" reaction?  Doesn't look right.
        return combinations *( this->rate )
            / std::pow( avogadrosNumber * volume,
                        ( this->arity ) - 1 );
    }
}

#endif // FND_STOCHREACTION_H
