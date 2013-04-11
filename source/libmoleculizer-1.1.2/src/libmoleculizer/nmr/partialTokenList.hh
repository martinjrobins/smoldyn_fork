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


#ifndef __PARTIALTOKENLIST_HH
#define __PARTIALTOKENLIST_HH

#include "utl/defs.hh"
#include "nmr/abstractMol.hh"

namespace nmr
{
    // PartialTokenList Notes
    
    // PartialTokenLists are designed for comparing partial name sequences, lists
    // produced by isomorphic lists -- ie lists produced by identical complexes.
    
    // Because of this assumption, we conclude that the MolList 'theMol' is completely known,
    // and consists of the same members, between any two PartialTokenLists that are allowed
    // to interact, we conclude that the words produced by any two PartialTokenLists will
    // always be equal.
    
    // Thus, at minimum, PartialTokenList::operator< and operator== use this assumption in their
    // code.
    
    // Do not try to compare non-isomorphic PartialTokenList's.  Results will be undefined, and
    // will likely result in a crash.
    
    DECLARE_CLASS( PartialTokenList );
		class PartialTokenList
    {
				public:
        typedef int MolNdx;
        typedef int MolBindingSite;
        typedef std::pair<MolNdx, MolBindingSite> BindingSite;
        typedef std::pair<BindingSite, BindingSite> Binding;
        typedef Binding BindingToken;
        
        typedef std::string ModificationSite;
        typedef std::string ModificationValue;
        typedef std::pair<MolNdx, std::pair<ModificationSite, ModificationValue> > Modification;
        typedef Modification ModificationToken;
        
        typedef std::vector<MolPtr> MolList;
        typedef std::vector<Binding> BindingList;
        typedef BindingList::iterator BindingListIter;
        typedef BindingList::const_iterator ConstBindingListIter;
        typedef std::vector<Modification> ModificationList;
        typedef ModificationList::iterator ModificationListIter;
        typedef ModificationList::const_iterator ConstModificationListIter;
        
        PartialTokenList();
        PartialTokenList( const PartialTokenList& aPns )
            :
            theMols( aPns.theMols.begin(), aPns.theMols.end() ),
            theBindings( aPns.theBindings.begin(), aPns.theBindings.end() ),
            theModifications( aPns.theModifications.begin(), aPns.theModifications.end() ),
            isComplete( aPns.isComplete )
        {}
        
        ~PartialTokenList()
        {
            // PartialTokenLists never memory manages anything it uses.
        }
        
        void
        print() const;
        
        void clear();
        
    public:
        bool isSubTokenListOf( const PartialTokenList& aPns ) const;
        bool isEquivalentTo( const PartialTokenList& aPns ) const;
        
        bool operator< ( const PartialTokenList& aPns ) const;
        bool operator== ( const PartialTokenList& aPns ) const;

	std::string repr() const;
        
    public:
        
        // Data Members
        //
        
        MolList theMols;
        BindingList theBindings;
        ModificationList theModifications;
        bool isComplete;
    };
}

#endif
