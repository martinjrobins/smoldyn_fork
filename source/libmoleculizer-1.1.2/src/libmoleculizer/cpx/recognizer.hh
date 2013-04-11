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

#ifndef CPX_RECOGNIZER_H
#define CPX_RECOGNIZER_H

#include "cpx/plexIso.hh"
#include "cpx/plexFamily.hh"

namespace cpx
{
    template<class plexT,
             class plexFamilyT>
    class recognizer
    {
    public:
        typedef plexT plexType;
        typedef plexFamilyT plexFamilyType;
        
    private:
        // In some cases, we have to know the isomorphism between
        // the recognized plex and the paradigm of its family.  This
        // is computed when the plex is first recognized, and saved
        // in the cache.
        class recognition
        {
        public:
            plexFamilyType* pPlexFamily;
            plexIso iso;
        };
        
        // Cache for immediate recognition of previously encountered plexes.
        std::map<plexType, recognition> recognizedCache;
        
    public:
        // Publicized in order to traverse all the plexFamilies.
        //
        // In particular, for plexUnit::prepareToRun().
        std::multimap<int, plexFamilyType*> plexHasher;
        
        virtual
        ~recognizer( void );
        
        virtual plexFamilyType*
        makePlexFamily( const plexType& rPlex ) const = 0;
        
        int familyCount( void ) const
        {
            return plexHasher.size();
        }
        
        // Finds the plexFamily of a plex, and gives the isomorphism of the
        // given plex with the plexFamily's paradigm.  This just runs the
        // bare constructor of the plexFamily, leaving undone the "phase
        // 2" part of plexFamily initialization: connection to features and
        // generation of the default parameter.
        //
        // I expect this to be used during setup, rather than at runtime.
        // Its purpose is to give finer control over the construction of
        // the plexFamily for a user-defined complex, as opposed to an
        // automatically generated one.  The user can make arbitrary
        // allosteric modifications, so we need (?) a way of unifying
        // families before constructing their default parameter/species.
        
        // This is a replacement function for the above.  It has an unpleasant
        // interface, but it thereby avoids some replication of code.
        bool
        unify( const plexType& rPlex,
               plexFamilyType*& rpFamily,
               plexIso* pIso = 0 );
        
        // Recognizes an ordinary plex, and produces a fully initialized
        // plexFamily.  I expect this to be used at runtime.
        plexFamilyType*
        operator()( const plexType& aPlex );
        
        // Recongizes an ordinary plex, and produces a fully initialized
        // plexFamily, as the above, but also reports the isomorphism from
        // the given plex to the plexFamily's paradigm.
        //
        // This is used in the decomposition reaction.
        plexFamilyType*
        operator()( const plexType& aPlex,
                    plexIso& rIso );
        
        // Output routine.
        void
        insertSpecies( xmlpp::Element* pExplicitSpeciesElt,
                       double molarFactor ) const
            throw( std::exception );
    };
}

#include "cpx/recognizerImpl.hh"

#endif // CPX_RECOGNIZER_H
