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

#ifndef PRM_H
#define PRM_H

/*! \file prm.hh
  \ingroup plexSpeciesGroup
  
  \brief Defines parameters making up plexParam, the a complex's parameter. */

#include <vector>
#include <map>
#include "cpx/siteShape.hh"
#include "cpx/molState.hh"
#include "cpx/ftrSpec.hh"
#include "cpx/siteToShapeMap.hh"

namespace cpx
{
    class plex;
    
    /*! \ingroup plexSpeciesGroup
      
      \brief Parameter giving the strength of a binding in a complex. */
    typedef double bindingParam;
    
    /*! \ingroup plexSpeciesGroup
      \brief Parameter for plexSpecies.
      
      This parameter is what distinguishes structurally identical species
      of complexes from one another.  It is constructed from one of its
      parts, the vector of molParams, together with the allosteric
      properties specified by the user for the structure. */
    class plexParam
    {
    public:
        // Tue Sep 14 12:11:22 PDT 2004 Eliminating bindingParams
        // and changing so that siteParams contains all binding
        // site shapes, instead of just those of free binding
        // sites.
        siteToShapeMap siteParams;
        std::vector<molParam> molParams;
        
        // When we intall the various parts of the default parameter,
        // we have to make sure that the mol and binding indices
        // will be valid.
        plexParam( const plex& rParadigm );
        
        // This is for inserting the molParams using push_back.
        plexParam( void )
        {}
        
        // Using the ordinary index operator [] on a map is essentially
        // non-const, since it may cause the creation of entries in the
        // map.  This is safely const. No good reason to return a reference,
        // though.
        const siteParam&
        getSiteParam( const siteSpec& rSpec ) const
            throw( utl::xcpt );
        
        siteParam&
        getSiteParam( const siteSpec& rSpec )
            throw( utl::xcpt );
        
        // This could be done with an accumulator.
        double
        getWeight( void ) const;
        
        bool operator< ( const plexParam& rRightParam ) const
        {
            if ( siteParams < rRightParam.siteParams ) return true;
            if ( rRightParam.siteParams < siteParams ) return false;
            if ( molParams < rRightParam.molParams ) return true;
            if ( rRightParam.molParams < molParams ) return false;
            return false;
        }
    };
}

#endif
