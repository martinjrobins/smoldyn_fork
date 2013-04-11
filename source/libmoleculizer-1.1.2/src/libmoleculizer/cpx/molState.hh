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

#ifndef MOLSTATE_H
#define MOLSTATE_H

namespace cpx
{
    /*! \ingroup plexSpeciesGroup
      \brief (Concrete) base class for state of molecules. */
    class molState
    {
    protected:
        // Other contributions from molecular state may go into the true
        // molecular weight; hence the name.
        double baseWeight;
        
    public:
        molState( double molWeight ) :
            baseWeight( molWeight )
        {}
        
        virtual ~molState()
        {}
        
        virtual double
        getMolWeight( void ) const
        {
            return baseWeight;
        }
        
        bool
        operator< ( const molState& rRight ) const
        {
            return baseWeight < rRight.baseWeight;
        }
        
    };
    
    // Now not sure if this was such a good idea...
    typedef const molState* molParam;
}

#endif // MOLSTATE_H
