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

#ifndef CPX_RECOGNIZERIMPL_H
#define CPX_RECOGNIZERIMPL_H

#include <iostream>

namespace cpx
{
    // For deleting plexFamilies at the end of time.  plexFamilies are managed
    // by the recognizer, and plexSpecies are managed by their plexFamilies.
    template<class plexFamilyT>
    class deleteHasherValue :
        public std::unary_function<std::pair<int, plexFamilyT*>, void>
    {
    public:
        void
        operator()( const std::pair<int, plexFamilyT*>& rValue ) const
        {
            delete rValue.second;
        }
    };
    
    template<class plexT,
             class plexFamilyT>
    recognizer<plexT,
               plexFamilyT>::
    ~recognizer( void )
    {
        // The deleter for map values should work with multimaps, too.
        for_each( plexHasher.begin(),
                  plexHasher.end(),
                  deleteHasherValue<plexFamilyType>() );
    }
    
    // Function class for finding a plex species among those with the same
    // hash value.  This version tracks the isomorphism with the
    // plexFamily's paradigm.
    template<class plexT,
             class plexFamilyT>
    class thisPlexFamilyIso :
        public std::unary_function<std::pair<const int, plexFamilyT*>, bool>
    {
        const plexT& rPlex;
        plexIso& rIsomorphism;
        
    public:
        thisPlexFamilyIso( const plexT& rPlexToClassify,
                           plexIso& rIso ) :
            rPlex( rPlexToClassify ),
            rIsomorphism( rIso )
        {}
        
        bool operator()( const std::pair<const int, plexFamilyT*>& rHashEntry )
        {
            return
                reportIsoSearch<plexT>
                ( rPlex,
                  rHashEntry.second->getParadigm(),
                  rIsomorphism ).findIso();
        }
    };
    
    // Function class for finding a plex species among those with the same
    // hash value.  This one doesn't do tracking of the isomorphism.
    template<class plexT,
             class plexFamilyT>
    class isThisPlexFamily :
        public std::unary_function<std::pair<const int, plexFamilyT*>, bool>
    {
        const plexT& rPlex;
    public:
        isThisPlexFamily( const plexT& rPlexToClassify ) :
            rPlex( rPlexToClassify )
        {}
        
        bool operator()( const std::pair<const int, plexFamilyT*>& rHashEntry ) const
        {
            return this->plexIsoSearch< ( rPlex, rHashEntry.second->getParadigm() ).findIso();
        }
    };
    
    template<class plexT,
             class plexFamilyT>
    bool
    recognizer<plexT, plexFamilyT>::
    unify( const plexType& aPlex,
           plexFamilyType*& rpFamily,
           plexIso* pIso )
    {
        // Check the cache to see if this plex has ever been recognized
        // before.
        std::pair<typename std::map<plexType, recognition>::iterator, bool> insertResult
            = recognizedCache.insert( std::make_pair( aPlex,
                                                      recognition() ) );
        
        // We use these references to correct the entry in the
        // cache if the insert (of the invalid recognition)
        // succeeds.
        plexFamilyType*& rFamilyPtr = insertResult.first->second.pPlexFamily;
        plexIso& rIso = insertResult.first->second.iso;
        
        // If the insert succeeded, then the plex is either totally
        // unknown or is isomorphic to a known plex.
        bool familyIsNew = false;
        if ( insertResult.second )
        {
            // Get the plex's hash value.
            int plexHashValue = aPlex.hashValue();
            
            // Get iterator range to all plex iso classes with this hash
            // value.
            std::pair<typename std::multimap<int, plexFamilyType*>::iterator,
                typename std::multimap<int, plexFamilyType*>::iterator> rangeIterPair
                = plexHasher.equal_range( plexHashValue );
            
            // Scan the plex isomorphism classes having the same hash value
            // as this plex for the correct isomorphism class of this plex.
            typename std::multimap<int, plexFamilyType*>::iterator iEntry
                = find_if( rangeIterPair.first,
                           rangeIterPair.second,
                           thisPlexFamilyIso<plexType, plexFamilyType> ( aPlex,
                                                                         rIso ) );
            
            if ( iEntry == rangeIterPair.second )
            {
                // We have never seen the plex before, so we have to construct
                // its plexFamily.
                familyIsNew = true;
                
                // Construct a new plexFamily.
                rFamilyPtr = makePlexFamily( aPlex );
                
                // The given plex is the paradigm of the new plexFamily,
                // so the isomorphism is the identity.
                rIso = plexIso::makeIdentity( aPlex.mols.size(),
                                              aPlex.bindings.size() );
                
                // Rememember this family, in case we ever see it again.
                plexHasher.insert( std::make_pair( plexHashValue,
                                                   rFamilyPtr ) );
            }
            else
            {
                // The plex belongs to a family that we've already seen before,
                // but was not in the cache.
                rFamilyPtr = iEntry->second;
            }
        }
        
        // Return plexFamily pointer that was installed in the map.
        rpFamily = rFamilyPtr;
        
        // Optionally return (i.e. copy out) the isomorphism from the
        // plex to the family's paradigm plex.
        if ( pIso ) *pIso = rIso;
        
        // Return whether plexFamily was recognized for the first time.
        return familyIsNew;
    }
    
    // Finds the isomorphism class (plexFamily) of a given plex.  Does not
    // track the isomorphism with the plexFamily's paradigm.
    //
    // Note that this routine doesn't let you know if the plex was new or
    // old.  That feature would be easy to add, and it would make it
    // possible to notify the user if (s)he'd defined the same plex under
    // two different names.
    //
    // This is the most commonly used form of recognition. It is used at
    // "run time," after all definitions and specifications are complete.
    // It recognizes omniPlexes in the recognized plexFamily.
    template<class plexT,
             class plexFamilyT>
    plexFamilyT*
    recognizer<plexT,
               plexFamilyT>::
    operator()( const plexType& aPlex )
    {
        // Do the basic recognition, without initialization of the plexFamily.
        plexFamilyType* pPlexFamily;
        if ( unify( aPlex,
                    pPlexFamily,
                    0 ) )
        {
            // Connect the plexFamily to all its features.
            pPlexFamily->connectToFeatures();
        }
        
        return pPlexFamily;
    }
    
    // Does recognition, including finding active subcomplexes (omniPlexes),
    // and reports the isomorphism with the plexFamily's paradigm.  This
    // version is for use at run time, and assumes that all omniplexes are
    // already specified.
    //
    // This is used in the decomposition reaction.
    template<class plexT,
             class plexFamilyT>
    plexFamilyT*
    recognizer<plexT,
               plexFamilyT>::
    operator()( const plexType& aPlex,
                plexIso& rIso )
    {
        // Do the basic recognition, without initialization of the plexFamily.
        plexFamilyType* pPlexFamily;
        if ( unify( aPlex,
                    pPlexFamily,
                    &rIso ) )
        {
            // Connect the plexFamily to all its features.
            pPlexFamily->connectToFeatures();
        }
        
        return pPlexFamily;
    }
}


#endif // CPX_RECOGNIZERIMPL_H
