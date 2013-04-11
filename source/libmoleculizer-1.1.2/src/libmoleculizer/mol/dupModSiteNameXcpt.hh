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

#ifndef MOL_DUPMODSITENAMEXCPT_H
#define MOL_DUPMODSITENAMEXCPT_H

#include "utl/xcpt.hh"

namespace bnd
{
    // Excpetion thrown when the same name is given to more than
    // one modification site on a mzrModMol.
    class dupModSiteNameXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rBadModSiteName );
        
    public:
        dupModSiteNameXcpt( const std::string& rBadModSiteName ) :
            utl::xcpt( mkMsg( rBadModSiteName ) )
        {}
    };
}

#endif // MOL_DUPMODSITENAMEXCPT_H
