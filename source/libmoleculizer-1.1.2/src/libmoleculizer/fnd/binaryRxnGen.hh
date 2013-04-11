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

#ifndef BINARYRXNGEN_H
#define BINARYRXNGEN_H

/*! \file
  \ingroup rxnGenGroup
  \brief Defines reaction generator template for "binary" reaction families.
  
  Three templates are given in all.  The first two support the last,
  which is the only one intended for direct instantiation.  This last
  template is useful as a base class for reaction generators for any
  reactionFamily that is sensitive to the creation of new species
  reported by two members of feature.
  
  For example, the dimerizeFam family of dimerization reactions is
  sensitive to two free binding site features.  Whenever a complex
  species that displays one of these free binding sites is created,
  the dimerizeFam must create new dimerizations between the new
  species of complex and all the species displaying a complementary
  site.  */

#include <functional>
#include "fnd/featureContext.hh"

namespace fnd
{
    // These templates implement reaction generation for binary reaction
    // families.  The family is binary in the sense that it can pay attention
    // to two features and generate all the appropriate new reactions when
    // either feature reports a new species.
    //
    // This is done by implementing two reaction generators, one for each
    // of the two features to which the reaction family is connected.
    //
    // The two generators are the same, but are used in complementary
    // fashion in the binary family.  Below, binaryRxnGenPair
    // implements a complementary pair for use in creating new families of
    // binary reactions.
    
    /*! \ingroup rxnGenGroup
      \brief Support template for the binaryRxnGenPair template.
      
      The full reaction generator for a binary reaction has two mirror-image
      "sub-generators."  Each sees to adding reactions coming from one of
      the features to which the reaction family is sensitive. */
    template<class newContextClass, class otherFeatureClass>
    class binaryRxnGen :
        public fnd::rxnGen<newContextClass>
    {
        
        typedef typename otherFeatureClass::contextType otherContextType;
        
        otherFeatureClass& rOtherFtr;
        
        // Generates the reactions for the new context paired with a single
        // context from the other feature.
        class forOneOtherContext :
            public std::unary_function<otherContextType, void>
        {
            const newContextClass& rNew;
            const binaryRxnGen& rRxnGen;
            int notificationDepth;
            
        public:
            forOneOtherContext( const newContextClass& rNewContext,
                                const binaryRxnGen& rThisRxnGen,
                                int generateDepth ) :
                rNew( rNewContext ),
                rRxnGen( rThisRxnGen ),
                notificationDepth( generateDepth )
            {}
            
            // Make and install the parameters resulting from combining the new
            // context rNew with one context coming from the other rxnGen.
            void operator()( const otherContextType& rOtherContext ) const
            {
                rRxnGen.makeBinaryReactions( rNew,
                                             rOtherContext,
                                             notificationDepth );
            }
        };
        friend class forOneOtherContext;
        
    public:
        binaryRxnGen( otherFeatureClass& rOtherFeature ) :
            rOtherFtr( rOtherFeature )
        {}
        
        void
        respond( const fnd::featureStimulus<newContextClass>& rStimulus )
        {
            // Note that rStimulus inherits from newContextClass, upon which
            // it is templated.
            
            forOneOtherContext anotherContext( rStimulus.getContext(),
                                               *this,
                                               rStimulus.getNotificationDepth() );
            
            // Somehow this was not working as a for_each loop, overrunning where 
            // the rOtherFtr.contexts vector and crashing.  Writing the loop
            // explicitly like so fixed the problem.
            for ( unsigned int ii = 0; ii != rOtherFtr.contexts.size(); ++ii )
            {
                anotherContext( rOtherFtr.contexts[ii] );
            }
        }
        
        // Note that the "generateDepth" param here comes straight from
        // the featureStimulus; it has not yet been decremented.
        virtual void
        makeBinaryReactions( const newContextClass& rNewContext,
                             const otherContextType& rOtherContext,
                             int generateDepth ) const = 0;
        
    };
    
    /*! \ingroup rxnGenGroup
      \brief Template reaction generator for binary reaction.
      
      Basically, two complementary reaction generators that act in tandem
      are implemented, one for each of the features.  */
    template<class leftFeatureClass, class rightFeatureClass>
    class binaryRxnGenPair
    {
        typedef typename leftFeatureClass::contextType leftContextType;
        typedef typename rightFeatureClass::contextType rightContextType;
        
        class leftRxnGenClass :
            public binaryRxnGen<leftContextType, rightFeatureClass>
        {
            binaryRxnGenPair& rPair;
            
            void
            makeBinaryReactions( const leftContextType& rNewContext,
                                 const rightContextType& rOtherContext,
                                 int generateDepth ) const
            {
                rPair.makeBinaryReactions( rNewContext,
                                           rOtherContext,
                                           generateDepth );
            }
            
        public:
            
            leftRxnGenClass( rightFeatureClass& rRightFeature,
                             binaryRxnGenPair& rParentPair ) :
                binaryRxnGen<leftContextType, rightFeatureClass> ( rRightFeature ),
                rPair( rParentPair )
            {}
        };
        friend class leftRxnGenClass;
        
        leftRxnGenClass leftRxnGen;
        
        class rightRxnGenClass :
            public binaryRxnGen<rightContextType, leftFeatureClass>
        {
            binaryRxnGenPair& rPair;
            
            void
            makeBinaryReactions( const rightContextType& rNewContext,
                                 const leftContextType& rOtherContext,
                                 int generateDepth ) const
            {
                rPair.makeBinaryReactions( rOtherContext,
                                           rNewContext,
                                           generateDepth );
            }
            
        public:
            
            rightRxnGenClass( leftFeatureClass& rLeftFeature,
                              binaryRxnGenPair& rParentPair ) :
                binaryRxnGen<rightContextType, leftFeatureClass> ( rLeftFeature ),
                rPair( rParentPair )
            {}
        };
        friend class rightRxnGenClass;
        
        rightRxnGenClass rightRxnGen;
        
    public:
        
        binaryRxnGenPair( leftFeatureClass& rLeftFeature,
                          rightFeatureClass& rRightFeature ) :
            leftRxnGen( rRightFeature,
                        *this ),
            rightRxnGen( rLeftFeature,
                         *this )
        {}
        
        virtual ~binaryRxnGenPair( void )
        {}
        
        // This virtual function must be supplied to tell how to construct
        // a reaction from the new contexts.
        virtual void
        makeBinaryReactions( const leftContextType& rLeftContext,
                             const rightContextType& rRightContext,
                             int generateDepth ) const = 0;
        
        /*! \brief Sub-generator accessors.
          
          These two functions allow adding the rxnGens to the features. */
        //@{
        typename fnd::rxnGen<leftContextType>*
        getLeftRxnGen()
        {
            return &leftRxnGen;
        }
        
        typename fnd::rxnGen<rightContextType>*
        getRightRxnGen()
        {
            return &rightRxnGen;
        }
        //@}
    };
}

#endif // BINARYRXNGEN_H
