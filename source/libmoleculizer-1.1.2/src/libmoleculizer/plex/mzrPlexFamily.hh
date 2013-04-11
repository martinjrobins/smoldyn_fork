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

#ifndef PLEXFAMILY_H
#define PLEXFAMILY_H

/*! \defgroup plexStructGroup Structure
  \ingroup plexGroup
  \brief Structural equivalence, structural families of complexes. */

/*! \file mzrPlexFamily.hh
  \ingroup plexStructGroup
  \brief Defines plexFamily, a structural family of species of complexes. */

#include "utl/defs.hh"
#include "cpx/plexFamily.hh"
#include "plex/mzrPlex.hh"
#include "plex/mzrPlexSpecies.hh"

namespace nmr
{
    DECLARE_CLASS( nmrUnit );
    DECLARE_CLASS( nameAssembler );
}

namespace bnd
{
    DECLARE_CLASS( mzrMol );
}

namespace plx
{
    DECLARE_CLASS( mzrOmniPlex );
    DECLARE_CLASS( mzrPlex );
    DECLARE_CLASS( mzrPlexFamily );
    DECLARE_CLASS( mzrOmniPlex );
    
    /*! \ingroup plexStructGroup
      
      \brief A structural family of species of complexes.
      
      The parameter that is used to classify complexes is the vector
      of molParams of the complex.  All other properties of the
      complex are computed from these using the allostery
      function of the structural family. */
    class mzrPlexFamily
        : public cpx::plexFamily<bnd::mzrMol,
                                 mzrPlex,
                                 mzrPlexSpecies,
                                 mzrPlexFamily,
                                 mzrOmniPlex>
    {
        nmr::nmrUnit& rNmrUnit;
        
    public:
        // The arguments other than the paradigm plex are passed on to the base
        // class constructor.  The knownBindings and the set of all omniPlexes
        // are maintained by the plexUnit.
        mzrPlexFamily( const mzrPlex& rParadigm,
                       cpx::knownBindings<bnd::mzrMol, fnd::feature<cpx::cxBinding<mzrPlexSpecies, mzrPlexFamily> > >& refKnownBindings,
                       std::set<mzrPlexFamily*>& refOmniplexFamilies,
                       nmr::nmrUnit& refNmrUnit );
        
        // Fulfills plexFamily::constructSpecies pure virtual function.
        // This exists so that mzrPlexSpecies can have a reference to
        // the precise mzrPlexFamily class.
        mzrPlexSpecies*
        constructSpecies( const cpx::siteToShapeMap& rSiteParams,
                          const std::vector<cpx::molParam>& rMolParams );
        
        const nmr::NameAssembler*
        getNamingStrategy() const;
        
        
        // Output routine.
        void
        insertSpecies( xmlpp::Element* pExplicitSpeciesElt,
                       double molarFactor ) const
            throw( std::exception );
    };
}

#endif

