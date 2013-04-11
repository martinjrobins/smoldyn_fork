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

#ifndef MODMOLMIXIN_H
#define MODMOLMIXIN_H

// These classes mix in the ability to add modifications at named sites to
// mols.  The mol base class is a fundamental class for dealing with
// essentially symmetric binding between complexes.  Modifications are
// for dealing with binding with entities that are treated in the
// simulation as featureless, except for molecular weight, typically
// small molecules.  Binding to ATP or phosphate are important cases
// in our current simulations.
//
// There is one mixin for the mol class and another for the molState
// class.

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "cpx/modification.hh"
#include "cpx/modStateMixin.hh"

namespace cpx
{
    // This mixin adds the ability to access modification states.
    //
    // An elaboration here would be for each modification site to have a list of
    // acceptable modifications.
    class modMolMixin
    {
    public:
        // These mappings allow the modification sites of the mol to be accessed
        // by name or by index.
        std::map<std::string, int> modSiteNameToNdx;
        std::vector<std::string> modSiteNames;
        
        // This may be superfluous and repeated elsewhere, however, I cannot find it.
        // If there is an easier/better way to get at the default modifications, someone
        // let me know and remove this.  That said, it really isn't that heavyweight.
        // Probably adds 3 bytes per modification.
        // std::vector<const cpx::modification*> defaultModifications;
        
        // Look up modification site index by name.  Returns true if there
        // is a modification site with the given name, and returns the index
        // at the given reference. Otherwise, returns false.
        bool
        getModSiteNdx( const std::string& rModSiteName,
                       int& rSiteNdx ) const;


        // Removed this code because it doesn't seem to be used anywhere.        
//         const cpx::modification*
//         getDefaultModForSite( const std::string& modSiteName ) const
//         {
//             int ndx = modSiteNameToNdx.find( modSiteName )->second;
//             return defaultModifications[ ndx ];
//         }
        
//         const std::string&
//         getDefaultModNameForSite( const std::string& modSiteName ) const
//         {
//             int ndx = modSiteNameToNdx.find( modSiteName )->second;
//             return defaultModifications[ ndx ]->getName();
//         }
        
        int
        modSiteCount( void ) const
        {
            return modSiteNames.size();
        }
        
        // I will use a map (encoded into XML) from modification site names
        // to modification names in the following circumstances:
        //
        // 1. To simultaneously define the modification sites and give their
        // default modifications.
        //
        // 2. To specify a single modification state by giving its differences
        // from the default state.  That is, any blank entries are treated
        // as representing the default state.
        //
        // 3. To specify a collection of modification states by specifying
        // the modifications at some of the sites.  Blank entries are
        // treated as wildcards; i.e. no filtering is done on unspecified
        // modification sites.  I will have to be able to "or" these
        // together.
        
        // Constructs the mol's map from modification site names to modification
        // site indices.  This accomplishes part of goal 1 above; the rest
        // of goal 1 comes from also using indexModMap below.
        modMolMixin
        ( const std::map<std::string, const modification*>& rDefaultModMap )
            throw( utl::xcpt );
        
        // Constructs a modStateMixin by doing substitutions on a provided
        // modStateMixin.  This would be used to construct a state from
        // a map and the default state.  This accomplishes goal 2 above.
        modStateMixin
        substituteModMap
        ( const std::map<
          std::string, const modification*>& rModMap,
          const modStateMixin& rSourceStateMixin )
            throw( utl::xcpt );
        
        // Uses the map from modification site names to modification
        // site indices to convert a map from modification site names
        // to modifications into a vector of modification*'s.
        //
        // If any modification site is unmapped, then the corresponding
        // pointer in the modStateMixin will be null.  This makes this
        // function useful for constructing "regexp" modStateMixin's for use
        // with modStateMatch below.  This is the bulk of goal 3.
        //
        // Another use is in the construction of the modStateMixin
        // part of the default state.
        modStateMixin
        indexModMap( const std::map<std::string, const modification*>& rModMap )
            throw( utl::xcpt );
        
        // This predicate can be used to test modStateMixin's against a
        // "regexp" modStateMixin.  If a null pointer occurs as an entry in
        // the "regexp," then it matches any modification.  Otherwise the
        // modifications (modification*'s) must match exactly.
        //
        // The function indexModMap above should be useful for constructing
        // these "regexps", since null pointers will naturally appear at
        // unmapped indices.  Together with indexModMap above, this
        // accomplishes goal 3.
        class modStateMatch :
            public std::unary_function<modStateMixin, bool>
        {
            const modStateMixin& rMatch;
        public:
            modStateMatch( const modStateMixin& rMatchStateMixin ) :
                rMatch( rMatchStateMixin )
            {}
            
            bool
            operator()( const modStateMixin& rMixinToTest ) const;
        };
    };
}

#endif // MODMOLMIXIN_H
