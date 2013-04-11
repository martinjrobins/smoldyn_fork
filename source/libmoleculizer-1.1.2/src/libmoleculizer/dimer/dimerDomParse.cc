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

#include <vector>
#include "utl/domXcpt.hh"
#include "fnd/pchem.hh"
#include "mzr/moleculizer.hh"
#include "mzr/mzrUnit.hh"
#include "mzr/mzrEltName.hh"
#include "mol/molEltName.hh"
#include "plex/plexUnit.hh"
#include "plex/plexEltName.hh"
#include "dimer/dimerUnit.hh"
#include "dimer/dimerizeFam.hh"
#include "dimer/decompFam.hh"
#include "dimer/dimerEltName.hh"
#include <libxml++/libxml++.h>
#include "mzr/mzrSpeciesDumpable.hh"

namespace dimer
{
    // I may need an anonymous namespace for this.  Should be doing that
    // in many places.
    typedef std::pair<bnd::mzrMol*, int> bindingPartner;
    
    class parseBindingPartner :
        public std::unary_function<xmlpp::Node*, bindingPartner>
    {
        bnd::molUnit& rMolUnit;
        
    public:
        parseBindingPartner( bnd::molUnit& refMolUnit ) :
            rMolUnit( refMolUnit )
        {}
        
        bindingPartner
        operator()( xmlpp::Node* pMolRefNode )
        {
            // Cast the mol-ref node to element; probably unnecessarily dynamically.
            xmlpp::Element* pMolRefElt
                = utl::dom::mustBeElementPtr( pMolRefNode );
            
            // Get the mol name.
            std::string molName
                = utl::dom::mustGetAttrString( pMolRefElt,
                                               plx::eltName::molRef_nameAttr );
            
            // Look up the mol.
            bnd::mzrMol* pMol
                = rMolUnit.mustFindMol( molName,
                                        pMolRefElt );
            
            // Get the binding site name.
            xmlpp::Element* pSiteRefElt
                = utl::dom::mustGetUniqueChild( pMolRefElt,
                                                eltName::siteRef );
            std::string siteName
                = utl::dom::mustGetAttrString( pSiteRefElt,
                                               eltName::siteRef_nameAttr );
            
            // Ask the mol to look up the index of the binding site
            // using the binding site name.
            int siteNdx
                = pMol->mustFindSite( siteName,
                                      pSiteRefElt );
            
            return std::make_pair( pMol,
                                   siteNdx );
        }
    };
    
    class installDefaultKinetics :
        public std::unary_function<const std::pair<const std::string, cpx::siteShape>&, void>
    {
        const cpx::siteShape* pLeftShape;
        double onR;
        double offR;
        
        // Debugging matter.
        const bnd::mzrMol* pLMol;
        const bnd::mzrBndSite& rLSite;
        const bnd::mzrMol* pRMol;
        const bnd::mzrBndSite& rRSite;
        
        dimerizeExtrapolator* pDimerizeExtrap;
        decomposeExtrapolator* pDecomposeExtrap;
        
    public:
        installDefaultKinetics( const cpx::siteShape* pLeftSiteShape,
                                double onRate,
                                double offRate,
                                const bnd::mzrMol* pLeftMol,
                                const bnd::mzrBndSite& rLeftSite,
                                const bnd::mzrMol* pRightMol,
                                const bnd::mzrBndSite& rRightSite,
                                dimerizeExtrapolator* pDimerizeExtrapolator,
                                decomposeExtrapolator* pDecomposeExtrapolator ) :
            pLeftShape( pLeftSiteShape ),
            onR( onRate ),
            offR( offRate ),
            pLMol( pLeftMol ),
            rLSite( rLeftSite ),
            pRMol( pRightMol ),
            rRSite( rRightSite ),
            pDimerizeExtrap( pDimerizeExtrapolator ),
            pDecomposeExtrap( pDecomposeExtrapolator )
        {}
        
        // The 'const' on the string key here DOES make a difference.
        // Without it, the compiler generates a local pair, apparently
        // because it sees a type difference.  It is enabled to do
        // something about it by the const'ness of the pair argument,
        // which legitimizes the construction of a temporary pair
        // of the right type, which again it seems to be able to do.
        //
        // Never mind that the pair is const, so that the string key is implicitly
        // const: const pair<int, int> is apparently not the same as const
        // pair<const int, const int>.
        void
        operator()( const std::pair<const std::string, cpx::siteShape>&
                    rRightSiteShapeEntry )
            const throw( utl::xcpt )
        {
            const cpx::siteShape* pRightShape = & ( rRightSiteShapeEntry.second );
            
            pDimerizeExtrap->setRate( pLeftShape,
                                      pRightShape,
                                      onR );
            
            pDecomposeExtrap->setRate( pLeftShape,
                                       pRightShape,
                                       offR );
        }
    };
    
    class installDefaultKineticsToLeft : public
    std::unary_function<const std::pair<const std::string, cpx::siteShape>&, void>
    {
        const std::map<std::string, cpx::siteShape>& rRightShapeMap;
        double onR;
        double offR;
        
        // Debugging matter.
        const bnd::mzrMol* pLMol;
        const bnd::mzrBndSite& rLSite;
        const bnd::mzrMol* pRMol;
        const bnd::mzrBndSite& rRSite;
        
        dimerizeExtrapolator* pDimerizeExtrap;
        decomposeExtrapolator* pDecomposeExtrap;
        
    public:
        // Note that most of the arguments here are for debugging.
        installDefaultKineticsToLeft
        ( const std::map<std::string, cpx::siteShape>& rRightSiteShapeMap,
          double onRate,
          double offRate,
          const bnd::mzrMol* pLeftMol,
          const bnd::mzrBndSite& rLeftSite,
          const bnd::mzrMol* pRightMol,
          const bnd::mzrBndSite& rRightSite,
          dimerizeExtrapolator* pDimerizeExtrapolator,
          decomposeExtrapolator* pDecomposeExtrapolator ) :
            rRightShapeMap( rRightSiteShapeMap ),
            onR( onRate ),
            offR( offRate ),
            pLMol( pLeftMol ),
            rLSite( rLeftSite ),
            pRMol( pRightMol ),
            rRSite( rRightSite ),
            pDimerizeExtrap( pDimerizeExtrapolator ),
            pDecomposeExtrap( pDecomposeExtrapolator )
        {}
        
        void
        operator()( const std::pair<const std::string, cpx::siteShape>&
                    rLeftSiteShapeEntry )
            const throw( utl::xcpt )
        {
            std::for_each( rRightShapeMap.begin(),
                           rRightShapeMap.end(),
                           installDefaultKinetics( & ( rLeftSiteShapeEntry.second ),
                                                   onR,
                                                   offR,
                                                   pLMol,
                                                   rLSite,
                                                   pRMol,
                                                   rRSite,
                                                   pDimerizeExtrap,
                                                   pDecomposeExtrap ) );
        }
    };
    
    class getNameAttr :
        public std::unary_function<xmlpp::Node*, std::string>
    {
    public:
        std::string
        operator()( xmlpp::Node* pSiteShapeRefNode ) const throw( utl::xcpt )
        {
            // Cast the site-shape-ref node* to element*; probably unnecessarily
            // dynamically.
            xmlpp::Element* pSiteShapeRefElt
                = utl::dom::mustBeElementPtr( pSiteShapeRefNode );
            
            // Get the name of the site shape.
            return utl::dom::mustGetAttrString( pSiteShapeRefElt,
                                                bnd::eltName::siteShapeRef_nameAttr );
        }
    };
    
    class insertAlloRates :
        public std::unary_function<xmlpp::Node*, void>
    {
        const bnd::mzrBndSite& rLeftSite;
        const bnd::mzrMol* pLeftMol;
        
        const bnd::mzrBndSite& rRightSite;
        const bnd::mzrMol* pRightMol;
        
        dimerizeExtrapolator* pDimerizeExtrap;
        decomposeExtrapolator* pDecomposeExtrap;
        
    public:
        insertAlloRates( const bnd::mzrBndSite& rLeftBindingSite,
                         const bnd::mzrMol* pLeftBindingMol,
                         const bnd::mzrBndSite& rRightBindingSite,
                         const bnd::mzrMol* pRightBindingMol,
                         dimerizeExtrapolator* pDimerizeExtrapolator,
                         decomposeExtrapolator* pDecomposeExtrapolator ) :
            rLeftSite( rLeftBindingSite ),
            pLeftMol( pLeftBindingMol ),
            rRightSite( rRightBindingSite ),
            pRightMol( pRightBindingMol ),
            pDimerizeExtrap( pDimerizeExtrapolator ),
            pDecomposeExtrap( pDecomposeExtrapolator )
        {}
        
        void
        operator()( xmlpp::Node* pAlloRatesNode ) const throw( utl::xcpt )
        {
            // Cast the allo-rates node* to element*; probably unnecessarily
            // dynamically.
            xmlpp::Element* pAlloRatesElt
                = utl::dom::mustBeElementPtr( pAlloRatesNode );
            
            // Parse the site-shape-ref child elements, of which there must be 2,
            // into a more easily accessible form.
            xmlpp::Node::NodeList siteShapeRefNodes
                = pAlloRatesElt->get_children( bnd::eltName::siteShapeRef );
            if ( siteShapeRefNodes.size() != 2 )
                throw
                    utl::dom::
                    badChildCountXcpt::general( pAlloRatesNode,
                                                bnd::eltName::siteShapeRef,
                                                2,
                                                ( int ) siteShapeRefNodes.size() );
            
            // Get the site shape pointer corresponding to the first (left) site-ref
            // node.
            xmlpp::Node::NodeList::iterator iSiteShapeRefNode
                = siteShapeRefNodes.begin();
            xmlpp::Element* pLeftSiteShapeRefElt
                = utl::dom::mustBeElementPtr( *iSiteShapeRefNode );
            std::string leftSiteShapeName
                = utl::dom::mustGetAttrString( pLeftSiteShapeRefElt,
                                               bnd::eltName::siteShapeRef_nameAttr );
            cpx::siteParam leftSiteParam
                = rLeftSite.mustGetShape( pLeftMol,
                                          leftSiteShapeName,
                                          pLeftSiteShapeRefElt );
            
            // Get the site shape pointer corresponding to the second (right)
            // site-ref node.
            ++iSiteShapeRefNode;
            xmlpp::Element* pRightSiteShapeRefElt
                = utl::dom::mustBeElementPtr( *iSiteShapeRefNode );
            std::string rightSiteShapeName
                = utl::dom::mustGetAttrString( pRightSiteShapeRefElt,
                                               bnd::eltName::siteShapeRef_nameAttr );
            cpx::siteParam rightSiteParam
                = rRightSite.mustGetShape( pRightMol,
                                           rightSiteShapeName,
                                           pRightSiteShapeRefElt );
            
            // Get the on-rate.
            xmlpp::Element* pOnRateElt
                = utl::dom::mustGetUniqueChild( pAlloRatesElt,
                                                eltName::onRate );
            double onRate
                = utl::dom::mustGetAttrDouble( pOnRateElt,
                                               eltName::onRate_valueAttr );
            
            // Get the off-rate.
            xmlpp::Element* pOffRateElt
                = utl::dom::mustGetUniqueChild( pAlloRatesElt,
                                                eltName::offRate );
            double offRate
                = utl::dom::mustGetAttrDouble( pOffRateElt,
                                               eltName::offRate_valueAttr );
            
            // Install the allosteric rates, overwriting the default entries
            // which are already there.
            pDimerizeExtrap->setRate( leftSiteParam,
                                      rightSiteParam,
                                      onRate );
            
            pDecomposeExtrap->setRate( leftSiteParam,
                                       rightSiteParam,
                                       offRate );
        }
    };
    
    class addDimerizeDecompose :
        public std::unary_function<xmlpp::Node*, void>
    {
        mzr::mzrUnit& rMzrUnit;
        bnd::molUnit& rMolUnit;
        dimerUnit& rDimerUnit;
        plx::plexUnit& rPlexUnit;
        
    public:
        addDimerizeDecompose( mzr::mzrUnit& refMzrUnit,
                              bnd::molUnit& refMolUnit,
                              dimerUnit& refDimerUnit,
                              plx::plexUnit& refPlexUnit ) :
            rMzrUnit( refMzrUnit ),
            rMolUnit( refMolUnit ),
            rDimerUnit( refDimerUnit ),
            rPlexUnit( refPlexUnit )
        {}
        
        void
        operator()( xmlpp::Node* pDimerizationGenNode ) const throw( utl::xcpt )
        {
            // Cast the node pointer to element pointer; probably unnecessarily
            // dynamically.
            xmlpp::Element* pDimerizationGenElt
                = utl::dom::mustBeElementPtr( pDimerizationGenNode );
            
            // Get the elements giving the binding partners.
            xmlpp::Node::NodeList molRefs
                = pDimerizationGenElt->get_children( plx::eltName::molRef );
            
            // Make sure there are two of them.
            if ( 2 != molRefs.size() )
                throw
                    utl::dom::
                    badChildCountXcpt::general( pDimerizationGenElt,
                                                plx::eltName::molRef,
                                                2,
                                                ( int ) molRefs.size() );
            
            // Parse the binding partners.
            std::vector<bindingPartner> bindingPartners( 2 );
            std::transform( molRefs.begin(),
                            molRefs.end(),
                            bindingPartners.begin(),
                            parseBindingPartner( rMolUnit ) );
            
            // Extract left binding partner's info.
            bnd::mzrMol* pLeftMol = bindingPartners[0].first;
            int leftSiteNdx = bindingPartners[0].second;
            const bnd::mzrBndSite& rLeftSite
                = ( *pLeftMol )[leftSiteNdx];
            const std::map<std::string, cpx::siteShape>& rLeftSiteShapeMap
                = rLeftSite.shapesByName;
            double leftMolWeight = pLeftMol->getDefaultParam()->getMolWeight();
            
            // Extract right binding partner's info.
            bnd::mzrMol* pRightMol = bindingPartners[1].first;
            int rightSiteNdx = bindingPartners[1].second;
            const bnd::mzrBndSite& rRightSite
                = ( *pRightMol )[rightSiteNdx];
            const std::map<std::string, cpx::siteShape>& rRightSiteShapeMap
                = rRightSite.shapesByName;
            double rightMolWeight = pRightMol->getDefaultParam()->getMolWeight();
            
            // Install the binding feature for the two binding sites.
            fnd::feature<cpx::cxBinding<plx::mzrPlexSpecies, plx::mzrPlexFamily> >*
                pBindingFeature
                = rPlexUnit.addBindingFeature( pLeftMol,
                                               leftSiteNdx,
                                               pRightMol,
                                               rightSiteNdx );
            
            // As of now, this is done elsewhere...
            // Check if the user specified a reaction rate extrapolator.
            //             xmlpp::Attribute* pRateExtrapolatorAttr
            //                 = pDimerizationGenElt->get_attribute( eltName::dimerizationGen_rateExtrapAttr );
            
            // Construct the dimerization reaction rate extrapolator according to
            // the option.  Reaction rate extrapolators are memory-managed by the
            // reaction generators they are attached to.
            dimerizeExtrapolator* pDimerizeExtrap = 0;
            decomposeExtrapolator* pDecompExtrap = 0;
            
            
            if( rMzrUnit.rMolzer.getRateExtrapolation() )
            {
                pDimerizeExtrap = new dimerizeMassExtrap(leftMolWeight,
                                                         rightMolWeight);
                
                // What is this?
                // pDimerizeExtrap = new dimerizeConstantRate();
            }
            else
            {
                pDimerizeExtrap = new dimerizeNoExtrap();
            }
            
            // There is only one option for decomposition rate extrapolation.
            pDecompExtrap = new decomposeNoExtrap();
            
            // Parse the default rates.
            xmlpp::Element* pOnRateElt
                = utl::dom::mustGetUniqueChild( pDimerizationGenElt,
                                                eltName::defaultOnRate );
            double onRate
                = utl::dom::mustGetAttrDouble( pOnRateElt,
                                               eltName::defaultOnRate_valueAttr );
            xmlpp::Element* pOffRateElt
                = utl::dom::mustGetUniqueChild( pDimerizationGenElt,
                                                eltName::defaultOffRate );
            double offRate
                = utl::dom::mustGetAttrDouble( pOffRateElt,
                                               eltName::defaultOffRate_valueAttr );
            
            // Install the default rates as kinetics for all possible
            // combinations of site shapes for the two binding partners.
            //
            // Passing along the mols and binding sites for debugging.
            // NOTE: MOST PARAMETERS HERE ARE FOR DEBUGGING.
            std::for_each( rLeftSiteShapeMap.begin(),
                           rLeftSiteShapeMap.end(),
                           installDefaultKineticsToLeft( rRightSiteShapeMap,
                                                         onRate,
                                                         offRate,
                                                         pLeftMol,
                                                         rLeftSite,
                                                         pRightMol,
                                                         rRightSite,
                                                         pDimerizeExtrap,
                                                         pDecompExtrap ) );
            
            // Parse the allosteric shape pairs; i.e. the pairs of binding
            // site shapes whose kinetic differ from the default.
            xmlpp::Node::NodeList alloRatesNodes
                = pDimerizationGenElt->get_children( eltName::alloRates );
            std::for_each( alloRatesNodes.begin(),
                           alloRatesNodes.end(),
                           insertAlloRates( rLeftSite,
                                            pLeftMol,
                                            rRightSite,
                                            pRightMol,
                                            pDimerizeExtrap,
                                            pDecompExtrap ) );
            
            // Construct the decomposition reaction family, and register for memory
            // management.
            decompFam* pDecompFam = new decompFam( rMzrUnit,
                                                   rPlexUnit,
                                                   pDecompExtrap );
            rMzrUnit.addReactionFamily( pDecompFam );
            
            // Attach the decomposition reaction generator to the binding feature.
            pBindingFeature->insert( pDecompFam->getRxnGen() );
            
            // Construct the dimerizaton reaction family, and register it for memory
            // management in the mzrUnit.
            bnd::siteFeature& rLeftSiteFeature
                = ( *pLeftMol )[leftSiteNdx];
            bnd::siteFeature& rRightSiteFeature
                = ( *pRightMol )[rightSiteNdx];
            dimerizeFam* pDimerizeFam
                = new dimerizeFam( rLeftSiteFeature,
                                   rRightSiteFeature,
                                   rMzrUnit,
                                   rPlexUnit,
                                   pDimerizeExtrap );
            rMzrUnit.addReactionFamily( pDimerizeFam );
            
            // Attach the dimerization reaction generator the site features.
            rLeftSiteFeature.addSensitive( pDimerizeFam->getLeftRxnGen() );
            rRightSiteFeature.addSensitive( pDimerizeFam->getRightRxnGen() );

        }
    };
    
    void
    dimerUnit::parseDomInput( xmlpp::Element* pRootElement,
                              xmlpp::Element* pModelElement,
                              xmlpp::Element* pStreamElt ) throw( std::exception )
    {

        // Get the header node for all reaction generators.
        xmlpp::Element* pReactionGensElt
            = utl::dom::mustGetUniqueChild( pModelElement,
                                            mzr::eltName::reactionGens );
        
        // Get all the dimerization-gen elements.  Eventually, I will have to
        // deal with the many reaction generators for the scaffold kinase reactions
        // and other special reactions.
        xmlpp::Node::NodeList dimerizationGens
            = pReactionGensElt->get_children( eltName::dimerizationGen );


        // Make dimerization and decomposition generators.
        std::for_each( dimerizationGens.begin(),
                       dimerizationGens.end(),
                       addDimerizeDecompose( rMzrUnit,
                                             rMolUnit,
                                             *this,
                                             rPlexUnit ) );

        // Record how many dimerization/decomposition generators were created.
        numDimerDecomGens += dimerizationGens.size();
        

    }
}
