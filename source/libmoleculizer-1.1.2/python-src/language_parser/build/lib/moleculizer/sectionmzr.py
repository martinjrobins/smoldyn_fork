###############################################################################
# Copyright (C) 2007, 2008, 2009 The Molecular Sciences Institute
# Original Author:
#   Nathan Addy, Scientific Programmer	Email: addy@molsci.org
#   The Molecular Sciences Institute    
#
###############################################################################

from parsedline import ParsedLine
from xmlobject import XmlObject
import pdb

class MoleculizerSection( object ) :
    translation_mode = "STRICT"

    def __init__(self, sectionName, sectionBlock):
        self.section_name = sectionName
        self.original_block = sectionBlock[:]
        self.parsed_lines = []
        for line in self.original_block:
            try:
                self.parsed_lines.append( ParsedLine(line) )
            except:
                print "Error in parsing line %s" % line

    def getMolName(self, molName, ndx):
        return molName + "-" + str(ndx)
        
    def getParsedLines(self):
        return self.parsed_lines

    def getSectionName(self):
        return self.section_name

    def getOriginalBlock(self):
        return self.original_block[:]

    def writeBoundStates(self, complex, patternMatchingSitesSpec, xmlobject):
        boundPatterns = [x for x in patternMatchingSitesSpec if x[2] == "+"]

        boundSectionElmt = XmlObject("bound-sites", xmlobject)

        for pattern in boundPatterns:
            mol_ndx, site_ndx = pattern[0], pattern[1]
            instanceRef = XmlObject("instance-ref", boundSectionElmt)
            instanceRef.addAttribute("name", \
                                     self.getMolName(complex.getMols()[mol_ndx].getName(), \
                                                     mol_ndx))

            siteRef = XmlObject("site-ref", instanceRef)
            siteRef.addAttribute("name", pattern[1])
        
        return

    def writeUnboundStates(self, complex, patternMatchingSitesSpec, xmlobject):
        unboundPatterns = [x for x in patternMatchingSitesSpec if x[2] == "-"]

        unboundSectionElmt = XmlObject("unbound-sites", xmlobject)

        for pattern in unboundPatterns:
            mol_ndx, site_ndx = pattern[0], pattern[1]
            instanceRef = XmlObject("instance-ref", unboundSectionElmt)
            instanceRef.addAttribute("name", \
                                     self.getMolName(complex.getMols()[mol_ndx].getName(), \
                                                     mol_ndx))

            siteRef = XmlObject("site-ref", instanceRef)
            siteRef.addAttribute("name", pattern[1])        
        return 

    def writeBoundUnboundStates(self, complex, patternMatchingSitesSpec, xmlobject):
        codes = [ x[2] for x in patternMatchingSitesSpec]
        if "-" in codes:
            self.writeUnboundStates(complex, patternMatchingSitesSpec, xmlobject)
        if "+" in codes:
            self.writeBoundStates(complex, patternMatchingSitesSpec, xmlobject)
        
        return 

    def writeParsedComplexInstanceStates(self, complex, xmlobject):
        # This is quite inelegant, but I can't obviously think of a
        # better way to do it.

        for mol_ndx, parsed_mol in enumerate(complex.getMols()):
            if not parsed_mol.isModMol():
                continue

            if not parsed_mol.getModificationSiteList():
                continue
            break
        else:
            # If we get 
            return 
        
        instanceStatesElmt = XmlObject("instance-states", xmlobject)
        # For each mol in complex which is a mod map that has modifications specified
        for mol_ndx, parsed_mol in enumerate(complex.getMols()):
            if not parsed_mol.isModMol():
                continue

            if not parsed_mol.getModificationSiteList():
                continue
            
            modMolInstanceRef = XmlObject("mod-mol-instance-ref", instanceStatesElmt)
            modMolInstanceRef.addAttribute( "name", self.getMolName( parsed_mol.getName(), mol_ndx ))

            modMapElmt = XmlObject("mod-map", modMolInstanceRef)

            for modification in parsed_mol.getModificationSites():
                modSiteRefNameElmt = XmlObject("mod-site-ref", modMapElmt)
                modSiteRefNameElmt.addAttribute("name", modification.getName())

                modRefElmt = XmlObject("mod-ref", modSiteRefNameElmt)

                modState = modification.getModificationSiteSpecification().getList()[0]

                modRefElmt.addAttribute("name", modState)

        return 
        
    def writeParsedComplexAsPlex( self, complex, xmlobject, writeComplexInstanceStates = False, writeBoundUnboundStates = False):

        #
        if writeBoundUnboundStates:
            # This should be a list of things like (2, 1, +) = the
            # first binding on the second mol must be bound.
            patternMatchingSitesSpec = complex.externalizePatternMatchingSites()
        
        plexElmt = XmlObject( "plex", xmlobject)

        for ndx in range(len(complex.getMols())):
            parsedMol = complex.getMols()[ndx]

            molInst = XmlObject("mol-instance", plexElmt)
            molInst.addAttribute( "name", self.getMolName( parsedMol.getName(), ndx ))
            
            molTypeElmt = XmlObject("mol-ref", molInst)
            molTypeElmt.addAttribute( "name", parsedMol.getName() )

        for bindingID in complex.getBindings().keys():
            # Find the two bindings with that 
            newBindingElmt = XmlObject("binding", plexElmt)

            try:
                molNdx1, molNdx2 = complex.getBindings()[bindingID]
            except ValueError:
                raise Exception("Error unpacking bindings '%s' associated with complex '%s' -- Binding ID only has one value." % (repr(complex.getBindings()[bindingID]), complex.getOriginalLine(), bindingID))
            
            parsedMol1 = complex.getMols()[molNdx1]
            parsedMol2 = complex.getMols()[molNdx2]
            bindingSite1 = ""
            bindingSite2 = ""

            if parsedMol1.isModMol():
                for bindingSite in parsedMol1.getBindingSiteList():
                    parsedBindingSite = parsedMol1.getBindingSiteWithName( bindingSite )
                    if parsedBindingSite.hasBindingToken(bindingID):
                        bindingSite1 = parsedBindingSite.getName()
                        break
                else:
                    print "Error, binding site 1 not found"
                    raise Exception()
            else:
                bindingSite1 = parsedMol1.getName()
         
            if parsedMol2.isModMol():
                for bindingSite in parsedMol2.getBindingSiteList():
                    bindingSite = parsedMol2.getBindingSiteWithName( bindingSite )
                    if bindingSite.hasBindingToken( bindingID ):
                        bindingSite2 = bindingSite.getName()
                        break
                else:
                    print "Error, binding site 2 not found"
                    raise Exception()
            else:
                bindingSite2 = parsedMol2.getName()
            
            molInstance1 = XmlObject("mol-instance-ref", newBindingElmt)
            molInstance1.addAttribute( "name", self.getMolName(parsedMol1.getName(), molNdx1))
            
            bindingSiteRef1 = XmlObject("binding-site-ref", molInstance1)
            bindingSiteRef1.addAttribute("name", bindingSite1)


            molInstance2 = XmlObject("mol-instance-ref", newBindingElmt)
            molInstance2.addAttribute( "name", self.getMolName(parsedMol2.getName(), molNdx2))
            
            bindingSiteRef2 = XmlObject("binding-site-ref", molInstance2)
            bindingSiteRef2.addAttribute("name", bindingSite2)

        #pdb.set_trace()
        if writeComplexInstanceStates:
            self.writeParsedComplexInstanceStates(complex, xmlobject)

        if writeBoundUnboundStates:
            self.writeBoundUnboundStates(complex, patternMatchingSitesSpec, xmlobject)

        return plexElmt
        

#     def __molHasBindingToken( self, parsedMol, bindingToken):
#         for parsedBnd in parsedMol.getBindingSiteList():
#             if parsedBnd.hasBindingToken( bindingToken ):
#                 return True
#         else:
#             return False


