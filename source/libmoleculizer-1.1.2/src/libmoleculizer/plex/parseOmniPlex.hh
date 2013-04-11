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

#ifndef PARSEOMNIPLEX_H
#define PARSEOMNIPLEX_H

#include "utl/dom.hh"
#include "mol/molUnit.hh"
#include "plex/parserPlex.hh"
#include "plex/plexUnit.hh"

namespace plx
{
    
    // This generates the omniPlex, installs it in its plexFamily, and adds its
    // plexFamily to the plexUnit's list of all omniplex-bearing plexFamilies.
    //
    // Note that the plexFamily is found using "unify," so that if the
    // plexFamily is actually created, because seen for the first time, the
    // plexFamily is not initialized in the usual way
    // (plexFamily::connectToFeatures).  This is because connectToFeatures
    // requires all the omniPlexes to be in place.
    //
    // The way the plexUnit deals with omniPlexes is a two-step process: first,
    // all omniPlexes to be parsed and 'unified into' the database, which must
    // happen before any plexFamily connects to its features.  Second, we sweep
    // through all the omniPlex families again, connecting them to their
    // features.Subsequent "recognitions" can work in the usual way, in which
    // new plexFamilies are initialized (connected to their features)
    // immediately after being created.
    class parseOmniPlex :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        
        
    public:
        parseOmniPlex( mzr::mzrUnit& refMzrUnit,
                       bnd::molUnit& refMolUnit,
                       plexUnit& refPlexUnit )
            :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit )
        {}
        
        void
        operator()( xmlpp::Node* pParentNode ) const
            throw( utl::xcpt );
    };
    
    // For use by modules to find an omniPlex parsed for them by the plexUnit.
    //
    // This routine reparses the omniPlex elements below the given parent node,
    // generating a parserPlex.  (This allows module writers to get access to
    // the mol instances, bindings, etc. of the omniPlex for comparison
    // with other things parsed, say, to create a reaction generator.)
    //
    // After findOmni returns, the parserPlex will correspond to the plex below
    // pParentNode, with its mol instances arranged as in the recognizer
    // database; that is, as in the paradigm plex of the omniPlex's plexFamily.
    //
    // With the changes to omniplex implementation, this routine becomes
    // embarrasingly simple, but leaving it as-is for now.
    mzrOmniPlex*
    findOmni( xmlpp::Node* pParentNode,
              bnd::molUnit& rMolUnit,
              plexUnit& rPlexUnit,
              parserPlex& rParsedPlex )
        throw( utl::xcpt );
    
    // Parses allosteric-omni element.
    class parseAllostericOmni :
        public std::unary_function<xmlpp::Node*, mzrOmniPlex*>
    {
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        
    public:
        parseAllostericOmni( bnd::molUnit& refMolUnit,
                             plexUnit& refPlexUnit ) :
            rMolUnit( refMolUnit ),
            rPlexUnit( refPlexUnit )
        {}
        
        mzrOmniPlex*
        operator()( xmlpp::Node* pParentNode ) const
            throw( utl::xcpt );
    };
    
    //Parses omni-species-stream element.
    class parseOmniSpeciesStream :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        plexUnit& rPlexUnit;
        
    public:
        parseOmniSpeciesStream (mzr::mzrUnit& refMzrUnit,
                                bnd::molUnit& refMolUnit,
                                plexUnit& refPlexUnit) :
            rMzrUnit (refMzrUnit),
            rMolUnit (refMolUnit),
            rPlexUnit (refPlexUnit)
        {}
        
        void
        operator() (xmlpp::Node* pOmniSpeciesStreamNode) const
            throw (utl::xcpt);
    };
}

#endif // PARSEOMNIPLEX_H
