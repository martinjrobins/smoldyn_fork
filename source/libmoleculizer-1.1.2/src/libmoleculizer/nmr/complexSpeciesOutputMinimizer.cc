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


#include "nauty/nauty.h"
#include "complexSpeciesOutputMinimizer.hh"
#include <iostream>
#include <sstream>
#include <cmath>

namespace nmr
{

    std::string ComplexSpeciesOutputMinimizer::reprColorMap() const
    {
        std::ostringstream oss;
        
        for (unsigned int ndx = 0; ndx != partitionSpecification.size(); ++ndx)
        {
            oss << partitionSpecification[ndx] << "-";
        }
        return oss.str();
    }
    
    std::string ComplexSpeciesOutputMinimizer::reprBindings() const
    {
        std::ostringstream oss;
        
        for(unsigned int ndx = 0; ndx != complexGraphEdgeMap.size(); ++ndx)
        {
            oss << '(' << ndx << ": ";
            
            for (std::set<int>::const_iterator setndx = complexGraphEdgeMap[ndx]->begin();
                 setndx != complexGraphEdgeMap[ndx]->end(); ++setndx)
            {
                oss << *setndx << ", ";
            }
            
            oss <<  ')';
        }
        
        return oss.str();
    }
    
    
		unsigned int
		ComplexSpeciesOutputMinimizer::getAutomorphismSize( ComplexSpeciesCref rComplexSpecies)
		{
        ComplexSpecies aComplexSpecies( rComplexSpecies );
        
        setupDataStructuresForCalculation( aComplexSpecies );
        
        unsigned int auto_sz =
            getAutomorphismGroupForColoredGraph( complexGraphEdgeMap,
																																																	partitionSpecification );
        
        return auto_sz;
		}
    
    
    
    ComplexOutputState
    ComplexSpeciesOutputMinimizer::getMinimalOutputState( ComplexSpeciesCref theComplexSpecies )
    {
        
        ComplexSpecies aComplexSpecies( theComplexSpecies );
        
        setupDataStructuresForCalculation( aComplexSpecies );
        
        ComplexOutputState debugOutputState;
        aComplexSpecies.constructOutputState(debugOutputState);
        // hoho -- DEBUG
        // std::cout << "Post Scrambled: " << debugOutputState.repr() << std::endl;
        // std::cout << "Color Map: " << this->reprColorMap() << std::endl;
        // std::cout << "Bindings: " << this->reprBindings() << std::endl;
        
        Permutation theMinimizingPermutation =
            calculateCanonicalPermutationForColoredGraph( complexGraphEdgeMap,
                                                          partitionSpecification );
        
        //std::cout << "Permutation: " << theMinimizingPermutation.repr() << std::endl;
        //std::cout << "PrePerm:" << aComplexSpecies << std::endl;
        aComplexSpecies.applyPermutationToComplex( theMinimizingPermutation );
        //std::cout << "PostPerm:" << aComplexSpecies << std::endl;
        // debugOutputState.applyPermutationToComplex(theMinimizingPermutation );
        
        ComplexOutputState theCanonicalOutputState;
        aComplexSpecies.constructOutputState( theCanonicalOutputState );
        return theCanonicalOutputState;
        
    }
    
    void
    ComplexSpeciesOutputMinimizer::setupDataStructuresForCalculation( ComplexSpeciesRef aComplexSpecies )
    {
        
        // Ensure graph is standard, with at most one binding between any two mols in the complex.
        if ( !checkComplexSpeciesIsSimpleGraph( aComplexSpecies ) ) throw NonSimpleGraphXcpt( aComplexSpecies );
        
        Permutation molOrderingPermutation = calculateMolSortingPermutationForComplex( aComplexSpecies );
        aComplexSpecies.applyPermutationToComplex( molOrderingPermutation );
        
        setupComplexEdgeMap( aComplexSpecies );
        setupComplexColorPartition( aComplexSpecies );
        return;
    }
    
    void ComplexSpeciesOutputMinimizer::setupComplexEdgeMap( ComplexSpeciesCref aComplexSpecies )
    {
        
        for(GraphEdgeList::iterator i = complexGraphEdgeMap.begin();
            i != complexGraphEdgeMap.end();
            ++i)
        {
            delete (*i);
        }
        
        for ( unsigned int ii = 0; ii != aComplexSpecies.getNumberOfMolsInComplex(); ++ii )
        {
            complexGraphEdgeMap.push_back( new std::set<int>() );
        }
        
        typedef std::pair<int, int> KeyType;
        
        
        ComplexSpecies::BindingListCref theBindingList = aComplexSpecies.getBindingList();
        
        // Create the edge-map for the complex.
        for ( ComplexSpecies::BindingList::const_iterator bndIter = theBindingList.begin();
              bndIter != theBindingList.end();
              ++bndIter )
        {
            
            int molNdx1 = ( *bndIter ).second.first;
            int molNdx2 = ( *bndIter ).first.first;
            
            complexGraphEdgeMap[molNdx1]->insert( molNdx2 );
            complexGraphEdgeMap[molNdx2]->insert( molNdx1 );
        }
    }
    
    void ComplexSpeciesOutputMinimizer::setupComplexColorPartition( ComplexSpeciesCref aComplexSpecies )
    {
        int complexSize = aComplexSpecies.getNumberOfMolsInComplex();
        
        partitionSpecification.clear();
        partitionSpecification.reserve( complexSize );
        partitionSpecification.resize( complexSize );
        
        ComplexSpecies::MolListCref theMolList = aComplexSpecies.getMolList();
        
        std::string current_name( "This cannot possibly be a mol-name" );
        
        std::string description_1, description_2;
        
        for ( int molNdx = 0; molNdx != ( complexSize - 1 ); ++molNdx )
        {
            
            description_1 = theMolList[molNdx]->getMolDescriptor();
            description_2 = theMolList[molNdx + 1]->getMolDescriptor();
            
            if ( theMolList[molNdx]->getMolDescriptor() != theMolList[molNdx + 1]->getMolDescriptor() )
            {
                partitionSpecification[molNdx] = 0;
            }
            else
            {
                partitionSpecification[molNdx] = 1;
            }
        }
        
        // The last item ALWAYS ends a cell.
        partitionSpecification[complexSize - 1] = 0;
        
        return;
    }
    
    
    bool
    ComplexSpeciesOutputMinimizer::checkComplexSpeciesIsSimpleGraph( ComplexSpeciesCref aComplexSpecies )
    {
        typedef std::pair<int, int> IntPair;
        
        std::map<IntPair, int> bindingsBetweenMap;
        for ( int firstNdx = 0; firstNdx != aComplexSpecies.getNumberOfMolsInComplex(); ++firstNdx )
        {
            for ( int secondNdx = 0; secondNdx != aComplexSpecies.getNumberOfMolsInComplex(); ++secondNdx )
            {
                IntPair theKey = std::make_pair( firstNdx, secondNdx );
                int theValue = 0;
                
                bindingsBetweenMap.insert( std::make_pair( theKey, theValue ) );
            }
        }
        
        
// Now iterate through each of the bindings and count up the number of connections
        ComplexSpecies::BindingListCref theBindingList = aComplexSpecies.getBindingList();
        for ( ComplexSpecies::BindingList::const_iterator bndIter = theBindingList.begin();
              bndIter != theBindingList.end();
              ++bndIter )
        {
            int bindingIndex1 = ( *bndIter ).first.first;
            int bindingIndex2 = ( *bndIter ).second.first;
            
// This means that there is a binding from a mol to itself within a complex species.
            if ( bindingIndex1 == bindingIndex2 ) return false;
            
            IntPair theKey;
            
            if ( bindingIndex2 < bindingIndex1 ) theKey = std::make_pair( bindingIndex2, bindingIndex2 );
            else theKey = std::make_pair( bindingIndex1, bindingIndex2 );
            
            if ( bindingsBetweenMap[theKey] == 1 ) return false;
            else bindingsBetweenMap[theKey] = 1;
        }
        
        return true;
    }
    
    Permutation
    ComplexSpeciesOutputMinimizer::calculateCanonicalPermutationForColoredGraph( const GraphEdgeList& graphEdgeList,
                                                                                 const ColoringPartition& theColoring )
    {
        static DEFAULTOPTIONS_GRAPH( options );
        options.getcanon = TRUE;
        options.defaultptn = FALSE;
        
        DYNALLSTAT( graph,g,g_sz );
        DYNALLSTAT( int,lab,lab_sz );
        DYNALLSTAT( int,ptn,ptn_sz );
        DYNALLSTAT( int,orbits,orbits_sz );
        DYNALLSTAT( setword,workspace,workspace_sz );
        DYNALLSTAT( graph, canong, canong_sz );
        
        statsblk stats;
        
        int n = theColoring.size();
        int m = ( n + WORDSIZE - 1 ) / WORDSIZE;
        
        set *gv;
        
// This dynamically allocates g, the workspace, lab, ptn, and orbits
        
        char error[] = "malloc";
        DYNALLOC2( graph,g,g_sz,m,n,error );
        DYNALLOC1( setword, workspace, workspace_sz, 50*m, error );
        DYNALLOC1( int, lab, lab_sz, n, error );
        DYNALLOC1( int, ptn, ptn_sz, n, error );
        DYNALLOC1( int, orbits, orbits_sz, n, error );
        DYNALLOC2( graph, canong, canong_sz, m, n, error );
        
// Create the graph here.
        for ( int vertexNumber = 0; vertexNumber != n; ++vertexNumber )
        {
            gv = GRAPHROW( g,vertexNumber,m );
            EMPTYSET( gv, m );
            
            for ( std::set<int>::const_iterator iter = complexGraphEdgeMap[vertexNumber]->begin();
                  iter != complexGraphEdgeMap[vertexNumber]->end();
                  ++iter )
            {
                ADDELEMENT( gv, *iter );
            }
        }
        
// Create the coloring partition here.
        for ( unsigned int ii = 0; ii != n; ++ii )
        {
            lab[ii] = ii;
            ptn[ii] = partitionSpecification[ii];
        }
        
        nauty( g, lab, ptn, NULL, orbits, &options, &stats, workspace, 50*m, m, n, canong );
        
        std::vector<int> basicPerm( n, 0 );
        
        for ( unsigned int ndx = 0;
              ndx != n;
              ++ndx )
        {
            basicPerm[ndx] = lab[ndx];
        }
        
        DYNFREE( g, g_sz );
        DYNFREE( lab, lab_sz );
        DYNFREE( ptn, ptn_sz );
        DYNFREE( orbits, orbits_sz );
        DYNFREE( workspace, workspace_sz );
        DYNFREE( canong, canong_sz );
        
        return Permutation( basicPerm );
    }

		unsigned int
		ComplexSpeciesOutputMinimizer::getAutomorphismGroupForColoredGraph( const GraphEdgeList& graphEdgeList,
																																																																						const ColoringPartition& theColoring )
    {

						// This is the function which should get everything out.
        static DEFAULTOPTIONS_GRAPH( options );
        options.getcanon = TRUE;
        options.defaultptn = FALSE;
        
        DYNALLSTAT( graph,g,g_sz );
        DYNALLSTAT( int,lab,lab_sz );
        DYNALLSTAT( int,ptn,ptn_sz );
        DYNALLSTAT( int,orbits,orbits_sz );
        DYNALLSTAT( setword,workspace,workspace_sz );
        DYNALLSTAT( graph, canong, canong_sz );
        
        statsblk stats;
        
        int n = theColoring.size();
        int m = ( n + WORDSIZE - 1 ) / WORDSIZE;
        
        set *gv;
        
								// This dynamically allocates g, the workspace, lab, ptn, and orbits
        char error[] = "malloc";
        DYNALLOC2( graph,g,g_sz,m,n,error );
        DYNALLOC1( setword, workspace, workspace_sz, 50*m, error );
        DYNALLOC1( int, lab, lab_sz, n, error );
        DYNALLOC1( int, ptn, ptn_sz, n, error );
        DYNALLOC1( int, orbits, orbits_sz, n, error );
        DYNALLOC2( graph, canong, canong_sz, m, n, error );
        
								// Create the graph here.
        for ( int vertexNumber = 0; vertexNumber != n; ++vertexNumber )
        {
            gv = GRAPHROW( g,vertexNumber,m );
            EMPTYSET( gv, m );
            
            for ( std::set<int>::const_iterator iter = complexGraphEdgeMap[vertexNumber]->begin();
                  iter != complexGraphEdgeMap[vertexNumber]->end();
                  ++iter )
            {
                ADDELEMENT( gv, *iter );
            }
        }
        
								// Create the coloring partition here.
        for ( unsigned int ii = 0; ii != n; ++ii )
        {
            lab[ii] = ii;
            ptn[ii] = partitionSpecification[ii];
        }

								// I think the automorphism goes somewhere here...
        nauty( g, lab, ptn, NULL, orbits, &options, &stats, workspace, 50*m, m, n, canong );

								// This is the size of the automorphism group.
								double grpsize1, grpsize2;
								grpsize1 = stats.grpsize1;
								grpsize2 = stats.grpsize2;
								double auto_sz_dbl = grpsize1 * std::pow(10.0, grpsize2);
								unsigned int auto_sz = static_cast<unsigned int>(auto_sz_dbl);
        
        DYNFREE( g, g_sz );
        DYNFREE( lab, lab_sz );
        DYNFREE( ptn, ptn_sz );
        DYNFREE( orbits, orbits_sz );
        DYNFREE( workspace, workspace_sz );
        DYNFREE( canong, canong_sz );
        
        return auto_sz;
    }

    
    Permutation
    ComplexSpeciesOutputMinimizer::calculateMolSortingPermutationForComplex( ComplexSpeciesCref aComplexSpecies )
    {
        //initialize a vector with entries 0...size-1
        std::vector<int> permutationVect;
        for ( unsigned int i=0;
              i != aComplexSpecies.getNumberOfMolsInComplex();
              ++i )
        {
            permutationVect.push_back( i );
        }
        
        //Sort permutation using a function which compares the theMols belonging at that index.
        std::sort( permutationVect.begin(),
                   permutationVect.end(),
                   MolIndexLessThanCmp( aComplexSpecies ) );
        
        Permutation inversePerm = Permutation( permutationVect );
        Permutation forwardPerm = inversePerm.invertPermutation();
        return forwardPerm;
    }
    
    
    ComplexSpeciesOutputMinimizer::MolIndexLessThanCmp::MolIndexLessThanCmp( ComplexSpeciesCref aComplexSpeciesForCmp )
        :
        theComparisonMolList( aComplexSpeciesForCmp.getMolList() )
    {}
    
    bool
    ComplexSpeciesOutputMinimizer::MolIndexLessThanCmp::operator()( int ndx1, int ndx2 )
    {
        return *theComparisonMolList[ndx1] < *theComparisonMolList[ndx2];
    }
    
}
