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

#include "partialTokenList.hh"

namespace nmr
{
    PartialTokenList::PartialTokenList()
        :
        theMols(),
        theBindings(),
        theModifications(),
        isComplete()
    {
        ; // do nothing
    }
    
    void
    PartialTokenList::clear()
    {
        theMols.clear();
        theBindings.clear();
        theModifications.clear();
    }
    
    bool PartialTokenList::isSubTokenListOf( const PartialTokenList& aPns ) const
    {
        if ( theBindings.size() > aPns.theBindings.size() ) return false;
        
        for ( ConstBindingListIter thisBindingListIter = theBindings.begin(),
                  aPnsBindingListIter = aPns.theBindings.begin();
              thisBindingListIter != theBindings.end();
              ++thisBindingListIter, ++aPnsBindingListIter )
        {
            if ( *thisBindingListIter != *aPnsBindingListIter ) return false;
        }
        
        // If we make it here, the binding list of this is a subset of the binding list of aPns.
        // Therefore aPns does not have fewer modifications in its ModificationList than this.
        // (If this has an incomplete binding list, it will have 0 modifications in it's list.
        // If it has a complete binding list, then so does aPns, which means that every modification
        // in the complex being studied is in the aPns MolList.
        
        ConstModificationListIter thisModificationListIter;
        ConstModificationListIter aPnsModificationListIter;
        
        for ( thisModificationListIter = theModifications.begin(), aPnsModificationListIter = aPns.theModifications.begin();
              thisModificationListIter != theModifications.end();
              ++thisModificationListIter, ++aPnsModificationListIter )
        {
            if ( *thisModificationListIter != *aPnsModificationListIter )
            {
                return false;
            }
        }
        
        return true;
    }
    
    bool PartialTokenList::operator< ( const PartialTokenList& aPns ) const
    {
        // Important! This function is only properly defined when this and aPns come from
        // the some complex.  Don't try to compare PartialTokenLists from two
        // non-isomorphic complexes.  It may or may not crash, and it defenitly won't give you
        // meaningful results.
        
        
        // Beacuse we assume that both aPns and this come from trying to canonicalize the same
        // complex, we can assume their MolLists are identical, and therefore we start our comparisons
        // with the binding lists.
        
        ConstBindingListIter thisBindingListIter;
        ConstBindingListIter aPnsBindingListIter;
        
        for ( ConstBindingListIter thisBindingListIter = theBindings.begin(),
                  aPnsBindingListIter = aPns.theBindings.begin();
              thisBindingListIter != theBindings.end() &&
                  aPnsBindingListIter != aPns.theBindings.end();
              ++thisBindingListIter, ++aPnsBindingListIter )
        {
            if ( *thisBindingListIter < *aPnsBindingListIter ) return true;
            if ( *thisBindingListIter > *aPnsBindingListIter ) return false;
        }
        
        // If not both are complete, we cannot say.  Therefore return false, for this
        // cannot be guarenteed to be less than aPns.
        if ( !( this->isComplete && aPns.isComplete ) ) return false;
        
        // So their binding lists are complete, which means that we can completely compare their
        // modification lists.  Since they both come from the same complex, we know they must
        // have the same number of modifications.
        return theModifications < aPns.theModifications;
    }
    
    bool PartialTokenList::operator== ( const PartialTokenList& aPns ) const
    {
        // TODO: The logical meaning of this predicate is XXX
        if ( this->isSubTokenListOf( aPns ) ) return true;
        else if ( aPns.isSubTokenListOf( *this ) ) return true;
        else return false;
    }
    
    bool PartialTokenList::isEquivalentTo( const PartialTokenList& aPns ) const
    {
        // TODO: The logical meaning of this predicate is XXX
        if ( theBindings == aPns.theBindings && theModifications == aPns.theModifications )
        {
            // Check the mols now.
            if ( theMols.size() != aPns.theMols.size() ) return false;
            
            for ( unsigned int ndx = 0;
                  ndx != theMols.size();
                  ++ndx )
            {
                // TODO: Check this remains accurate....
                if ( theMols[ndx]->getMolType() != aPns.theMols[ndx]->getMolType() ) return false;
            }
            
            return true;
        }
        else
        {
            return false;
        }
    }


    std::string PartialTokenList::repr() const
    {
	std::ostringstream oss;

	oss << theMols.size()  << ", " << theBindings.size() << '\n';

	for( MolList::const_iterator iter = theMols.begin(); 
	     iter != theMols.end();
	     ++iter)
	{
	    oss << (*iter)->getMolType() << ":";
	}
	
	oss << "\n";

	for( BindingList::const_iterator bndIter = theBindings.begin();
	     bndIter != theBindings.end();
	     ++bndIter)
	{
	    oss << "(" << (*bndIter).first.first
		<< "," << (*bndIter).first.second 
		<< "," << (*bndIter).second.first 
		<< "," << (*bndIter).second.second << ")";
		
	}
	oss << "\n";

	return oss.str();
	
    }

}

