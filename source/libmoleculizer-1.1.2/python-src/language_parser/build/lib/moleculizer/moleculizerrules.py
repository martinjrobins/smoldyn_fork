###############################################################################
# Copyright (C) 2007, 2008, 2009 The Molecular Sciences Institute
# Original Author:
#   Nathan Addy, Scientific Programmer	Email: addy@molsci.org
#   The Molecular Sciences Institute    
#
###############################################################################

import pdb

import re

import util

from xmlobject import XmlObject
import StringIO

from sectionparameter import SymbolicExpressionEvaluator
from sectionmodifications import ModificationsSection
from sectionmols import MolsSection
from sectionallostery import AllostericPlexesSection, AllostericOmnisSection
from sectionreactionrules import ReactionRulesSection
from sectionspeciesstreams import SpeciesStreamsSection
from sectionexplicitspeciesblock import ExplicitSpeciesSection

from moleculizer_xcpt import *
class MoleculizerRulesFile:
    """
    This object acts as an parsing thing that outputs moleculizer files xml,
    suitable for processing internally by a mzr::moleculizer instance."""

    def BlockPassesSanityCheck( linearray ):
        linearray = [x for x in linearray if x.strip() != ""]
        if len(linearray) == 0: return True
        
        everyLineEndsWithSemiColon = [ x[-1] == ";" and x.count(";") == 1for x in linearray]
        noWhiteSpace = [ (x.count("\n") + x.count(" ") + x.count("\t") == 0) for x in linearray]
        return reduce(util.And, everyLineEndsWithSemiColon) and reduce(util.And, noWhiteSpace)

    BlockPassesSanityCheck = staticmethod( BlockPassesSanityCheck )

    def addWholeRulesString( self, rulesString):

        lines = rulesString.split("\n")

        self.clearMemory()

        output_style, parameterBlock, modificationsBlock, molsBlock, allostericPlexes, allostericOmnis,\
                        reactionRulesBlock, dimerizationGenBlock, omniGenBlock, \
                        explicitSpeciesBlock, speciesStreamBlock = parseBlockTypesFromRulesFile( lines )

        self.output_params = output_style
        self.addParameterBlock( parameterBlock )
        self.addModicationsBlock( modificationsBlock )
        self.addMolsBlock( molsBlock )
        self.addAllostericPlexesBlock( allostericPlexes )
        self.addAllostericOmnisBlock( allostericOmnis )
        self.addReactionRulesBlock( reactionRulesBlock, dimerizationGenBlock, \
                                          omniGenBlock, [] )
        self.addExplicitSpeciesBlock( explicitSpeciesBlock )
        self.addSpeciesStreamsBlock( speciesStreamBlock )

        return

    def addWholeRulesFile(self, rulesFile):
        output_style, parameterBlock, modificationsBlock, molsBlock, allostericPlexes, allostericOmnis, \
                        reactionRulesBlock, dimerizationGenBlock, omniGenBlock, \
                        explicitSpeciesBlock, speciesStreamBlock = parseBlockTypesFromRulesFile( open(rulesFile).readlines() )

        self.clearMemory()
        self.output_params = output_style

        self.addParameterBlock( parameterBlock )
        self.addModicationsBlock( modificationsBlock )
        self.addMolsBlock( molsBlock )
        self.addAllostericPlexesBlock( allostericPlexes )
        self.addAllostericOmnisBlock( allostericOmnis )
        self.addReactionRulesBlock( reactionRulesBlock, dimerizationGenBlock, \
                                          omniGenBlock, [] )
        self.addExplicitSpeciesBlock( explicitSpeciesBlock )
        self.addSpeciesStreamsBlock( speciesStreamBlock )

        return

    def addParameterStatement(self, paramStatement):
        paramStatement = self.PreProcessStatement( paramStatement )

        print "Adding param line: '%s'" % paramStatement

        self.parameterBlock.append( paramStatement)
        self.parameterEE = SymbolicExpressionEvaluator( self.parameterBlock )

        return

    def addModificationStatement(self, modLine):
        modLine = self.PreProcessStatement( modLine )

        print "Adding mod line: '%s'" % modLine

        self.modificationsBlock.append( modLine)
        self.modificationsSection = ModificationsSection( self.modificationsBlock )
        
        return

    def addMolsStatement(self, molsLine):
        molsLine = self.PreProcessStatement( molsLine )

        self.molsBlock.append( molsLine )
        self.molsSection = MolsSection( molsBlock )

        return 

    def addAllostericPlexStatement(self, alloPlexLine):
        alloPlexLine = self.PreProcessStatement( alloPlexLine )

        self.allostericPlexes.append( alloPlexLine )
        self.allostericPlexesSection = AllostericPlexesSection( self.allostericPlexes )
        
        return

    def addAllostericOmniStatement(self, alloOmniLine):

        alloOmniLine = self.PreProcessStatement( alloOmniLine )

        self.allostericOmnis.append( alloOmniLine )
        self.allostericOmnisSection = AllostericOmnisSection( self.allostericOmnis )

        return 

    def addDimerizationGenStatement(self, dimerGenLine):
        dimerGenLine = self.PreProcessStatement( dimerGenLine )

        self.dimerizationGenBlock.append(dimerGenLine)

        self.reactionGensSection = ReactionRulesSection( self.reactionRulesBlock, 
                                                         self.dimerizationGenBlock, 
                                                         self.omniGenBlock, 
                                                         self.uniMolGenBlock)
        
        return 

    def addOmniGenStatement(self, omniGenLine):
        omniGenLine = self.PreProcessStatement( omniGenLine )

        self.omniGenLine.append( omniGenLine )

        self.reactionGensSection = ReactionRulesSection( self.reactionRulesBlock, 
                                                         self.dimerizationGenBlock, 
                                                         self.omniGenBlock, 
                                                         self.uniMolGenBlock)        

        return 

    def addUniMolGenStatement(self, uniMolGenLine):
        uniMolGenLine = self.PreProcessStatement( uniMolGenLine )

        self.uniMolGenBlock.append( uniMolGenLine )

        self.reactionGensSection = ReactionRulesSection( self.reactionRulesBlock, 
                                                         self.dimerizationGenBlock, 
                                                         self.omniGenBlock, 
                                                         self.uniMolGenBlock)        
        
        return

    def addExplicitSpeciesStatement(self, explicitSpeciesStatement):
        explicitSpeciesStatement = self.PreProcessStatement( explicitSpeciesStatement )

        self.explicitSpeciesBlock.append( explicitSpeciesStatement )
        self.explicitSpeciesSection = ExplicitSpeciesSection( self.explicitSpeciesBlock )

        return 

    def addSpeciesStreamStatement(self, speciesStreamLine):
        speciesStreamLine = self.PreProcessStatement( speciesStreamLine )

        self.speciesStreamBlock.append( speciesStreamLine )
        self.speciesStreamSection = SpeciesStreamsSection( self.speciesStreamBlock  )
        
        return

    def __init__(self):
        self.clearMemory()

    def getOutputFileName(self):
        return self.outputFileName
        
    def write(self):
        self.openXmlFile = open(self.outputFileName, 'w')
        self.__writeOutput(self.openXmlFile)
        return

    def clearMemory(self):
        # These are the lines of input, in one statement per line form, with no whitespace
        self.parameterBlock = []
        self.modificationsBlock = []
        self.molsBlock = []
        self.allostericPlexes = []
        self.allostericOmnis = []
        self.reactionRulesBlock = []
        self.dimerizationGenBlock = []
        self.omniGenBlock = []
        self.uniMolGenBlock = []
        self.explicitSpeciesBlock = []
        self.speciesStreamBlock = []

        # These are the objects that will be used to process the parsed 
        # data.

        # A section is an intermediate between a rules file (they have lines, for example,
        # and can answer questions about what has been parsed ) and an xml section (it can
        # write out an xml section -
        # Parameters doesn't write anything out currently, but easily could
	self.parameterSection = 0 
        self.modificationsSection = 0
	self.molsSection = 0
        self.allostericPlexesSection = 0
        self.allostericOmnisSection = 0
	self.reactionGensSection = 0
        self.explicitSpeciesSection = 0
        self.speciesStreamSection = 0


    def writeToString(self):
        myString = StringIO.StringIO()
        self.__writeOutput( myString )

        xmlstring = myString.getvalue()

        if self.output_params[0]:
            self.outputFileName = self.output_params[1]
            out_file = open(self.outputFileName, 'w')
            out_file.write(xmlstring)
            out_file.flush()
        
        return xmlstring

    def close(self):
        self.openXmlFile.close()

    def addParameterBlock(self, parameterBlock, overwrite = False):
	if self.parameterBlock and not overwrite: 
            raise MzrExceptions.MoleculizerException("Error: Cannot add a parameter block twice.")

        if not self.BlockPassesSanityCheck( parameterBlock ):
            raise InsaneBlockOnTheLooseException(parameterBlock, "parameter block")

	self.parameterBlock.extend( parameterBlock )
        self.parameterEE = SymbolicExpressionEvaluator( self.parameterBlock )

    def addModicationsBlock(self, modificationsBlock, overwrite = False):
        if self.modificationsBlock and not overwrite: 
            raise MzrExceptions.MoleculizerException("Error: Cannot add a modifications block twice.")

        if not self.BlockPassesSanityCheck( modificationsBlock ):
            raise InsaneBlockOnTheLooseException(modificationsBlock, "modifications block")

        self.modificationsBlock.extend( modificationsBlock )
        self.modificationsSection = ModificationsSection( self.modificationsBlock  )

        return


    def addMolsBlock(self, molsBlock):
        if self.molsBlock and not overwrite: 
            raise MzrExceptions.MoleculizerException("Error: Cannot add a mols block twice.")

        if not self.BlockPassesSanityCheck( molsBlock ):
            raise InsaneBlockOnTheLooseException(molsBlock, "mols block")

        self.molsBlock.extend( molsBlock )
        self.molsSection = MolsSection( molsBlock  )


    def addAllostericPlexesBlock(self, apBlock, overwrite = False):
        if self.allostericPlexes and not overwrite: 
            raise MzrExceptions.MoleculizerException("Error: Cannot add an allosteric plexes block twice.")

        if not self.BlockPassesSanityCheck( apBlock ):
            raise InsaneBlockOnTheLooseException(apBlock, "allosteric plexes block")

        self.allostericPlexes.extend( apBlock )
        self.allostericPlexesSection = AllostericPlexesSection( self.allostericPlexes )

    def addAllostericOmnisBlock(self, aoBlock, overwrite = False):
        if self.allostericOmnis and not overwrite: raise MzrExceptions.MoleculizerException("Error: Cannot add an allosteric omnis block twice.")
        
        if not self.BlockPassesSanityCheck( aoBlock ):
            raise InsaneBlockOnTheLooseException( aoBlock, "allosteric omnis block")
        self.allostericOmnis.extend( aoBlock )
        self.allostericOmnisSection = AllostericOmnisSection( self.allostericOmnis )

    def addReactionRulesBlock( self, rrBlock, dimerGenBlock, omniGenBlock, uniMolGenBlock, overwrite = False):

        if self.reactionRulesBlock and not overwrite: 
            raise MzrExceptions.MoleculizerException("Error: Cannot add a reaction rules block twice.")

        if not self.BlockPassesSanityCheck( rrBlock ):
            raise InsaneBlockOnTheLooseException(rrBlock, "reaction rules")

        if not self.BlockPassesSanityCheck( dimerGenBlock ):
            raise InsaneBlockOnTheLooseException(dimerGenBlock, "dimerization gen block")

        if not self.BlockPassesSanityCheck( omniGenBlock ):
            raise InsaneBlockOnTheLooseException(omniGenBlock, "omni-gen block")

        if not self.BlockPassesSanityCheck( uniMolGenBlock ):
            raise InsaneBlockOnTheLooseException(uniMolGenBlock, "uni-mol-gen block")


        self.reactionRulesBlock.extend( rrBlock )
        self.dimerizationGenBlock.extend( dimerGenBlock )
        self.omniGenBlock.extend( omniGenBlock )
        self.uniMolGenBlock.extend( uniMolGenBlock )

        self.reactionGensSection = ReactionRulesSection( self.reactionRulesBlock, 
                                                         self.dimerizationGenBlock, 
                                                         self.omniGenBlock, 
                                                         self.uniMolGenBlock)

    def addExplicitSpeciesBlock( self, esBlock, overwrite = False):
        if self.explicitSpeciesBlock and not overwrite: 
            raise MzrExceptions.MoleculizerException("Error: Cannot add an explicit species block twice.")
        
        if not self.BlockPassesSanityCheck( esBlock  ):
            raise InsaneBlockOnTheLooseException(esBlock, "explicit-species")
        
        self.explicitSpeciesBlock.extend( esBlock )
        self.explicitSpeciesSection = ExplicitSpeciesSection( esBlock )


    def addSpeciesStreamsBlock(self, ssBlock, overwrite = False):
        if self.speciesStreamBlock and not overwrite: 
            raise MzrExceptions.MoleculizerException("Error: Cannot add a species stream block twice.")

        if not self.BlockPassesSanityCheck( ssBlock ):
            raise InsaneBlockOnTheLooseException(ssBlock, "")
        
        self.speciesStreamBlock.extend( ssBlock )
        self.speciesStreamSection = SpeciesStreamsSection( self.speciesStreamBlock  )


    def __processAllostericRulesBlocks( self, allostericPlexBlock, allostericOmniBlock):
        return 0
    def __processReactionRulesBlocks( self, rxnRulesBlock, dimerBlock, omniGenBlock, uniGenBlock):
        return 0

    def __processExplicitSpeciesBlock( self, explicitSpeciesBlock):
        return 0

    def __processSpeciesStreamBlock( self, ssBlock):
        return 0
    
    def __writeOutput(self, openXMLFile):
        xmlobject = self.__constructXMLRepresentation()
        xmlobject.writeall(openXMLFile)
        openXMLFile.flush()
        return 

    def __constructXMLRepresentation(self):
        
        rootNode = XmlObject("moleculizer-input")
        modelElmt = XmlObject("model")
        modelElmt.attachToParent(rootNode)

        streamsElmt = XmlObject("streams", rootNode)

        self.__addModifications( modelElmt )

        self.__addMols( modelElmt )
        self.__addAllostericPlexes( modelElmt )
        self.__addAllostericOmnis( modelElmt )
        self.__addReactionGens( modelElmt )
        self.__addExplicitSpecies( modelElmt )
        self.__addExplicitReactions( modelElmt )

        self.__addSpeciesStreams( streamsElmt )

        return rootNode

    def __addModifications(self, parentObject):
        modificationsSection = XmlObject("modifications", parentObject)
        
        if self.modificationsSection:
            self.modificationsSection.writeModificationsSections( modificationsSection )
        return

    def __addMols(self, parentObject):
        molsSection = XmlObject("mols", parentObject)

        if self.molsSection:
            self.molsSection.writeMolsSection( molsSection)
        
        return 

    def __addAllostericPlexes(self, parentObject):
        allostericPlexes = XmlObject("allosteric-plexes", parentObject)

        if self.allostericPlexesSection:
            self.allostericPlexesSection.writeAllostericPlexesSection(allostericPlexes)

        return 

    def __addAllostericOmnis(self, parentObject):
        allostericOmnis = XmlObject("allosteric-omnis", parentObject)

        if self.allostericOmnisSection:
            self.allostericOmnisSection.writeAllostericOmnisSection( allostericOmnis )

        return 

    def __addReactionGens(self, parentObject):
        reactionGenElmt = XmlObject("reaction-gens", parentObject)

        if self.reactionGensSection:
            self.reactionGensSection.writeReactionGensSection( reactionGenElmt )

        return 

    def __addSpeciesStreams( self, parentObject):

        speciesStreamsElement = XmlObject("species-streams", parentObject)

        if self.speciesStreamSection:
            self.speciesStreamSection.writeSpeciesStreamSection( speciesStreamsElement )

    def __addExplicitSpecies(self, parentObject):
        explicitSpeciesElmt = XmlObject("explicit-species", parentObject)
        
        if self.explicitSpeciesSection:
            self.explicitSpeciesSection.writeExplicitSpeciesSection( explicitSpeciesElmt )

        return

    def __addExplicitReactions( self, modelElmt ):
        explicitReactionsElmt = XmlObject("explicit-reactions", modelElmt)
        return 

# Global file
def parseBlockTypesFromRulesFile(textRulesFile):
    # Magic debugging check
    textRulesFile = [re.sub("#.*$", "", x) for x in textRulesFile] # Delete all comments
    # textRulesFile = [re.sub("//.*$", "", x) for x in textRulesFile] # Delete all comments
    textRulesFile = [re.sub(r"\s*", "", x) for x in textRulesFile] # Delete all whitespace
    textRulesFile = [x.strip() for x in textRulesFile] # Strip it for good measure
    textRulesFile = [x for x in textRulesFile if x != ""] # This must be last, because line.strip() results in some empty lines.

    for ndx, line in enumerate(textRulesFile):
        if line.startswith("xml_output"):
            filename = line[len("xml_output"):].strip()
            rettype = (True, filename)
            textRulesFile.pop(ndx)
            break
    else:
        rettype = (False, False)

    parameterBlock = []
    modificationsBlock = []
    molsBlock = []
    allostericPlexes = []
    allostericOmnis = []
    reactionRulesBlock = []
    dimerizationGenBlock = []
    omniGenBlock = [] 
    uniMolGenBlock = [] 
    explicitSpeciesBlock = [] 
    speciesStreamBlock = []

#     textRulesFile = '\n'.join(textRulesFile)
#     textRulesFile = re.sub(r"\\\s*\n\s*", " ", textRulesFile)
#     textRulesFile = textRulesFile.split("\n")

    blockCodes = ["Parameters", "Modifications", "Molecules", "Explicit-Allostery", "Allostery-Classes", 
                  "Reaction-Rules", "Association-Reactions", "Transformation-Reactions", 
                  "Explicit-Species", "Species-Classes" ] 

    blockObjNdx = -1
    blockDataObj = [ (blockCodes[0], parameterBlock), \
                     (blockCodes[1], modificationsBlock), \
                     (blockCodes[2], molsBlock), \
                     (blockCodes[3], allostericPlexes), 
                     (blockCodes[4], allostericOmnis), 
                     (blockCodes[5], reactionRulesBlock), \
                     (blockCodes[6], dimerizationGenBlock), \
                     (blockCodes[7], omniGenBlock), \
                     (blockCodes[8], explicitSpeciesBlock),\
                     (blockCodes[9], speciesStreamBlock) ]

    currentDmp = []

    try:
        assert( textRulesFile[0].startswith("="))
    except:
        raise Exception("Line '%s' should start with a '=', but does not." % textRulesFile[0])
       

    blockObjNdx = -1
    for line in textRulesFile:
        if line.startswith("="):
            blockObjNdx = returnNewIndex(line, blockDataObj)
            currentDmp = blockDataObj[blockObjNdx][1]
        else:
            currentDmp.append(line)


    return rettype, \
           getFormattedArray(parameterBlock), getFormattedArray(modificationsBlock), getFormattedArray(molsBlock), getFormattedArray(allostericPlexes), getFormattedArray(allostericOmnis), \
           getFormattedArray(reactionRulesBlock), getFormattedArray(dimerizationGenBlock), getFormattedArray(omniGenBlock), \
           getFormattedArray(explicitSpeciesBlock), getFormattedArray(speciesStreamBlock)

def returnNewIndex(lineOfText, blockObjData):
    key = lineOfText.strip().strip("=").strip()

    for ndx in range(len(blockObjData)):
        if key == blockObjData[ndx][0]:
            return ndx
    raise Exception("Section title '%s' cannot be found" % key)

    return -1

def barf(msg):
    sys.stderr.write(msg + '\n')
    sys.stderr.write("Crashing....\n")
    sys.exit(1)

def printerror(msg):
    sys.stderr.write(msg + '\n')
    return

def getFormattedArray( arrayToFormat ):
    tmpArray = getBalancedArray( arrayToFormat )
    tmpString = "".join( tmpArray )
    if tmpString == "": 
        return []

    try:
        assert( tmpString[-1] == ";" )
    except:
        raise Exception("Error parsing line %s.  Line appears to not end in ';'." % repr(arrayToFormat[-1]))

    tmpArray = tmpString.split(";") 
    tmpArray.pop() # Last entry is blank
    tmpArray = [tok + ";" for tok in tmpArray]
    
    return tmpArray
    

def getBalancedArray( arrayToBalance ):
    if not EachEntryIsParenBalanced( arrayToBalance ):
    # Combine the ..., ndx_i, ndx_(i+1) where ndx_i is the smallest i not balanced
        return getBalancedArray( GetIncrementallyBetterArray( arrayToBalance ) )
    else:
        return arrayToBalance

def GetIncrementallyBetterArray( anArray ):

    values = [ StringIsParenBalenced(x) for x in anArray]

    # This is correct: this function should only be used if the array does not pass
    # EachEntryIsParenBalanced.
    assert( False in values)

    badNdx = values.index( False )
    
    combinedTokens = anArray[badNdx] + anArray[badNdx + 1]

    returnArray = anArray[ : badNdx]
    returnArray.append( combinedTokens )
    returnArray.extend( anArray[badNdx + 2 : ] )
    
    return returnArray
    
def EachEntryIsParenBalanced( array ):
    entries = [ StringIsParenBalenced(x) for x in array ]
    
    returnVal = True
    for val in entries:
        returnVal = returnVal and val

    return returnVal

def StringIsParenBalenced(line):
    return ( line.count("(") == line.count(")") and 
             line.count("[") == line.count("]") and 
             line.count("{") == line.count("}") )
