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

#ifndef PLEXSPECIES_H
#define PLEXSPECIES_H

/*! \file plexSpecies.hh
  \ingroup plexStructGroup
  \brief Defines plexSpecies, a species of protein complex. */

#include "utl/dom.hh"
#include "fnd/basicDumpable.hh"
#include "cpx/plexSpcsMixin.hh"
#include "mzr/mzrSpecies.hh"
#include "mzr/mzrSpeciesDumpable.hh"


namespace mzr
{
    template <typename T>
    class querySpeciesDumpable;

    template <typename T>
    class multiSpeciesDumpable;
}

namespace plx
{
    class mzrPlexFamily;
    
    class mzrPlexSpecies :
        public mzr::mzrSpecies,
        public cpx::plexSpeciesMixin<mzrPlexFamily>
    {
    private:
        
        mutable bool nameGenerated;
        mutable std::string name;
    public:
        
        typedef mzr::querySpeciesDumpable<mzrPlexSpecies> queryDumpableType;
        
        typedef mzr::multiSpeciesDumpable<mzrPlexSpecies> msDumpableType;
        
        mzrPlexSpecies( mzrPlexFamily& rContainingFamily,
                        const cpx::siteToShapeMap& rSiteParams,
                        const std::vector<cpx::molParam>& rMolParams ) :
            cpx::plexSpeciesMixin<mzrPlexFamily> ( rContainingFamily,
                                                   rSiteParams,
                                                   rMolParams )
        {
            nameGenerated = false;
            name = "";
        }
        
        ~mzrPlexSpecies( void )
        {
        }
        
        // This fulfils the pure virtual function in the ancestor class
        // fnd::massive.
        double
        getWeight( void ) const;
        
        // This fulfills the pure virtual function in the ancestor class
        // cpx::notifier.
        void
        notify( int generateDepth );

        // This is the thing that does the informing.
        virtual void
        inform();

        // This overrides basicSpecies::getName(), which just returns a tag.
        virtual std::string
        getName( void ) const;

						unsigned int 
						getSizeOfAutomorphismGroup() const;

						xmlpp::Element*
						insertElt( xmlpp::Element* pExplicitSpeciesElt,
																	double molarFactor ) const
						throw( std::exception );
    };


}

#endif
