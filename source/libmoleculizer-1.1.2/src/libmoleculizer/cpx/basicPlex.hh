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

#ifndef CPX_BASICPLEX_H
#define CPX_BASICPLEX_H

#include "cpx/binding.hh"
#include "cpx/ftrSpec.hh"
#include "cpx/plexIso.hh"

namespace cpx
{
    template<class molT>
    class basicPlex
    {
    public:
        typename std::vector<molT*> mols;
        typename std::vector<binding> bindings;
        
        typedef molT molType;
        
        // Used in finding the free sites on a plex.
        void
        makeSiteToBindings( typename std::map<siteSpec, bindingSpec>&
                            rSiteToBindings ) const;
        
        // Finds the free sites on a plex.
        void
        makeFreeSiteVector( typename std::vector<siteSpec>& rFreeSiteVector ) const;
        
        // Used in finding connected components of a plex.
        void
        makeMolToBindings( typename std::multimap<int, int>& rMolToBindings ) const;
        
        // Used in finding connected components of a plex.
        void
        pushConnectedBindings( int molNdx,
                               const typename std::multimap<int, int>& rMolToBindings,
                               plexIso& rIso,
                               basicPlex& component ) const;
        
        // Used in finding connected components of a plex.
        int
        pushConnectedMol( int molNdx,
                          plexIso& rIso,
                          basicPlex& component ) const;
        
        // Loads the connected component of molNdx into component, which should
        // be empty when the routine is called.  An partial isomoprhism from the
        // subcomplex of this plex onto the component is recorded in rIso. That
        // plexIso should be large enough to hold the entire plex, in case
        // this plex is connected.  Create this isomorphism as is done in
        // makeConnectedComponent:
        // plexIso iso(mols.size(), bindings.size());
        void
        makeTrackedComponent( int molNdx,
                              basicPlex& component,
                              plexIso& rIso ) const;
        
        // This does the same as above, but creates and disposes of an appropriate
        // sized isomorphism.
        void
        makeConnectedComponent( int molNdx,
                                basicPlex& component ) const;
        
        // Uses the above to determine if this plex is connected.
        bool
        plexIsConnected( void ) const;
        
        // Computation of the "representation-invariant" hash value of 'this'
        // plex.  Two plexes that represent the same complex, but for example
        // have their mols or bindings in a different order, should return the
        // same hash value.
        int
        hashValue( void ) const;
        
        std::string
        getName() const
        {
            std::string name( "(" );

            for( typename std::vector<molT*>::const_iterator iter = mols.begin();
                 iter != mols.end();
                 ++iter)
            {
                name += (*iter)->getName();
                name += ", ";
            }
            name += ")";

            return name;
        }
        
        // Used in the recognition cache.
        //
        // This seems to be the same as
        // std::pair<std::vector<molT*>, std::vector<binding> >::operator<
        // though written differently.
        bool
        operator< ( const basicPlex& rRightPlex ) const
        {
            if ( mols < rRightPlex.mols ) return true;
            if ( rRightPlex.mols < mols ) return false;
            return bindings < rRightPlex.bindings;
        }
    };
}

#include "cpx/basicPlexImpl.hh"

#endif // CPX_BASICPLEX_H
