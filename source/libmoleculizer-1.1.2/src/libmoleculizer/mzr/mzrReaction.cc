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

#include "mzr/mzrSpecies.hh"
#include "mzr/mzrReaction.hh"
#include "mzr/respondReaction.hh"
#include "mzr/mzrEltName.hh"
#include "mzr/mzrUnit.hh"
#include "mzr/dumpUtils.hh"
#include "mzr/unitsMgr.hh"
#include "mzr/moleculizer.hh"
#include "mzr/mzrSpeciesDumpable.hh"
#include <libxml++/libxml++.h>
#include <iostream>

namespace mzr
{
    void
    mzrReaction::
    respond( const mzrReactionStimulus& rStimulus )
    {
        // This reschedules the reaction.  Since all of this seems to be
        // unnecessary, maybe instead we can just recalculate the propensity here.
        
        // Deleted all this code.  It had recalculated the propensity and then rescheduled the reaction.
    }
    
    class updateOneSpecies :
        public std::unary_function<fnd::basicReaction<mzrSpecies>::multMap::value_type, void>
    {
        fnd::sensitivityList<mzrReaction>& rAffected;
        int depth;
        
    public:
        updateOneSpecies( fnd::sensitivityList<mzrReaction>& rAffectedReactions,
                          int generateDepth ) :
            rAffected( rAffectedReactions ),
            depth( generateDepth )
        {}
        
        void
        operator()( const argument_type& rSpeciesMultPair ) const
        {
            mzrSpecies* pSpecies = rSpeciesMultPair.first;
            pSpecies->expandReactionNetwork( depth );
        }
    };
    
    class notifyOneSpecies:
        public std::unary_function<fnd::basicReaction<mzrSpecies>::multMap::value_type, void>
    {
        int depth;
        
    public:
        notifyOneSpecies( int generateDepth ) :
            depth( generateDepth )
        {}
        
        void
        operator()( const argument_type& rSpeciesMultPair ) const
        {
            mzrSpecies* pSpecies = rSpeciesMultPair.first;
            pSpecies->expandReactionNetwork( depth );
        }
    };
    
    
    void
    mzrReaction::expandReactionNetwork()
    {
        int generateDepth = getGenerateDepth();

        this->ensureNotified( generateDepth );
        
//         std::for_each( products.begin(),
//                        produ.end(),
//                        notifyOneSpecies( generateDepth ) );
    }
    
    // What a reaction event does, given what the reaction does.
    fnd::eventResult
    mzrReaction::
    happen( moleculizer& rMolzer )
        throw( std::exception )
    {
        // Reactions whose reactants are affected by this reaction
        // are accumulated in this set by doReaction.
        fnd::sensitivityList<mzrReaction> affectedReactions;
        
        //         // This is a leftover of the hack for "no reactant" arrows.  Since
        //         // they're not sensitive to anything, they have to reschedule
        //         // themselves.
        //         respondReaction doRespondReaction(rMolzer);
        //         if(reactants.size() == 0)
        //         {
        //             // This is a hacky special case, and that's why I don't like it.
        //             lastPropensity = -1.0;
        //             doRespondReaction(this);
        //         }
        
        // Do the reaction deltas.
        //         int generateDepth
        //             = rMolzer.pUserUnits->pMzrUnit->getGenerateDepth();
        
        int generateDepth
            = getGenerateDepth();
        std::for_each( deltas.begin(),
                       deltas.end(),
                       updateOneSpecies( affectedReactions,
                                         generateDepth ) );
        
        // The reaction event was descheduled before being executed.
        // This should probably be a part of descheduling.
        //
        // This is bad bad bad.  It was connected with the "tolerance"
        // acceleration, which has left such spoor all over the place.
        //
        // Are there any other times that a reaction is descheduled,
        // besides this and when the reaction is executed?
        // lastPropensity = -1.0;
        
        // Reschedule the reactions whose reactants were affected by this
        // reaction.
        //affectedReactions.forEachSensitive(doRespondReaction);
        
        // For the dumpable that gives the total number of reactions so far.
        reactionEventCount++;
        
        return fnd::go;
    }
    
    
    fnd::eventResult
    mzrReaction::
    happen() throw( std::exception )
    {
        
        fnd::sensitivityList<mzrReaction> affectedReactions;
        
        // Do the reaction deltas.
        int generateDepth
            = getGenerateDepth();
        std::for_each( deltas.begin(),
                       deltas.end(),
                       updateOneSpecies( affectedReactions,
                                         generateDepth ) );
        
        // The reaction event was descheduled before being executed.
        // This should probably be a part of descheduling.
        //
        // This is bad bad bad.  It was connected with the "tolerance"
        // acceleration, which has left such spoor all over the place.
        //
        // Are there any other times that a reaction is descheduled,
        // besides this and when the reaction is executed?
        //         lastPropensity = -1.0;
        
        //         // Reschedule the reactions whose reactants were affected by this
        //         // reaction.
        //         affectedReactions.forEachSensitive(doRespondReaction);
        
        //         // For the dumpable that gives the total number of reactions so far.
        reactionEventCount++;
        
        return fnd::go;
    }
    
    
    
    // Following are connected with generating state dump output.
    
    class insertReactantSpeciesRef :
        public std::unary_function<std::map<mzrSpecies*, int>::value_type, void>
    {
        xmlpp::Element* pReactionElt;
    public:
        insertReactantSpeciesRef( xmlpp::Element* pReactionElement ) :
            pReactionElt( pReactionElement )
        {
        }
        
        void
        operator()( const argument_type& rSpeciesMultPair ) const
            throw( std::exception )
        {
            // Insert element for reaction reactant species.
            xmlpp::Element* pReactantSpeciesRefElt
                = pReactionElt->add_child( eltName::taggedSubstrate );
            
            // Add the name or tag of the substrate species as attribute.
            pReactantSpeciesRefElt
                ->set_attribute( eltName::taggedSubstrate_tagAttr,
                                 rSpeciesMultPair.first->getTag() );
            
            // Add multiplicity of substrate as attribute.
            pReactantSpeciesRefElt
                ->set_attribute( eltName::taggedSubstrate_multAttr,
                                 utl::stringify<int> ( rSpeciesMultPair.second ) );
        }
    };
    
    // It looks like I'm going back to emitting reaction substrates and
    // products, rather than substrates and deltas.  This is mainly for the
    // "immovable object," SBML.
    class insertProductSpeciesRef : public
    std::unary_function<std::map<mzrSpecies*, int>::value_type, void>
    {
        xmlpp::Element* pReactionElt;
    public:
        insertProductSpeciesRef( xmlpp::Element* pReactionElement ) :
            pReactionElt( pReactionElement )
        {}
        
        void
        operator()( const argument_type& rSpeciesMultPair ) const
            throw( std::exception )
        {
            // Insert element for reaction product.
            xmlpp::Element* pProductSpeciesRefElt
                = pReactionElt->add_child( eltName::taggedProduct );
            
            // Add species tag as attribute.
            pProductSpeciesRefElt
                ->set_attribute( eltName::taggedProduct_tagAttr,
                                 rSpeciesMultPair.first->getTag() );
            
            // Add multiplicity as attribute.
            pProductSpeciesRefElt
                ->set_attribute( eltName::taggedProduct_multAttr,
                                 utl::stringify<int> ( rSpeciesMultPair.second ) );
        }
    };
    
    xmlpp::Element*
    mzrReaction::insertElt( xmlpp::Element* pParentElt ) const
        throw( std::exception )
    {
        xmlpp::Element* pReactionElt
            = pParentElt->add_child( eltName::tagReaction );
        
        std::for_each( reactants.begin(),
                       reactants.end(),
                       insertReactantSpeciesRef( pReactionElt ) );
        
        std::for_each( products.begin(),
                       products.end(),
                       insertProductSpeciesRef( pReactionElt ) );
        
        // Additional scientific notation here for use by ECell.
        mzr::addDoubleParamChild( pReactionElt,
                                  eltName::rate,
                                  eltName::rate_valueAttr,
                                  rate );
        return pReactionElt;
    }
    
    void
    mzrReaction::setGenerateDepth( unsigned int newDepth )
    {
        mzrReaction::reactionDepth = newDepth;
    }

    void mzrReaction::notify( int generateDepth )
    {
        // Notify each of the species in the products
        for( multMap::const_iterator prodIter = products.begin();
             prodIter != products.end();
             ++prodIter)
        {
            if (!prodIter->first->hasNotified())
            {
                prodIter->first->notify( generateDepth);
            }
        }
                 
    }
    
    // Reaction related dumpable globals.
    int mzrReaction::reactionCount = 0;
    int mzrReaction::reactionEventCount = 0;
    
    // The default reaction depth
    unsigned int mzrReaction::reactionDepth = 1;
}
