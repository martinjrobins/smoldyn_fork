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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//


#ifndef __MINIMALMOL_HH
#define __MINIMALMOL_HH

#include "abstractMol.hh"

#include <map>
#include <string>
#include <vector>
#include <set>
#include <sstream>

namespace nmr
{
    
    // This class is a "MinimalMol" because it has no a priori structure.
    // BindingSites and ModificationSites are always presumed to exist.
    
    DECLARE_CLASS( MinimalMol );
    class MinimalMol
        :
        public Mol
    {
    public:
        
        MinimalMol( MolTypeCref molType )
            :
            Mol( molType )
        {}
        
        bool checkIfBindingSiteExists( BindingSiteCref aBindingSite ) const;
        bool checkIfModificationSiteExists( ModificationSiteCref aModificationSite ) const;
        
        bool
        checkIfBindingSiteIsBound( BindingSiteCref aBindingSite )
            const throw( NoSuchBindingSiteXcpt );
        
        void
        addNewBindingSite( BindingSiteCref aBindingSite );

        std::string
        getCompleteModificationName() const;
        
        
        ModificationValue
        getModificationValueAtModificationSite( ModificationSiteCref aModificationSite )
            const throw( NoSuchModificationSiteXcpt );
        
        BindingSiteList
        getBindingList() const;
        
        ModificationList
        getModificationList() const;

        std::string
        getModificationString() const;

        std::string getMolDescriptor() const;
        
        void
        bindAtBindingSite( BindingSiteCref aBindingSite )
            throw( NoSuchBindingSiteXcpt );
        
        void
        unbindAtBindingSite( BindingSiteCref aBindingSite )
            throw( NoSuchBindingSiteXcpt );
        
        void
        updateModificationState( ModificationSiteCref aModificationSite,
                                 ModificationValueCref aModificationValue )
            throw( NoSuchModificationSiteXcpt );
        
        int
        getBindingSiteInteger( BindingSiteCref aBindingSite )
            const throw( NoSuchBindingSiteXcpt );
        
        int
        getModificationSiteInteger( ModificationSiteCref aModificationSite )
            const throw( NoSuchModificationSiteXcpt );
        
        void
        addNewModificationSite( ModificationSiteCref newModSite,
                                ModificationValueCref modValue );

        virtual bool
        operator< ( MolCref rhsMol ) const
        {
std::ostringstream mol_1, mol_2;
            mol_1 << getMolType();
            mol_2 << rhsMol.getMolType();

            mol_1 << this->getModificationString();
            mol_2 << rhsMol.getModificationString();

            return mol_1.str() < mol_2.str();
        }
    protected:
        // std::map<BindingSite, bool> theBindingSiteStates; // true means bound, false,unbound.
        std::map<BindingSite, unsigned int> bindingSiteNameToNdxMap;
        
        // I read that vector<bool> is pretty darn unsafe.  I think in this case, because I'm not
        // doing any kind of anything other than getting, setting, and push_back'ing in the private
        // interface, I think I should be ok, but I should still change it though.
        std::vector<bool> bindingSiteIsBound;
        
        std::map<ModificationSite, ModificationValue> theModificationStates;
        std::map<ModificationSite, std::set<ModificationValue> > theLegalModifications;
    };
    
}

#endif

