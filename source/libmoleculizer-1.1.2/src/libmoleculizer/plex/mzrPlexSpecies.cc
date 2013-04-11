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

#include "utl/dom.hh"
#include "utl/utility.hh"
#include "nmr/complexSpeciesOutputMinimizer.hh"
#include "plex/mzrPlexSpecies.hh"
#include "plex/mzrPlexFamily.hh"
#include "plex/plexEltName.hh"
#include "mzr/mzrSpeciesDumpable.hh"
#include <libxml++/libxml++.h>

namespace plx
{
    double
    mzrPlexSpecies::
    getWeight( void ) const
    {
        return cpx::plexSpeciesMixin<mzrPlexFamily>::getWeight();
    }
    
    void
    mzrPlexSpecies::
    notify( int generateDepth )
    {
        rFamily.respond( fnd::newSpeciesStimulus<mzrPlexSpecies> ( this,
                                                                   generateDepth ) );
    }

    void 
    mzrPlexSpecies::
    inform()
    {
        rFamily.dumpablesRespond( fnd::newSpeciesStimulus<mzrPlexSpecies> ( this,
                                                                            0 ) );
    }
    
    std::string
    mzrPlexSpecies::
    getName( void ) const
    {

        if ( nameGenerated )
        {
            return name;
        }
        else
        {

#ifdef TMP_DEBUGGING
	std::cout << "Generating name for " << getInformativeName() << std::endl;
#endif
        if ( nameGenerated )
            nameGenerated = true;
            const nmr::NameAssembler* pNameAssembler = rFamily.getNamingStrategy();
            name = getCanonicalName( pNameAssembler );

#ifdef TMP_DEBUGGING
	    std::cout << "Done!" << std::endl;
#endif
            return name;
        }
    }
    
    xmlpp::Element*
    mzrPlexSpecies::
    insertElt( xmlpp::Element* pExplicitSpeciesElt,
               double molarFactor ) const
        throw( std::exception )
    {
        // Insert tagged-plex-species element.
        xmlpp::Element* pTaggedPlexSpeciesElt
            = pExplicitSpeciesElt->add_child( eltName::taggedPlexSpecies );
        
        pTaggedPlexSpeciesElt->set_attribute( eltName::taggedPlexSpecies_tagAttr,
                                              getTag() );
        
        pTaggedPlexSpeciesElt->set_attribute( eltName::taggedPlexSpecies_nameAttr,
                                              getName() );
        
        // Insert the paradigm plex.
        rFamily.getParadigm().insertElt( pTaggedPlexSpeciesElt );
        
        // Insert the non-default instance states.
        //
        // Might be easier and non-harmful to insert all instance states.
        xmlpp::Element* pInstanceStatesElt
            = pTaggedPlexSpeciesElt->add_child( eltName::instanceStates );
        
        // I need molNdx to generate a pseudo instance name.
        for ( int molNdx = 0;
              molNdx < ( int ) molParams.size();
              ++molNdx )
        {
            bnd::mzrMol* pMol = rFamily.getParadigm().mols[molNdx];
            
            pMol->insertInstanceState( pInstanceStatesElt,
                                       molNdx,
                                       molParams[molNdx] );
        }
        
        //         xmlpp::Element* pPopulationElt
        //         = pTaggedPlexSpeciesElt->add_child (eltName::population);
        
        //         pPopulationElt->set_attribute (eltName::population_countAttr,
        //                                        utl::stringify<int> (getPop() ) );
        
        // Adding redundant concentration element for use by ODE solver.  An
        // alternative would be to convert population to concentration (using
        // Java?)  during translation of state dump for ODE solver.
        //        double concentration = getPop() /molarFactor;
        
        //         xmlpp::Element* pConcentrationElt
        //         = pTaggedPlexSpeciesElt->add_child (eltName::concentration);
        //         pConcentrationElt->set_attribute (eltName::concentration_valueAttr,
        //                                           utl::stringify<double> (concentration) );
        
        // Add the updated flag for use by parametrizer.
        if ( hasNotified() )
        {
            pTaggedPlexSpeciesElt->add_child( eltName::updated );
        }
        
        return pTaggedPlexSpeciesElt;
    }

		unsigned int 
		mzrPlexSpecies::getSizeOfAutomorphismGroup() const
		{

				nmr::ComplexSpecies aComplexSpecies;
				this->createComplexRepresentation( aComplexSpecies );

				// This is basically an interface to nauty.
				nmr::ComplexSpeciesOutputMinimizer out_minimizer;
				unsigned int auto_sz = out_minimizer.getAutomorphismSize(aComplexSpecies);
				return auto_sz;
		}

}
