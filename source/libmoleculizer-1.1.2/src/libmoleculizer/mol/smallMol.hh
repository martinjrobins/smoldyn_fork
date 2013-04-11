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

#ifndef SMALLMOL_H
#define SMALLMOL_H

#include "utl/dom.hh"
#include "mol/mzrMol.hh"
#include "cpx/smallMol.hh"

namespace bnd
{
    class smallMol :
        public cpx::smallMol<bnd::mzrMol>
    {
        // Constructs a vector just containing one binding site with
        // one site shape, both with the given name.
        static std::vector<mzrBndSite>
        makeBindingSites( const std::string& rMolName );
        
    public:
        smallMol( const std::string& rName,
                  double molecularWeight ) :
            cpx::smallMol<bnd::mzrMol> ( mzrMol( rName,
                                                 makeBindingSites( rName ) ),
                                         molecularWeight )
        {}
        
        virtual
        std::string
        genInstanceName( int molInstanceNdx ) const;
        
        xmlpp::Element*
        insertElt( xmlpp::Element* ) const
            throw( std::exception );
    };
    
}

#endif // SMALLMOL_H
