###############################################################################
# Copyright (C) 2007, 2008, 2009 The Molecular Sciences Institute
# Original Author:
#   Nathan Addy, Scientific Programmer	Email: addy@molsci.org
#   The Molecular Sciences Institute    
#
###############################################################################

from sectionmzr import MoleculizerSection
from xmlobject import XmlObject
from section_xcpt import *
import pdb

class SpeciesStreamsSection( MoleculizerSection ):
    def __init__(self, speciesStreamBlock):
        MoleculizerSection.__init__(self, "SpeciesStreamsSection", speciesStreamBlock)
        return 

    def writeSpeciesStreamSection( self, parentElmt):
        for line in self.getParsedLines():
            self.assertSanityOfSpeciesStreamLine( line )
            self.writeSpeciesStreamLineToParent( line, parentElmt)

        return

    def assertSanityOfSpeciesStreamLine(self, line):
        if not line.getParsedComponents()[0].isComplex(): 
            raise Exception()

        if not line.hasAssignment( "name" ):
            raise Exception()
        
        return 
    
    def writeSpeciesStreamLineToParent( self, line, parentElmt):
        omniSpeciesStreamElmt = XmlObject("omni-species-stream", parentElmt)
        omniSpeciesStreamElmt.addAttribute("name", line.getAssignment("name"))

        self.writeParsedComplexAsPlex( line.getParsedComponents()[0], \
                                       omniSpeciesStreamElmt, \
                                       writeComplexInstanceStates = True, \
                                       writeBoundUnboundStates = True)
        
        return
        
