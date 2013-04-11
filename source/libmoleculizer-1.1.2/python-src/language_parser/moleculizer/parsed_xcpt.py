class ParsedMolException( Exception ):
    def __init__(self, molName, message):
        theMessage = "ParsedMolException thrown for mol '%s'.\nError Message: %s" % (molName, message)

        Exception.__init__(self, theMessage)

class ParsedMolSemanticException( ParsedMolException ):
    def __init__(self, molName, message):
        ParsedMolException.__init__(self, molName, message)


class SmallMolSemanticException( ParsedMolSemanticException ):
    def __init__(self, molName, message):
        ParsedMolSemanticException.__init__(molName, message)

