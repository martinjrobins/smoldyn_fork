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

#ifndef STOCHSPECIES_H
#define STOCHSPECIES_H

/*! \file stochSpecies.hh
  \ingroup stochGroup
  \brief Defines stochSpecies, simple molecules with no binding sites. */

#include "utl/dom.hh"
#include "mzr/mzrSpecies.hh"

namespace stoch
{
    /// A simple molecule with no binding sites.  This is the class for "small
    /// molecules." The "small-molecule" command defines one of these.  It
    /// actually has slightly more information than a species in Stochastirator
    /// would have, since molecular weight is given.
    class stochSpecies :
        public mzr::mzrSpecies
    {
        const double weight;
        
        // This is to fulfill the need for an "informative," rather than
        // canonical, name associated directly to the species.
        const std::string name;
        
    public:
        stochSpecies( const std::string& rName,
                      double molWeight = 1.0 ) :
            weight( molWeight ),
            name( rName )
        {}
        
        // Stoch species do not participate in automatic species/reaction
        // generation.
        void
        notify( int notificationDepth )
        {}
        
        std::string
        getName( void ) const
        {
            return name;
        }
        
        double
        getWeight( void ) const
        {
            return weight;
        }
        
        xmlpp::Element*
        insertElt( xmlpp::Element* pExplicitSpeciesElt,
                   double molarFactor ) const throw( std::exception );
    };
}

#endif
