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

#ifndef FTRELTNAME_H
#define FTRELTNAME_H

#include <string>

namespace ftr
{
    namespace eltName
    {
        // Generic omni-based reaction generator.
        extern const std::string omniGen;
        extern const std::string enablingOmniplex;
        
        extern const std::string smallMolExchanges;
        extern const std::string smallMolExchange;
        extern const std::string smallMolInstanceRef;
        extern const std::string smallMolInstanceRef_nameAttr;
        extern const std::string smallMolRef;
        extern const std::string smallMolRef_nameAttr;
        
        extern const std::string modificationExchanges;
        extern const std::string modificationExchange;
        extern const std::string modMolInstanceRef;
        extern const std::string modMolInstanceRef_nameAttr;
        extern const std::string modSiteRef;
        extern const std::string modSiteRef_nameAttr;
        extern const std::string installedModRef;
        extern const std::string installedModRef_nameAttr;
        
        extern const std::string installedModRef;
        extern const std::string installedModRef_nameAttr;
        
        extern const std::string additionalReactantSpecies;
        extern const std::string additionalReactantSpecies_nameAttr;
        extern const std::string additionalProductSpecies;
        extern const std::string additionalProductSpecies_nameAttr;
        
        extern const std::string rate;
        extern const std::string rate_valueAttr;
        
        extern const std::string uniMolGen;
        extern const std::string enablingMol;
        extern const std::string enablingMol_nameAttr;
        extern const std::string enablingModifications;
        extern const std::string modRef;
        extern const std::string modRef_nameAttr;
        extern const std::string modificationExchanges;
        extern const std::string modificationExchange;
        extern const std::string additionalReactantSpecies;
        extern const std::string additionalProductSpecies;
    }
}

#endif // FTRELTNAME_H
