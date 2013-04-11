from parsedmzrtoken import ParsedMzrToken

class ParsedModificationSiteSpecification( ParsedMzrToken ):
    def ModSiteSpecificationPassesSanityCheck(ParsedMzrToken):
        if ParsedMzrToken[0] == "{" and ParsedMzrToken[-1] == "}" and ParsedMzrToken.count("{") == 1 and ParsedMzrToken.count("}"):
            return True
        else:
            return False

    ModSiteSpecificationPassesSanityCheck = staticmethod( ModSiteSpecificationPassesSanityCheck )

    
    def __init__(self, modSiteSpecToken):
        ParsedMzrToken.__init__(self, modSiteSpecToken)

        self.is_transformation = False
        self.transformation_entry = ""

        self.is_list = False
        self.mod_value_list = []

        self.parse()

    def parse(self):
        if not self.ModSiteSpecificationPassesSanityCheck( self.getOriginalLine() ):
            raise Exception()

        parsedListToken = self.getOriginalLine()[1:-1] # Strip the {}
        
        if "<-*" in parsedListToken:
            self.__parseTransformation( parsedListToken )
        else:
            self.__parseList( parsedListToken )
    def isList(self):
        return self.is_list

    def isTransformation(self):
        return self.is_transformation

    def getList(self):
        if not self.isList():
            raise Exception()

        else:
            return self.mod_value_list[:]

    def getTransformation(self):
        if not self.isTransformation():
            raise Exception()

        else:
            return self.transformation_entry



    def __parseTransformation(self, token):
        assert( "<-*" in token )
        self.is_transformation = True
        self.transformation_entry = token.split("<-")[0]

    def __parseList(self, token):
        self.is_list = True

        theTokens = token.split(",")
        self.mod_value_list.extend( theTokens )

class ParsedModificationSite( ParsedMzrToken ):
    def ModificationTokenPassesSanityCheck( token ):
        # Starts with a *, there are as many rbraces as lbraces, and no more than 1 of either.
        return token.startswith("*") and token.count("{") <= 1 and token.count("}") <= 1 and (token.count("{") - token.count("}") == 0)

    ModificationTokenPassesSanityCheck = staticmethod(ModificationTokenPassesSanityCheck)

    def __init__(self, modificationSiteToken):
        ParsedMzrToken.__init__(self, modificationSiteToken)

        self.modification_site_name = ""

        self.has_modification_site_spec = False
        self.parsed_mod_site_specification = 0

        self.parse()

        return

    def parse(self):
        if not self.ModificationTokenPassesSanityCheck( self.getOriginalLine() ):
            raise Exception()

        modificationSiteToken = self.getOriginalLine()
        modificationSiteToken = modificationSiteToken[1:] # Kill the initial *

        if modificationSiteToken.count("{") > 0:
            lBraceIndex = modificationSiteToken.index("{")

            self.modification_site_name = modificationSiteToken[ :lBraceIndex]

            self.has_modification_site_spec = True
            self.parsed_mod_site_specification = ParsedModificationSiteSpecification( modificationSiteToken[lBraceIndex:] )

        else:
            self.modification_site_name = modificationSiteToken

        return

    def getName(self):
        return self.modification_site_name
    
    def hasModificationSiteSpecification(self):
        return self.has_modification_site_spec

    def getModificationSiteSpecification(self):
        if not self.hasModificationSiteSpecification():
            raise Exception()

        return self.parsed_mod_site_specification
        
                                             

    

