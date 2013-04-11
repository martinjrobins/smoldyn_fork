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

#ifndef CPX_PLEXISOIMPL_H
#define CPX_PLEXISOIMPL_H

namespace cpx
{
    template<class plexT>
    bool
    plexIso::
    tryMapBinding( const plexT& rSrcPlex,
                   int srcBindingNdx,
                   const plexT& rTgtPlex,
                   int tgtBindingNdx )
    {
        bool flipForward, flipBackward;
        
        // Test both the forward and backward maps for compatibility with
        // the given mapping of bindings, noting whether the binding needs
        // to be flipped or not.
        //
        // If the binding needs to be flipped for the forward map, then it
        // should need to be flipped for the backward map, too.  This shows
        // that I'm doing the flip check twice, but it's really a cost-free
        // byproduct the necessary check of consistency of the mol mapping,
        // I think...
        if ( forward.canMapBinding( rSrcPlex,
                                    srcBindingNdx,
                                    rTgtPlex,
                                    tgtBindingNdx,
                                    flipForward )
             &&
             backward.canMapBinding( rTgtPlex,
                                     tgtBindingNdx,
                                     rSrcPlex,
                                     srcBindingNdx,
                                     flipBackward ) )
        {
            forward.doMapBinding( rSrcPlex,
                                  srcBindingNdx,
                                  rTgtPlex,
                                  tgtBindingNdx,
                                  flipForward );
            
            backward.doMapBinding( rTgtPlex,
                                   tgtBindingNdx,
                                   rSrcPlex,
                                   srcBindingNdx,
                                   flipBackward );
            return true;
        }
        else return false;
    }
}

#endif // CPX_PLEXISOIMPL_H
