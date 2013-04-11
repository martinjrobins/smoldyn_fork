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

#include "stoch/stochEltName.hh"

namespace stoch
{
    namespace eltName
    {
        const std::string stochSpecies( "stoch-species" );
        const std::string stochSpecies_nameAttr( "name" );
        
        // For state output.
        
        const std::string taggedStochSpecies( "tagged-stoch-species" );
        const std::string taggedStochSpecies_tagAttr( "tag" );
        const std::string taggedStochSpecies_nameAttr( "name" );
        const std::string weight( "weight" );
        const std::string weight_daltonsAttr( "daltons" );
        const std::string population( "population" );
        const std::string population_countAttr( "count" );
        const std::string concentration( "concentration" );
        const std::string concentration_valueAttr( "value" );
    }
}
