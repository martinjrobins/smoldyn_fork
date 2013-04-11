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

#ifndef FND_VARDUMPABLE_H
#define FND_VARDUMPABLE_H

#include "fnd/dumpable.hh"

namespace fnd
{
    // Assumes that the value of the state variable can be written
    // directly to the output stream.
    template<class stateVarT,
             class dumpArgT>
    class varDumpable :
        public dumpable<dumpArgT>
    {
        const stateVarT* pVar;
        
    public:
        varDumpable( const std::string& rName,
                     const stateVarT* pStateVariable ) :
            dumpable<dumpArgT> ( rName ),
            pVar( pStateVariable )
        {}
        
        ~varDumpable( void )
        {}
        
        const stateVarT*
        getVar( void ) const
        {
            return pVar;
        }
        
        virtual void
        doDump( const dumpArgT& rDumpArg ) const
        {
            rDumpArg.getOstream() << getVar()->getValue();
        }
    };
}

#endif // FND_VARDUMPABLE_H
