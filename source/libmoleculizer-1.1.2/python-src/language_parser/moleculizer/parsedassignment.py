from parsedmzrtoken import ParsedMzrToken

class ParsedAssignment( ParsedMzrToken ):
    HAVE_EE = False

    def getExpressionEvaluator(cls):
        return cls.HAVE_EE

    getExpressionEvaluator = classmethod( getExpressionEvaluator )

    def setExpressionEvaluator( cls, expression_evaluator):
        cls.HAVE_EE = expression_evaluator

    setExpressionEvaluator = classmethod( setExpressionEvaluator )

    
    def getValue( cls, expression ):
        if (HAVE_EE):
            value = self.evaluateExpression( expression )
            return value
        else:
            MissingExpressionEvaluatorException()

    getValue = classmethod( getValue )


    def AssignmentPassesSanityCheck( token):
        return "=" in token and not "->" in token and not "<-" in token

    AssignmentPassesSanityCheck = staticmethod( AssignmentPassesSanityCheck )

    def __init__(self, assignmentLine):
        ParsedMzrToken.__init__(self, assignmentLine)
        
        self.lhs = 0


        self.has_value = False
        self.has_expression = False

        self.rhs_value = 0
        self.rhs_expression = 0

        self.parse()

    def parse(self):
        token = self.getOriginalLine()
        
        if not self.AssignmentPassesSanityCheck(token):
            raise Exception()

        self.lhs = token.split("=")[0]

        rhsToken = token.split("=")[1]

        try:
            number = float( rhsToken )
            self.has_value = True
            self.rhs_value = number
        except:
            self.has_expression = True
            self.rhs_expression = rhsToken


    def getName(self):
        return self.lhs

    def getValue(self):
        if self.has_value:
            return self.rhs_value
        
        # I need to tie the parameter evaluator into this thing real quick.
        elif self.has_expression:
            return self.rhs_expression
            
        
        return 

    def isAssignment(self):
        return True
