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

class DimerGenSection( MoleculizerSection ):
    def __init__(self, dimerBlock):
        MoleculizerSection.__init__(self, "DimerGenSection", dimerBlock)
        return 

    def writeDimerGenSection(self, parentElmt):
        for line in self.getParsedLines():
            self.assertSanityOfDimerGenLine( line )
            self.writeDimerGenLineToParent( line, parentElmt)

        return

    def assertSanityOfDimerGenLine(self, line):
        return 
    
    def writeDimerGenLineToParent( self, line, reactionGens):
        dimerGen = XmlObject("dimerization-gen", reactionGens)

        parsedRxn = line.getParsedComponents()[0]
        theReactants = parsedRxn.getReactants()
        theProducts = parsedRxn.getProducts()

        reactant1 = theReactants[0].getMols()[0]
        reactant2 = theReactants[1].getMols()[0]

        molSpec1 = XmlObject("mol-ref", dimerGen)
        molSpec2 = XmlObject("mol-ref", dimerGen)
        
        molSpec1.addAttribute("name", reactant1.getName() )
        molSpec2.addAttribute("name", reactant2.getName() )

        siteRef1 = XmlObject("site-ref", molSpec1)
        siteRef2 = XmlObject("site-ref", molSpec2)

        self.__addSiteNameToElement( reactant1, siteRef1)
        self.__addSiteNameToElement( reactant2, siteRef2)

        defaultOnElmt = XmlObject("default-on-rate", dimerGen)
        defaultOffElmt = XmlObject("default-off-rate", dimerGen)

        defaultOnElmt.addAttribute("value", line.getAssignment("kon"))
        defaultOffElmt.addAttribute("value", line.getAssignment("koff"))



        if len( line.getParsedComponents())  == 3:
            return

        # Now add the allosteric on- and off- rates.
        allosteryComponents = line.getParsedComponents()[3:]
        
        allostery_index = 3
        while (len(allosteryComponents) > 0):
            # This is used in a hacky way to find the correct parameter

            alloRatesElmt = XmlObject("allo-rates", dimerGen)

            alloReactants = allosteryComponents[0].getReactants()

            siteShapeRef1 = XmlObject("site-shape-ref", alloRatesElmt)
            siteShapeRef2 = XmlObject("site-shape-ref", alloRatesElmt)
            onRate = XmlObject("on-rate", alloRatesElmt)
            offRate = XmlObject("off-rate", alloRatesElmt)

            if alloReactants[0].getMols()[0].isModMol():
                if alloReactants[0].getMols()[0].getBindingSites()[0].hasBindingSiteSpecification():
                    siteShapeRef1.addAttribute("name", alloReactants[0].getMols()[0].getBindingSites()[0].getBindingSiteSpecification().getShapeSpecification().getShapeList()[0])
                else:
                    siteShapeRef1.addAttribute("name", "default")

            else:
                siteShapeRef1.addAttribute("name", "default")

            if alloReactants[1].getMols()[0].isModMol():
                if alloReactants[1].getMols()[0].getBindingSites()[0].hasBindingSiteSpecification():
                    siteShapeRef2.addAttribute("name", alloReactants[1].getMols()[0].getBindingSites()[0].getBindingSiteSpecification().getShapeSpecification().getShapeList()[0])
                else:
                    siteShapeRef2.addAttribute("name", "default")
            else:
                siteShapeRef2.addAttribute("name", "default")

            onRate.addAttribute("value", line.getAssignment( "kon", allostery_index) )
            offRate.addAttribute("value", line.getAssignment( "koff", allostery_index) )
                                       
            allosteryComponents = allosteryComponents[3:]
            allostery_index += 3

        return

    def __addSiteNameToElement( self, parsedMolSpec, siteRefElmt):

        if ( parsedMolSpec.isSmallMol() ):
            siteRefElmt.addAttribute( "name", parsedMolSpec.getName() )
        else:
            # This better be true...
            # This is a dimerGen function, which must specify exactly one binding site per mol on the
            # reactant side.
            siteRefElmt.addAttribute( "name", parsedMolSpec.getBindingSiteList()[0] )

        return siteRefElmt
        
        
        
class OmniGenSection( MoleculizerSection ):
    def __init__(self, omniBlock):
        MoleculizerSection.__init__(self, "OmniGenSection", omniBlock)
        return

    def writeOmniGenSection( self, parentElmt ):
        for line in self.getParsedLines():
            self.assertSanityOfOmniGenLine( line )
            self.writeOmniGenLineToParent( line, parentElmt)
        return

    def assertSanityOfOmniGenLine(self, line):
        return


    def writeOmniGenLineToParent(self, line, parElmt):
        omniGenElment = XmlObject("omni-gen", parElmt)

        reaction = line.getParsedComponents()[0]
        
        enablingOmniplex = reaction.getReactants()[0]
        additionalReactant = 0
        if len(reaction.getReactants()) > 1:
            additionalReactant = reaction.getReactants()[1]

        product = reaction.getProducts()[0]
        additionalProduct = 0
        if len(reaction.getProducts()) > 1:
            additionalProduct = reaction.getProducts()[1]

        self.__writeEnablingOmniPlexElementToXml(enablingOmniplex, omniGenElment)

        self.__writeModificationExchanges( reaction, omniGenElment)
        self.__writeSmallMolExchange( reaction, omniGenElment)

        rateElmt = XmlObject("rate", omniGenElment)
        rateElmt.addAttribute("value", line.getAssignment("k"))
        
        return

    def __writeModificationExchanges( self, reaction, parElmt):
        modExchanges = XmlObject("modification-exchanges", parElmt)

        omniplex = reaction.getProducts()[0]

        for ndx in range(len(omniplex.getMols())):
            mol = omniplex.getMols()[ndx]
            if mol.isSmallMol():
                continue

            for mod_site in mol.getModificationSites():
                modExchange = XmlObject("modification-exchange", modExchanges)
                modMolInstance = XmlObject("mod-mol-instance-ref", modExchange)

                modMolInstance.addAttribute("name", self.getMolName( mol.getName(), ndx))
                
                modSiteRefElmt = XmlObject("mod-site-ref", modMolInstance)
                modSiteRefElmt.addAttribute("name", mod_site.getName())

                installedModRefElmt = XmlObject("installed-mod-ref", modExchange)
                installedModRefElmt.addAttribute("name", mod_site.getModificationSiteSpecification().getList()[0])
                    
        return 

    def __writeSmallMolExchange(self, reaction, parElmt):
        smallMolExchangesElmt = XmlObject("small-mol-exchanges", parElmt)

        reactant_omniplex = reaction.getReactants()[0]
        product_omniplex = reaction.getProducts()[0]

        assert(len(reactant_omniplex.getMols()) == len(product_omniplex.getMols()))
        for ndx in range(len(reactant_omniplex.getMols())):
            if reactant_omniplex.getMols()[ndx].getName() != product_omniplex.getMols()[ndx].getName():
                assert( reactant_omniplex.getMols()[ndx].isSmallMol() )

                smallMolExchangeElmt = XmlObject("small-mol-exchage", smallMolExchangesElmt)

                smallMolInstRef = XmlObject("small-mol-instance-ref", smallMolExchangeElmt)
                smallMolRef = XmlObject("small-mol-ref", smallMolExchangeElmt)

                smallMolInstRef.addAttribute("name", self.getMolName(reactant_omniplex.getMols()[ndx].getName(), ndx))
                smallMolRef.addAttribute("name", product_omniplex.getMols()[ndx].getName() )
    
        return

    def __writeEnablingOmniPlexElementToXml(self, omniplex, omniGenElment):
        enablingOmniplex = XmlObject("enabling-omniplex", omniGenElment)

        self.writeParsedComplexAsPlex( omniplex, enablingOmniplex )

        # We shouldn't always parse this, is this ok?
        self.__writeInstanceStateToXml( omniplex, enablingOmniplex)

        return enablingOmniplex

    def __writeInstanceStateToXml( self, omniplex, enablingOmniplexElmt ):
        written_instance_states = False
        instStatesElmt = 0

        for ndx in  range(len(omniplex.getMols())):
            mol = omniplex.getMols()[ndx]
        
            if mol.isSmallMol():
                continue

            for modSite in mol.getModificationSites():
                if modSite.hasModificationSiteSpecification() and modSite.getModificationSiteSpecification().isList():
                    if not written_instance_states:
                        instStatesElmt = XmlObject( "instance-states",  enablingOmniplexElmt)
                        written_instance_states = True
                self.__writeModMolInstanceToInstanceStates(mol, ndx, instStatesElmt)
                    
            

    def __writeModMolInstanceToInstanceStates(self, parsedMol, molNdx, parentElmt):
        modMolInstanceRefElmt = XmlObject("mod-mol-instance-ref", parentElmt)
        modMolInstanceRefElmt.addAttribute("name", self.getMolName( parsedMol.getName(), molNdx))

        modMapElmt = XmlObject("mod-map", modMolInstanceRefElmt)
        
        for modSite in parsedMol.getModificationSites():
            modSiteRefElmt = XmlObject("mod-site-ref", modMapElmt)
            modSiteRefElmt.addAttribute("name", modSite.getName())

            modRefElmt = XmlObject("mod-ref", modSiteRefElmt)
            modRefElmt.addAttribute("name", modSite.getModificationSiteSpecification().getList()[0])

        
            
        


class UniGenSection( MoleculizerSection ):
    def __init__(self, uniGenBlock):
        MoleculizerSection.__init__(self, "unigensection", uniGenBlock)
        return 

    def writeUniMolGenSection( self, parentelmt ):
        for line in self.getParsedLines():
            self.assertsanityofunimolgenline( line )
            self.writeUniMolGenLineToParent( line, parentElmt)
        return

    def assertSanityOfUniMolGenLine( self, line):
        # TODO
        return

    def writeUniMolGenLineToParent( self, line, parent):

        return


class ReactionRulesSection( MoleculizerSection ):
    def __init__(self, rxnBlock, dimerGenBlock, omniGenBlock, uniGenBlock):
        MoleculizerSection.__init__(self, "ReactionRulesSection", rxnBlock)

        self.the_dimer_section = DimerGenSection( dimerGenBlock )
        self.the_omni_gen_section = OmniGenSection( omniGenBlock )
        self.the_uni_gen_section = UniGenSection( uniGenBlock )

        return

    def writeReactionGensSection(self, parentSection):

        if self.the_dimer_section:
            self.the_dimer_section.writeDimerGenSection( parentSection )

        if self.the_omni_gen_section:
            self.the_omni_gen_section.writeOmniGenSection( parentSection )

        if self.the_uni_gen_section:
            self.the_uni_gen_section.writeUniMolGenSection( parentSection )
