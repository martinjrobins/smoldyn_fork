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

#ifndef MZRMODMOL_H
#define MZRMODMOL_H

#include "utl/xcpt.hh"
#include "cpx/modMol.hh"
#include "mol/mzrMol.hh"

namespace bnd
{
    class mzrModMol :
        public cpx::modMol<mzrMol>
    {
    public:
        mzrModMol( const std::string& rName,
                   const std::vector<mzrBndSite>& rSites,
                   double molecularWeight,
                   const std::map<std::string, const cpx::modification*>& rDefaultModMap );
        
        // Parse time lookup of a modification site by name.  If no such
        // modification,, an exception is thrown.  If a requesting node is given,
        // an Xpath to it is included in the exception message.
        int
        mustGetModSiteNdx( const std::string& rModSiteName,
                           xmlpp::Node* pRequestingNode = 0 ) const
            throw( utl::xcpt );
        
        // Fulfills abstract virtual function in cpx::modMol for which the mol
        // must give a type (here "mod-mol") and the instance index.  This is
        // used to serialize automatically-generated plexSpecies.
        std::string
        genInstanceName( int molInstanceNdx ) const;
        
        std::string
        getInformativeModificationName() const;
        
        // Insert the state of a mod-mol for the serialization of a plex
        // species.  This should be a virtual member funtion of stateMol,
        // I suspect, but I've been avoiding putting (de)serialization into
        // those templates.
        xmlpp::Element*
        insertInstanceState( xmlpp::Element* pInstanceStatesElt,
                             int molInstanceNdx,
                             cpx::molParam param ) const;
        
        // Insert element into state output.
        xmlpp::Element*
        insertElt( xmlpp::Element* pMolsElt ) const
        throw( utl::xcpt );
    };
}

#endif // MZRMODMOL_H
