import util, pdb
from parsedmzrtoken import ParsedMzrToken
from parsedbindingsite import ParsedBindingSite, ParsedHalfBindingSpecification
from parsedmodificationsite import ParsedModificationSite

class ParsedMol( ParsedMzrToken ):
    def MolTokenPassesSanityChecks( line ):
        return True

    MolTokenPassesSanityChecks = staticmethod( MolTokenPassesSanityChecks )


    def __init__(self, molDefinition ):
        ParsedMzrToken.__init__(self, molDefinition )

        # I want to be strict about forbidding bindings
        # Parsing of complexes must factor them out first

        self.is_small_mol = False
        self.has_small_mol_binding = False
        self.small_mol_binding = 0

        self.mol_name = ""

        self.the_parsed_bindings = []
        self.the_parsed_modification_sites = []

        self.parse()

    def isSmallMol(self):
        return self.is_small_mol;

    def isModMol(self):
        return not self.is_small_mol

    def getName(self):
        return self.mol_name

    def getBindingSites(self):
        if self.isSmallMol():
            raise SmallMolSemanticException(self.mol_name, "Binding sites requested for small mol")

        return self.the_parsed_bindings

    def getBindingSiteList(self):
        if self.isSmallMol():
            raise SmallMolSemanticException(self.mol_name, "Binding sites requested for small mol")
        return [parsed_binding.getName() for parsed_binding in self.the_parsed_bindings]

    def hasPatternBinding(self):
        for bnd in self.the_parsed_bindings:
            if bnd.isPatternMatchBinding():
                return True
        else:
            return False

    def externalizePatternBinding(self):
        bnd_spec = []
        ndxs_to_pop = []

        for ndx, bnd in enumerate(self.the_parsed_bindings):
            if bnd.isPatternMatchBinding():
                patterntype = bnd.binding_site_specification.getHalfBindingSpecification().getBindingToken()

                ndxs_to_pop.append(ndx)
                bnd_spec.append( (self.the_parsed_bindings[ndx].getName(), patterntype) )

        ndxs_to_pop.sort()
        ndxs_to_pop.reverse()

        for ndx in ndxs_to_pop:
            self.the_parsed_bindings.pop(ndx)
        
        return bnd_spec

    def representsBoundMolecule(self):
        if self.isSmallMol():
            return self.has_small_mol_binding

        elif self.isModMol():
            for binding in self.the_parsed_bindings:
                if binding.hasBindingSiteSpecification() and binding.getBindingSiteSpecification().hasHalfBindingSpecification():
                    return True
            else:
                return False
        else:
            raise Exception()

    def getSmallMolBindingToken(self):
        if not self.isSmallMol():
            raise Exception()

        if not self.representsBoundMolecule():
            raise Exception()
        
        return self.small_mol_binding.getBindingToken()

    def getModificationSiteList(self):
        if self.isSmallMol():
            raise SmallMolSemanticException(self.mol_name, "Modification sites requested for small mol")

        return [ parsed_modification.getName() for parsed_modification in self.the_parsed_modification_sites]

    def getModificationSites(self):
        if self.isSmallMol():
            raise SmallMolSemanticException( self.mol_name, "Modification sites requested for small mol '%s'" % self.getName())
        
        return self.the_parsed_modification_sites

        
    def getBindingSiteWithName(self, bindingName ):
        if self.isSmallMol():
            raise SmallMolSemanticException(self.mol_name, "Binding sites requested for small mol")

        for bindingSite in self.the_parsed_bindings:
            if bindingSite.getName() == bindingName:
                return bindingSite
        else:
            raise ParsedModMolSemanticException("Binding site with name '%s' not found" % bindingName )

        return

    def getModificationSiteWithName(self, modificationSite ):
        if self.isSmallMol():
            raise SmallMolSemanticException(self.mol_name, "Binding sites requested for small mol")

        for modSite in self.the_parsed_modification_sites:
            if modSite.getName() == modificationSite:
                return modSite
        else:
            raise ParsedModMolException("Modification site not found")

        return 

    def parse(self):

        # Token here means a single mol definition alpha, or alpha(first,second), or whatever.
        tokenToParse = self.getOriginalLine()
        
        if not self.MolTokenPassesSanityChecks( tokenToParse ):
            raise Exception()

        # The only time a small mol will have a ( in it is if it is "mol_name(!"
        if "(" not in tokenToParse:
            self.__parseSmallMol()
        elif "(!" in tokenToParse:
            self.__parseSmallMol()
        elif "()" in tokenToParse:
            self.__parseSmallMol()
        else:
            self.__parseModMol()

    def __parseSmallMol(self):
        self.is_small_mol = True
        smallMolTokenToParse = self.getOriginalLine()

        if "(" in smallMolTokenToParse:
            self.mol_name = smallMolTokenToParse.split("(")[0]

            if "()" not in smallMolTokenToParse:
                self.has_small_mol_binding = True
                self.small_mol_binding = ParsedHalfBindingSpecification( smallMolTokenToParse.split("(")[1][:-1] )
        else:
            self.mol_name = smallMolTokenToParse

    def __parseModMol(self):
        # The Token is a Mod Mol, parse it

        self.is_small_mol =False
        
        modMolTokenToParse = self.getOriginalLine()

#        assert( modMolTokenToParse.count("(") == 1 and modMolTokenToParse.count(")") == 1)
#             print "Error in string %s: modMolTokenToParse.count(\"(\") == 1 != modMolTokenToParse.count(\")\") == 1" % self.getOriginalLine()
#             raise Exception()

        toks = modMolTokenToParse.split("(")
        self.mol_name = toks[0]
        
        structTokensStr = toks[1][:-1] # The last charector is the last ")" at the end of the mol.
        
        structTmpTokens = structTokensStr.split(",")
        structTmpTokens = [x + "," for x in structTmpTokens]
        structTmpTokens = util.getBalancedArray( structTmpTokens )
        structTmpTokens = [ x[:-1] for x in structTmpTokens ]
        
        for struct_token in structTmpTokens:
            if struct_token.startswith("*"):
                self.the_parsed_modification_sites.append( ParsedModificationSite(struct_token) )
            else:
                self.the_parsed_bindings.append( ParsedBindingSite( struct_token ))
        


        
        
        
