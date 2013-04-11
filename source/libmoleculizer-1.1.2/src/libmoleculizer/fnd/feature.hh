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

#ifndef FEATURE_H
#define FEATURE_H

#include <vector>
#include <functional>
#include <algorithm>
#include "fnd/sensitivityList.hh"
#include "fnd/featureContext.hh"
#include "fnd/newContextStimulus.hh"
#include "fnd/rxnGen.hh"

namespace fnd
{
    template<class contextT>
    class feature :
        public sensitive<newContextStimulus<contextT> >,
        public sensitivityList<rxnGen<contextT> >
    {
    public:
        typedef contextT contextType;
        
    private:
        class respondRxnGen :
            public std::unary_function<rxnGen<contextT>*, void>
        {
            const featureStimulus<contextT> stim;
        public:
            respondRxnGen( const newContextStimulus<contextT>& rNewContextStim,
                           feature& rFeature ) :
                stim( rNewContextStim,
                      rFeature )
            {}
            
            void
            operator()( rxnGen<contextT>* pRxnGen ) const
            {
                pRxnGen->respond( stim );
            }
        };
        
    public:
        // This is very heavyweight, and not really needed???
        // This really is a service to the dimerization generator,
        // and other possible binary reaction generators, that they
        // could do for themselves.
        std::vector<contextT> contexts;
        
        virtual
        ~feature( void )
        {}
        
        // omniPlexFeatures respond differently, for example.
        virtual
        void
        respond( const newContextStimulus<contextT>& rStimulus )
        {
            contexts.push_back( rStimulus.getContext() );
            this->forEachSensitive( respondRxnGen( rStimulus,
																																																			*this ) );
        }

        // This is part of the refactoring of things.
        virtual void
        dumpablesRespond( const newContextStimulus<contextT>& rStimulus )
        {
            return;
        }
    };
}

#endif
