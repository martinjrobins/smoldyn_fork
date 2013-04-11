# CMakeLists.txt file to build Libmoleculizer using CMake.
# Written by Steve Andrews, April 20, 2012.
# This file, and other files of the Smoldyn project, are licensed with LGPL license.
# The primary Smoldyn web site is http://www.smoldyn.org.
# Please report bugs and problems to support@smoldyn.org.

########## Basic setup ##########

project(moleculizer)
cmake_minimum_required(VERSION 2.6)

set(MZR_SOURCE_DIR src/libmoleculizer)

##### cpx #####

set(HEADER_FILES
	${MZR_SOURCE_DIR}/cpx/alloMol.hh
	${MZR_SOURCE_DIR}/cpx/alloMolImpl.hh
	${MZR_SOURCE_DIR}/cpx/basicBndSite.hh
	${MZR_SOURCE_DIR}/cpx/basicMol.hh
	${MZR_SOURCE_DIR}/cpx/basicMolImpl.hh
	${MZR_SOURCE_DIR}/cpx/basicPlex.hh
	${MZR_SOURCE_DIR}/cpx/basicPlexImpl.hh
	${MZR_SOURCE_DIR}/cpx/binding.hh
	${MZR_SOURCE_DIR}/cpx/cpxXcpt.hh
	${MZR_SOURCE_DIR}/cpx/cxBinding.hh
	${MZR_SOURCE_DIR}/cpx/cxBindingImpl.hh
	${MZR_SOURCE_DIR}/cpx/cxMol.hh
	${MZR_SOURCE_DIR}/cpx/cxMolImpl.hh
	${MZR_SOURCE_DIR}/cpx/cxOmni.hh
	${MZR_SOURCE_DIR}/cpx/cxOmniImpl.hh
	${MZR_SOURCE_DIR}/cpx/cxSite.hh
	${MZR_SOURCE_DIR}/cpx/cxSiteImpl.hh
	${MZR_SOURCE_DIR}/cpx/ftrSpec.hh
	${MZR_SOURCE_DIR}/cpx/hashMolRec.hh
	${MZR_SOURCE_DIR}/cpx/hashMolRecImpl.hh
	${MZR_SOURCE_DIR}/cpx/isoSearch.hh
	${MZR_SOURCE_DIR}/cpx/isoSearchImpl.hh
	${MZR_SOURCE_DIR}/cpx/knownBindings.hh
	${MZR_SOURCE_DIR}/cpx/modMixinQuery.hh
	${MZR_SOURCE_DIR}/cpx/modMol.hh
	${MZR_SOURCE_DIR}/cpx/modMolImpl.hh
	${MZR_SOURCE_DIR}/cpx/modMolMixin.hh
	${MZR_SOURCE_DIR}/cpx/modMolState.hh
	${MZR_SOURCE_DIR}/cpx/modMolStateQuery.hh
	${MZR_SOURCE_DIR}/cpx/modStateMixin.hh
	${MZR_SOURCE_DIR}/cpx/modification.hh
	${MZR_SOURCE_DIR}/cpx/molState.hh
	${MZR_SOURCE_DIR}/cpx/molStateQuery.hh
	${MZR_SOURCE_DIR}/cpx/omniPlex.hh
	${MZR_SOURCE_DIR}/cpx/omniPlexFeature.hh
	${MZR_SOURCE_DIR}/cpx/omniStructureQuery.hh
	${MZR_SOURCE_DIR}/cpx/omniStructureQueryImpl.hh
	${MZR_SOURCE_DIR}/cpx/plexFamily.hh
	${MZR_SOURCE_DIR}/cpx/plexFamilyImpl.hh
	${MZR_SOURCE_DIR}/cpx/plexIso.hh
	${MZR_SOURCE_DIR}/cpx/plexIsoImpl.hh
	${MZR_SOURCE_DIR}/cpx/plexMap.hh
	${MZR_SOURCE_DIR}/cpx/plexMapImpl.hh
	${MZR_SOURCE_DIR}/cpx/plexQuery.hh
	${MZR_SOURCE_DIR}/cpx/plexQueryImpl.hh
	${MZR_SOURCE_DIR}/cpx/plexSpcsMixin.hh
	${MZR_SOURCE_DIR}/cpx/plexSpcsMixinCanonicalNamingImpl.hh
	${MZR_SOURCE_DIR}/cpx/plexSpcsMixinImpl.hh
	${MZR_SOURCE_DIR}/cpx/prm.hh
	${MZR_SOURCE_DIR}/cpx/queryAlloList.hh
	${MZR_SOURCE_DIR}/cpx/queryAlloListImpl.hh
	${MZR_SOURCE_DIR}/cpx/recognizer.hh
	${MZR_SOURCE_DIR}/cpx/recognizerImpl.hh
	${MZR_SOURCE_DIR}/cpx/reportIsoSearch.hh
	${MZR_SOURCE_DIR}/cpx/siteShape.hh
	${MZR_SOURCE_DIR}/cpx/siteToShapeMap.hh
	${MZR_SOURCE_DIR}/cpx/siteToShapeMapImpl.hh
	${MZR_SOURCE_DIR}/cpx/smallMol.hh
	${MZR_SOURCE_DIR}/cpx/stateMol.hh
	${MZR_SOURCE_DIR}/cpx/structuralBinding.hh
	${MZR_SOURCE_DIR}/cpx/subPlexSpec.hh
)

set(SRC_FILES
	${MZR_SOURCE_DIR}/cpx/basicBndSite.cc
	${MZR_SOURCE_DIR}/cpx/cpxXcpt.cc
	${MZR_SOURCE_DIR}/cpx/modMolMixin.cc
	${MZR_SOURCE_DIR}/cpx/modStateMixin.cc
	${MZR_SOURCE_DIR}/cpx/plexIso.cc
	${MZR_SOURCE_DIR}/cpx/plexMap.cc
	${MZR_SOURCE_DIR}/cpx/siteToShapeMap.cc
)


####### dimer ######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/dimer/decompFam.hh
	${MZR_SOURCE_DIR}/dimer/decompRxnGen.hh
	${MZR_SOURCE_DIR}/dimer/decomposeExtrap.hh
	${MZR_SOURCE_DIR}/dimer/dimerEltName.hh
	${MZR_SOURCE_DIR}/dimer/dimerUnit.hh
	${MZR_SOURCE_DIR}/dimer/dimerXcpt.hh
	${MZR_SOURCE_DIR}/dimer/dimerizeExtrap.hh
	${MZR_SOURCE_DIR}/dimer/dimerizeFam.hh
	${MZR_SOURCE_DIR}/dimer/dimerizeRxnGen.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/dimer/decompRxnGen.cc
	${MZR_SOURCE_DIR}/dimer/decomposeExtrap.cc
	${MZR_SOURCE_DIR}/dimer/dimerDomParse.cc
	${MZR_SOURCE_DIR}/dimer/dimerEltName.cc
	${MZR_SOURCE_DIR}/dimer/dimerUnit.cc
	${MZR_SOURCE_DIR}/dimer/dimerXcpt.cc
	${MZR_SOURCE_DIR}/dimer/dimerizeExtrap.cc
	${MZR_SOURCE_DIR}/dimer/dimerizeRxnGen.cc

)

######## fnd #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/fnd/basicDmpColumn.hh
	${MZR_SOURCE_DIR}/fnd/basicDumpable.hh
	${MZR_SOURCE_DIR}/fnd/basicReaction.hh
	${MZR_SOURCE_DIR}/fnd/basicSpecies.hh
	${MZR_SOURCE_DIR}/fnd/binaryRxnGen.hh
	${MZR_SOURCE_DIR}/fnd/coreRxnGen.hh
	${MZR_SOURCE_DIR}/fnd/dmpColumn.hh
	${MZR_SOURCE_DIR}/fnd/dumpStream.hh
	${MZR_SOURCE_DIR}/fnd/dumpable.hh
	${MZR_SOURCE_DIR}/fnd/event.hh
	${MZR_SOURCE_DIR}/fnd/feature.hh
	${MZR_SOURCE_DIR}/fnd/featureContext.hh
	${MZR_SOURCE_DIR}/fnd/featureMap.hh
	${MZR_SOURCE_DIR}/fnd/featureStimulus.hh
	${MZR_SOURCE_DIR}/fnd/fndXcpt.hh
	${MZR_SOURCE_DIR}/fnd/gillspReaction.hh
	${MZR_SOURCE_DIR}/fnd/massive.hh
	${MZR_SOURCE_DIR}/fnd/multiSpeciesDumpable.hh
	${MZR_SOURCE_DIR}/fnd/newContextStimulus.hh
	${MZR_SOURCE_DIR}/fnd/newSpeciesStimulus.hh
	${MZR_SOURCE_DIR}/fnd/notifier.hh
	${MZR_SOURCE_DIR}/fnd/pchem.hh
	${MZR_SOURCE_DIR}/fnd/physConst.hh
	${MZR_SOURCE_DIR}/fnd/query.hh
	${MZR_SOURCE_DIR}/fnd/queryImpl.hh
	${MZR_SOURCE_DIR}/fnd/querySpeciesDumpable.hh
	${MZR_SOURCE_DIR}/fnd/reactionNetworkDescription.hh
	${MZR_SOURCE_DIR}/fnd/reactionNetworkDescriptionImpl.hh
	${MZR_SOURCE_DIR}/fnd/reactionNetworkComponent.hh
	${MZR_SOURCE_DIR}/fnd/rxnGen.hh
	${MZR_SOURCE_DIR}/fnd/sensitive.hh
	${MZR_SOURCE_DIR}/fnd/sensitiveOnce.hh
	${MZR_SOURCE_DIR}/fnd/sensitivityList.hh
	${MZR_SOURCE_DIR}/fnd/stateVar.hh
	${MZR_SOURCE_DIR}/fnd/varDumpable.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/fnd/dumpStream.cc
	${MZR_SOURCE_DIR}/fnd/fndXcpt.cc
	${MZR_SOURCE_DIR}/fnd/pchem.cc
	${MZR_SOURCE_DIR}/fnd/physConst.cc
)

######## ftr #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/ftr/badModMolInstanceXcpt.hh
	${MZR_SOURCE_DIR}/ftr/badSmallMolInstanceXcpt.hh
	${MZR_SOURCE_DIR}/ftr/badSmallMolXcpt.hh
	${MZR_SOURCE_DIR}/ftr/exchangedModXcpt.hh
	${MZR_SOURCE_DIR}/ftr/ftrEltName.hh
	${MZR_SOURCE_DIR}/ftr/ftrUnit.hh
	${MZR_SOURCE_DIR}/ftr/omniExtrap.hh
	${MZR_SOURCE_DIR}/ftr/omniFam.hh
	${MZR_SOURCE_DIR}/ftr/omniGen.hh
	${MZR_SOURCE_DIR}/ftr/parseOmniGen.hh
	${MZR_SOURCE_DIR}/ftr/parseUniMolGen.hh
	${MZR_SOURCE_DIR}/ftr/uniMolExtrap.hh
	${MZR_SOURCE_DIR}/ftr/uniMolFam.hh
	${MZR_SOURCE_DIR}/ftr/uniMolGen.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/ftr/badModMolInstanceXcpt.cc
	${MZR_SOURCE_DIR}/ftr/badSmallMolInstanceXcpt.cc
	${MZR_SOURCE_DIR}/ftr/badSmallMolXcpt.cc
	${MZR_SOURCE_DIR}/ftr/exchangedModXcpt.cc
	${MZR_SOURCE_DIR}/ftr/ftrDomParse.cc
	${MZR_SOURCE_DIR}/ftr/ftrEltName.cc
	${MZR_SOURCE_DIR}/ftr/omniExtrap.cc
	${MZR_SOURCE_DIR}/ftr/omniGen.cc
	${MZR_SOURCE_DIR}/ftr/parseOmniGen.cc
	${MZR_SOURCE_DIR}/ftr/parseUniMolGen.cc
	${MZR_SOURCE_DIR}/ftr/uniMolExtrap.cc
	${MZR_SOURCE_DIR}/ftr/uniMolGen.cc
)

######## mol #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/mol/badModMolXcpt.hh
	${MZR_SOURCE_DIR}/mol/badMolParamXcpt.hh
	${MZR_SOURCE_DIR}/mol/badSmallMolXcpt.hh
	${MZR_SOURCE_DIR}/mol/dupModNameXcpt.hh
	${MZR_SOURCE_DIR}/mol/dupModSiteNameXcpt.hh
	${MZR_SOURCE_DIR}/mol/dupMolNameXcpt.hh
	${MZR_SOURCE_DIR}/mol/dupSiteNameXcpt.hh
	${MZR_SOURCE_DIR}/mol/molEltName.hh
	${MZR_SOURCE_DIR}/mol/molUnit.hh
	${MZR_SOURCE_DIR}/mol/molXcpt.hh
	${MZR_SOURCE_DIR}/mol/mzrBndSite.hh
	${MZR_SOURCE_DIR}/mol/mzrModMol.hh
	${MZR_SOURCE_DIR}/mol/mzrMol.hh
	${MZR_SOURCE_DIR}/mol/parseBndSite.hh
	${MZR_SOURCE_DIR}/mol/parseMod.hh
	${MZR_SOURCE_DIR}/mol/parseModMap.hh
	${MZR_SOURCE_DIR}/mol/parseModSite.hh
	${MZR_SOURCE_DIR}/mol/parseSiteShapeName.hh
	${MZR_SOURCE_DIR}/mol/siteFeature.hh
	${MZR_SOURCE_DIR}/mol/smallMol.hh
	${MZR_SOURCE_DIR}/mol/unkModSiteXcpt.hh
	${MZR_SOURCE_DIR}/mol/unkModXcpt.hh
	${MZR_SOURCE_DIR}/mol/unkMolXcpt.hh
	${MZR_SOURCE_DIR}/mol/unkSiteShapeXcpt.hh
	${MZR_SOURCE_DIR}/mol/unkSiteXcpt.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/mol/badModMolXcpt.cc
	${MZR_SOURCE_DIR}/mol/badMolParamXcpt.cc
	${MZR_SOURCE_DIR}/mol/badSmallMolXcpt.cc
	${MZR_SOURCE_DIR}/mol/dupModNameXcpt.cc
	${MZR_SOURCE_DIR}/mol/dupModSiteNameXcpt.cc
	${MZR_SOURCE_DIR}/mol/dupMolNameXcpt.cc
	${MZR_SOURCE_DIR}/mol/dupSiteNameXcpt.cc
	${MZR_SOURCE_DIR}/mol/molDomInsert.cc
	${MZR_SOURCE_DIR}/mol/molDomParse.cc
	${MZR_SOURCE_DIR}/mol/molEltName.cc
	${MZR_SOURCE_DIR}/mol/molUnit.cc
	${MZR_SOURCE_DIR}/mol/mzrBndSite.cc
	${MZR_SOURCE_DIR}/mol/mzrModMol.cc
	${MZR_SOURCE_DIR}/mol/mzrMol.cc
	${MZR_SOURCE_DIR}/mol/smallMol.cc
	${MZR_SOURCE_DIR}/mol/unkModSiteXcpt.cc
	${MZR_SOURCE_DIR}/mol/unkModXcpt.cc
	${MZR_SOURCE_DIR}/mol/unkMolXcpt.cc
	${MZR_SOURCE_DIR}/mol/unkSiteShapeXcpt.cc
	${MZR_SOURCE_DIR}/mol/unkSiteXcpt.cc
)

######## mzr #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/mzr/createEvent.hh
	${MZR_SOURCE_DIR}/mzr/dumpUtils.hh
	${MZR_SOURCE_DIR}/mzr/inputCapTest.hh
	${MZR_SOURCE_DIR}/mzr/libmzr_c_interface.h
	${MZR_SOURCE_DIR}/mzr/molarFactor.hh
	${MZR_SOURCE_DIR}/mzr/moleculizer.hh
	${MZR_SOURCE_DIR}/mzr/mzrEltName.hh
	${MZR_SOURCE_DIR}/mzr/mzrEvent.hh
	${MZR_SOURCE_DIR}/mzr/mzrException.hh
	${MZR_SOURCE_DIR}/mzr/mzrHelperFunctions.hh
	${MZR_SOURCE_DIR}/mzr/mzrReaction.hh
	${MZR_SOURCE_DIR}/mzr/mzrSpecies.hh
	${MZR_SOURCE_DIR}/mzr/mzrSpeciesDumpable.hh
	${MZR_SOURCE_DIR}/mzr/mzrSpeciesDumpableImpl.hh
	${MZR_SOURCE_DIR}/mzr/mzrStream.hh
	${MZR_SOURCE_DIR}/mzr/mzrUnit.hh
	${MZR_SOURCE_DIR}/mzr/pythonRulesManager.hh
	${MZR_SOURCE_DIR}/mzr/respondReaction.hh
	${MZR_SOURCE_DIR}/mzr/rxnDescriptionInterface.hh
	${MZR_SOURCE_DIR}/mzr/spatialExtrapolationFunctions.hh
	${MZR_SOURCE_DIR}/mzr/unit.hh
	${MZR_SOURCE_DIR}/mzr/unitsMgr.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/mzr/dumpUtils.cc
	${MZR_SOURCE_DIR}/mzr/libmzr_c_interface.cc
	${MZR_SOURCE_DIR}/mzr/moleculizer.cc
	${MZR_SOURCE_DIR}/mzr/mzrSpeciesAutomorphism.cc
	${MZR_SOURCE_DIR}/mzr/mzrEltName.cc
	${MZR_SOURCE_DIR}/mzr/mzrException.cc
	${MZR_SOURCE_DIR}/mzr/mzrReaction.cc
	${MZR_SOURCE_DIR}/mzr/mzrSpecies.cc
	${MZR_SOURCE_DIR}/mzr/mzrUnit.cc
	${MZR_SOURCE_DIR}/mzr/mzrUnitInsert.cc
	${MZR_SOURCE_DIR}/mzr/mzrUnitParse.cc
	${MZR_SOURCE_DIR}/mzr/pythonRulesManager.cc
	${MZR_SOURCE_DIR}/mzr/spatialExtrapolationFunctions.cc
	${MZR_SOURCE_DIR}/mzr/unit.cc
	${MZR_SOURCE_DIR}/mzr/unitsMgr.cc
)

######## nmr #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/nmr/abstractMol.hh
	${MZR_SOURCE_DIR}/nmr/basicNameAssembler.hh
	${MZR_SOURCE_DIR}/nmr/complexFormNameAssembler.hh
	${MZR_SOURCE_DIR}/nmr/complexOutputState.hh
	${MZR_SOURCE_DIR}/nmr/complexSpeciesEncoderNames.hh
	${MZR_SOURCE_DIR}/nmr/complexSpecies.hh
	${MZR_SOURCE_DIR}/nmr/complexSpeciesOutputMinimizer.hh
	${MZR_SOURCE_DIR}/nmr/mangledNameAssembler.hh
	${MZR_SOURCE_DIR}/nmr/nameAssembler.hh
	${MZR_SOURCE_DIR}/nmr/nameAssemblers.hh
	${MZR_SOURCE_DIR}/nmr/namedMolecule.hh
	${MZR_SOURCE_DIR}/nmr/nameEncoderFactory.hh
	${MZR_SOURCE_DIR}/nmr/nmrEltName.hh
	${MZR_SOURCE_DIR}/nmr/nmrExceptions.hh
	${MZR_SOURCE_DIR}/nmr/nmrUnit.hh
	${MZR_SOURCE_DIR}/nmr/partialTokenList.hh
	${MZR_SOURCE_DIR}/nmr/permutation.hh
	${MZR_SOURCE_DIR}/nmr/permutationName.hh
	${MZR_SOURCE_DIR}/nmr/readableNameAssembler.hh 
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/nmr/basicNameAssembler.cc
	${MZR_SOURCE_DIR}/nmr/complexFormNameAssembler.cc
	${MZR_SOURCE_DIR}/nmr/complexOutputState.cc
	${MZR_SOURCE_DIR}/nmr/complexSpecies.cc
	${MZR_SOURCE_DIR}/nmr/complexSpeciesEncoderNames.cc
	${MZR_SOURCE_DIR}/nmr/complexSpeciesOutputMinimizer.cc
	${MZR_SOURCE_DIR}/nmr/mangledNameAssembler.cc
	${MZR_SOURCE_DIR}/nmr/namedMolecule.cc
	${MZR_SOURCE_DIR}/nmr/nmrEltName.cc
	${MZR_SOURCE_DIR}/nmr/nmrUnit.cc
	${MZR_SOURCE_DIR}/nmr/partialTokenList.cc
	${MZR_SOURCE_DIR}/nmr/permutation.cc
	${MZR_SOURCE_DIR}/nmr/permutationName.cc
	${MZR_SOURCE_DIR}/nmr/readableNameAssembler.cc
)

######## plex #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/plex/dupNodeOmniXcpt.hh
	${MZR_SOURCE_DIR}/plex/multBoundSiteXcpt.hh
	${MZR_SOURCE_DIR}/plex/mzrOmniPlex.hh
	${MZR_SOURCE_DIR}/plex/mzrOmniStructureQueries.hh
	${MZR_SOURCE_DIR}/plex/mzrPlex.hh
	${MZR_SOURCE_DIR}/plex/mzrPlexFamily.hh
	${MZR_SOURCE_DIR}/plex/mzrPlexQueries.hh
	${MZR_SOURCE_DIR}/plex/mzrPlexSpecies.hh
	${MZR_SOURCE_DIR}/plex/mzrRecognizer.hh
	${MZR_SOURCE_DIR}/plex/noKineticConstsXcpt.hh
	${MZR_SOURCE_DIR}/plex/noOmniForNodeXcpt.hh
	${MZR_SOURCE_DIR}/plex/parseOmniPlex.hh
	${MZR_SOURCE_DIR}/plex/parsePlex.hh
	${MZR_SOURCE_DIR}/plex/parsedPlexNotConnXcpt.hh
	${MZR_SOURCE_DIR}/plex/parserPlex.hh
	${MZR_SOURCE_DIR}/plex/plexEltName.hh
	${MZR_SOURCE_DIR}/plex/plexExceptions.hh
	${MZR_SOURCE_DIR}/plex/plexUnit.hh
	${MZR_SOURCE_DIR}/plex/unkMolInstXcpt.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/plex/dupNodeOmniXcpt.cc
	${MZR_SOURCE_DIR}/plex/multBoundSiteXcpt.cc
	${MZR_SOURCE_DIR}/plex/mzrPlex.cc
	${MZR_SOURCE_DIR}/plex/mzrPlexFamily.cc
	${MZR_SOURCE_DIR}/plex/mzrPlexSpecies.cc
	${MZR_SOURCE_DIR}/plex/mzrRecognizer.cc
	${MZR_SOURCE_DIR}/plex/noKineticConstsXcpt.cc
	${MZR_SOURCE_DIR}/plex/noOmniForNodeXcpt.cc
	${MZR_SOURCE_DIR}/plex/parseOmniPlex.cc
	${MZR_SOURCE_DIR}/plex/parsePlex.cc
	${MZR_SOURCE_DIR}/plex/parsedPlexNotConnXcpt.cc
	${MZR_SOURCE_DIR}/plex/parserPlex.cc
	${MZR_SOURCE_DIR}/plex/plexEltName.cc
	${MZR_SOURCE_DIR}/plex/plexUnit.cc
	${MZR_SOURCE_DIR}/plex/plexUnitParse.cc
	${MZR_SOURCE_DIR}/plex/unkMolInstXcpt.cc
)

######## stoch #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/stoch/badStochSpeciesTagXcpt.hh
	${MZR_SOURCE_DIR}/stoch/stochEltName.hh
	${MZR_SOURCE_DIR}/stoch/stochSpecies.hh
	${MZR_SOURCE_DIR}/stoch/stochUnit.hh
	${MZR_SOURCE_DIR}/stoch/unkStochSpeciesXcpt.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/stoch/badStochSpeciesTagXcpt.cc
	${MZR_SOURCE_DIR}/stoch/stochDomParse.cc
	${MZR_SOURCE_DIR}/stoch/stochEltName.cc
	${MZR_SOURCE_DIR}/stoch/stochSpecies.cc
	${MZR_SOURCE_DIR}/stoch/stochUnit.cc
	${MZR_SOURCE_DIR}/stoch/unkStochSpeciesXcpt.cc
)

######## utl #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/utl/arg.hh
	${MZR_SOURCE_DIR}/utl/AssocVector.h
	${MZR_SOURCE_DIR}/utl/autoCache.hh
	${MZR_SOURCE_DIR}/utl/autoCatalog.hh
	${MZR_SOURCE_DIR}/utl/autoVector.hh
	${MZR_SOURCE_DIR}/utl/defs.hh
	${MZR_SOURCE_DIR}/utl/dom.hh
	${MZR_SOURCE_DIR}/utl/domJob.hh
	${MZR_SOURCE_DIR}/utl/domJobImpl.hh
	${MZR_SOURCE_DIR}/utl/domXcpt.hh
	${MZR_SOURCE_DIR}/utl/forBoth.hh
	${MZR_SOURCE_DIR}/utl/forceInsert.hh
	${MZR_SOURCE_DIR}/utl/frexp10.hh
	${MZR_SOURCE_DIR}/utl/funcInsert.hh
	${MZR_SOURCE_DIR}/utl/linearHash.hh
	${MZR_SOURCE_DIR}/utl/message.hh
	${MZR_SOURCE_DIR}/utl/utility.hh
	${MZR_SOURCE_DIR}/utl/utlEltName.hh
	${MZR_SOURCE_DIR}/utl/utlHelper.hh
	${MZR_SOURCE_DIR}/utl/utlXcpt.hh
	${MZR_SOURCE_DIR}/utl/warn.hh
	${MZR_SOURCE_DIR}/utl/writeOutputGraph.hh
	${MZR_SOURCE_DIR}/utl/xcpt.hh
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/utl/arg.cc
	${MZR_SOURCE_DIR}/utl/dom.cc
	${MZR_SOURCE_DIR}/utl/domXcpt.cc
	${MZR_SOURCE_DIR}/utl/frexp10.cc
	${MZR_SOURCE_DIR}/utl/linearHash.cc
	${MZR_SOURCE_DIR}/utl/utlXcpt.cc
	${MZR_SOURCE_DIR}/utl/utility.cc
	${MZR_SOURCE_DIR}/utl/utlEltName.cc
)

######## nauty #######

set(HEADER_FILES ${HEADER_FILES}
	${MZR_SOURCE_DIR}/nauty/gtools.h
	${MZR_SOURCE_DIR}/nauty/gutils.h
	${MZR_SOURCE_DIR}/nauty/naugroup.h
	${MZR_SOURCE_DIR}/nauty/nausparse.h
	${MZR_SOURCE_DIR}/nauty/nautaux.h
	${MZR_SOURCE_DIR}/nauty/nautinv.h
	${MZR_SOURCE_DIR}/nauty/naututil.h
	${MZR_SOURCE_DIR}/nauty/nauty.h
	${MZR_SOURCE_DIR}/nauty/planarity.h
	${MZR_SOURCE_DIR}/nauty/rng.h
)

set(SRC_FILES ${SRC_FILES}
	${MZR_SOURCE_DIR}/nauty/nauty.c
	${MZR_SOURCE_DIR}/nauty/nautil.c
	${MZR_SOURCE_DIR}/nauty/naugraph.c
)

set_source_files_properties(${SRC_FILES} PROPERTIES LANGUAGE CXX )

include_directories(${MZR_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

######## Python ###########

include(FindPythonInterp)
include(FindPythonLibs)

include_directories(${PYTHON_INCLUDE_DIRS})


######## LibXML2 and LibXML++ ###########

include_directories(src/libxml++-1.0.5)


############## Targets ###############

add_library(moleculizer STATIC ${SRC_FILES} ${HEADER_FILES})


########## install ###########

install(TARGETS moleculizer
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

