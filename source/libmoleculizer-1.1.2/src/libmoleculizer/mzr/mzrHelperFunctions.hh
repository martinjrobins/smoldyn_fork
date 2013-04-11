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


#ifndef MZRHELPERFUNCTIONS_HH
#define MZRHELPERFUNCTIONS_HH

#include <iostream>
#include "mzrSpecies.hh"

namespace mzr
{
    namespace aux
    {
        
        class checkIfSpeciesPtrIsLive
        {
        public:
            bool operator()( const mzr::mzrSpecies* pSpecies )
            {
                return ( !pSpecies->hasNotified() );
            }
        };
        
        class checkIfSpeciesListCatalogPtrIsLive
        {
        public:
            bool operator()( const std::pair<std::string*, mzr::mzrSpecies*>& theObj )
            {
                return ( ! theObj.second->hasNotified() );
            }
        };
        
        // My first thought is that it seems like templatizing operator() is a better
        // strategy, as this means users don't have to provide template paramaters.
        
        // On the other hand, it means we cannot derive from std::unary_function...
        // TODO -- the derivation
        class printPtrWithName
        {
        public:
            
            template <typename T>
            void
            operator()( const T* pObjectWithName ) const
            {
                std::cout << pObjectWithName->getName() << std::endl;
            }
        };
        
        class printPtrWithIndexedName
        {
        public:
            
            template <typename T>
            void
            operator()( const T* pObjectWithName )
            {
                std::cout << ndx++ << ":\t" << pObjectWithName->getName() << std::endl;
            }
            
        private:
            unsigned int ndx;
        };
        
        class getListOfNodes
        {
        public:
            getListOfNodes( std::vector<std::string*>& theVect )
                :
                theStringVector( theVect )
            {}
            
            void
            operator()( mzrReaction* rxn )
            {
                
                // All the following generates a vector of reactants and products.
                std::vector<std::string> reactants, products;
                
                for ( mzrReaction::multMap::const_iterator iter = rxn->getReactants().begin();
                      iter != rxn->getReactants().end();
                      ++iter )
                {
                    reactants.push_back( iter->first->getName() );
                }
                
                for ( mzrReaction::multMap::const_iterator iter = rxn->getProducts().begin();
                      iter != rxn->getProducts().end();
                      ++iter )
                {
                    products.push_back( iter->first->getName() );
                }
                
                
                std::string theReactant, theProduct;
                for ( unsigned int ii = 0; ii != reactants.size(); ++ii )
                {
                    for ( unsigned int jj = 0; jj != products.size(); ++jj )
                    {
                        theReactant = reactants[ii];
                        theProduct = products[jj];
                        
                        std::string* ptrOutputString = new std::string;
                        
                        theStringVector.push_back( ptrOutputString );
                        
                        if ( theReactant < theProduct )
                        {
                            ptrOutputString->append( theReactant );
                            ptrOutputString->append( "--" );
                            ptrOutputString->append( theProduct );
                        }
                        else
                        {
                            ptrOutputString->append( theProduct );
                            ptrOutputString->append( "--" );
                            ptrOutputString->append( theReactant );
                        }
                        
                    }
                }
            }
            
        private:
            std::vector<std::string*>& theStringVector;
        };
        
    }
    
}

#endif
