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

#ifndef CPX_PLEXMAP_H
#define CPX_PLEXMAP_H

#include <vector>
#include "cpx/ftrSpec.hh"

namespace cpx
{
    /*! \ingroup plexStructGroup
      \brief Structure-preserving map from one complex to another. */
    class plexMap
    {
    public:
        std::vector<int> molMap;
        std::vector<int> bindingMap;
        
        plexMap( void )
        {}
        
        plexMap( int molCount,
                 int bindingCount ) :
            molMap( molCount, -1 ),
            bindingMap( bindingCount, -1 )
        {}
        
        // Methods for applying a plexMap to the specs of various physical
        // features of the complex.
        siteSpec
        applyToSiteSpec( const siteSpec& rSourceSiteSpec ) const;
        
        // Ever used?
        bindingSpec
        applyToBindingSpec( bindingSpec sourceBindingSpec ) const
        {
            return bindingMap[sourceBindingSpec];
        }
        
        molSpec
        applyToMolSpec( molSpec sourceMolSpec ) const
        {
            return molMap[sourceMolSpec];
        }
        
        // Leaving out subPlexSpec for the time being; it's slightly
        // more complicated, and I doubt it's needed.
        
        bool
        molUnmapped( int molNdx ) const
        {
            return molMap[molNdx] < 0;
        }
        
        bool
        bindingUnmapped( int bindingNdx ) const
        {
            return bindingMap[bindingNdx] < 0;
        }
        
        // Returns true if 'this' map agrees with the assignment of the source
        // mol to the target mol already, or if the source mol is unmapped
        // and the source mol and target mol are the same.  If this is the case,
        // then the map can be "legally" extended by mapping the source mol
        // to the target mol.
        template<class plexT>
        bool
        canMapMol( const plexT& rSrcPlex,
                   int srcMolNdx,
                   const plexT& rTgtPlex,
                   int tgtMolNdx ) const;
        
        // Similar to the above: returns true if the mols on which the binding
        // sites occur are "mappable" according to canMapMol above and the source
        // binding site and target binding site are the same binding site (on the
        // same mol.)
        template<class plexT>
        bool
        canMapSite( const plexT& rSrcPlex,
                    const siteSpec& rSrcSite,
                    const plexT& rTgtPlex,
                    const siteSpec& rTgtSite ) const;
        
        // Similar to the above: returns true if 'this' map can be extended by
        // mapping the "from" binding to the "to" binding.  The "must flip" flag
        // is set if the sites of the bindings are ordered oppositely to the way
        // that they must be matched up by the map.
        template<class plexT>
        bool
        canMapBinding( const plexT& rSrcPlex,
                       int fromBindingNdx,
                       const plexT& rTgtPlex,
                       int toBindingNdx,
                       bool& rMustFlip ) const;
        
        // Destructively modifies 'this' map by extending the mapping over another
        // binding, which has previously been tested with "canMapBinding."
        template<class plexT>
        void
        doMapBinding( const plexT& rSrcPlex,
                      int srcBindingNdx,
                      const plexT& rTgtPlex,
                      int tgtBindingNdx,
                      bool flipBinding );
        
        // Generates the identity map from a given complex to itself.
        static plexMap
        makeIdentity( int molCount,
                      int bindingCount );
        
        // So that plexMaps and plexIsoPairs can be sorted/used for map keys.
        bool
        operator< ( const plexMap& rRightMap ) const
        {
            return (( molMap < rRightMap.molMap )
                    || (( molMap == rRightMap.molMap )
                        && ( bindingMap < rRightMap.bindingMap ) ) );
        }
        
        // This is used in the comparison function for plexIsos.
        bool
        operator== ( const plexMap& rRightMap ) const
        {
            return (( molMap == rRightMap.molMap )
                    && ( bindingMap == rRightMap.bindingMap ) );
        }
    };
}

#include "cpx/plexMapImpl.hh"

#endif // CPX_PLEXMAP_H
