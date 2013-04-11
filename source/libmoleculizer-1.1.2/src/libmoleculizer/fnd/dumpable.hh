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

#ifndef FND_DUMPABLE_H
#define FND_DUMPABLE_H

#include <string>
#include "fnd/basicDumpable.hh"

namespace fnd
{
    // This class represents one column in a tab-separated columnar
    // output file.  The file itself is represented by the dumpStream
    // class.
    template<class dumpArgT>
    class dumpable :
        public basicDumpable
    {
    public:
        dumpable( const std::string& rName ) :
            basicDumpable( rName )
        {}
        
        virtual void
        doDump( const dumpArgT& rDumpArg ) const = 0;
        
        virtual void
        dumpHeader( const dumpArgT& rDumpArg ) const
        {
            rDumpArg.getOstream() << getName();
        }
    };
}

#endif // FND_DUMPABLE_H
