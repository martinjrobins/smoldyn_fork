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

class ExplicitSpeciesSection( MoleculizerSection ):
    def __init__(self, explicitSpeciesBlock ):
        MoleculizerSection.__init__(self, "ExplicitSpeciesSection", explicitSpeciesBlock)
        return
        
    def writeExplicitSpeciesSection(self, xmlobject):
        for line in self.getParsedLines():
            self.assertSanityOfExplicitSpeciesLine( line )
            self.writeExplicitSpeciesLineToXml( line, xmlobject )

    def assertSanityOfExplicitSpeciesLine( self, line):
        if not line.getParsedComponents()[0].isComplex(): 
            raise Exception()

        if not line.hasAssignment( "name" ):
            raise Exception()
        
        return 

    def writeExplicitSpeciesLineToXml( self, parsedLine, parentObject):
        parsedComplex = parsedLine.getParsedComponents()[0]
        name = parsedLine.getAssignment("name")

        plexSpeciesElmt = XmlObject( "plex-species", parentObject)
        plexSpeciesElmt.addAttribute( "name", name)

        self.writeParsedComplexAsPlex( parsedComplex, plexSpeciesElmt, writeComplexInstanceStates = True)
        
        
        
