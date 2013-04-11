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

#include <sstream>
#include "cpx/modMolMixin.hh"

namespace cpx
{
    class duplicateModSiteNameXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rBadModSiteName )
        {
            std::ostringstream msgStream;
            msgStream << "Duplicate modification site name `"
                      << rBadModSiteName
                      << "'.";
            return msgStream.str();
        }
        
    public:
        duplicateModSiteNameXcpt( const std::string& rBadModSiteName ) :
            utl::xcpt( mkMsg( rBadModSiteName ) )
        {}
    };
    
    modMolMixin::
    modMolMixin( const std::map<std::string, const modification*>& rDefaultModMap )
        throw( utl::xcpt )
    {
        for ( std::map<std::string, const modification*>::const_iterator
                  iModEntry = rDefaultModMap.begin();
              iModEntry != rDefaultModMap.end();
              ++iModEntry )
        {
            const std::string& rSiteName = iModEntry->first;
            int siteNdx = modSiteNames.size();
            
            // Checking uniqueness of modification site names.
            bool insertOk
                = modSiteNameToNdx.insert( std::make_pair( rSiteName,
                                                           siteNdx ) ).second;
            if ( insertOk )
            {
                modSiteNames.push_back( rSiteName );
            }
            else
            {
                throw duplicateModSiteNameXcpt( rSiteName );
            }
        }
        
        // Removed this code because it doesn't seem to be used anywhere.
        
        //         // Now we construct the defaultModifications vector.
        //         typedef std::pair<std::string, const modification*> DefaultModMapPairType;
        //         BOOST_FOREACH( const DefaultModMapPairType& nameDefaultModPair, rDefaultModMap )
        //         {
        //             int index = modSiteNameToNdx[ nameDefaultModPair.first];
        //             defaultModifications[index] = nameDefaultModPair.second;
        //         }
        
    }
    
    class unknownModSiteXcpt :
        public utl::xcpt
    {
        static std::string
        mkMsg( const std::string& rModSiteName )
        {
            std::ostringstream msgStream;
            msgStream << "Mod-mol (name unknown) "
                      << "has no modification site named `"
                      << rModSiteName
                      << "'.";;
            return msgStream.str();
        }
    public:
        unknownModSiteXcpt( const std::string& rModSiteName ) :
            utl::xcpt( mkMsg( rModSiteName ) )
        {}
    };
    
    class modSubstituter :
        public std::unary_function<std::pair<std::string, const modification*>, void>
    {
        const modMolMixin& rModMol;
        modStateMixin& rTarget;
    public:
        modSubstituter( const modMolMixin& rModMolMixin,
                        modStateMixin& rTargetStateMixin ) :
            rModMol( rModMolMixin ),
            rTarget( rTargetStateMixin )
        {}
        
        void
        operator()( const std::pair<std::string, const modification*>& rEntry ) const
            throw( utl::xcpt )
        {
            const std::string& rSiteName = rEntry.first;
            const modification* pMod = rEntry.second;
            
            std::map<std::string, int>::const_iterator iCatEntry
                = rModMol.modSiteNameToNdx.find( rSiteName );
            
            // I don't really have a modMol here, just a modMolMixin, so I can't
            // get the name of the mol.
            if ( iCatEntry == rModMol.modSiteNameToNdx.end() )
                throw unknownModSiteXcpt( rSiteName );
            
            int modNdx = iCatEntry->second;
            
            rTarget[modNdx] = pMod;
        }
    };
    
    modStateMixin modMolMixin::
    substituteModMap( const std::map<std::string, const modification*>& rModMap,
                      const modStateMixin& rSourceStateMixin )
        throw( utl::xcpt )
    {
        modStateMixin resultStateMixin( rSourceStateMixin );
        
        for_each( rModMap.begin(),
                  rModMap.end(),
                  modSubstituter( *this,
                                  resultStateMixin ) );
        
        return resultStateMixin;
    }
    
    // modMixin.hh has notes on this.
    modStateMixin modMolMixin::
    indexModMap( const std::map<std::string, const modification*>& rModMap )
        throw( utl::xcpt )
    {
        modStateMixin resultStateMixin( 0, modSiteNames.size() );
        
        for_each( rModMap.begin(),
                  rModMap.end(),
                  modSubstituter( *this,
                                  resultStateMixin ) );
        
        return resultStateMixin;
    }
    
    // modMixin.hh has notes on this.
    bool
    modMolMixin::modStateMatch::
    operator()( const modStateMixin& rMixinToTest ) const
    {
        int modNdx = rMatch.size();
        while ( 0 < modNdx-- )
        {
            const modification* pMatchMod = rMatch[modNdx];
            
            if (( 0 != pMatchMod )
                && ( rMixinToTest[modNdx] != pMatchMod ) ) return false;
        }
        return true;
    }
    
    bool
    modMolMixin::
    getModSiteNdx( const std::string& rModSiteName,
                   int& rSiteNdx ) const
    {
        std::map<std::string, int>::const_iterator iEntry
            = modSiteNameToNdx.find( rModSiteName );
        
        if ( modSiteNameToNdx.end() == iEntry )
        {
            return false;
        }
        else
        {
            rSiteNdx = iEntry->second;
            return true;
        }
    }
    
    
}
 
