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

#ifndef CPT_NOTIFIER_H
#define CPT_NOTIFIER_H

namespace fnd
{
    // A notifier could really be a specialization of sensitive<void>, whose
    // "respond" member function takes no arguments.  Usually, notifiers are the
    // starting points of chains of sensitive<previousSensitive>, so it would
    // make sense for them to be sensitive<void>.
    //
    // For some reason, that seems confusing.
    //
    // Also note that notify is non-const: once-notifiers modify themselves when
    // they notify, so that they can remember not to notify again.
    class notifier
    {
    public:
        virtual
        ~notifier( void )
        {}
        
        virtual
        void
        notify( int notifyDepth ) = 0;
    };


    class informer
    {
    public:
        virtual
        ~informer( void )
        {}
        
        virtual
        void
        inform() = 0;
    };
    
    // This is used in the population update routine of species that participate
    // in automatic species generation. The species wants to notify the reaction
    // generation system the first time that its population is updated, but not
    // thereafter.
    //
    // Typically, the species notifies a cascade of responders ("featureMaps"
    // and "features") which terminates with reaction generators that are
    // interested in the updated species. These reaction generators have
    // connected themselves to some "feature," such as the presence of a
    // particular free binding site, that is essential to the kind of reaction
    // genereted, such as a particular kind of dimerization reaction.
    
    class onceNotifier :
        public notifier
    {
        bool notified;
        
    public:
        onceNotifier( void ) :
            notified( false )
        {}
        
        bool
        hasNotified( void ) const
        {
            return notified;
        }
        
        void
        ensureNotified( int notifyDepth )
        {
            // Here I'm checking the notifyDepth because of a trick I'm trying in
            // the compartmental version to keep new species/reaction from being
            // created just because of diffusion.
            if (( ! notified ) && ( 0 <= notifyDepth ) )
            {
                // Must set notified to true because notification could
                // recurse back to this notifier.
                //
                // When a species notifies, it is likely to bring about the creation
                // of reactions that have that very species as a product, causing
                // notification of this species when the reaction occurs.
                notified = true;
                
                notify( notifyDepth );
            }
        }
    };

    class onceInformer :
        public informer
    {
        bool informed;
        
    public:
        onceInformer( void ) :
            informed( false )
        {}
        
        bool
        hasInformed( void ) const
        {
            return informed;
        }
        
        void
        ensureInfomed()
        {
            // Here I'm checking the notifyDepth because of a trick I'm trying in
            // the compartmental version to keep new species/reaction from being
            // created just because of diffusion.
            if (!informed)
            {
                // Must set notified to true because notification could
                // recurse back to this notifier.
                //
                // When a species notifies, it is likely to bring about the creation
                // of reactions that have that very species as a product, causing
                // notification of this species when the reaction occurs.
                informed= true;
                
                inform();
            }
        }
    };


}

#endif // CPT_NOTIFIER_H
