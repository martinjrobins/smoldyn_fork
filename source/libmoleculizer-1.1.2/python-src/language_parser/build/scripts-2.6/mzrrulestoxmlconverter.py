#!/usr/bin/python

import getopt, sys, re, pdb
import moleculizer

PROGRAM_NAME = "MzrRulesToXmlConverter"
CURRENT_VERSION = 1.0
CURRENT_VERSION_STR = str( CURRENT_VERSION )

def main():
    # Handle command line options
    options = {}
    options["inputfile"] = ""
    options["outputfile"] = "moleculizer-rules-output.mzr"
    options["verbose"] = False

    processOptions(options)

    if not ensureAppropriateOptions(options):
        usage()
        sys.exit(0)

    rulesFile = open(options["inputfile"]).readlines()

    parameterBlock = []
    modificationsBlock  =  []
    molsBlock = []
    allostericPlexes = []
    allostericOmnis = []
    reactionRulesBlock = []
    dimerizationGenBlock = []
    omniGenBlock = []
    explicitSpeciesBlock = []
    speciesStreamBlock = []

    try:
        parameterBlock, modificationsBlock, molsBlock, allostericPlexes, allostericOmnis, \
        reactionRulesBlock, dimerizationGenBlock, omniGenBlock, \
        explicitSpeciesBlock, speciesStreamBlock = parseBlockTypesFromRulesFile( rulesFile )
        
    except Exception, e:
        print e.message
        barf("Error, Text rules file could not be parsed.")

    if options["verbose"]:
        print "Parameters:\n\t", parameterBlock
        print "Modifications:\n\t", modificationsBlock
        print "Molecules:\n\t", molsBlock
        print "Explicit-Allostery:\n\t", allostericPlexes
        print "Allostery-Classes omnis:\n\t", allostericOmnis
        print "Reaction Rules:\n\t", reactionRulesBlock
        print "Association-Reactions:\n\t", dimerizationGenBlock
        print "Transformation-Reactions:\n\t", omniGenBlock
        print "Explicit-Species:\n\t", explicitSpeciesBlock
        print "Species-Classes:\n\t", speciesStreamBlock

    outputFile = moleculizer.MoleculizerRulesFile( options["outputfile"] )

    outputFile.addParameterBlock( parameterBlock )
    outputFile.addModicationsBlock( modificationsBlock )
    outputFile.addMolsBlock( molsBlock )
    outputFile.addAllostericPlexesBlock( allostericPlexes )
    outputFile.addAllostericOmnisBlock( allostericOmnis )
    outputFile.addReactionRulesBlock( reactionRulesBlock, dimerizationGenBlock, \
                                          omniGenBlock, [] )
    outputFile.addExplicitSpeciesBlock( explicitSpeciesBlock )
    outputFile.addSpeciesStreamsBlock( speciesStreamBlock )

#    outputFile.initialize_DEBUG()

    outputFile.write()
    outputFile.close()
    
    print("Done.")

def processOptions(optionsDict):
    try:
	options, arguments = getopt.getopt(sys.argv[1:], "hf:o:vm:", ["help", "file=", "output=", "verbose", "version", "mixin="])
    except getopt.error, msg:
	print msg
	print "For help, use --help"
	sys.exit(0)

    for opt, atr in options:

	if opt in ("--help", "-h"):
	    usage()
            sys.exit(0)
	if opt in ("--version",):
	    version()
            sys.exit(0)

        if opt in ("--verbose", "-v"):
            optionsDict["verbose"] = True
	if opt in ("--file", "-f"):
	    optionsDict["inputfile"] = atr
	    fileRequired = True
	if opt in ("--output", "-o"):
	    optionsDict["outputfile"] = atr
        if opt in ("--mixin", "-m"):
            optionsDict["events_and_streams_mixin"] = atr

def ensureAppropriateOptions(options):
    if not options["inputfile"]:
        return False
    else:
        return True

def usage():
    print "%s version %s" % (PROGRAM_NAME, CURRENT_VERSION)
    print "\t-- Creates a Moleculizer XML input file from a human-readable text version of biochemical network rules."
    print
    print "Usage:"
    print "mzrrulestoxmlconverter.py -f inputfile [-o outputfile] [-m eventsandstreamsmixin]"
    print "mzrrulestoxmlconverter.py [-h] or [--help]"
    print 
    print "Author:"
    print "Nathan Addy (nathan.addy@gmail.com)"
    print "Molecular Sciences Institute, Berkeley, CA"
    print "March 3, 2009"
    print
    print 
    print "This is a parser that takes in a moleculizer rules file in text file format and writes it to a moleculizer xml file"
    print "for input into a moleculizer run."

def version():
    print "%s %s -- Copyright (C) 2009 The Moleculizer Sciences Institute" % (PROGRAM_NAME, CURRENT_VERSION)
    print
    print "Author:"
    print "Nathan Addy (nathan.addy@gmail.com)"
    print "Molecular Sciences Institute, Berkeley, CA"
    print "March 3, 2009"
    print 
    print "Licensed under LGPL v3."
    print 

    print "Moleculizer is free software; you can redistribute it and/or modify"
    print "it under the terms of the GNU Lesser General Public License as published by"
    print "the Free Software Foundation; either version 3 of the License, or"
    print "(at your option) any later version."
    print
    print "Moleculizer is distributed in the hope that it will be useful,"
    print "but WITHOUT ANY WARRANTY; without even the implied warranty of"
    print "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
    print "GNU Lesser General Public License for more details."
    print
    print "You should have received a copy of the GNU Lesser General Public License"
    print "along with Moleculizer; if not, write to the Free Software"
    print "Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA"
    print 

def parseBlockTypesFromRulesFile(textRulesFile):
    textRulesFile = [re.sub("#.*$", "", x) for x in textRulesFile] # Delete all comments
    # textRulesFile = [re.sub("//.*$", "", x) for x in textRulesFile] # Delete all comments
    textRulesFile = [re.sub(r"\s*", "", x) for x in textRulesFile] # Delete all whitespace
    textRulesFile = [x.strip() for x in textRulesFile] # Strip it for good measure
    textRulesFile = [x for x in textRulesFile if x != ""] # This must be last, because line.strip() results in some empty lines.

# This will attach lines connected with "\"
#     bngFile = '\n'.join(bngFile)
#     bngFile = re.sub(r"\\\s*\n\s*", " ", bngFile)
#     bngFile = bngFile.split("\n")

    parameterBlock = []
    modificationsBlock = []
    molsBlock = []
    allostericPlexes = []
    allostericOmnis = []
    reactionRulesBlock = []
    dimerizationGenBlock = []
    omniGenBlock = [] 
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

    assert( textRulesFile[0].startswith("="))

    blockObjNdx = -1
    for line in textRulesFile:
        if line.startswith("="):
            blockObjNdx = returnNewIndex(line, blockDataObj)
            currentDmp = blockDataObj[blockObjNdx][1]
        else:
            currentDmp.append(line)


    return getFormattedArray(parameterBlock), getFormattedArray(modificationsBlock), getFormattedArray(molsBlock), getFormattedArray(allostericPlexes), getFormattedArray(allostericOmnis), \
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
        raise Exception("Error parsing block '%s'.  Line does not end in ';'." % repr(arrayToFormat))

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

if __name__ == "__main__":
    main()


