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

#ifndef CPX_PLEXMAPIMPL_H
#define CPX_PLEXMAPIMPL_H

namespace cpx
{
    template<class plexT>
    bool
    plexMap::
    canMapMol( const plexT& rSrcPlex,
               int srcMolNdx,
               const plexT& rTgtPlex,
               int tgtMolNdx ) const
    {
        return
            ( molMap[srcMolNdx] == tgtMolNdx )
            || ( molUnmapped( srcMolNdx )
                 && ( rSrcPlex.mols[srcMolNdx] == rTgtPlex.mols[tgtMolNdx] ) );
    }
    
    template<class plexT>
    bool
    plexMap::
    canMapSite( const plexT& rSrcPlex,
                const siteSpec& rSrcSite,
                const plexT& rTgtPlex,
                const siteSpec& rTgtSite ) const
    {
        return
            canMapMol( rSrcPlex,
                       rSrcSite.molNdx(),
                       rTgtPlex,
                       rTgtSite.molNdx() )
            &&
            rSrcSite.siteNdx() == rTgtSite.siteNdx();
    }
    
    template<class plexT>
    bool
    plexMap::
    canMapBinding( const plexT& rSrcPlex,
                   int fromBindingNdx,
                   const plexT& rTgtPlex,
                   int toBindingNdx,
                   bool& rMustFlip ) const
    {
        // First check the binding map to see if there is already
        // a conflicting mapping of the binding.
        if ( bindingUnmapped( fromBindingNdx )
             || bindingMap[fromBindingNdx] == toBindingNdx )
        {
            // Since the binding can be mapped, or is already mapped,
            // we check whether the mols can be mapped consistently,
            // noting whether the binding has to be "flipped" to get
            // the consistent mapping of mols.
            
            const siteSpec& rLeftSrcSite
                = rSrcPlex.bindings[fromBindingNdx].leftSite();
            const siteSpec& rRightSrcSite
                = rSrcPlex.bindings[fromBindingNdx].rightSite();
            const siteSpec& rLeftTgtSite
                = rTgtPlex.bindings[toBindingNdx].leftSite();
            const siteSpec& rRightTgtSite
                = rTgtPlex.bindings[toBindingNdx].rightSite();
            
            // Try to match up the mols in the unflipped order.
            if ( canMapSite( rSrcPlex,
                             rLeftSrcSite,
                             rTgtPlex,
                             rLeftTgtSite )
                 &&
                 canMapSite( rSrcPlex,
                             rRightSrcSite,
                             rTgtPlex,
                             rRightTgtSite ) )
            {
                // Unflipped mapping of the mols is consistent.
                rMustFlip = false;
                return true;
            }
            // Try to match up the mols in the flipped order.
            else if ( canMapSite( rSrcPlex,
                                  rRightSrcSite,
                                  rTgtPlex,
                                  rLeftTgtSite )
                      &&
                      canMapSite( rSrcPlex,
                                  rLeftSrcSite,
                                  rTgtPlex,
                                  rRightTgtSite ) )
            {
                // Flipped mapping of the mols is consistent.
                rMustFlip = true;
                return true;
            }
            // Although the binding map is okay, there is no consistent
            // way to map the mols.
            else return false;
        }
        // The binding map conflicts with the proposed mapping of the
        // binding.
        else return false;
    }
    
    template<class plexT>
    void
    plexMap::
    doMapBinding( const plexT& rSrcPlex,
                  int srcBindingNdx,
                  const plexT& rTgtPlex,
                  int tgtBindingNdx,
                  bool flipBinding )
    {
        int leftSrcMolNdx
            = rSrcPlex.bindings[srcBindingNdx].leftSite().molNdx();
        int rightSrcMolNdx
            = rSrcPlex.bindings[srcBindingNdx].rightSite().molNdx();
        int leftTgtMolNdx
            = rTgtPlex.bindings[tgtBindingNdx].leftSite().molNdx();
        int rightTgtMolNdx
            = rTgtPlex.bindings[tgtBindingNdx].rightSite().molNdx();
        
        if ( flipBinding )
        {
            molMap[leftSrcMolNdx] = rightTgtMolNdx;
            molMap[rightSrcMolNdx] = leftTgtMolNdx;
        }
        else
        {
            molMap[leftSrcMolNdx] = leftTgtMolNdx;
            molMap[rightSrcMolNdx] = rightTgtMolNdx;
        }
        
        bindingMap[srcBindingNdx] = tgtBindingNdx;
    }
}

#endif // CPX_PLEXMAPIMPL_H
