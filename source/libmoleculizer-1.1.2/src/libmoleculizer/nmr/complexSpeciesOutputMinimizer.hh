//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//        This file is part of Libmoleculizer
//
//        Copyright (C) 2001-2008 The Molecular Sciences Institute.
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

#ifndef COMPLEXSPECIESOUTPUTMINIMIZER_HPP
#define COMPLEXSPECIESOUTPUTMINIMIZER_HPP


#include "complexSpecies.hh"
#include "permutation.hh"
#include <vector>
#include <set>


namespace nmr
{
    class ComplexSpeciesOutputMinimizer
    {
    public:
        
        typedef std::vector<int> ColoringPartition;
        typedef std::vector< std::set<int>* > GraphEdgeList;
        
        ComplexSpeciesOutputMinimizer()
        {}

						// This is the new function I'm adding.
						unsigned int getAutomorphismSize( ComplexSpeciesCref rComplexSpecies);
        
        ComplexOutputState
        getMinimalOutputState( ComplexSpeciesCref aComplexSpecies );
        
        class NonSimpleGraphXcpt : public GeneralNmrXcpt
        {
        public:
            static
            std::string
            mkMsg( ComplexSpeciesCref aComplexSpecies )
            {
                std::ostringstream oss;
                oss << "Error: the complex species '"
                    << aComplexSpecies
                    << "' does not represent a simple graph.  Please contact "
                    << "the developer <addy@molsci.org>.";
                return oss.str();
            }
            
            NonSimpleGraphXcpt( ComplexSpeciesCref aComplexSpecies )
                :
                GeneralNmrXcpt( mkMsg( aComplexSpecies ) )
            {}
        };
        
    private:
        std::string reprColorMap() const;
        std::string reprBindings() const;
        
        Permutation
        calculateMolSortingPermutationForComplex( ComplexSpeciesCref aComplexSpecies );

						unsigned int getAutomorphismGroupForColoredGraph( const GraphEdgeList& graphEdgeList,
																																																								const ColoringPartition& theColoring );
        
        Permutation
        calculateCanonicalPermutationForColoredGraph( const GraphEdgeList& refGraphEdgeList,
                                                      const ColoringPartition& refColPart );
        
        bool checkComplexSpeciesIsSimpleGraph( ComplexSpeciesCref aComplexSpecies );
        void setupDataStructuresForCalculation( ComplexSpeciesRef aComplexSpecies );
        void setupComplexEdgeMap( ComplexSpeciesCref aComplexSpecies );
        void setupComplexColorPartition( ComplexSpeciesCref aComplexSpecies );
        
        struct MolIndexLessThanCmp : public std::binary_function<int, int, bool>
        {
            DECLARE_TYPE( ComplexSpecies::MolList, MolList );
            
            MolIndexLessThanCmp( ComplexSpeciesCref aComplexSpeciesForCmp );
            bool operator()( int ndx1, int ndx2 );
            
        private:
            MolListCref theComparisonMolList;
        };
        
    private:
        ColoringPartition partitionSpecification;
        GraphEdgeList complexGraphEdgeMap;
        // std::vector<std::set<int>* > complexGraphEdgeMap;
    };
}

#endif
