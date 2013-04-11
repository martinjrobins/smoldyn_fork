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

#include <libxml++/libxml++.h>
#include "cpx/plexMap.hh"
#include "cpx/binding.hh"
#include "mol/molEltName.hh"
#include "plex/mzrPlex.hh"
#include "plex/plexEltName.hh"

namespace plx
{
    class insertBindingSiteElt :
        public std::unary_function<cpx::basicBndSite, void>
    {
        xmlpp::Element* pBindingElt;
        const mzrPlex& rPlx;
        
    public:
        insertBindingSiteElt( xmlpp::Element* pBindingElement,
                              const mzrPlex& rPlex ) :
            pBindingElt( pBindingElement ),
            rPlx( rPlex )
        {}
        
        void
        operator()( const cpx::siteSpec& rSiteSpec ) const
            throw( std::exception )
        {
            xmlpp::Element* pMolInstanceRefElt
                = pBindingElt->add_child( eltName::molInstanceRef );
            
            // Cause the mol on which the site occurs to generate a
            // fake instance name from the instance index.
            const bnd::mzrMol& rMol = * ( rPlx.mols[rSiteSpec.molNdx()] );
            pMolInstanceRefElt->set_attribute( eltName::molInstanceRef_nameAttr,
                                               rMol.genInstanceName( rSiteSpec.molNdx() ) );
            
            // Insert reference by name to binding site.
            xmlpp::Element* pBindingSiteRefElt
                = pMolInstanceRefElt->add_child( bnd::eltName::bindingSiteRef );
            
            const bnd::mzrBndSite& rBindingSite
                = rMol[rSiteSpec.siteNdx()];
            
            pBindingSiteRefElt->set_attribute( bnd::eltName::bindingSiteRef_nameAttr,
                                               rBindingSite.getName() );
        }
    };
    
    class insertBindingElt :
        public std::unary_function<cpx::binding, xmlpp::Element*>
    {
        xmlpp::Element* pPlexElt;
        const mzrPlex& rPlx;
        
    public:
        insertBindingElt( xmlpp::Element* pPlexElement,
                          const mzrPlex& rPlex ) :
            pPlexElt( pPlexElement ),
            rPlx( rPlex )
        {}
        
        xmlpp::Element*
        operator()( const cpx::binding& rBinding ) const
            throw( std::exception )
        {
            // Insert the binding element.
            xmlpp::Element* pBindingElt
                = pPlexElt->add_child( eltName::binding );
            
            // Generate child element of binding element for each of the two
            // bound sites.
            insertBindingSiteElt insertSite( pBindingElt,
                                             rPlx );
            insertSite( rBinding.leftSite() );
            insertSite( rBinding.rightSite() );
            
            return pBindingElt;
        }
    };
    
    xmlpp::Element*
    mzrPlex::
    insertElt( xmlpp::Element* pParentElt ) const
        throw( std::exception )
    {
        xmlpp::Element* pPlexElt
            = pParentElt->add_child( eltName::plex );
        
        // Insert elements for each mol instance.  I'm using the index
        // to generate instance names.  Users will be disappointed that
        // their own instance names have been forgotten.
        for ( int molNdx = 0;
              molNdx < ( int ) mols.size();
              ++molNdx )
        {
            xmlpp::Element* pMolInstanceElt
                = pPlexElt->add_child( eltName::molInstance );
            
            bnd::mzrMol* pMol = mols[molNdx];
            
            // Stringify the instance index as an instance name.
            pMolInstanceElt->set_attribute( eltName::molInstance_nameAttr,
                                            pMol->genInstanceName( molNdx ) );
            
            xmlpp::Element* pMolRefElt
                = pMolInstanceElt->add_child( eltName::molRef );
            
            pMolRefElt->set_attribute( eltName::molRef_nameAttr,
                                       mols[molNdx]->getName() );
        }
        
        // Insert elements for each binding.
        std::for_each( bindings.begin(),
                       bindings.end(),
                       insertBindingElt( pPlexElt,
                                         *this ) );
        
        return pPlexElt;
    }
}
