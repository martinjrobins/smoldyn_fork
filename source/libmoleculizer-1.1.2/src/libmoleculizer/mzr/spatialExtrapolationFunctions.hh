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

#ifndef SPATIALEXTRAPOLATION_HH
#define SPATIALEXTRAPOLATION_HH

namespace mzr
{
    class mzrSpecies;
    class mzrReaction;
    
    // Units returned are in micrometers^2/sec.
    double getDiffusionCoeffForSpecies( const mzr::mzrSpecies* pSpecies);
    double getProteinDiffusionCoeff();
    double getSmallMolDiffusionCoeff();
    
    // Units must be in micrometer^2/sec.
    void setProteinDiffusionCoeff(double rate);
    void setSmallMolDiffusionCoeff(double rate);
    
    double extrapolateIntrinsicReactionRate(const mzr::mzrReaction* pRxn);

    inline double getSmallMolProteinCutoff();
    void setSmallMolProteinCutoff(const double& cut);
    
    double extrapolateMolecularRadius( const mzr::mzrSpecies* pSPecies);
    double extrapolateMolecularRadius(const double& mass);

    extern double proteinDiffusionCoeff;
    extern double smallMolDiffusionCoeff;
    extern double smallMolProteinCutoff;
    

    
}


#endif
