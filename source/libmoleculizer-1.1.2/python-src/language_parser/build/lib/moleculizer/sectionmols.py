from sectionmzr import MoleculizerSection
from xmlobject import XmlObject
from section_xcpt import *
import pdb

class MolsSection( MoleculizerSection ) :
    def __init__(self, molsBlockToInterpret):
        MoleculizerSection.__init__(self, "mols", molsBlockToInterpret)
        return

    def writeMolsSection(self, molsElmt):
        for parsedLine in self.getParsedLines():
            self.assertParsedLineSanityAsMolDefinition( parsedLine )
            self.writeParsedLineAsXmlMol( parsedLine, molsElmt )

    def assertParsedLineSanityAsMolDefinition(self, parsedLine):
        # We should check and make sure only the first thing

        if not parsedLine.getParsedComponents()[0].isComplex():
           raise Exception()

        if not len(parsedLine.getParsedComponents()[0].getMols()) == 1:
            raise Exception()
        
        theComplexes = [x.isComplex() for x in parsedLine.getParsedComponents()]

        if not theComplexes.count( True ) == 1:
            raise Exception()

        if not parsedLine.hasAssignment("mass") and MoleculizerSection.translation_mode == "STRICT":
            raise Exception("Parsed line '%s' should have a mass component, but it cannot be found." % parsedLine.getOriginalLine() )

    def writeParsedLineAsXmlMol(self, parsedLine, xmlobject):
        parsedComplex = parsedLine.getParsedComponents()[0]
        assert( len( parsedComplex.getMols() ) == 1)

        parsedMol = parsedComplex.getMols()[0]

        newMolElement = 0
        if parsedMol.isSmallMol():
            newMolElement = self.addSmallMolToXml( parsedMol, xmlobject )
        else:
            newMolElement = self.addModMolToXml( parsedMol, xmlobject)


        # Now add the mass element to the thing.

        mass = 0.0

        if ( parsedLine.hasAssignment("mass") ):
            mass = parsedLine.getAssignment("mass")
        elif self.getTranslationLevel == self.STRICT:
            raise Exception()

        
        weightElmt = XmlObject("weight")
        weightElmt.addAttribute("daltons", mass)

        newMolElement.addSubElement( weightElmt )
            

    def addSmallMolToXml( self, parsedSmallMol, xmlElement ):
        smallMolElt = XmlObject("small-mol")
        smallMolElt.addAttribute( "name", parsedSmallMol.getName() )

        smallMolElt.attachToParent( xmlElement)

        return smallMolElt


    def addModMolToXml( self, parsedMol, xmlObject):
        modMolElmt = XmlObject( "mod-mol")
        modMolElmt.attachToParent(xmlObject)
        modMolElmt.addAttribute( "name",  parsedMol.getName() )

        for bindingSiteName in parsedMol.getBindingSiteList():

            bindingSiteElmt = XmlObject("binding-site")
            bindingSiteElmt.attachToParent( modMolElmt)
            bindingSiteElmt.addAttribute("name", bindingSiteName)
            
            parsedBindingSite = parsedMol.getBindingSiteWithName( bindingSiteName )

            if( parsedBindingSite.hasBindingSiteSpecification() and parsedBindingSite.getBindingSiteSpecification().hasShapeSpecification()):

                bindingShapeNames = parsedBindingSite.getBindingSiteSpecification().getShapeSpecification().getShapeList()

                assert( len(bindingShapeNames) > 0 )
                
                defaultShapeElmt = XmlObject("default-shape-ref")
                defaultShapeElmt.addAttribute("name", bindingShapeNames[0] )
                defaultShapeElmt.attachToParent( bindingSiteElmt )

                for shape_name in bindingShapeNames:
                    newShapeElmt = XmlObject("site-shape")
                    newShapeElmt.addAttribute("name", shape_name)
                    newShapeElmt.attachToParent( bindingSiteElmt )
            else:
                defaultShapeElmt = XmlObject( "default-shape-ref" )
                defaultShapeElmt.addAttribute( "name", "default")
                defaultShapeElmt.attachToParent( bindingSiteElmt )
                
                shapeElmt = XmlObject("site-shape")
                shapeElmt.addAttribute("name", "default")
                shapeElmt.attachToParent( bindingSiteElmt )
                    

        for modSiteName in parsedMol.getModificationSiteList():
            modSiteElmt = XmlObject("mod-site", modMolElmt)
            modSiteElmt.addAttribute("name", modSiteName)

            parsedModificationSite = parsedMol.getModificationSiteWithName( modSiteName )
            if not parsedModificationSite.hasModificationSiteSpecification():
                defaultModElmt = XmlObject("default-mod-ref", modSiteElmt)
                defaultModElmt.addAttribute("name", "none")
            else:
                modValueArray = parsedModificationSite.getModificationSiteSpecification().getList()
                try:
                    assert( len(modValueArray) > 1)
                except:
                    print "Error, in sectionmols::addModMolToXml for mol with line '%s'.  Mod Value Array '%s' should have more than one component..." % ( parsedMol.getOriginalLine(), str(modValueArray) )
                    
                defaultModification = modValueArray[0]

                defaultModElmt = XmlObject("default-mod-ref", modSiteElmt)
                defaultModElmt.addAttribute("name", defaultModification )
                
        return modMolElmt
            
            
            

    
            
        
        
