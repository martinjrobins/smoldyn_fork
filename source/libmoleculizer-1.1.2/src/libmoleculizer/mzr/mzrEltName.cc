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

#include "mzr/mzrEltName.hh"

namespace mzr
{
    namespace eltName
    {
        const std::string moleculizerInput ("moleculizer-input");
        
        
        const std::string executionParameters("runtime-info");
        const std::string reactionNetworkGenerationMode("generation-method-mode");
        const std::string methodAttr("extrapolation-method");
        
        
        const std::string configurationSwitches ("configuration");
        const std::string generationMethod("generation-method");
        const std::string extrapolationMethod("extrapolation-method");
        const std::string modeAttr("mode");
        const std::string spatialMode("spatial");
        const std::string nonSpatialMode("non-spatial");
        const std::string noReactionExtrapolation("no-extrapolate-reaction-rates");
        const std::string reactionExtrapolation("extrapolate-reaction-rates");
        
        const std::string model ("model");
        const std::string streams ("streams");
        const std::string events ("events");
        const std::string generatedNetwork("generated-network");
        
        const std::string explicitSpecies ("explicit-species");
        
        const std::string speciesStreams ("species-streams");
        
        const std::string dumpStreams ("dump-streams");
        const std::string dumpStream ("dump-stream");
        const std::string dumpStream_dumpPeriodAttr ("dump-period");
        const std::string targetFile ("target-file");
        const std::string targetFile_fileNameAttr ("file-name");
        const std::string speciesStreamRef ("species-stream-ref");
        const std::string speciesStreamRef_nameAttr ("name");
        const std::string speciesRef ("species-ref");
        const std::string speciesRef_nameAttr ("name");
        const std::string statStreamRef ("stat-stream-ref");
        const std::string statStreamRef_nameAttr ("name");
        const std::string statStream_simTime ("sim-time");
        const std::string statStream_clockTime ("clock-time");
        const std::string statStream_clockSeconds ("clock-seconds");
        const std::string statStream_activationCount ("activation-count");
        const std::string statStream_speciesCount ("species-count");
        const std::string statStream_reactionCount ("reaction-count");
        const std::string statStream_reactionEventCount ("reaction-event-count");
        const std::string statStream_volume ("volume");
        
        const std::string reactionGens ("reaction-gens");
        
        const std::string explicitReactions ("explicit-reactions");
        const std::string reaction ("reaction");
        const std::string substrateSpeciesRef ("substrate-species-ref");
        const std::string substrateSpeciesRef_nameAttr ("name");
        const std::string substrateSpeciesRef_multAttr ("multiplicity");
        const std::string productSpeciesRef ("product-species-ref");
        const std::string productSpeciesRef_nameAttr ("name");
        const std::string productSpeciesRef_multAttr ("multiplicity");
        const std::string rate ("rate");
        const std::string rate_valueAttr ("value");
        
        //         const std::string volume ("volume");
        //         const std::string volume_litersAttr ("liters");
        
        const std::string createEvent ("create-event");
        const std::string createEvent_timeAttr ("time");
        //        const std::string population ("population");
        const std::string population_countAttr ("count");
        const std::string dumpStreamRef ("dump-stream-ref");
        const std::string dumpStreamRef_nameAttr ("name");
        const std::string dumpStateEvent ("dump-state-event");
        const std::string dumpStateEvent_timeAttr ("time");
        
        const std::string noReactEvent ("no-react-event");
        const std::string noReactEvent_timeAttr ("time");
        
        const std::string stopEvent ("stop-event");
        const std::string stopEvent_timeAttr ("time");
        
        // Elements pertaining to output.
        
        // Names for dumping scientific notation for double-valued parameters,
        // intentded for use in SBML, etc.
        const std::string sciNote ("sci-note");
        const std::string sciNote_fractionAttr ("fraction");
        const std::string sciNote_exponentAttr ("exponent");
        
        const std::string moleculizerState ("moleculizer-state");
        const std::string unitsStates ("unit-states");
        
        const std::string explicitSpeciesTags ("explicit-species-tags");
        const std::string explicitSpeciesTag ("explicit-species-tag");
        const std::string explicitSpeciesTag_nameAttr ("name");
        const std::string explicitSpeciesTag_tagAttr ("tag");
        
        const std::string taggedSpecies ("tagged-species");
        
        const std::string tagReactions ("tag-reactions");
        const std::string tagReaction ("tag-reaction");
        const std::string taggedSubstrate ("tagged-substrate");
        const std::string taggedSubstrate_tagAttr ("tag");
        const std::string taggedSubstrate_multAttr ("multiplicity");
        const std::string taggedProduct ("tagged-product");
        const std::string taggedProduct_tagAttr ("tag");
        const std::string taggedProduct_multAttr ("multiplicity");
        
        const std::string time ("time");
        const std::string time_secondsAttr ("seconds");
        
        const std::string taggedSpeciesStreams ("tagged-species-streams");
        const std::string taggedSpeciesStream ("tagged-species-stream");
        const std::string taggedSpeciesStream_nameAttr ("name");
        const std::string taggedSpeciesRef ("tagged-species-ref");
        const std::string taggedSpeciesRef_tagAttr ("tag");
        
        const std::string taggedDumpStreams ("tagged-dump-streams");
        const std::string taggedDumpStream ("tagged-dump-stream");
        const std::string taggedDumpStream_fileNameAttr ("file-name");
        const std::string taggedDumpStream_dumpPeriodAttr ("dump-period");
        const std::string taggedSpeciesStreamRef ("tagged-species-stream-ref");
        const std::string taggedSpeciesStreamRef_nameAttr ("name");
    }
}
