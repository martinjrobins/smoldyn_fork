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

#ifndef PLEXELTNAME_H
#define PLEXELTNAME_H

#include <string>

namespace plx
{
    namespace eltName
    {
        extern const std::string allostericPlexes;
        extern const std::string allostericPlex;
        extern const std::string allostericOmnis;
        extern const std::string allostericOmni;
        extern const std::string plex;
        extern const std::string molInstance;
        extern const std::string molInstance_nameAttr;
        extern const std::string molRef;
        extern const std::string molRef_nameAttr;
        extern const std::string binding;
        extern const std::string molInstanceRef;
        extern const std::string molInstanceRef_nameAttr;
        extern const std::string allostericSites;
        
        extern const std::string plexSpecies;
        extern const std::string plexSpecies_nameAttr;
        
        extern const std::string plexSpeciesStream;
        extern const std::string plexSpeciesStream_nameAttr;
        extern const std::string omniSpeciesStream;
        extern const std::string omniSpeciesStream_nameAttr;
        
        extern const std::string instanceStates;
        extern const std::string modMolInstanceRef;
        extern const std::string modMolInstanceRef_nameAttr;
        
        extern const std::string unboundSites;
        extern const std::string boundSites;
        extern const std::string instanceRef;
        extern const std::string instanceRef_nameAttr;
        extern const std::string siteRef;
        extern const std::string siteRef_nameAttr;
        
        // For state dump.
        
        extern const std::string taggedPlexSpecies;
        extern const std::string taggedPlexSpecies_tagAttr;
        extern const std::string taggedPlexSpecies_nameAttr;
        extern const std::string population;
        extern const std::string population_countAttr;
        extern const std::string concentration;
        extern const std::string concentration_valueAttr;
        // To give the flag in the dump.  Was trying to avoid this.
        // For now, an optional element that is present if the species
        // has ever been updated.
        extern const std::string updated;
    }
}

#endif
