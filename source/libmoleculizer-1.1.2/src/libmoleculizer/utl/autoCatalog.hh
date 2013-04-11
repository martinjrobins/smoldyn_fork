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

#ifndef UTL_AUTOCATALOG_H
#define UTL_AUTOCATALOG_H

#include <map>


namespace utl
{
    template<class catObject>
    class catalog :
        public std::map<std::string, catObject*>
    {
    public:
        catObject*
        findEntry( const std::string& rObjectName ) const
        {
            typename catalog::const_iterator iNamePtrPair;
            iNamePtrPair = this->find( rObjectName );
            return this->end() == iNamePtrPair
                ? 0
                : iNamePtrPair->second;
        }
        
        /*!  Returns false if the insert fails because there is already an
          entry with the given name. */
        bool
        addEntry( const std::string& rObjectName,
                  catObject* pObject )
        {
            typename std::pair<typename catalog::iterator, bool> insertResult
                = this->insert( typename catalog::value_type( rObjectName,pObject ) );
            return insertResult.second;
        }
    };
    
    template<class catObject>
    class autoCatalog
        : public catalog<catObject>
    {
        class doDelete
            : public std::unary_function<typename autoCatalog::value_type, void>
        {
        public:
            void operator()( const
                             typename doDelete::argument_type&
                             rNamePtrPair ) const
            {
                delete rNamePtrPair.second;
            }
        };
        
    public:
        ~autoCatalog( void )
        {
            for_each( this->begin(),
                      this->end(),
                      doDelete() );
        }
    };
}

#endif // UTL_AUTOCATALOG_H


