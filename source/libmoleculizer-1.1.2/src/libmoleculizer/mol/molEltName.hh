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

#ifndef MOLELTNAME_H
#define MOLELTNAME_H

#include <string>

namespace bnd
{
    namespace eltName
    {
        extern const std::string modifications;
        extern const std::string modification;
        extern const std::string modification_nameAttr;
        extern const std::string weightDelta;
        extern const std::string weightDelta_daltonsAttr;
        
        extern const std::string mols;
        extern const std::string modMol;
        extern const std::string modMol_nameAttr;
        extern const std::string weight;
        extern const std::string weight_daltonsAttr;
        extern const std::string bindingSite;
        extern const std::string bindingSite_nameAttr;
        extern const std::string defaultShapeRef;
        extern const std::string defaultShapeRef_nameAttr;
        extern const std::string siteShape;
        extern const std::string siteShape_nameAttr;
        extern const std::string modSite;
        extern const std::string modSite_nameAttr;
        extern const std::string defaultModRef;
        extern const std::string defaultModRef_nameAttr;
        
        extern const std::string smallMol;
        extern const std::string smallMol_nameAttr;
        
        extern const std::string allostericState;
        extern const std::string modMap;
        extern const std::string modSiteRef;
        extern const std::string modSiteRef_nameAttr;
        extern const std::string modRef;
        extern const std::string modRef_nameAttr;
        extern const std::string siteShapeMap;
        extern const std::string bindingSiteRef;
        extern const std::string bindingSiteRef_nameAttr;
        extern const std::string siteShapeRef;
        extern const std::string siteShapeRef_nameAttr;
    }
}

#endif

