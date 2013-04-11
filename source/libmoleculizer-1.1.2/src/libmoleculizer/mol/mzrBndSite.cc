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

#include "mol/mzrBndSite.hh"
#include "mol/molEltName.hh"
#include "mol/unkSiteShapeXcpt.hh"
#include "plex/mzrPlexFamily.hh"
#include <libxml++/libxml++.h>

namespace bnd
{
    mzrBndSite::
    mzrBndSite( const std::string& rName,
                const std::set<std::string>& rShapeNames,
                const std::string& rDefaultShapeName ) :
        cpx::basicBndSite( rName,
                           rShapeNames,
                           rDefaultShapeName )
    {}
    
    const cpx::siteShape*
    mzrBndSite::
    mustGetShape( const mzrMol* pMol,
                  const std::string& rShapeName,
                  const xmlpp::Node* pRequestingNode ) const
        throw( utl::xcpt )
    {
        const cpx::siteShape* pShape
            = getShape( rShapeName );
        
        if ( ! pShape )
            throw unkSiteShapeXcpt( pRequestingNode,
                                    *this,
                                    pMol,
                                    rShapeName );
        
        return pShape;
    }
    
    class insertShapeElt :
        public std::unary_function<std::pair<const std::string, cpx::siteShape>, void>
    {
        xmlpp::Element* pBindingSiteElt;
    public:
        insertShapeElt( xmlpp::Element* pBindingSiteElement ) :
            pBindingSiteElt( pBindingSiteElement )
        {}
        
        void
        operator()( const std::pair<const std::string, cpx::siteShape>& rEntry ) const
            throw( std::exception )
        {
            xmlpp::Element* pSiteShapeElt
                = pBindingSiteElt->add_child( eltName::siteShape );
            
            pSiteShapeElt->set_attribute( eltName::siteShape_nameAttr,
                                          rEntry.second.getName() );
        }
    };
    
    xmlpp::Element*
    mzrBndSite::
    insertElt( xmlpp::Element* pMolElt ) const
        throw( utl::xcpt )
    {
        xmlpp::Element* pBindingSiteElt
            = pMolElt->add_child( eltName::bindingSite );
        
        pBindingSiteElt->set_attribute( eltName::bindingSite_nameAttr,
                                        getName() );
        
        // Put in the name of this binding site's default shape.
        xmlpp::Element* pDefaultShapeRefElt
            = pBindingSiteElt->add_child( eltName::defaultShapeRef );
        
        pDefaultShapeRefElt->set_attribute( eltName::defaultShapeRef_nameAttr,
                                            getDefaultShape()->getName() );
        
        // Put in all the binding sites shapes.
        std::for_each( shapesByName.begin(),
                       shapesByName.end(),
                       insertShapeElt( pBindingSiteElt ) );
        
        return pBindingSiteElt;
    }
}
