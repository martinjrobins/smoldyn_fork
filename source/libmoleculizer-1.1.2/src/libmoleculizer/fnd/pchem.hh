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

#ifndef FND_PCHEM_H
#define FND_PCHEM_H

namespace fnd
{
    /*! \defgroup chemGroup Chemistry
      \ingroup mzrGroup
      \brief Formulae and constants for extrapolating reactions. */
    
    /*! \file pchem.hh
      \ingroup chemGroup
      \brief Reaction rate corrections by molecular weight. */
    
    /*! \ingroup chemGroup
      \brief Basic rate correction by molecular weight. */
    double
    reducedMass( double leftMass,
                 double rightMass );
    
    /*! \ingroup chemGroup
      \brief Converts binding rate to weight-invariant form. */
    double
    bindingInvariant( double bindingRate,
                      double leftMass,
                      double rightMass );
    
    /*! \ingroup chemGroup
      \brief Converts weight-invariant for binding to rate. */
    double
    bindingRate( double bindingInvariant,
                 double leftMass,
                 double rightMass );
    
    /*! \ingroup chemGroup
      \brief Converts dissociation constant to weight-invariant form.
      
      This is done by following through on the changes in the reaction
      rates to get the change in dissociation constant. */
    double
    dissocInvariant( double dissocConst,
                     double leftMass,
                     double rightMass );
    
    /*! \ingroup chemGroup
      \brief Converts weight-invariant to dissociation constant. */
    double
    dissocConst( double dissocInvariant,
                 double leftMass,
                 double rightMass );
    
    /*! \ingroup chemGroup
      \brief Converts Michaelis constant to weight-invariant form.
      
      This is done by following through on the changes in the reaction
      rates to get the change in Michalis constant. */
    double
    michaelisInvariant( double michaelisConst,
                        double leftMass,
                        double rightMass );
    
    /*! \ingroup chemGroup
      \brief Converts weight-invariant to Michaelis constant. */
    double
    michaelisConst( double michaelisInvariant,
                    double leftMass,
                    double rightMass );
}

#endif // FND_PCHEM_H
