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

#ifndef FND_DUMPSTREAM_H
#define FND_DUMPSTREAM_H

#include <fstream>
#include <vector>
#include "utl/xcpt.hh"
#include "utl/autoVector.hh"
#include "fnd/dumpable.hh"
#include "fnd/dmpColumn.hh"

namespace fnd
{
    class dumpStream :
        public utl::autoVector<basicDmpColumn>
    {
        // Now that the output stream and everything else is passed to
        // the dumpables through a dumpArg, these are here mainly to
        // memory-manage the stream, to emphasize that this class is what
        // corresponds to a .dmp file, and to facilitate the construction
        // of dumpArg's through getOstream(), in particular.
        
        std::string fileName;
        std::ostream* pOs;
        std::ofstream* pFileStream;
        
    public:
        // Use a genuine file path, "-" for std::cout, "+" for std::cerr.
        dumpStream( const std::string& rFileName )
            throw( utl::xcpt );
        
        ~dumpStream( void )
        {
            // This is null if the output stream not actually a file.
            delete pFileStream;
        }
        
        std::ostream&
        getOstream( void ) const
        {
            return *pOs;
        }
        
        // Returns the file name given at construction time, including
        // the special cases.
        const std::string&
        getFileName( void ) const
        {
            return fileName;
        }
        
        // Initializes output stream and writes column headers.
        void
        init( void )
            throw( utl::xcpt );
        
        // Writes a line in the output file.
        void
        doDump( void );
    };
}

#endif // FND_DUMPSTREAM_H
