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

#include "utl/utility.hh"
#include "mzr/mzrEltName.hh"
#include "mol/molEltName.hh"
#include "stoch/stochEltName.hh"
#include "stoch/stochSpecies.hh"
#include <libxml++/libxml++.h>

namespace stoch
{
    xmlpp::Element*
    stochSpecies::insertElt( xmlpp::Element* pTaggedSpeciesElt,
                             double molarFactor ) const
        throw( std::exception )
    {
        xmlpp::Element* pTaggedStochSpeciesElt
            = pTaggedSpeciesElt->add_child( eltName::taggedStochSpecies );
        
        pTaggedStochSpeciesElt->set_attribute( eltName::taggedStochSpecies_tagAttr,
                                               getTag() );
        
        pTaggedStochSpeciesElt->set_attribute( eltName::taggedStochSpecies_nameAttr,
                                               getName() );
        
        xmlpp::Element* pWeightElt
            = pTaggedStochSpeciesElt->add_child( eltName::weight );
        
        pWeightElt->set_attribute( eltName::weight_daltonsAttr,
                                   utl::stringify<double> ( getWeight() ) );
        
        //         xmlpp::Element* pPopulationElt
        //         = pTaggedStochSpeciesElt->add_child (eltName::population);
        
        //         pPopulationElt->set_attribute (eltName::population_countAttr,
        //                                        utl::stringify<int> (getPop() ) );
        
        // Adding redundant concentration element for use by ODE solver.
        // An alternative would be to convert population to concentration
        // (using Java?) during translation of state dump fro ODE solver.
        //         double concentration
        //         = getPop() / molarFactor;
        
        //         xmlpp::Element* pConcentrationElt
        //         = pTaggedStochSpeciesElt->add_child (eltName::concentration);
        
        //         pConcentrationElt->set_attribute (eltName::concentration_valueAttr,
        //                                           utl::stringify<double> (concentration) );
        
        return pTaggedStochSpeciesElt;
    }
}
