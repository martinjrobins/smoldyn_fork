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

#ifndef REACTION_H
#define REACTION_H

/*! \defgroup reactionGroup Reactions
  \ingroup mzrGroup
  \brief Reactions and their families. */

/*! \file reaction.hh
  \ingroup reactionGroup
  \brief Defines basic reaction classes and scheduling functions. */

#include "fnd/gillspReaction.hh"
#include "fnd/sensitive.hh"
#include "fnd/sensitivityList.hh"
#include "fnd/reactionNetworkComponent.hh"
#include "utl/dom.hh"
#include "mzr/mzrEvent.hh"

#include <iostream>

namespace mzr
{
    class mzrSpecies;
    class moleculizer;
    
    // This "message" is sent to a reaction when the reaction should
    // reschedule itself in the reaction queue.
    //
    // An alternative to this is to make the argument in
    // sensitive<stimulusType>::respond(const stimulusType& rStimulus)
    // non-const.  Also, rMoleculizer seems "odd" as a stimulus;
    // it's really like an auxiliary parameter of a void stimulus.
    class mzrReactionStimulus
    {
    public:
        moleculizer& rMzr;
        
        mzrReactionStimulus( moleculizer& rMoleculizer ) :
            rMzr( rMoleculizer )
        {}
    };
    
    
    class mzrReaction :
        public fnd::gillspReaction<mzrSpecies>,
        public fnd::sensitive<mzrReactionStimulus>,
        public mzrEvent,
        public fnd::reactionNetworkComponent
    {
        
        // Support for global state variables: variables to which
        // all reactions are sensitive, such as volume and temperature.
        typedef fnd::sensitivityList<mzrReaction> globalVar;
        
        class sensitizeToGlobal :
            public std::unary_function<globalVar*, void>
        {
            mzrReaction* pRxn;
        public:
            sensitizeToGlobal( mzrReaction* pReaction ) :
                pRxn( pReaction )
            {}
            void
            operator()( globalVar* pGlobal ) const
            {
                pGlobal->addSensitive( pRxn );
            }
        };
        
        // Support for "tolerance" optimization.
        double lastPropensity;
        static double lowSensitive;
        static double highSensitive;
        static unsigned int reactionDepth;
        
    public:
        static void
        setGenerateDepth( unsigned int i );
        
        static unsigned int
        getGenerateDepth()
        {
            return reactionDepth;
        }
        
        virtual void
        expandReactionNetwork();
        
        /*! \brief Keeps running count of all reaction events.
          This is a dumpable quantity. */
        static int reactionEventCount;
        
        /*! \brief The number of reactions that exist.
          This is a dumpable quantity. */
        static int reactionCount;
        
        // This constructor enforces sensitization to global state variables, for
        // now just volume in Moleculizer, but e.g. temperature might also be
        // added.
        //
        // lastPropensity is set to -1 for the first time that the reaction
        // is rescheduled.
        template<class senseListIterator>
        mzrReaction( senseListIterator beginGlobalStateVars,
                     senseListIterator endGlobalStateVars,
                     double reactionRate = 0.0 ) :
            fnd::gillspReaction<mzrSpecies> ( reactionRate ),
            lastPropensity( -1.0 )
        {
            std::for_each( beginGlobalStateVars,
                           endGlobalStateVars,
                           sensitizeToGlobal( this ) );
            
            ++reactionCount;
        }
        
        // Response of this reaction to message that one of its
        // reactants has changed population
        void
        respond( const mzrReactionStimulus& rStimulus );

        // This fulfills the pure virtual function in the ancestor class
        // cpx::notifier.
        void notify( int generateDepth );
        
        /*! \brief What a reaction does. */
        fnd::eventResult
        happen( moleculizer& rMolzer )
            throw( std::exception );
        
        fnd::eventResult
        happen() throw( std::exception );
        
        // Output generation for state dump.
        xmlpp::Element*
        insertElt( xmlpp::Element* pParentElt ) const
        throw( std::exception );
        
    };
}

#endif
