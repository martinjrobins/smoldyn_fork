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

#include "plex/mzrOmniPlex.hh"
#include "plex/mzrPlexFamily.hh"
#include "nmr/nmrUnit.hh"
#include "plex/mzrPlex.hh"
#include "plex/mzrPlexSpecies.hh"
#include "nmr/nmrUnit.hh"
#include "mzr/mzrSpeciesDumpable.hh"

namespace plx
{
    mzrPlexFamily::
    mzrPlexFamily( const mzrPlex& rParadigm,
                   cpx::knownBindings<bnd::mzrMol, fnd::feature<cpx::cxBinding<mzrPlexSpecies, mzrPlexFamily> > >& refKnownBindings,
                   std::set<mzrPlexFamily*>& refOmniplexFamilies,
                   nmr::nmrUnit& refNmrUnit ) :
        cpx::plexFamily<bnd::mzrMol,
                        mzrPlex,
                        mzrPlexSpecies,
                        mzrPlexFamily,
                        mzrOmniPlex> ( rParadigm,
                                       refKnownBindings,
                                       refOmniplexFamilies ),
        rNmrUnit( refNmrUnit )
    {}
    
    mzrPlexSpecies*
    mzrPlexFamily::
    constructSpecies( const cpx::siteToShapeMap& rSiteParams,
                      const std::vector<cpx::molParam>& rMolParams )
    {
        return new mzrPlexSpecies( *this,
                                   rSiteParams,
                                   rMolParams );
    }
    
    class insertMzrPlexSpecies :
        public std::unary_function<mzrPlexFamily::value_type, void>
    {
        xmlpp::Element* pExplicitSpeciesElt;
        double molFact;
        
    public:
        insertMzrPlexSpecies( xmlpp::Element* pExplicitSpeciesElement,
                              double molarFactor ) :
            pExplicitSpeciesElt( pExplicitSpeciesElement ),
            molFact( molarFactor )
        {}
        
        void
        operator()( const argument_type& rPlexFamilyEntry ) const
            throw( std::exception )
        {
            mzrPlexSpecies* pSpecies = rPlexFamilyEntry.second;
            pSpecies->insertElt( pExplicitSpeciesElt,
                                 molFact );
        }
    };
    
    void
    mzrPlexFamily::insertSpecies( xmlpp::Element* pExplicitSpeciesElt,
                                  double molarFactor ) const
        throw( std::exception )
    {
        std::for_each( begin(),
                       end(),
                       insertMzrPlexSpecies( pExplicitSpeciesElt,
                                             molarFactor ) );
    }
    
    const nmr::NameAssembler*
    mzrPlexFamily::getNamingStrategy() const
    {
        return rNmrUnit.getNameEncoder();
    }
    
}
