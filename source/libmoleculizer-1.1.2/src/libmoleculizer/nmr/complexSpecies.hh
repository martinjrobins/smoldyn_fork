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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//

#ifndef __COMPLEXSPECIES_HH
#define __COMPLEXSPECIES_HH


#include "utl/defs.hh"
#include "nmr/nmrExceptions.hh"

#include "nmr/complexOutputState.hh"
#include "nmr/partialTokenList.hh"
#include "nmr/permutation.hh"
#include "nmr/abstractMol.hh"
#include "nmr/namedMolecule.hh"


namespace nmr
{
    
    DECLARE_CLASS( ComplexSpecies );
    class ComplexSpecies
    {
        
    public:
        DECLARE_TYPE( std::string, Alias );
        DECLARE_TYPE( unsigned int, MolNdx );
        DECLARE_TYPE( std::vector<MinimalMol*>, MolList );
        DECLARE_TYPE( unsigned int, BndNdx );
        DECLARE_TYPE( MinimalMol::BindingSite,  BindingSite );
        DECLARE_TYPE( MinimalMol::ModificationList, ModificationList )
        typedef std::pair<MolNdx, BndNdx> __HalfBinding;
        DECLARE_TYPE( __HalfBinding, HalfBinding );
        typedef std::pair<HalfBinding, HalfBinding> __Binding;
        DECLARE_TYPE( __Binding, Binding );
        DECLARE_TYPE( std::vector<Binding>, BindingList )
        
        public:
        
        ComplexSpecies();
        ComplexSpecies( ComplexSpeciesCref aComplexSpecies );
        ComplexSpecies( ComplexOutputStateCref aComplexOutputState );
        
        ~ComplexSpecies();
        
        ComplexSpecies& operator= ( const ComplexSpecies& crefComplexSpecies );
        
        void addMolToComplex( MinimalMol* ptrMol, AliasCref anAlias )
            throw( DuplicateMolAliasXcpt );
        
        void addBindingToComplex( AliasCref firstMolAlias,
                                  BindingSiteCref firstMolSiteAlias,
                                  AliasCref secondMolAlias,
                                  BindingSiteCref secondMolSiteAlias ) throw( MissingMolAliasXcpt,
                                                                              MissingBindingSiteXcpt );
        unsigned int
        getNumberOfMolsInComplex() const;
        
        unsigned int
        getNumberOfBindingsInComplex() const;
        
        MolListCref
        getMolList() const;
        
        BindingListCref
        getBindingList() const;
        
        MolListRef
        getMolList();
        
        BindingListRef
        getBindingList();
        
        void
        applyPermutationToComplex( const Permutation& aPermutation );
        
        // This is a partialNameSentence with everything "stringified".  This plexOutputState is
        // the object passed to a particular nameAssembler.
        void constructOutputState( ComplexOutputState& rOutputState ) const;
        
        std::string repr() const;
        
    protected:
        typedef std::map<Alias, MolNdx> _molMap;
        DECLARE_TYPE( _molMap, MolMap );
        
        typedef MolMap::iterator MolMapIter;
        DECLARE_TYPE( ComplexOutputState::MolTokenStr, MolTokenStr );
        DECLARE_TYPE( ComplexOutputState::BindingTokenStr, BindingTokenStr );
        DECLARE_TYPE( ComplexOutputState::ModificationTokenStr, ModificationTokenStr );
        
        // void constructPartialTokenList( PartialTokenListRef rComplexPartialTokenList ) const;
        void sortBinding( BindingRef aBinding );

        MolList theMols;        
        MolMap theMolAliasToNdxMap;
        BindingList theBindings;
        
    };
    
}


std::ostream&
operator<<( std::ostream& stream, nmr::ComplexSpeciesCref aComplexSpecies );

#endif

