import pdb
from parsedmzrtoken import ParsedMzrToken

class ParsedBindingShapeSpecification( ParsedMzrToken ):
    def BindingShapeTokenPassesSanityCheck( bindingShapeToken ):
        if bindingShapeToken[0] == "{" and bindingShapeToken[-1] == "}" and bindingShapeToken.count("{")==1 and bindingShapeToken.count("}")==1:
            return True
        else:
            return False
    BindingShapeTokenPassesSanityCheck = staticmethod( BindingShapeTokenPassesSanityCheck )

    def __init__(self, bindingShapeSpecification):
        ParsedMzrToken.__init__(self, bindingShapeSpecification)
        
        self.the_list = []
        self.the_transformation = 0

        self.is_transformation = False
        self.is_shape_list = False
        
        self.parse()

    def parse( self ):
        if not self.BindingShapeTokenPassesSanityCheck( self.getOriginalLine() ):
            print "BindingShapeToken is insane %s " % self.getOriginalLine()
            raise Exception()


        tokenToParse = self.getOriginalLine()[1:-1]

        if "<-" in tokenToParse:
            self.is_transformation = True
            self.the_transformation = tokenToParse.split("<-")[0]
        else:
            self.is_shape_list = True
            self.the_list.extend( tokenToParse.split(",") )

        # Check for the degenerate case
        if len(self.the_list) == 1 and self.the_list[0] == "":
            self.the_list = []
        

    def isTransformation(self):
        return self.is_transformation

    def getTransformation(self):
        if not self.isTransformation():
            raise Exception()
        
        return self.the_transformation

    def isShapeList(self):
        return self.is_shape_list

    def getShapeList(self):
        if not self.isShapeList():
            raise Exception()

        return self.the_list[:]




class ParsedHalfBindingSpecification( ParsedMzrToken ):
    def HalfBindingTokenPassesSanityCheck( line):
        return line[0] == "!" and line.count("!") == 1

    HalfBindingTokenPassesSanityCheck = staticmethod( HalfBindingTokenPassesSanityCheck )

    def __init__(self, halfBinding):
        ParsedMzrToken.__init__(self, halfBinding)
        
        self.is_bound = False
        self.binding_token = 0

        self.parse()

    def parse(self):
        if not self.HalfBindingTokenPassesSanityCheck( self.getOriginalLine() ):
            print "Half Binding Token Insane: %s" % self.getOriginalLine()
            raise Exception()

        self.is_bound = True
        self.binding_token = self.getOriginalLine()[1:]
        
    def isBound(self):
        return self.is_bound
    
    def getBindingToken(self):
        if not self.isBound():
            raise Exception

        return self.binding_token

class ParsedBindingSiteSpecification( ParsedMzrToken ):
    def __init__(self, bindingSiteSpec):
        ParsedMzrToken.__init__(self, bindingSiteSpec)

        self.has_half_binding = False
        self.half_binding = 0

        self.has_shape_specification = False
        self.shape_specification = 0
        
        self.parse()


    def isPhysicalBinding(self):
        return not self.isPatternMatchBinding()

    def isPatternMatchBinding(self):
        if not self.has_half_binding:
            return False
        bnd_token = self.getHalfBindingSpecification().getBindingToken()
        return bnd_token in ['+', '-']

    def hasHalfBindingSpecification(self):
        return self.has_half_binding

    def getHalfBindingSpecification(self):
        if not self.hasHalfBindingSpecification():
            raise Exception()

        return self.half_binding

    def hasShapeSpecification(self):
        return self.has_shape_specification
    
    def getShapeSpecification(self):
        if not self.hasShapeSpecification():
            raise Exception("Error.  getShapeSpecification was called on parsed line '%s', which has no shape specification" % self.getOriginalLine())
        
        return self.shape_specification

    def parse(self):
        token = self.getOriginalLine()

        if token.startswith("!"):
            if token.count("{") == 0:
                # The whole thing is only a half-binding
                self.has_half_binding = True
                self.has_shape_specification = False
                
                self.half_binding = ParsedHalfBindingSpecification(token)

            else:
                # We have !Token{List}
                self.has_half_binding = True
                self.has_shape_specification = True
                
                halfBindingToken = token.split("{")[0]
                shapeSpecificationToken  = "{" + token.split("{")[1]

                self.half_binding = ParsedHalfBindingSpecification( halfBindingToken )
                self.shape_specification = ParsedBindingShapeSpecification( shapeSpecificationToken )

        elif token.startswith("{"):
            if token.count("!") == 0:
                # The whole thing is a list
                self.has_half_binding = False
                self.has_shape_specification = True

                self.shape_specification = ParsedBindingShapeSpecification(token)
            else:
                # We have {list}!1
                self.has_half_binding = True
                self.has_shape_specification = True

                halfBindingToken = "!" + token.split("!")[1]
                shapeSpecificationToken = token.split("!")[0]

                self.half_binding = ParsedHalfBindingSpecification( halfBindingToken )
                self.shape_specification = ParsedBindingShapeSpecification( shapeSpecificationToken )
        
class ParsedBindingSite( ParsedMzrToken ):
    def __init__(self, bindingDefinition ):
        ParsedMzrToken.__init__(self, bindingDefinition)

        self.binding_site_name = ""
        
        self.has_specification = False
        self.binding_site_specification = 0
        
        self.parse()

    def isPatternMatchBinding(self):
        if not self.has_specification:
            return False

        return self.binding_site_specification.isPatternMatchBinding()
        
        return 
        
    def parse(self):
        token = self.getOriginalLine()

        if "!" in token or "{" in token:
            minimum = min([ token.index(x) for x in ["!", "{" ] if token.count(x) > 0])

            self.binding_site_name = token[:minimum]

            self.has_specification = True
            self.binding_site_specification = ParsedBindingSiteSpecification( token[minimum:] )

        else:
            self.binding_site_name = token

    def getName(self):
        return self.binding_site_name

    def hasBindingSiteSpecification(self):
        return self.has_specification

    def getBindingSiteSpecification(self):
        if not self.hasBindingSiteSpecification():
            raise Exception("Binding site '%s' was requested for a binding site specification, but has none." % self.getOriginalLine())

        return self.binding_site_specification

    def hasBindingToken( self, bindingToken):
        return self.hasBindingSiteSpecification() and self.getBindingSiteSpecification().hasHalfBindingSpecification() and self.getBindingSiteSpecification().getHalfBindingSpecification().getBindingToken() == bindingToken
