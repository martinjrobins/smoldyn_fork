from sectionmzr import MoleculizerSection
from parsedline import ParsedLine
from xmlobject import XmlObject
from section_xcpt import *
import pdb


class ModificationsSection( MoleculizerSection ):
    def __init__(self, modificationsBlock):
        MoleculizerSection.__init__(self, "modifications", modificationsBlock)
        return

    def writeModificationsSections(self, modificationsElmt):
        for parsedLine in self.getParsedLines():
            self.assertParsedModificationsLineSanity( parsedLine )
            self.writeModificationToXml( parsedLine, modificationsElmt)

    def assertParsedModificationsLineSanity(self, parsedModLine):
        if not len(parsedModLine.getParsedComponents()) == 2:
            raise Exception()

    def writeModificationToXml(self, modLine, modsElmt):
        
        modElmt = XmlObject("modification", modsElmt)
        modElmt.addAttribute( "name", modLine.getAssignment( "name" ))
        
        weightDeltaElmt = XmlObject("weight-delta", modElmt)
        weightDeltaElmt.addAttribute("daltons", modLine.getAssignment("mass"))

        return 
