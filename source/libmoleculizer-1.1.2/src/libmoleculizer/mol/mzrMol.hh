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

#ifndef MOL_MZRMOL_H
#define MOL_MZRMOL_H

#include "fnd/feature.hh"
#include "mol/mzrBndSite.hh"
#include "cpx/basicMol.hh"
#include "cpx/molState.hh"
#include "cpx/cxMol.hh"
#include "plex/mzrPlexSpecies.hh"

namespace bnd
{
    // Base of mol types used in plain Moleculizer.
    class mzrMol :
        public cpx::basicMol<bnd::mzrBndSite>,
        public fnd::feature<cpx::cxMol<plx::mzrPlexSpecies, plx::mzrPlexFamily> >
    {
    public:
        virtual
        ~mzrMol( void )
        {}
        
        mzrMol( const std::string& rName,
                const std::vector<mzrBndSite>& rSites );
        
        // Returns site index corresponding to site name, or throws
        // an execption if there is no site with the given name.
        int
        mustFindSite( const std::string& rSiteName,
                      xmlpp::Node* pRequestingNode = 0 ) const
            throw( utl::xcpt );
        
        // Returns site with given name, or throws an exception if there is no
        // site with the given name.
        mzrBndSite*
        mustGetSite( const std::string& rSiteName,
                     xmlpp::Node* pRequestingNode = 0 )
        throw( utl::xcpt );
        
        virtual
        std::string
        genInstanceName( int molInstanceNdx ) const;
        
        // It's bad that this virtual function does nothing, but this class has to
        // be constructible, so it can't be pure virtual.  This is due to the
        // weird template arrangement where stateMol inherits from a base class
        // given as a template argument.  The base class has to be constructible,
        // since it must (?) be accepted as an argument to the constructor for
        // stateMol.
        virtual
        xmlpp::Element*
        insertElt( xmlpp::Element* pMolsElt ) const
            throw( std::exception )
        {
            return 0;
        }
        
        // Mol classes that have state insert their state in the serialization of
        // a plex species with this virtual function.  modMol does something here,
        // smallMol does not.
        //
        // Returns pointer to inserted element or null if no element inserted.
        //
        // I suspect this actually throws std::exception from xmlpp.
        virtual
        xmlpp::Element*
        insertInstanceState( xmlpp::Element* pInstanceStatesElt,
                             int molInstanceNdx,
                             cpx::molParam param ) const
        {
            return 0;
        }
    };
}

#endif // MOL_MZRMOL_H
