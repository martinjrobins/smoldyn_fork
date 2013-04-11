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

#ifndef CPX_HASHMOLRECIMPL_H
#define CPX_HASHMOLRECIMPL_H

namespace cpx
{
    template<class molT>
    size_t
    hashMolRec<molT>::
    operator()( int molNdx,
                int depth ) const
    {
        typename utl::linearHash lh;
        size_t hashValue = 0;
        
        // Have we seen this mol?
        //
        // Not by pointer, since who knows when we might encounter
        // a particular mol species in a random traversal.
        if ( rMolsSeen.find( molNdx ) == rMolsSeen.end() )
        {
            // We have now seen the mol that this path goes to.
            rMolsSeen.insert( molNdx );
            
            // Initialize the hash value using the mol pointer and
            // the depth.
            molT* pMol = rPlex.mols[molNdx];
            hashValue = lh( lh(( size_t ) pMol )
                            + ( size_t ) depth );
            
            // Traverse the sites on this mol.  We can use the ordering
            // of the sites on the mol.
            for ( int siteNdx = 0;
                  siteNdx < ( int ) pMol->getSiteCount();
                  siteNdx++ )
            {
                siteSpec curSiteSpec( molNdx, siteNdx );
                
                typename std::map<siteSpec, int>::const_iterator iPair
                    = rSiteToBindings.find( curSiteSpec );
                
                if ( iPair != rSiteToBindings.end() )
                {
                    int bindingNdx = iPair->second;
                    const binding& rBinding
                        = rPlex.bindings[bindingNdx];
                    
                    if ( rBinding.leftSite().molNdx() == molNdx
                         && rBinding.leftSite().siteNdx() == siteNdx )
                    {
                        hashValue
                            = lh(( size_t ) rBinding.rightSite().siteNdx()
                                 + lh(( size_t ) siteNdx
                                      + lh( hashValue ) ) );
                        
                        hashValue = lh(( *this )( rBinding.rightSite().molNdx(),
                                                  depth + 1 )
                                       + lh( hashValue ) );
                    }
                    else
                    {
                        hashValue
                            = lh(( size_t ) rBinding.leftSite().siteNdx()
                                 + lh(( size_t ) siteNdx
                                      + lh( hashValue ) ) );
                        
                        hashValue = lh(( *this )( rBinding.leftSite().molNdx(),
                                                  depth + 1 )
                                       + lh( hashValue ) );
                    }
                    
                }
            }
        }
        return hashValue;
    }
}

#endif // CPX_HASHMOLRECIMPL_H
