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

#include "dimer/dimerEltName.hh"

namespace dimer
{
    namespace eltName
    {
        const std::string dimerizationGen( "dimerization-gen" );
        const std::string dimerizationGen_rateExtrapAttr( "rate-extrapolator" );
        const std::string dimerizationGen_rateExtrap_none( "none" );
        const std::string dimerizationGen_rateExtrap_mass( "mass" );
        const std::string siteRef( "site-ref" );
        const std::string siteRef_nameAttr( "name" );
        const std::string defaultOnRate( "default-on-rate" );
        const std::string defaultOnRate_valueAttr( "value" );
        const std::string defaultOffRate( "default-off-rate" );
        const std::string defaultOffRate_valueAttr( "value" );
        const std::string alloRates( "allo-rates" );
        const std::string onRate( "on-rate" );
        const std::string onRate_valueAttr( "value" );
        const std::string offRate( "off-rate" );
        const std::string offRate_valueAttr( "value" );
    }
}

