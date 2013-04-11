from sectionmzr import MoleculizerSection
from xmlobject import XmlObject
from section_xcpt import *
import pdb

class AllosterySection( MoleculizerSection ):
    def __init__(self, name, allosterySection):
        MoleculizerSection.__init__(self, name, allosterySection)
        return

    def assertSanityOfAllosteryLine(self, line):
        if not len(line.getParsedComponents() ) == 1:
            raise Exception()

        if not line.getParsedComponents()[0].isComplex():
            raise Exception()

    def writeAllostericSitesElementToElement( self, parsedComplex, parentElement):
        allostericSitesElmt = XmlObject("allosteric-sites", parentElement)

        for ndx in range(len(parsedComplex.getMols())):
            mol = parsedComplex.getMols()[ndx]
            if mol.isSmallMol():
                continue

#             [x for x in mol.getBindingSites() \
#                  if x.hasBindingSiteSpecification() and \
#                  x.getBindingSiteSpecification().hasShapeSpecification()  and \
#                  x.getBindingSiteSpecification().getShapeSpecification().isTransformation() ]:
            
            for bndSiteWithTrans in [x for x in mol.getBindingSites() \
                                         if x.hasBindingSiteSpecification() and \
                                         x.getBindingSiteSpecification().hasShapeSpecification()  and \
                                         x.getBindingSiteSpecification().getShapeSpecification().isTransformation() ]:


                transformation = \
                    bndSiteWithTrans.getBindingSiteSpecification().getShapeSpecification().getTransformation()
                
                molElmt = XmlObject("mol-instance-ref", allostericSitesElmt)
                molElmt.addAttribute("name", self.getMolName( mol.getName(), ndx) )

                bindingRefElmt = XmlObject( "binding-site-ref", molElmt)
                bindingRefElmt.addAttribute( "name", bndSiteWithTrans.getName() )
                
                siteShapeRefElmt = XmlObject( "site-shape-ref", bindingRefElmt )
                siteShapeRefElmt.addAttribute( "name", transformation )


            
class AllostericPlexesSection( AllosterySection ):
    def __init__(self, allostericPlexSection):
        AllosterySection.__init__(self, "AllostericPlex", allostericPlexSection)

    def writeAllostericPlexesSection(self, xmlobject):
        for line in self.getParsedLines():
            self.assertSanityOfAllosteryLine(line)
            self.writeAllostericPlexLineToXml( line, xmlobject)

    def writeAllostericPlexLineToXml( self, lineToWrite, xmlobject):
        alloPlexElmt = XmlObject("allosteric-plex", xmlobject)

        self.writeParsedComplexAsPlex( lineToWrite.getParsedComponents()[0], alloPlexElmt)
        self.writeParsedComplexInstanceStates(lineToWrite.getParsedComponents()[0], alloPlexElmt)
        self.writeAllostericSitesElementToElement( lineToWrite.getParsedComponents()[0], alloPlexElmt)

        return




class AllostericOmnisSection( AllosterySection ):
    def __init__(self, allostericOmnisSection):
        AllosterySection.__init__(self, "AllostericOmni", allostericOmnisSection)

    def writeAllostericOmnisSection(self, xmlobject):
        for line in self.getParsedLines():
            self.assertSanityOfAllosteryLine(line)
            self.writeAllostericOmniLineToXml( line, xmlobject)

    def writeAllostericOmniLineToXml( self, lineToWrite, xmlobject):
        alloOmniElmt = XmlObject("allosteric-omni", xmlobject)

        self.writeParsedComplexAsPlex( lineToWrite.getParsedComponents()[0], alloOmniElmt )
        self.writeParsedComplexInstanceStates(lineToWrite.getParsedComponents()[0], alloOmniElmt)
        self.writeAllostericSitesElementToElement( lineToWrite.getParsedComponents()[0], alloOmniElmt )
        
        return
