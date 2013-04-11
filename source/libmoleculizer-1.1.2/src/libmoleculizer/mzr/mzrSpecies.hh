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

#ifndef SPECIES_H
#define SPECIES_H

#include "fnd/basicSpecies.hh"
#include "fnd/massive.hh"
#include "mzr/mzrReaction.hh"
#include "fnd/reactionNetworkComponent.hh"

namespace plx
{
    class mzrPlexSpecies;
}

namespace mzr
{
    class moleculizer;
    
    class mzrSpecies :
        public fnd::basicSpecies<mzrReaction>,
        public fnd::onceInformer,
        public fnd::massive,
        public fnd::reactionNetworkComponent
    {
    public:

	plx::mzrPlexSpecies* 
	getComplexSpecies();
        
        virtual void
        expandReactionNetwork();
        
        void expandReactionNetwork( unsigned int i );
        
        static void
        setGenerateDepth( unsigned int i );
        
        static unsigned int
        getGenerateDepth();

        virtual std::string
        getTag( void ) const
        {
            return this->getName();
        }

        virtual void inform(){}
        
        static unsigned int generateDepth;
        
        
    };
}

#endif



