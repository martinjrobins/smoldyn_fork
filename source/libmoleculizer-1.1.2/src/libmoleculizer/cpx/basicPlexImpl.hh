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

#ifndef CPX_BASICPLEXIMPL_H
#define CPX_BASICPLEXIMPL_H

#include "cpx/hashMolRec.hh"

namespace cpx
{
    template<class molT>
    void
    basicPlex<molT>::
    makeSiteToBindings( typename std::map<siteSpec, bindingSpec>& rSiteToBindings ) const
    {
        bindingSpec spec = bindings.size();
        while ( 0 < spec-- )
        {
            const binding& rBinding = bindings[spec];
            
            rSiteToBindings.insert( std::make_pair( rBinding.leftSite(),
                                                    spec ) );
            
            rSiteToBindings.insert( std::make_pair( rBinding.rightSite(),
                                                    spec ) );
        }
    }
    
    template<class molT>
    void
    basicPlex<molT>::
    makeFreeSiteVector( typename std::vector<siteSpec>& rFreeSiteVector ) const
    {
        // Make the map from sites to the bindings they're involved in.
        std::map<siteSpec, int> siteToBindings;
        makeSiteToBindings( siteToBindings );
        
        // Enumerate the sites in this plex.  If a site doesn't appear
        // in the sites to bindings map, it's a free site, so insert it
        // into the vector of free sites.
        int molNdx = mols.size();
        while ( 0 < molNdx-- )
        {
            molT* pMol = mols[molNdx];
            
            int siteNdx = pMol->getSiteCount();
            while ( 0 < siteNdx-- )
            {
                siteSpec theSpec( molNdx, siteNdx );
                if ( siteToBindings.find( theSpec ) == siteToBindings.end() )
                    rFreeSiteVector.push_back( theSpec );
            }
        }
    }
    
    template<class molT>
    void
    basicPlex<molT>::
    makeMolToBindings( std::multimap<int, int>& rMolToBindings ) const
    {
        int bindingNdx = bindings.size();
        while ( 0 < bindingNdx-- )
        {
            const binding& rBinding = bindings[bindingNdx];
            rMolToBindings.insert( std::make_pair( rBinding.leftSite().molNdx(),
                                                   bindingNdx ) );
            rMolToBindings.insert( std::make_pair( rBinding.rightSite().molNdx(),
                                                   bindingNdx ) );
        }
    }
    
    template<class molT>
    void
    basicPlex<molT>::
    pushConnectedBindings( int molNdx,
                           const std::multimap<int, int>& rMolToBindings,
                           plexIso& rIso,
                           basicPlex& component ) const
    {
        // Which bindings are on mol at molNdx?
        typename std::pair<std::multimap<int, int>::const_iterator,
            std::multimap<int, int>::const_iterator> rangeIterators
        = rMolToBindings.equal_range( molNdx );
    
    // Install all of the connected bindings that aren't already
    // in the connected component.
    for ( typename std::multimap<int, int>::const_iterator iMolBindingNdx
              = rangeIterators.first;
          rangeIterators.second != iMolBindingNdx;
          iMolBindingNdx++ )
    {
        // Has this binding already been done?
        int bindingNdx = iMolBindingNdx->second;
        if ( rIso.forward.bindingUnmapped( bindingNdx ) )
        {
            const binding& rBinding = bindings[bindingNdx];
            
            // Make sure the mols at the ends of the binding are done.
            // Rather than try to figure out which is the mol opposite
            // to molNdx, just checking both of them.
            int leftMolNdx = rBinding.leftSite().molNdx();
            int leftTargetNdx = pushConnectedMol( leftMolNdx,
                                                  rIso,
                                                  component );
            int rightMolNdx = rBinding.rightSite().molNdx();
            int rightTargetNdx = pushConnectedMol( rightMolNdx,
                                                   rIso,
                                                   component );
            
            // Now put in the binding, with its ends remapped.
            siteSpec leftSiteSpec( leftTargetNdx,
                                   rBinding.leftSite().siteNdx() );
            siteSpec rightSiteSpec( rightTargetNdx,
                                    rBinding.rightSite().siteNdx() );
            binding targetBinding( leftSiteSpec,
                                   rightSiteSpec );
            int bindingTargetNdx = component.bindings.size();
            component.bindings.push_back( targetBinding );
            rIso.forward.bindingMap[bindingNdx] = bindingTargetNdx;
            rIso.backward.bindingMap[bindingTargetNdx] = bindingNdx;
        }
    }
    }
    
    template<class molT>
    int
    basicPlex<molT>::
    pushConnectedMol( int molNdx,
                      plexIso& rIso,
                      basicPlex& component ) const
    {
        int targetNdx;
        if ( rIso.forward.molUnmapped( molNdx ) )
        {
            targetNdx = ( int ) component.mols.size();
            component.mols.push_back( mols[molNdx] );
            rIso.forward.molMap[molNdx] = targetNdx;
            rIso.backward.molMap[targetNdx] = molNdx;
        }
        else
        {
            targetNdx = rIso.forward.molMap[molNdx];
        }
        return targetNdx;
    }
    
    template<class molT>
    void
    basicPlex<molT>::
    makeTrackedComponent( int molNdx,
                          basicPlex& component,
                          plexIso& rIso ) const
    {
        typename std::multimap<int, int> molToBindings;
        makeMolToBindings( molToBindings );
        
        // Push the bindings on this mol to the component (along with
        // their mols).
        pushConnectedMol( molNdx,
                          rIso,
                          component );
        
        // Propagate over the bindings by "data recursion" on the mols
        // of the connected component.
        for ( int compMolNdx = 0;
              compMolNdx < ( int ) component.mols.size();
              compMolNdx++ )
        {
            int srcMolNdx = rIso.backward.molMap[compMolNdx];
            pushConnectedBindings( srcMolNdx,
                                   molToBindings,
                                   rIso,
                                   component );
        }
    }
    
    template<class molT>
    void
    basicPlex<molT>::
    makeConnectedComponent( int molNdx,
                            basicPlex& component ) const
    {
        plexIso iso( mols.size(),
                     bindings.size() );
        
        makeTrackedComponent( molNdx,
                              component,
                              iso );
    }
    
    // Function to determine if plex is connected.
    template<class molT>
    bool
    basicPlex<molT>::
    plexIsConnected( void ) const
    {
        basicPlex<molT> theComponent;
        
        makeConnectedComponent( 0,
                                theComponent );
        
        return theComponent.mols.size() == this->mols.size();
    }
    
    template<class molT>
    int
    basicPlex<molT>::
    hashValue( void ) const
    {
        typename std::map<siteSpec, int> siteToBindings;
        makeSiteToBindings( siteToBindings );
        
        int molNdx = mols.size();
        size_t hashValue = 0;
        while ( 0 < molNdx-- )
        {
            typename std::set<int> molsSeen;
            
            hashMolRec<molT> hmr( *this,
                                  siteToBindings,
                                  molsSeen );
            
            hashValue += hmr( molNdx,
                              0 );
        }
        return hashValue;
    }
}

#endif // CPX_BASICPLEXIMPL_H
