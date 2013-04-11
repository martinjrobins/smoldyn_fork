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

#ifndef DUMPUTILS_H
#define DUMPUTILS_H

#include "utl/dom.hh"

namespace mzr
{
    // This may want to go into utl; I've never really "utilitized" output.
    
    // Inserts a stereotyped double-valued parameter element, with additional
    // elements giving the parameter value in scientific notation
    //
    // This is basically part of a fix to introduce scientific notation for all
    // parameter values so as to be able, using XSLT, to generate SBML and other
    // formats that have the fraction and exponent in separate XML constructs.
    void
    addDoubleParamChild( xmlpp::Node* pParentNode,
                         const std::string& rChildName,
                         const std::string& rParameterName,
                         double parameterValue );
}

#endif // DUMPUTILS_H
