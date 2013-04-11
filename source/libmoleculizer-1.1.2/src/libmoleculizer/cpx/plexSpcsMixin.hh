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

#ifndef CPX_PLEXSPECIES_H
#define CPX_PLEXSPECIES_H

#include "cpx/prm.hh"
#include "cpx/siteToShapeMap.hh"
#include "cpx/molState.hh"
#include "nmr/namedMolecule.hh"
#include "nmr/complexSpecies.hh"
#include "nmr/mangledNameAssembler.hh"
#include "nmr/basicNameAssembler.hh"
#include "nmr/readableNameAssembler.hh"

#include <vector>

namespace cpx
{
    // For mixing-in with a "base" species type to produce plexSpecies.
    template<class plexFamilyT>
    class plexSpeciesMixin
    {
    public:
        // The plexFamily to which this species belongs.
        plexFamilyT& rFamily;
        
        // The shapes of the plexSpecies's binding sites, both free and bound.
        //
        // This caches the allostery calculation done when the plexSpecies
        // is created in plexFamilyT::makeMember.
        siteToShapeMap siteParams;
        
        // The states of the plexSpecies's mols.
        std::vector<molParam> molParams;
        
        plexSpeciesMixin( plexFamilyT& rContainingFamily,
                          const siteToShapeMap& rSiteParams,
                          const std::vector<molParam>& rMolParams ) :
            rFamily( rContainingFamily ),
            siteParams( rSiteParams ),
            molParams( rMolParams )
        {}
        
        double
        getWeight( void ) const;
        
        // Notification has to be done in the final species.  It should notify
        // rFamily with a fnd::newSpeciesStimulus<finalSpecies>.
        
        // Generate non-canonical, "informative" name.  Not so sure that this
        // should go here.
        std::string
        getInformativeName( void ) const;
        
        std::string
        getCanonicalName( void ) const;
        
        std::string
        getCanonicalName( const nmr::NameAssembler* ptrNameAssembler ) const;
        
    protected:
        typedef nmr::ComplexSpecies ComplexRepresentation;
        
        void
        createComplexRepresentation( ComplexRepresentation& aComplexRepresentation ) const;
        
    };
}

#include "cpx/plexSpcsMixinImpl.hh"
#include "cpx/plexSpcsMixinCanonicalNamingImpl.hh"

#endif // CPX_PLEXSPECIES_H
