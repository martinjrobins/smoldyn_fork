/////////////////////////////////////////////////////////////////////////////
// Moleculizer - a stochastic simulator for cellular chemistry.
// Copyright (C) 2001  Walter Lawrence (Larry) Lok.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//    
// Contact information:
//   Larry Lok, Research Fellow          Voice: 510-981-8740
//   The Molecular Sciences Institute      Fax: 510-647-0699
//   2168 Shattuck Ave.                  Email: lok@molsci.org
//   Berkeley, CA 94704
/////////////////////////////////////////////////////////////////////////////

#include "mzr/mzrEltName.hh"
#include "mzr/mzrSpeciesDumpable.hh"

// Aren't these all the same? Could they go into mzrSpeciesStream?

namespace mzr
{
    template<class mzrSpeciesType>
    void
    singleSpeciesDumpable<mzrSpeciesType>::
    insertTaggedSpeciesStreamRef(xmlpp::Element* pParent) const
        throw(std::exception)
    {
	
	utl::dom::tmp::addChildWithAttribute( pParent, 
					      eltName::taggedSpeciesStreamRef, 
					      eltName::taggedSpeciesStreamRef_nameAttr, 
					      this->getName());


//         xmlpp::Element* pTaggedSpeciesStreamRefElt
//             = pParent->add_child(eltName::taggedSpeciesStreamRef);
        
//         pTaggedSpeciesStreamRefElt
//             ->set_attribute(eltName::taggedSpeciesStreamRef_nameAttr,
//                             this->getName());
    }
    
    template<class mzrSpeciesType>
    void
    singleSpeciesDumpable<mzrSpeciesType>::
    insertDumpedSpeciesTags(xmlpp::Element* pParentElt) const
        throw(std::exception)
    {
	utl::dom::tmp::addChildWithAttribute( pParentElt, 
					      eltName::taggedSpeciesStreamRef,
					      eltName::taggedSpeciesRef_tagAttr,
					      this->getVar()->getTag());
					      
//         xmlpp::Element* pSpeciesRefElt
//             = pParentElt->add_child(eltName::taggedSpeciesRef);
        
//         pSpeciesRefElt->set_attribute(eltName::taggedSpeciesRef_tagAttr,
//                                       this->getVar()->getTag());
    }
    
    template<class mzrSpeciesType>
    void
    multiSpeciesDumpable<mzrSpeciesType>::
    insertTaggedSpeciesStreamRef(xmlpp::Element* pParent) const
        throw(std::exception)
    {

	utl::dom::tmp::addChildWithAttribute( pParent, 
					      eltName::taggedSpeciesStreamRef,
					      eltName::taggedSpeciesStreamRef_nameAttr,
					      this->getName());
					      
					      

//        xmlpp::Element* pTaggedSpeciesStreamRefElt
//             = pParent->add_child(eltName::taggedSpeciesStreamRef);
        
//         pTaggedSpeciesStreamRefElt
//             ->set_attribute(eltName::taggedSpeciesStreamRef_nameAttr,
//                             this->getName());
    }
    
    template<class mzrSpeciesType>
    class insertMultiSpeciesDumpableTag :
        public std::unary_function<mzrSpeciesType*, void>
    {
        xmlpp::Element* pParentElt;
        
    public:
        insertMultiSpeciesDumpableTag(xmlpp::Element* pParentElement) :
            pParentElt(pParentElement)
        {}
        
        void
        operator()(const mzrSpeciesType* pSpecies) const
            throw(std::exception)
        {

	    utl::dom::tmp::addChildWithAttribute( pParentElt,
						  eltName::taggedSpeciesStreamRef,
						  eltName::taggedSpeciesRef_tagAttr,
						  pSpecies->getTag());

//             xmlpp::Element* pTaggedSpeciesRefElt
//                 = pParentElt->add_child(eltName::taggedSpeciesRef);
            
//             pTaggedSpeciesRefElt->set_attribute(eltName::taggedSpeciesRef_tagAttr,
//                                                 pSpecies->getTag());
        }
    };
    
    template<class mzrSpeciesType>
    void
    multiSpeciesDumpable<mzrSpeciesType>::
    insertDumpedSpeciesTags(xmlpp::Element* pParentElt) const
        throw(std::exception)
    {
        std::for_each(this->dumpedSpecies.begin(),
                      this->dumpedSpecies.end(),
                      insertMultiSpeciesDumpableTag<mzrSpeciesType>(pParentElt));
    }
    
    template<class mzrSpeciesType>
    void
    querySpeciesDumpable<mzrSpeciesType>::
    insertTaggedSpeciesStreamRef(xmlpp::Element* pParent) const
        throw(std::exception)
    {

	utl::dom::tmp::addChildWithAttribute( pParent, 
					      eltName::taggedSpeciesStreamRef,
					      eltName::taggedSpeciesStreamRef_nameAttr,
					      this->getName());

//         xmlpp::Element* pTaggedSpeciesStreamRefElt
//             = pParent->add_child(eltName::taggedSpeciesStreamRef);
        
//         pTaggedSpeciesStreamRefElt
//             ->set_attribute(eltName::taggedSpeciesStreamRef_nameAttr,
//                             this->getName());
    }
    
    template<class mzrSpeciesType>
    class insertQuerySpeciesDumpableTag :
        public std::unary_function<mzrSpeciesType*, void>
    {
        xmlpp::Element* pParentElt;
        
    public:
        insertQuerySpeciesDumpableTag(xmlpp::Element* pParentElement) :
            pParentElt(pParentElement)
        {}
        
        void
        operator()(const mzrSpeciesType* pSpecies) const
            throw(std::exception)
        {

	    utl::dom::tmp::addChildWithAttribute( pParentElt, 
						  eltName::taggedSpeciesRef,
						  eltName::taggedSpeciesRef_tagAttr,
						  pSpecies->getTag());

//             xmlpp::Element* pTaggedSpeciesRefElt
//                 = pParentElt->add_child(eltName::taggedSpeciesRef);
            
//             pTaggedSpeciesRefElt->set_attribute(eltName::taggedSpeciesRef_tagAttr,
//                                                 pSpecies->getTag());
        }
    };
    
    template<class mzrSpeciesType>
    void
    querySpeciesDumpable<mzrSpeciesType>::
    insertDumpedSpeciesTags(xmlpp::Element* pParentElt) const
        throw(std::exception)
    {
        std::for_each(this->dumpedSpecies.begin(),
                      this->dumpedSpecies.end(),
                      insertQuerySpeciesDumpableTag<mzrSpeciesType>(pParentElt));
    }
}
