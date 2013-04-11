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

#ifndef CPX_PLEXSPECIESIMPL_H
#define CPX_PLEXSPECIESIMPL_H

#include "binding.hh"

namespace cpx
{
    class addMolWeight :
        public std::unary_function<molParam, void>
    {
        double& rTotal;
    public:
        addMolWeight( double& refTotalMolWeight ) :
            rTotal( refTotalMolWeight )
        {
        }
        
        void
        operator()( molParam pState ) const
        {
            rTotal += pState->getMolWeight();
        }
    };
    
    template<class plexFamilyT>
    double
    plexSpeciesMixin<plexFamilyT>::
    getWeight( void ) const
    {
        double totalMolWeight = 0.0;
        
        std::for_each( molParams.begin(),
                       molParams.end(),
                       addMolWeight( totalMolWeight ) );
        
        return totalMolWeight;
    }
    
    template<class plexFamilyT>
    std::string
    plexSpeciesMixin<plexFamilyT>::
    getInformativeName( void ) const
    {
        // Let's start with the names of the mols in the order that
        // they appear in the pardigm, separated by colons.
        //
        // It might be nice to traverse the mols in some kind of "connectivity
        // order" later on, if called for.
        
        // Bad that the type "gets out" like this; could be avoided, of course,
        // by using lots more typedefs.
        typedef typename std::vector<typename plexFamilyT::molType*> molVectorType;
        
        const molVectorType& rMols
            = rFamily.getParadigm().mols;
        
        const std::vector<binding>& rBindings = rFamily.getParadigm().bindings;
        
        std::string theName;
        
        typename molVectorType::const_iterator iMol = rMols.begin();
        if ( rMols.end() != iMol )
        {
            typename plexFamilyT::molType* pMol = *iMol++;
            theName = pMol->getName();
            
            //             bnd::mzrModMol* pModMol = dynamic_cast<bnd::mzrModMol*>(pMol);
            //             if ( pMol )
            //             {
            //                 theName += '(' + pModMol->getInformativeModificationName() + ')';
            //             }
        }
        
        while ( rMols.end() != iMol )
        {
            typename plexFamilyT::molType* pMol = *iMol++;
            theName += "_";
            theName += pMol->getName();
            
            //             bnd::mzrModMol* pModMol = dynamic_cast<bnd::mzrModMol*>(pMol);
            //             if ( pMol )
            //             {
            //                 theName += '(' + pModMol->getInformativeModificationName() + ')';
            //             }
        }
        
        theName += "::";
        
        for ( std::vector<binding>::const_iterator iter = rBindings.begin();
              iter != rBindings.end();
              ++iter )
        {
            const binding& theBinding = *iter;
            theName += "( ";
            
            const typename plexFamilyT::molType& firstMol = *rMols[theBinding.first.first];
            const typename plexFamilyT::molType& secondMol = *rMols[theBinding.second.first];
            
            theName += firstMol.getName();
            theName += "//";
            theName += firstMol[theBinding.first.second].getName();
            
            theName += " -> ";
            
            theName += secondMol.getName();
            theName += "//";
            theName += secondMol[theBinding.second.second].getName();
            
            theName += ") ";
        }
        
        return theName;
    }
    
}

#endif // CPX_PLEXSPECIESIMPL_H
