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

#include "utl/utility.hh"
#include "complexSpecies.hh"
#include "namedMolecule.hh"
#include <iostream>

namespace nmr
{
    
    ComplexSpecies::ComplexSpecies()
        :
        theMols(),
        theMolAliasToNdxMap(),
        theBindings()
    {}

    ComplexSpecies::~ComplexSpecies()
    {
        for(unsigned int molPtrNdx = 0;
            molPtrNdx != theMols.size();
            ++molPtrNdx)
        {
            delete theMols[molPtrNdx];
        }
    }
    
    ComplexSpecies::ComplexSpecies( ComplexSpeciesCref aComplexSpecies )
        :
        theMols(),
        theMolAliasToNdxMap( aComplexSpecies.theMolAliasToNdxMap.begin(), aComplexSpecies.theMolAliasToNdxMap.end() ),
        theBindings( aComplexSpecies.theBindings.begin(), aComplexSpecies.theBindings.end() )
    {
        for( unsigned int ndx = 0; ndx != aComplexSpecies.theMols.size(); ++ndx)
        {
            MinimalMol* ptrMol = aComplexSpecies.theMols[ndx];

            MinimalMol* newMolPtr = new MinimalMol( *ptrMol );
            theMols.push_back( newMolPtr );
        }
    }
    
    ComplexSpecies::ComplexSpecies( ComplexOutputStateCref aComplexOutputState )
    {
        std::cerr << "Error in ComplexSpecies(ComplexOutputStateCref aComplexOutputState)" << std::endl;
        throw 10;
    }
    
    ComplexSpecies&
    ComplexSpecies::operator= ( const ComplexSpecies& crefComplexSpecies )
    {
        std::cerr << "Error in ComplexSpecies(ComplexOutputStateCref aComplexOutputState)" << std::endl;
        throw 10;
    }
    
    void
    ComplexSpecies::addMolToComplex( MinimalMol* someMol, AliasCref anAlias ) throw( DuplicateMolAliasXcpt )
    {
        // If the alias already exists throw an Exception.
        if ( theMolAliasToNdxMap.find( anAlias ) != theMolAliasToNdxMap.end() ) throw DuplicateMolAliasXcpt( someMol->getMolType(), anAlias );
        
        int newMolIndex = theMols.size();
        theMols.push_back( someMol );
        
        theMolAliasToNdxMap.insert( std::make_pair( anAlias,
                                                    newMolIndex ) );
    }
    
    void ComplexSpecies::addBindingToComplex( AliasCref firstMolAlias,
                                              BindingSiteCref firstMolBindingSiteAlias,
                                              AliasCref secondMolAlias,
                                              BindingSiteCref secondMolBindingSiteAlias ) throw( MissingMolAliasXcpt,
                                                                                                 MissingBindingSiteXcpt )
    {
        MolMapIter locFirstMol = theMolAliasToNdxMap.find( firstMolAlias );
        MolMapIter locSecondMol = theMolAliasToNdxMap.find( secondMolAlias );
        
        if ( locFirstMol == theMolAliasToNdxMap.end() )
        {
            throw MissingMolAliasXcpt( firstMolAlias );
        }
        
        if ( locSecondMol == theMolAliasToNdxMap.end() )
        {
            throw MissingMolAliasXcpt( secondMolAlias );
        }
        
        MolNdx firstMolNdx = theMolAliasToNdxMap[firstMolAlias];
        MolNdx secondMolNdx = theMolAliasToNdxMap[secondMolAlias];
        
        try
        {
            if ( theMols[firstMolNdx]->checkIfBindingSiteIsBound( firstMolBindingSiteAlias ) )
            {
                throw BindingSiteAlreadyBoundXcpt( firstMolAlias, firstMolBindingSiteAlias );
            }
        }
        catch ( NoSuchBindingSiteXcpt xcpt )
        {
            throw MissingBindingSiteXcpt( firstMolBindingSiteAlias, firstMolAlias );
        }
        
        try
        {
            if ( theMols[secondMolNdx]->checkIfBindingSiteIsBound( secondMolBindingSiteAlias ) )
            {
                throw BindingSiteAlreadyBoundXcpt( secondMolAlias, secondMolBindingSiteAlias );
            }
        }
        catch ( NoSuchBindingSiteXcpt xcpt )
        {
            throw MissingBindingSiteXcpt( secondMolBindingSiteAlias, secondMolAlias );
        }
        
        theMols[firstMolNdx]->bindAtBindingSite( firstMolBindingSiteAlias );
        theMols[secondMolNdx]->bindAtBindingSite( secondMolBindingSiteAlias );
        
        BndNdx firstMolBindingNdx = theMols[firstMolNdx]->getBindingSiteInteger( firstMolBindingSiteAlias );
        BndNdx secondMolBindingNdx = theMols[secondMolNdx]->getBindingSiteInteger( secondMolBindingSiteAlias );
        
        Binding aBinding;
        
        if ( firstMolNdx < secondMolNdx )
        {
            aBinding.first.first = firstMolNdx;
            aBinding.first.second = firstMolBindingNdx;
            aBinding.second.first = secondMolNdx;
            aBinding.second.second = secondMolBindingNdx;
        }
        else
        {
            aBinding.first.first = secondMolNdx;
            aBinding.first.second = secondMolBindingNdx;
            aBinding.second.first = firstMolNdx;
            aBinding.second.second = firstMolBindingNdx;
        }
        
        
        theBindings.insert( std::lower_bound( theBindings.begin(),
                                              theBindings.end(),
                                              aBinding ), aBinding );
        
        //        theBindings.push_back( aBinding);
        
        
        
    }
    
    
    
    unsigned int ComplexSpecies::getNumberOfMolsInComplex() const
    {
        return theMols.size();
    }
    
    
    unsigned int ComplexSpecies::getNumberOfBindingsInComplex() const
    {
        return theBindings.size();
    }
    
    
    
    ComplexSpecies::MolListCref
    ComplexSpecies::getMolList() const
    {
        return theMols;
    }
    
    ComplexSpecies::BindingListCref
    ComplexSpecies::getBindingList() const
    {
        return theBindings;
    }
    
    ComplexSpecies::MolListRef
    ComplexSpecies::getMolList()
    {
        return theMols;
    }
    
    ComplexSpecies::BindingListRef
    ComplexSpecies::getBindingList()
    {
        return theBindings;
    }
    
    void
    ComplexSpecies::constructOutputState( ComplexOutputState& anOutputState ) const
    {
        anOutputState.clear();
        
        for ( MolList::const_iterator ii = theMols.begin();
              ii != theMols.end();
              ++ii )
        {
            
            MolTokenStr aMolToken = ( *ii )->getMolType();
            anOutputState.addMolTokenToOutputState( aMolToken );
        }
        
        
        //Copy in the bindings.
        //The bindings are pairs of pair<int, int>, so we must unroll it ourselves.
        for ( BindingList::const_iterator i=theBindings.begin();
              i!=theBindings.end();
              ++i )
        {
            
            BindingTokenStr aBindingToken;
            
            int firstfirst, firstsecond, secondfirst, secondsecond;
            firstfirst = ( *i ).first.first;
            firstsecond = ( *i ).first.second;
            secondfirst = ( *i ).second.first;
            secondsecond = ( *i ).second.second;
            
            aBindingToken.first.first = utl::stringify( firstfirst );
            aBindingToken.first.second = utl::stringify( firstsecond );
            aBindingToken.second.first = utl::stringify( secondfirst );
            aBindingToken.second.second = utl::stringify( secondsecond );
            
            anOutputState.addBindingTokenToOutputState( aBindingToken );
        }
        
        for ( unsigned int molNdx=0;
              molNdx != this->getNumberOfMolsInComplex();
              ++molNdx )
        {
            std::string strMolNdx = utl::stringify( molNdx );
            ModificationList currentMolModificationList = theMols[molNdx]->getModificationList();
            
            for ( unsigned int modificationNdx = 0;
                  modificationNdx != currentMolModificationList.size();
                  ++modificationNdx )
            {
                std::string strModificationSite = currentMolModificationList[modificationNdx].first;
                std::string aModificationValue = currentMolModificationList[modificationNdx].second;
                
                ModificationTokenStr aModificationToken;
                
                // This should be stringified.
                
                aModificationToken.first = utl::stringify( molNdx );
                
                
                aModificationToken.second.first = strModificationSite;
                aModificationToken.second.second = aModificationValue;
                
                anOutputState.addModificationTokenToOutputState( aModificationToken );
            }
        }
        
        return;
    }
    
    
//     void ComplexSpecies::constructPartialTokenList( PartialTokenList& rComplexPartialTokenList ) const
//     {
//         for ( MolList::const_iterator index = theMols.begin();
//               index != theMols.end();
//               ++index )
//         {
//             rComplexPartialTokenList.theMols.push_back( *index );
//         }
//         for ( BindingList::const_iterator index = theBindings.begin();
//               index != theBindings.end();
//               ++index )
//         {
//             rComplexPartialTokenList.theBindings.push_back( *index );
//         }
        
//         for ( unsigned int molNdx = 0; molNdx != theMols.size(); ++molNdx )
//         {
//             ModificationList currentModNdxMolList = theMols[molNdx]->getModificationList();
            
//             for ( ModificationList::const_iterator i = currentModNdxMolList.begin();
//                   i != currentModNdxMolList.end();
//                   ++i )
//             {
//                 std::pair<int, std::pair<std::string, std::string> > unambiguousModification;
//                 unambiguousModification.first = molNdx;
//                 unambiguousModification.second = *i;
//                 rComplexPartialTokenList.theModifications.push_back( unambiguousModification );
//             }
            
//         }
        
//     }
    
    
    
    void ComplexSpecies::applyPermutationToComplex( const Permutation& aPermutation )
    {
        if ( !aPermutation.getIsComplete() )
        {
            std::cerr << aPermutation << std::endl;
            throw GeneralNmrXcpt( "Permutation is not complete" );
        }
        if ( aPermutation.getDimension() != this->getNumberOfMolsInComplex() )
        {
            throw GeneralNmrXcpt( "Permutation in not the same size as our mol" );
        }
        
        Permutation theInversePerm=aPermutation.invertPermutation();
        
        MolList updatedMolList;
        BindingList updatedBindingList;
        
        for ( unsigned int molNdx=0; molNdx!=theMols.size(); ++molNdx )
        {
            updatedMolList.push_back( theMols[theInversePerm[molNdx]] );
        }
        
        for ( BindingList::iterator bndIter = theBindings.begin();
              bndIter != theBindings.end();
              ++bndIter )
        {
            Binding copyOfBinding = ( *bndIter );
            Binding updatedBinding;
            
            updatedBinding.first.first = aPermutation[copyOfBinding.first.first];
            updatedBinding.first.second = copyOfBinding.first.second;
            updatedBinding.second.first = aPermutation[copyOfBinding.second.first];
            updatedBinding.second.second = copyOfBinding.second.second;
            
            sortBinding( updatedBinding );
            
            updatedBindingList.push_back( updatedBinding );
        }
        
        sort( updatedBindingList.begin(),
              updatedBindingList.end() );
        
        theMols.swap( updatedMolList );
        theBindings.swap( updatedBindingList );
        
        //Finally update the theMolAliasToNdxMap
        for ( MolMap::iterator iter =theMolAliasToNdxMap.begin();
              iter != theMolAliasToNdxMap.end();
              ++iter )
        {
            int originalIndex = ( *iter ).second;
            ( *iter ).second = aPermutation[originalIndex];
        }
    }
    
    
    void ComplexSpecies::sortBinding( Binding& aBinding )
    {
        if ( aBinding.first > aBinding.second )
        {
            swap( aBinding.first, aBinding.second );
        }
    }
    
    std::string
    ComplexSpecies::repr() const
    {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }
    
    
    
}

std::ostream&
operator<< ( std::ostream& stream, nmr::ComplexSpeciesCref aComplexSpecies )
{
    for( unsigned int ndx = 0; ndx != aComplexSpecies.getMolList().size(); ++ndx)
    {
        stream << "(" << aComplexSpecies.getMolList()[ndx]->getMolType() \
               << "| " << aComplexSpecies.getMolList()[ndx]->getModificationString() << ") - ";
    }

    for(unsigned int ndx = 0; ndx != aComplexSpecies.getBindingList().size(); ++ ndx)
    {
	stream << aComplexSpecies.getBindingList()[ndx].first.first 
	       << aComplexSpecies.getBindingList()[ndx].first.second
	       << "(" << aComplexSpecies.getMolList()[ aComplexSpecies.getBindingList()[ndx].first.first  ]->getBindingList()[ aComplexSpecies.getBindingList()[ndx].first.second ] << ")" 
	       << aComplexSpecies.getBindingList()[ndx].second.first 
	       << aComplexSpecies.getBindingList()[ndx].second.second
	       << "(" << aComplexSpecies.getMolList()[ aComplexSpecies.getBindingList()[ndx].second.first  ]->getBindingList()[ aComplexSpecies.getBindingList()[ndx].second.second ] << ")" 
	       << ", "; 
    }

    stream<< '\n';

    return stream;
}
