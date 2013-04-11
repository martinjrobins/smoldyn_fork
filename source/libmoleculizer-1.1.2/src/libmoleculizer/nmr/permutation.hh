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

#ifndef __NMR_PERMUTATION_HH
#define __NMR_PERMUTATION_HH

#include "nmr/nmrExceptions.hh"

#include "utl/defs.hh"


namespace nmr
{
    // The Permutation class represents a "partial permutation" on Z_n = {0, ..., n-1}.
    // A "partial permutation" is a relation on Z_n X Z_n that can be extended to a
    // permutation (which is a bijection from Z_n -> Z_n).
    
    // In this class, this is represented as a function from Z_n -> Z_n U Permutation::UNDEF,
    // where any domain element that conceptually has no partner in the range Z_n yet is
    // mapped to Permutation::UNDEF.  The class member function checkPermutationLegality
    // ensures that the Permutation is pre-bijective -- ie that any element of the range which
    // is not equal to Permutation::UNDEF only shows up once.
    
    DECLARE_CLASS( Permutation );
    class Permutation
    {
				public:
        // NOTES, TODO:
        // Right now the situation is that the domain is {0, ... , n-1}
        // and the range is {0, ... , n-1} U Permutation::UNDEF.
        // Consequently any of the domain elements (the indexes) are
        // represented as "unsigned ints" and all the range elements are
        // represented as "ints".  If I ever get the chance, I'd like to come
        // up with better types here.  (although it works along perfectly fine
        // at the moment)
        
    public:
        static const int UNDEF;
        
    public:
        DECLARE_TYPE( unsigned int, BindingNdx );
        DECLARE_TYPE( unsigned int, Dimension );
        DECLARE_TYPE( std::vector<unsigned int>, UnsignedIntegerVector );
        DECLARE_TYPE( std::vector<int>, IntegerVector );
        DECLARE_TYPE( IntegerVector, CorePermutationType );
        DECLARE_TYPE( std::set<Permutation>, SetOfPermutations );
        
    public:
        
        // Constructors
        //
        
        // This makes a completely undefined permutation on n objects.
        Permutation( Dimension dim = 0 );
        Permutation( PermutationCref aPermutation );
        Permutation( CorePermutationTypeCref aPermutationVector );
	~Permutation();

 	const Permutation&
 	operator=(const Permutation& aPerm);
        
        // This constructor copies aPermutation, and then adds the point perm(pos)=value to it.
        Permutation( PermutationCref aPermutation, BindingNdx pos, int value )
            throw( nmr::BadPermutationConstructorXcpt );
        
        Permutation( PermutationCref aPermutation,
                     PermutationCref bPermution );
        
        // API
        //
        int getValueAtPosition( BindingNdx pos ) const
            throw( nmr::BadPermutationIndexXcpt );
        
        void setValueAtPosition( BindingNdx pos, unsigned int val )
            throw( nmr::BadPermutationIndexXcpt, nmr::DuplicateValueXcpt ) ;
        
        void resetValueAtPosition( BindingNdx pos )
            throw( nmr::BadPermutationIndexXcpt );
        
        // Returns the Permutation (*this)( compositionPermutation(x) )
        Permutation
        of( PermutationCref compositionPermutation ) const
            throw( nmr::IncompatiblePermutationsXcpt );
        
        Permutation
        invertPermutation() const;
        
        void
        getPreimage( const int& rangeElement,
                     std::set<unsigned int>& refDomainElements ) const;
        
        void getUnfixedDomainElements( std::set<unsigned int>& refDomainElements ) const;
        
        //             bool
        //             getIsBijection() const;
        
        bool
        getIsComplete() const;
        
        bool
        getIsIncomplete() const ;
        
        Dimension
        getDimension() const;
        
        int& operator[]( const BindingNdx& n )
            throw( nmr::BadPermutationIndexXcpt );
        
        const int& operator[]( const BindingNdx& n ) const
            throw( nmr::BadPermutationIndexXcpt );
        
        unsigned int
        getLeastValueNotInPermutation() const
            throw( nmr::GeneralNmrXcpt );
        
        bool
        checkPermutationLegality() const;
        
        bool
        operator== ( PermutationCref pm );
        
        bool
        operator< ( PermutationCref pm ) const;
        
        CorePermutationTypeCref
        getCorePermutation() const
        {
            return thePermutation;
        }
        
        std::string
        repr() const;
        
        static Permutation
        generateIdentity( unsigned int N );
        
        
        static void
        generate_Sn( SetOfPermutationsRef setOfPermutations, unsigned int N );
        
        
        // Produce from S_{ Sum(signature)} the subgroup isomorphic to
        // S_n1 x S_n2 x ... x S_nk, where signature[i] = n_i.
        static void
        generateAllPermutationsMatchingSignature( SetOfPermutationsRef permSet,
                                                  const std::vector<unsigned int>& signature );
        
    protected:
        void maximallyExtend();
        unsigned int getNumberOfFixedElements() const;
        unsigned int getNumberOfUndefElements() const;
        
        CorePermutationType thePermutation;
        Dimension theDimension;
    };
    
}

std::ostream& operator<< ( std::ostream& ostr, const nmr::Permutation& thePerm );

#endif

