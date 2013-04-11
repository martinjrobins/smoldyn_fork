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

#ifndef MOLARFACTOR_H
#define MOLARFACTOR_H

#include "fnd/physConst.hh"
#include "fnd/stateVar.hh"
#include "mzr/mzrReaction.hh"

namespace mzr
{
    class molarFactorGlobal :
        public fnd::stateVar<double, mzrReaction>
    {
    public:
        // Default volume is 1 liter, so default molar factor is Avogadro's number.
        molarFactorGlobal( double initialVolume = 1.0 ) :
            fnd::stateVar<double, mzrReaction> ( initialVolume * fnd::avogadrosNumber )
        {}
        
        const double
        getFactor( void ) const
        {
            return getValue();
        }
        
        const double
        getVolume( void ) const
        {
            return getFactor() / fnd::avogadrosNumber;
        }
        
        void
        updateFactor( double simpleMolarFactor,
                      fnd::sensitivityList<mzrReaction>& rAffectedReactions )
        {
            updateValue( simpleMolarFactor,
                         rAffectedReactions );
        }
        
        void
        updateVolume( double newVolume,
                      fnd::sensitivityList<mzrReaction>& rAffectedReactions )
        {
            updateValue( newVolume * fnd::avogadrosNumber,
                         rAffectedReactions );
        }
    };
    
    class volumeGlobal :
        public fnd::stateVar<double, mzrReaction>
    {
    public:
        volumeGlobal( double initialVolume = 1.0 ) :
            fnd::stateVar<double, mzrReaction> ( initialVolume )
        {}
        
        const double
        getFactor( void ) const
        {
            return getVolume() * fnd::avogadrosNumber;
        }
        
        const double
        getVolume( void ) const
        {
            return getValue();
        }
        
        void
        updateFactor( double simpleMolarFactor,
                      fnd::sensitivityList<mzrReaction>& rAffectedReactions )
        {
            updateValue( simpleMolarFactor / fnd::avogadrosNumber,
                         rAffectedReactions );
        }
        
        void
        updateVolume( double newVolume,
                      fnd::sensitivityList<mzrReaction>& rAffectedReactions )
        {
            updateValue( newVolume,
                         rAffectedReactions );
        }
    };
}

#endif // MOLARFACTOR_H
