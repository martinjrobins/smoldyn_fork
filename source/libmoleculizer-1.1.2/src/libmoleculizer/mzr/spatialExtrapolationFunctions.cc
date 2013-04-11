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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2008
//
// Modifing Authors:
//
//

#include "spatialExtrapolationFunctions.hh"
#include "mzr/mzrSpecies.hh"
#include "mzr/mzrReaction.hh"
#include "mzr/mzrException.hh"
#include <cmath>

namespace mzr
{
    
    double getProteinDiffusionCoeff()
    {
        // Units returned are in micrometers^2/sec.
        return proteinDiffusionCoeff;
    }
    
    double getSmallMolDiffusionCoeff()
    {
        // Units returned are in micrometers^2/sec.
        return smallMolDiffusionCoeff;
    }
    
    
    void setProteinDiffusionCoeff(double rate)
    {
        // Units must be in micrometer^2/sec.
        proteinDiffusionCoeff = rate;
    }
    
    void setSmallMolDiffusionCoeff(double rate)
    {
        // Units must be in micrometer^2/sec.
        smallMolDiffusionCoeff = rate;
    }
    
    double calculateSumOfRadii( const mzr::mzrReaction* pRxn )
    {
        double theSum = 0.0f;
        
        typedef std::map<mzr::mzrSpecies*, int> multMap;
        for( multMap::const_iterator mmIter = pRxn->getReactants().begin();
             mmIter != pRxn->getReactants().end();
             ++mmIter)
        {
            theSum += (mmIter->second) * extrapolateMolecularRadius( mmIter->first );
        }

        return theSum;
    }

    double getSmallMolProteinCutoff()
    {
        return smallMolProteinCutoff;
    }

    void setSmallMolProteinCutoff(const double& cut) 
    {
        smallMolProteinCutoff = cut;
    }
    
    double getDiffusionCoeffForSpecies( const mzr::mzrSpecies* pSpecies)
    {
        // This is kind of hacky but probably works.  I am using 1500 daltons
        // as the cutoff for small molecules and proteins.  
        
        if ( pSpecies->getWeight() > getSmallMolProteinCutoff() )
        {
            return getSmallMolDiffusionCoeff();
        }
        else
        {
            return getProteinDiffusionCoeff();
        }
    }
    
    double calculateSumOfDiffusionCoefficients( const mzr::mzrReaction* pRxn )
    {
        double theSum = 0.0f;
        
        typedef std::map<mzr::mzrSpecies*, int> multMap;
        for( multMap::const_iterator mmIter = pRxn->getReactants().begin();
             mmIter != pRxn->getReactants().end();
             ++mmIter)
        {
            theSum += (mmIter->second) * getDiffusionCoeffForSpecies( mmIter->first );
        }
        
        return theSum;
    }
    
    double extrapolateIntrinsicReactionRate(const mzr::mzrReaction* pRxn)
    {
        if (pRxn->getArity() < 2) throw badPreconditionXcpt("Error in calculating the intrinsic reaction rate for reaction '" + pRxn->getName() + "'.  Only reactions with exactly two substrates can have an intrinsic reaction rate.");

        static const double FourPi = 4.0f * 3.141592653859;
        
        // We have to solve the equation 1/k = 1/kA + 1/ kD for kA, where 
        double k = pRxn->getRate();
        double kD = FourPi * calculateSumOfRadii(pRxn) * calculateSumOfDiffusionCoefficients( pRxn );
        
        return ( k * kD ) / ( kD - k );
    }
    
    double extrapolateMolecularRadius( const mzr::mzrSpecies* pSpecies)
    {
        // This number is obtained by converting to angstroms to m^3 by using the conversion
        // factor 1.22 g/cm^3 and then solving for R in the equation V = 4/3 * Pi * R^3
        const double Factor( 3.0774371428132849e30 );
        const double radical( 1.0 / 3.0 );
        
        double newSpeciesConvertedMass = pSpecies->getWeight() / Factor;
        return std::pow( newSpeciesConvertedMass, radical );
    }
    
    
    double extrapolateMolecularRadius(const double& mass)
    {
        // This number is obtained by converting to angstroms to m^3 by using the conversion
        // factor 1.22 g/cm^3 and then solving for R in the equation V = 4/3 * Pi * R^3
        const double Factor( 3.0774371428132849e30 );
        const double radical( 1.0 / 3.0 );
        
        double newSpeciesConvertedMass = mass / Factor;
        return std::pow( newSpeciesConvertedMass, radical );
        
    }

    double smallMolProteinCutoff = 1500.0f;
    double proteinDiffusionCoeff = 3.0f;
    double smallMolDiffusionCoeff = 100.0f;
}

