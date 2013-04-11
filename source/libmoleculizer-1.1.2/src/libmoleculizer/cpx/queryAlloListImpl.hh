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

namespace cpx
{
    template<class plexSpeciesT,
             class omniPlexT>
    void
    queryAllosteryList<plexSpeciesT,
                       omniPlexT>::
    addQueryAndMap( const queryType* pQuery,
                    const siteToShapeMap& rSiteToShapeMap )
    {
        push_back( std::make_pair( pQuery, rSiteToShapeMap ) );
    }
    
    template<class plexSpeciesT,
             class omniPlexT>
    class setShapesIfSatisfied :
        public std::unary_function<typename queryAllosteryList<plexSpeciesT,
                                                               omniPlexT>::value_type,
                                   void>
    {
        plexSpeciesT& rSpecies;
        
    public:
        setShapesIfSatisfied( plexSpeciesT& refSpecies ) :
            rSpecies( refSpecies )
        {}
        
        void
        operator()( const typename setShapesIfSatisfied::argument_type& rQueryShapeMapPair ) const
        {
            const andPlexQueries<plexSpeciesT, omniPlexT>& rQuery
                = * ( rQueryShapeMapPair.first );
            
            if ( rQuery( rSpecies ) )
            {
                const siteToShapeMap& rSiteToShapeMap = rQueryShapeMapPair.second;
                rSpecies.siteParams.setSiteShapes( rSiteToShapeMap );
            }
        }
    };
    
    template<class plexSpeciesT,
             class omniPlexT>
    void
    queryAllosteryList<plexSpeciesT,
                       omniPlexT>::
    setSatisfiedQuerySiteShapes( plexSpeciesT& rSpecies ) const
    {
        setShapesIfSatisfied<plexSpeciesT, omniPlexT>
            setShapes( rSpecies );
        
        std::for_each( this->begin(),
                       this->end(),
                       setShapes );
    }
    
    template<class plexSpeciesT,
             class omniPlexT>
    class setShapesIfSatisfiedTracked :
        public std::unary_function<typename queryAllosteryList<plexSpeciesT,
                                                               omniPlexT>::value_type,
                                   void>
    {
    public:
        typedef subPlexSpec<omniPlexT> specType;
        typedef andPlexQueries<plexSpeciesT, omniPlexT> queryType;
        
    private:
        plexSpeciesT& rSpecies;
        const specType& rSpec;
        
    public:
        setShapesIfSatisfiedTracked( plexSpeciesT& refSpecies,
                                     const specType& rSubPlexSpec ) :
            rSpecies( refSpecies ),
            rSpec( rSubPlexSpec )
        {}
        
        void
        operator()( const typename setShapesIfSatisfiedTracked::argument_type& rQueryShapeMapPair ) const
        {
            const queryType& rQuery = * ( rQueryShapeMapPair.first );
            if ( rQuery.applyTracked( rSpecies,
                                      rSpec ) )
            {
                const siteToShapeMap& rSiteToShapeMap
                    = rQueryShapeMapPair.second;
                rSpecies.siteParams.setSiteShapes( rSiteToShapeMap,
                                                   rSpec );
            }
        }
    };
    
    template<class plexSpeciesT,
             class omniPlexT>
    void
    queryAllosteryList<plexSpeciesT,
                       omniPlexT>::
    setSatisfiedQuerySiteShapes( plexSpeciesT& rSpecies,
                                 const specType& rSubPlexSpec ) const
    {
        setShapesIfSatisfiedTracked<plexSpeciesT, omniPlexT>
            setShapes( rSpecies,
                       rSubPlexSpec );
        
        std::for_each( this->begin(),
                       this->end(),
                       this->setMixinShapes );
    }
}
