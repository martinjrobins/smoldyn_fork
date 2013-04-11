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

#ifndef MOL_UNKMODSITEXCPT_H
#define MOL_UNKMODSITEXCPT_H

#include "utl/xcpt.hh"
#include "utl/dom.hh"
#include "mol/mzrModMol.hh"

namespace bnd
{
    // Exception thrown when the user refers to a modification site by an
    // incorrect name.
    class unkModSiteXcpt :
        public utl::xcpt
    {
        static std::string
        mkDomMsg( xmlpp::Node* pOffendingNode,
                  const std::string& rModSiteName,
                  const mzrModMol* pModMol );
        
        static std::string
        mkPlainMsg( const std::string& rModSiteName );
        
    public:
        unkModSiteXcpt( xmlpp::Node* pOffendingNode,
                        const std::string& rModSiteName,
                        const mzrModMol* pModMol ) :
            utl::xcpt( mkDomMsg( pOffendingNode,
                                 rModSiteName,
                                 pModMol ) )
        {}
        
        unkModSiteXcpt( const std::string& rModSiteName ) :
            utl::xcpt( mkPlainMsg( rModSiteName ) )
        {}
    };
}

#endif // MOL_UNKMODSITEXCPT_H
