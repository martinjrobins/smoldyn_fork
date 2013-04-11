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

#include "utl/utility.hh"
#include "utl/frexp10.hh"
#include "mzr/dumpUtils.hh"
#include "mzr/mzrEltName.hh"

#include <libxml++/libxml++.h>

namespace mzr
{
    void
    addDoubleParamChild( xmlpp::Node* pParentNode,
                         const std::string& rChildName,
                         const std::string& rParameterName,
                         double parameterValue )
    {
        xmlpp::Element* pChildElt
            = pParentNode->add_child( rChildName );
        
        pChildElt->set_attribute( rParameterName,
                                  utl::stringify<double> ( parameterValue ) );
        
        xmlpp::Element* pSciNoteElt
            = pChildElt->add_child( eltName::sciNote );
        
        // Generate scientific notation for use in generating SBML etc.
        int exponent = 0;
        double fraction = utl::frexp10( parameterValue,
                                        exponent );
        
        pSciNoteElt->set_attribute( eltName::sciNote_fractionAttr,
                                    utl::stringify<double> ( fraction ) );
        
        pSciNoteElt->set_attribute( eltName::sciNote_exponentAttr,
                                    utl::stringify<int> ( exponent ) );
    }
}
