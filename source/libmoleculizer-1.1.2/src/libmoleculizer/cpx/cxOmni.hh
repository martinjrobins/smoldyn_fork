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

#ifndef CXOMNIPARAM_H
#define CXOMNIPARAM_H

#include "fnd/featureContext.hh"
#include "cpx/subPlexSpec.hh"
#include "cpx/siteToShapeMap.hh"
#include "cpx/molState.hh"

namespace cpx
{
    // This class describes a subcomplex of a plexSpecies by giving an imbedding
    // of the subcomplex into the plexSpecies's complex, and "dressing" it with
    // accessors of the underlying species, etc.
    template<class molT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class cxOmni :
        public fnd::featureContext<plexSpeciesT,
                                   subPlexSpec<omniPlexT> >
    {
    public:
        typedef molT molType;
        typedef plexSpeciesT plexSpeciesType;
        typedef plexFamilyT plexFamilyType;
        typedef omniPlexT omniPlexType;
        
        typedef subPlexSpec<omniPlexType> specType;
        
        cxOmni( plexSpeciesType* pPlexSpecies,
                const specType& rSpec );
        
        // Retrieves the embedded omniPlex.
        omniPlexType*
        getOmni( void ) const;
        
        // Retrieves the embedding of the omniplex into the complex where
        // it was recognized.
        const plexIso&
        getEmbedding( void ) const;
        
        // Uses the embedding to translate mol indices in the omniplex
        // to the corresponding mol indices in the complex where the
        // omniplex was recognized.
        molSpec
        translateMolSpec( molSpec specInOmni ) const;
        
        // Uses the embedding to translate binding indices in the omniplex
        // to the corresponding binding indices in the complex where the
        // omniplex was recognized.
        bindingSpec
        translateBindingSpec( bindingSpec specInOmni ) const;
        
        // Uses the embedding to translate a site spec in the omniplex into
        // a site spec in the complex where the omniplex was recognized.
        siteSpec
        translateSiteSpec( siteSpec specInOmni ) const;
        
        // Get the plex family in which the omniplex was recognized.
        plexFamilyType&
        getPlexFamily( void ) const;
        
        // Returns the population of the particular species of complex where
        // the omni was found.
        int
        getPop( void ) const;
        
        // Used in many propensity calculations.
        double
        getPlexWeight( void ) const;
        
        // Extracts the site shapes from the plexSpecies.
        const siteToShapeMap&
        getSiteToShapeMap( void ) const;
        
        // Returns just the molParams of the particular complex where the
        // omni occurs.  These are usually used to "cook up" the vector of
        // molParams of a reaction product.  The remaining parameters of the
        // reaction product are usually created by the product complex's
        // allostery function.
        const std::vector<molParam>&
        getMolParams( void ) const;
        
        // Note that this does NOT translate the molSpec.
        molParam
        getMolParam( molSpec theMolSpec ) const;
        
        // Get the vector of mols of the plex.  Note that the indexing in
        // this vector is that given by the plex, not by the omniplex.  Use
        // translateMolNdx above to overcome this.
        //
        // Not building the translation routines into any of these, since
        // it's frequently a case of translate once, use many times.
        const std::vector<molType*>&
        getMols( void ) const;
    };
}

#include "cpx/cxOmniImpl.hh"

#endif
