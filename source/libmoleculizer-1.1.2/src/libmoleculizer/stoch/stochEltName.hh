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

#ifndef STOCHELTNAME_H
#define STOCHELTNAME_H

#include <string>

namespace stoch
{
    namespace eltName
    {
        extern const std::string stochSpecies;
        extern const std::string stochSpecies_nameAttr;
        
        // For state output.
        
        extern const std::string taggedStochSpecies;
        extern const std::string taggedStochSpecies_tagAttr;
        extern const std::string taggedStochSpecies_nameAttr;
        extern const std::string weight;
        extern const std::string weight_daltonsAttr;
        extern const std::string population;
        extern const std::string population_countAttr;
        
        // Redundant concentration element to avoid calculation upon
        // translation to ODE world.
        extern const std::string concentration;
        extern const std::string concentration_valueAttr;
    }
}

#endif
