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

#include "mzr/mzrSpeciesDumpable.hh"
#include "plex/mzrRecognizer.hh"
#include "plex/mzrPlexFamily.hh"
#include "plex/plexUnit.hh"

namespace plx
{
    mzrPlexFamily*
    mzrRecognizer::
    makePlexFamily( const mzrPlex& rPlex ) const
    {
        return new mzrPlexFamily( rPlex,
                                  rPlexUnit.bindingFeatures,
                                  rPlexUnit.omniPlexFamilies,
                                  rNmrUnit );
    }
    
    class insertFamilySpecies :
        public std::unary_function<std::map<int, mzrPlexFamily*>::value_type, void>
    {
        xmlpp::Element* pExplicitSpeciesElt;
        double molFact;
    public:
        insertFamilySpecies( xmlpp::Element* pExplicitSpeciesElement,
                             double molarFactor ) :
            pExplicitSpeciesElt( pExplicitSpeciesElement ),
            molFact( molarFactor )
        {}
        
        void
        operator()( const argument_type& rHasherEntry ) const
            throw( std::exception )
        {
            const mzrPlexFamily* pFamily = rHasherEntry.second;
            pFamily->insertSpecies( pExplicitSpeciesElt,
                                    molFact );
        }
    };
    
    void
    mzrRecognizer::
    insertSpecies( xmlpp::Element* pExplicitSpeciesElt,
                   double molarFactor ) const
        throw( std::exception )
    {
        std::for_each( plexHasher.begin(),
                       plexHasher.end(),
                       insertFamilySpecies( pExplicitSpeciesElt,
                                            molarFactor ) );
    }
}
