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

#ifndef BASICSPECIES_H
#define BASICSPECIES_H

#include <sstream>
#include "fnd/physConst.hh"
#include "utl/xcpt.hh"
#include "utl/utility.hh"
#include "fnd/notifier.hh"

namespace fnd
{
    template<class reactionType>
    class basicSpecies :
        public onceNotifier
    {
        static int speciesCount;

    public:
        
        basicSpecies()
        {
            speciesCount++;
        }
        
        virtual
        ~basicSpecies( void )
        {}
        
        static int
        getSpeciesCount( void )
        {
            return speciesCount;
        }
        
        // Gives this address as a hex string.
        typename std::string
        getTag( void ) const
        {
            return utl::stringify<const basicSpecies*> ( this );
        }
        
        // For possibly getting a more humanly-readable, informative name.
        virtual typename std::string
        getName( void ) const
        {
            return getTag();
        }

        virtual typename std::string
        getTaggedName( void ) const
        {
            return getTag();
        }




        
    };
    
    template<class reactionType>
    int basicSpecies<reactionType>::speciesCount = 0;
}

#endif // BASICSPECIES_H
