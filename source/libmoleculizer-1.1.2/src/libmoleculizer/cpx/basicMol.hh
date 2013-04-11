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

#ifndef CPX_BASICMOL_H
#define CPX_BASICMOL_H

#include <sstream>
#include <map>
#include <vector>
#include <string>
#include "utl/xcpt.hh"
#include "cpx/siteShape.hh"
#include "cpx/molState.hh"

namespace cpx
{
    // Base class for simple constituent of a complex species.
    template<class bndSiteT>
    class basicMol :
        public std::vector<bndSiteT>
    {
        // The name of the mol, e.g. Ste11.
        typename std::string name;
        
        // Index for looking up binding sites by name.
        typedef typename std::map<typename std::string, int> indexMap;
        typedef typename indexMap::value_type indexValueType;
        typedef typename indexMap::iterator indexIterator;
        typedef typename indexMap::const_iterator constIndexIterator;
        indexMap siteNameToNdx;
        
        std::vector<siteParam> defaultShapes;
    public:
        typedef bndSiteT bindingSiteType;
        typedef typename std::vector<bndSiteT>::iterator bindingSiteIterator;
        typedef typename std::vector<bndSiteT>::const_iterator const_bindingSiteIterator;
        
        // Throws an exception if two sites have the same name.
        basicMol( const typename std::string& rName,
                  const typename std::vector<bndSiteT>& rSites )
            throw( typename utl::xcpt );
        
        basicMol( const basicMol& rOriginal );
        
        virtual
        ~basicMol( void )
        {}
        
        virtual const std::string&
        getName( void ) const
        {
            return name;
        }
        
        bindingSiteIterator
        getBindingSitesBegin()
        {
            return this->begin();
        }
        
        bindingSiteIterator
        getBindingSitesEnd()
        {
            return this->end();
        }
        
        const_bindingSiteIterator
        getBindingSitesBegin() const
        {
            return this->begin();
        }
        
        const_bindingSiteIterator
        getBindingSitesEnd() const
        {
            return this->end();
        }
        
        int
        getSiteCount( void ) const
        {
            return this->size();
        }
        
        // Returns true if there is a binding site with the given name,
        // and returns the index of the binding site at rSiteNdx.
        bool
        findSite( const typename std::string& rName,
                  int& rSiteNdx ) const;
        
        // Returns pointer to binding site with given name if it exists.
        // Otherwise returns 0.
        bndSiteT*
        getSite( const typename std::string& rName );
        
        // This seems to be the only state-related thing that all mols must
        // do; e.g. to compute their mass.
        //
        // Don't like this returning 0.  Need to be able to construct this class,
        // though, e.g. in consructor of stateMol.  Is there a better way to do
        // that?  That was bad because of base class given by template parameter.
        virtual molParam
        getDefaultParam( void ) const
        {
            return 0;
        }
        
        // Used to get a starting point for values of the map (in real mol
        // classes, which have states) from state to binding site shapes.
        std::vector<siteParam>
        getDefaultSiteParams( void ) const;
        
        // In a stateMol, the returned vector of pointers to site shapes is "hot",
        // it is actually a reference to the vector of site shapes in the mol's
        // allostery map.
        virtual
        std::vector<siteParam>&
        allostery( molParam param )
            throw( utl::xcpt )
        {
            return defaultShapes;
        }
        
        // Returns a string combining the type (hence virtual) of mol with the
        // instance index.  E.g. small-mol_3.
        virtual
        std::string
        genInstanceName( int molInstanceNdx ) const;
    };
}

#include "cpx/basicMolImpl.hh"

#endif // CPX_BASICMOL_H
