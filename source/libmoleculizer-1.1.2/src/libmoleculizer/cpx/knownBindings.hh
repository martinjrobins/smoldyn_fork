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

#ifndef CPX_KNOWNBINDINGS_H
#define CPX_KNOWNBINDINGS_H

#include "cpx/structuralBinding.hh"

namespace cpx
{
    template<class molT, class bindingFeatureT>
    class knownBindings :
        public std::map<structuralBinding<molT>, bindingFeatureT>
    {
    public:
        
        // Looks for the binding in both directions.  Returns null pointer
        // if found in neither direction.
        bindingFeatureT*
        findFeature( const molT* pLeftMol,
                     int leftSiteNdx,
                     const molT* pRightMol,
                     int rightSiteNdx )
        {
            
            structuralSite<molT> leftSite( pLeftMol,
                                           leftSiteNdx );
            structuralSite<molT> rightSite( pRightMol,
                                            rightSiteNdx );
            
            structuralBinding<molT> theBinding( leftSite,
                                                rightSite );
            
            typename knownBindings::iterator iEntry
                = find( theBinding );
            
            if ( this->end() == iEntry )
            {
                iEntry = find( structuralBinding<molT> ( rightSite, leftSite ) );
            }
            
            return ( this->end() == iEntry )
                ? 0
                : & ( iEntry->second );
        }
        
        // Looks for the binding in both directions, then adds it if not found,
        // returning the bindingFeature as in findFeature in either case.
        bindingFeatureT*
        ensureFeature( const molT pLeftMol,
                       int leftSiteNdx,
                       const molT* pRightMol,
                       int rightSiteNdx )
        {
            structuralSite<molT> leftSite( pLeftMol,
                                           leftSiteNdx );
            
            structuralSite<molT> rightSite( pRightMol,
                                            rightSiteNdx );
            
            structuralBinding<molT> forwardBinding( leftSite, rightSite );
            structuralBinding<molT> reverseBinding( rightSite, leftSite );
            
            typename knownBindings::iterator iEntry = find( forwardBinding );
            
            if ( this->end() == iEntry )
            {
                iEntry = find( reverseBinding );
            }
            
            if ( this->end() == iEntry )
            {
                iEntry = insert( forwardBinding,
                                 bindingFeatureT() ).first;
            }
            
            return & ( iEntry->second );
        }
    };
}

#endif // CPX_KNOWNBINDINGS_H
