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

#ifndef OMNIPLEXFEATURE_H
#define OMNIPLEXFEATURE_H

/*! \file omniPlexFeature.hh
  \ingroup omniGroup
  \brief Defines subcomplex feature. */

#include "fnd/feature.hh"
#include "fnd/multiSpeciesDumpable.hh"
#include "fnd/newContextStimulus.hh"
#include "cpx/cxOmni.hh"

namespace cpx
{
    template<class molT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    class omniPlexFeature :
        public fnd::feature<cxOmni<molT,
                                   plexSpeciesT,
                                   plexFamilyT,
                                   omniPlexT> >
    {
    public:
        typedef plexSpeciesT plexSpeciesType;
        typedef plexFamilyT plexFamilyType;
        typedef omniPlexT omniPlexType;
        
        typedef typename plexSpeciesT::msDumpableType dumpableType;
        
        typedef andPlexQueries<plexSpeciesType,
                               omniPlexType> stateQueryType;
        
        typedef
        typename cpx::cxOmni<molT, plexSpeciesT, plexFamilyT, omniPlexT>
        contextType;
        
        typedef fnd::newContextStimulus<contextType> stimulusType;
        
    private:
        // If a dumpable is really attached to this feature, then
        // this points to it; otherwise null.
        dumpableType* pDumpable;
        
    public:
        omniPlexFeature( void ) :
            pDumpable(0)
        {}
        
        // Generate reactions
        //
        // Overrides fnd::feature<cpx::cxOmni>::respond to add new species
        // to possible dumpable.
        virtual
        void
        respond( const typename omniPlexFeature::stimulusType& rNewFeatureContext );


        virtual 
        void 
        dumpablesRespond( const typename omniPlexFeature::stimulusType& rStim );
        
        // To "turn on" dumping of the species in this omniplex.
        // These aren't query-based dumpables, since the omniplex
        // itself does all the querying.
        void
        setDumpable(typename omniPlexFeature::dumpableType* ptrDumpable)
        {
            pDumpable = ptrDumpable;
        }
    };
    
    template<class molT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    omniPlexFeature<molT,
                    plexSpeciesT,
                    plexFamilyT,
                    omniPlexT>::
    respond( const typename omniPlexFeature::stimulusType& rStim )
    {
        const typename omniPlexFeature::contextType& rNewContext
            = rStim.getContext();
        
        // Does the new species satisfy the omni's state query?
        omniPlexType* pOmni = rNewContext.getOmni();
        const typename omniPlexFeature::stateQueryType& rQuery
            = * ( pOmni->getStateQuery() );
        
        plexSpeciesType* pSpecies
            = rNewContext.getSpecies();
        const subPlexSpec<omniPlexType>& rSpec
            = rNewContext.getSpec();
        if ( rQuery.applyTracked( *pSpecies,
                                  rSpec ) )
        {
            // Notify reaction generators
            fnd::feature<contextType>::respond( rStim );
            
//             // Notify dumpable if any.
//             if(pDumpable)
//             {
//                 // Note that this just gets back the newSpeciesStimulus.
//                 // This really stinks.
//                 fnd::newSpeciesStimulus<plexSpeciesType>
//                     dumpStim(pSpecies,
//                              rStim.getNotificationDepth());
                
//                 pDumpable->respond(dumpStim);
//             }
        }
    }

    template<class molT,
             class plexSpeciesT,
             class plexFamilyT,
             class omniPlexT>
    void
    omniPlexFeature<molT,
                    plexSpeciesT,
                    plexFamilyT,
                    omniPlexT>::
    dumpablesRespond( const typename omniPlexFeature::stimulusType& rStim )
    {

        // Notify dumpable if any.
        if(pDumpable)
        {
            const typename omniPlexFeature::contextType& rNewContext
                = rStim.getContext();
        
            // Does the new species satisfy the omni's state query?
            omniPlexType* pOmni = rNewContext.getOmni();
            const typename omniPlexFeature::stateQueryType& rQuery
                = * ( pOmni->getStateQuery() );
        
            plexSpeciesType* pSpecies
                = rNewContext.getSpecies();
            const subPlexSpec<omniPlexType>& rSpec
                = rNewContext.getSpec();
            if ( rQuery.applyTracked( *pSpecies,
                                      rSpec ) )
            {
//             Notify reaction generators
//            fnd::feature<contextType>::respond( rStim );
            

                // Note that this just gets back the newSpeciesStimulus.
                // This really stinks.
                fnd::newSpeciesStimulus<plexSpeciesType>
                    dumpStim(pSpecies,
                             rStim.getNotificationDepth());

                pDumpable->respond(dumpStim);
            }
        }




    }
}

#endif
