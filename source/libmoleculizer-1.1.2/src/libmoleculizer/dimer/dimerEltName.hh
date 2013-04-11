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

#ifndef DIMERELTNAME_H
#define DIMERELTNAME_H

#include <string>

namespace dimer
{
    namespace eltName
    {
        extern const std::string dimerizationGen;
        extern const std::string dimerizationGen_rateExtrapAttr;
        extern const std::string dimerizationGen_rateExtrap_none;
        extern const std::string dimerizationGen_rateExtrap_mass;
        extern const std::string siteRef;
        extern const std::string siteRef_nameAttr;
        extern const std::string defaultOnRate;
        extern const std::string defaultOnRate_valueAttr;
        extern const std::string defaultOffRate;
        extern const std::string defaultOffRate_valueAttr;
        extern const std::string alloRates;
        extern const std::string onRate;
        extern const std::string onRate_valueAttr;
        extern const std::string offRate;
        extern const std::string offRate_valueAttr;
    }
}

#endif
