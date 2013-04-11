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

#ifndef INPUTELEMENTNAME_H
#define INPUTELEMENTNAME_H

#include <string>

namespace mzr
{
    namespace eltName
    {
        extern const std::string moleculizerInput;
        
        extern const std::string executionParameters;
        extern const std::string reactionNetworkGenerationMode;
        extern const std::string configurationSwitches;
        extern const std::string generationMethod;
        extern const std::string extrapolationMethod;
        extern const std::string modeAttr;
        extern const std::string methodAttr;
        extern const std::string spatialMode;
        extern const std::string nonSpatialMode;
        extern const std::string noReactionExtrapolation;
        extern const std::string reactionExtrapolation;
        
        extern const std::string model;
        extern const std::string streams;
        extern const std::string events;
        extern const std::string generatedNetwork;
        
        extern const std::string explicitSpecies;
        
        extern const std::string speciesStreams;
        
        extern const std::string dumpStreams;
        extern const std::string dumpStream;
        extern const std::string dumpStream_dumpPeriodAttr;
        extern const std::string targetFile;
        extern const std::string targetFile_fileNameAttr;
        extern const std::string speciesStreamRef;
        extern const std::string speciesStreamRef_nameAttr;
        extern const std::string speciesRef;
        extern const std::string speciesRef_nameAttr;
        extern const std::string statStreamRef;
        extern const std::string statStreamRef_nameAttr;
        extern const std::string statStream_simTime;
        extern const std::string statStream_clockTime;
        extern const std::string statStream_clockSeconds;
        extern const std::string statStream_activationCount;
        extern const std::string statStream_speciesCount;
        extern const std::string statStream_reactionCount;
        extern const std::string statStream_reactionEventCount;
        extern const std::string statStream_volume;
        
        extern const std::string reactionGens;
        
        extern const std::string explicitReactions;
        extern const std::string reaction;
        extern const std::string substrateSpeciesRef;
        extern const std::string substrateSpeciesRef_nameAttr;
        extern const std::string substrateSpeciesRef_multAttr;
        extern const std::string productSpeciesRef;
        extern const std::string productSpeciesRef_nameAttr;
        extern const std::string productSpeciesRef_multAttr;
        extern const std::string rate;
        extern const std::string rate_valueAttr;
        
        //         extern const std::string volume;
        //         extern const std::string volume_litersAttr;
        
        extern const std::string createEvent;
        extern const std::string createEvent_timeAttr;
        //         extern const std::string population;
        extern const std::string population_countAttr;
        extern const std::string dumpStreamRef;
        extern const std::string dumpStreamRef_nameAttr;
        extern const std::string dumpStateEvent;
        extern const std::string dumpStateEvent_timeAttr;
        
        extern const std::string noReactEvent;
        extern const std::string noReactEvent_timeAttr;
        
        extern const std::string stopEvent;
        extern const std::string stopEvent_timeAttr;
        
        // Elements pertaining to output.
        
        // Names for dumping scientific notation for double-valued parameters,
        // intentded for use in SBML, etc.
        extern const std::string sciNote;
        extern const std::string sciNote_fractionAttr;
        extern const std::string sciNote_exponentAttr;
        
        extern const std::string moleculizerState;
        extern const std::string unitsStates;
        
        extern const std::string explicitSpeciesTags;
        extern const std::string explicitSpeciesTag;
        extern const std::string explicitSpeciesTag_nameAttr;
        extern const std::string explicitSpeciesTag_tagAttr;
        
        extern const std::string taggedSpecies;
        
        extern const std::string tagReactions;
        extern const std::string tagReaction;
        extern const std::string taggedSubstrate;
        extern const std::string taggedSubstrate_tagAttr;
        extern const std::string taggedSubstrate_multAttr;
        extern const std::string taggedProduct;
        extern const std::string taggedProduct_tagAttr;
        extern const std::string taggedProduct_multAttr;
        
        extern const std::string time;
        extern const std::string time_secondsAttr;
        
        extern const std::string taggedSpeciesStreams;
        extern const std::string taggedSpeciesStream;
        extern const std::string taggedSpeciesStream_nameAttr;
        extern const std::string taggedSpeciesRef;
        extern const std::string taggedSpeciesRef_tagAttr;
        
        extern const std::string taggedDumpStreams;
        extern const std::string taggedDumpStream;
        extern const std::string taggedDumpStream_fileNameAttr;
        extern const std::string taggedDumpStream_dumpPeriodAttr;
        extern const std::string taggedSpeciesStreamRef;
        extern const std::string taggedSpeciesStreamRef_nameAttr;
    }
}

#endif // INPUTELEMENTNAME_H
