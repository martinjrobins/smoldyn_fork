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

#ifndef PARSERPLEX_H
#define PARSERPLEX_H

#include "plex/mzrPlex.hh"
#include "plex/unkMolInstXcpt.hh"

namespace plx
{
    /*! \ingroup plexSpeciesGroup
      \brief Complex embellished with instance names for the mols.
      
      This is for use in the parser: you need to be able to
      refer to the constituent mols in some easier way than, say,
      indices. */
    class parserPlex :
        public mzrPlex
    {
    public:
        // For incremental construction.
        parserPlex( void )
        {}
        
        // For constructing a parser plex (which has instance names) from
        // a plex and a naming of the instances.
        parserPlex( const std::map<std::string, int>& rNameToMolNdx,
                    const mzrPlex& rOriginal ) :
            mzrPlex( rOriginal ),
            nameToMolNdx( rNameToMolNdx )
        {}
        
        // Mapping of instance names to mol index.
        std::map<std::string, int> nameToMolNdx;
        
        // Appends a mol with the given instance name, returning
        // the new instance's index.
        int
        addMolByName( const std::string& rName,
                      bnd::mzrMol* pMol );
        
        // Returns -1 if the name isn't a mol instance name.
        int
        getMolNdxByName( const std::string& rName ) const;
        
        int
        mustGetMolNdxByName( xmlpp::Node* pRequestingNode,
                             const std::string& rInstanceName ) const
            throw( plx::unkMolInstXcpt );
        
        // Returns 0 if the name isn't a mol instance name.
        bnd::mzrMol*
        getMolByName( const std::string& rName ) const;
        
        bnd::mzrMol*
        mustGetMolByName( xmlpp::Node* pRequstingNode,
                          const std::string& rInstanceName ) const
        throw( plx::unkMolInstXcpt );
    };
}

#endif // PARSERPLEX_H
