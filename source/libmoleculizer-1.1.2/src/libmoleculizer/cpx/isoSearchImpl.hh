/////////////////////////////////////////////////////////////////////////////
// Moleculizer - a stochastic simulator for cellular chemistry.
// Copyright (C) 2001  Walter Lawrence (Larry) Lok.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//    
// Contact information:
//   Larry Lok, Research Fellow          Voice: 510-981-8740
//   The Molecular Sciences Institute      Fax: 510-647-0699
//   2168 Shattuck Ave.                  Email: lok@molsci.org
//   Berkeley, CA 94704
/////////////////////////////////////////////////////////////////////////////

#ifndef CPX_ISOSEARCHIMPL_H
#define CPX_ISOSEARCHIMPL_H

#include "cpx/cpxXcpt.hh"

namespace cpx
{
    template<class plexT>
    bool
    isoSearch<plexT>::
    mapRestBindings(int leftBindingNdx,
                    const plexIso& rCurrentIso) const
    {
        // Are we done?  
        if(((int) rLeft.bindings.size()) <= leftBindingNdx)
        {
            onSuccess(rCurrentIso);
            return true;
        }
        
        // Try to extend the given isomorphism over the given left binding
        // until one is found that can be extended to a full isomorpism.
        for(int rightBindingNdx = 0;
            rightBindingNdx < (int) rRight.bindings.size();
            rightBindingNdx++)
        {
            // Construct a new temporary isomorphism for this trial.
            plexIso tmpIso(rCurrentIso);
            
            if(tmpIso.tryMapBinding(rLeft,
                                    leftBindingNdx,
                                    rRight,
                                    rightBindingNdx)
               && mapRestBindings(leftBindingNdx + 1,
                                  tmpIso))
            {
                return true;
            }
        }
        return false;
    }
    
    template<class plexT>
    bool 
    isoSearch<plexT>::
    findInjection(void) const
    {
        plexIso tmpIso(rLeft.mols.size(),
                       rLeft.bindings.size(),
                       rRight.mols.size(),
                       rRight.bindings.size());
        
        // Since we're assuming that the left plex is connected,
        // we've got two cases, either every mol is on a binding
        // or there is only one mol.
        if(rLeft.bindings.size() > 0)
        {
            // Search for mappings of all the bindings in the pattern.
            if(mapRestBindings(0,
                               tmpIso))
            {
                return true;
            }
        }
        else if(1 == rLeft.mols.size())
        {
            // Attempt to match with each of the mols in the target complex.
            for(int tgtMolNdx = 0;
                tgtMolNdx < (int) rRight.mols.size();
                tgtMolNdx++)
            {
                if(tmpIso.forward.canMapMol(rLeft,
                                            0,
                                            rRight,
                                            tgtMolNdx))
                {
                    tmpIso.forward.molMap[0] = tgtMolNdx;
                    tmpIso.backward.molMap[tgtMolNdx] = 0;
                    onSuccess(tmpIso);
                    return true;
                }
            }
        }
        else
        {
            throw plexNotConnectedXcpt();
        }
        
        return false;
    }
    
    template<class plexT>
    bool 
    isoSearch<plexT>::
    findIso(void) const
    {
        if((rLeft.bindings.size() != rRight.bindings.size())
           ||
           (rLeft.mols.size() != rRight.mols.size()))
        {
            return false;
        }
        else
        {
            return findInjection();
        }
    }
}

#endif // CPX_ISOSEARCHIMPL_H
