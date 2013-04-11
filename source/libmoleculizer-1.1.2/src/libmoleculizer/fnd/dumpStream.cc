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

#include "utl/defs.hh"
#include "fnd/fndXcpt.hh"
#include "fnd/dumpStream.hh"

namespace fnd
{
    dumpStream::
    dumpStream( const std::string& rFileName )
        throw( utl::xcpt ) :
        fileName( rFileName )
    {
        // Establish what the output stream is, opening the output file
        // if called for.  Note that this file doesn't get closed until
        // the destruction of the dumpStream.
        if ( fileName == "-" ) pOs = &std::cout;
        else if ( fileName == "+" ) pOs = &std::cerr;
        else
        {
            // Open the file for appending, so that if a simulation is continued,
            // the output will also be continued instead of starting over at the
            // second start time.
            pFileStream = new std::ofstream( fileName.c_str(),
                                             std::ios_base::app );
            if ( !( *pFileStream ) )
                throw badDumpFileXcpt( fileName );
            
            pOs = pFileStream;
        }
    }
    
    void
    dumpStream::
    init( void )
        throw( utl::xcpt )
    {
        std::ostream& rOs = getOstream();
        
        // The header line is a "comment line" for gnuplot.
        rOs << "#";
        
        iterator iEntry = begin();
        if ( iEntry != end() )
        {
            basicDmpColumn* pColumn = *iEntry;
            
            pColumn->dumpHeader();
            
            while ( ++iEntry != end() )
            {
                rOs << '\t';
                pColumn = *iEntry;
                pColumn->dumpHeader();
            }
        }
        rOs << std::endl;
    }
    
    void
    dumpStream::
    doDump( void )
    {
        std::ostream& rOs = getOstream();
        
        iterator iEntry = begin();
        if ( iEntry != end() )
        {
            basicDmpColumn* pColumn = *iEntry;
            pColumn->doDump();
            
            while ( ++iEntry != end() )
            {
                rOs << '\t';
                pColumn = *iEntry;
                pColumn->doDump();
            }
        }
        rOs << std::endl;
    }
}
