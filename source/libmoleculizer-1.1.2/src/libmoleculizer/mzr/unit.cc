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

#include "unit.hh"
#include <libxml++/libxml++.h>

namespace mzr
{

    bool
    inputCapabilities::handlesModelContentElt( xmlpp::Element* pElt ) const
    {
	return ( modelContentNames.end()
		 != modelContentNames.find( pElt->get_name() ) );
    }

    bool
    inputCapabilities::handlesReactionGensContent( xmlpp::Element* pElt ) const
    {
	return ( reactionGenNames.end()
		 != reactionGenNames.find( pElt->get_name() ) );
    }


    bool
    inputCapabilities::handlesExplictSpeciesContent( xmlpp::Element* pElt ) const
    {
	return ( explicitSpeciesContentNames.end()
		 != explicitSpeciesContentNames.find( pElt->get_name() ) );
    }
        
    // To test if this unit handles a particular speciesStreams content element.
    bool
    inputCapabilities::handlesSpeciesStreamsContent( xmlpp::Element* pElt ) const
    {
	return ( speciesStreamsContentNames.end()
		 != speciesStreamsContentNames.find( pElt->get_name() ) );
    }
        
    // To test if this unit handles a particular events content element.
    bool
    inputCapabilities::handlesEventsContent( xmlpp::Element* pElt ) const
    {
	return ( eventsContentNames.end()
		 != eventsContentNames.find( pElt->get_name() ) );
    }


}
