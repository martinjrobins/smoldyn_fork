###############################################################################
# Copyright (C) 2007, 2008, 2009 The Molecular Sciences Institute
# Original Author:
#   Nathan Addy, Scientific Programmer	Email: addy@molsci.org
#   The Molecular Sciences Institute    
#
###############################################################################
# This line reads and parses a binding array 

import pdb
from util import *
from parsedmzrtoken import ParsedMzrToken

from parsedrxnrule import ParsedRxnRule
from parsedcomplex import ParsedComplex
from parsedassignment import ParsedAssignment

class ParsedLine( ParsedMzrToken):
    def ReactionLinePassesSanityCheck(tok):
        return tok[-1] == ";" and tok.count(";") == 1

    ReactionLinePassesSanityCheck = staticmethod( ReactionLinePassesSanityCheck )

    def isReactionToken(tok):
        return "->" in tok

    isReactionToken = staticmethod( isReactionToken )


    def isComplexToken(tok):
        return not ParsedLine.isAssignment(tok) and not ParsedLine.isReactionToken(tok)

    isComplexToken = staticmethod( isComplexToken)

    
    def isAssignment(tok):
        return "=" in tok

    isAssignment = staticmethod( isAssignment )

    def __init__(self, lineTxt):
        ParsedMzrToken.__init__(self, lineTxt)

        # A MzrRuleFileLine is a list of tokens (each of which can be a molDefinition,
        # 
        self.tokens = []
        
        # These are either ParsedReactions, ParsedComplexes, or (name = assignment) keys
        self.parsed_content = []

        self.__parse()

    def __parse(self):
        # We can assume that self.line text is a single comment, with a semi-colon ending it
        lineToken = self.getOriginalLine()

        if not self.ReactionLinePassesSanityCheck( lineToken ):
            raise Exception()
        
        tmpLine = lineToken[ :-1]

        tmp_unprocessed_tokens = tmpLine.split(",")
        tmp_unprocessed_tokens = [x.strip() + "," for x in tmp_unprocessed_tokens if x.strip() != ""]
         
        tmp_array = getBalancedArray( tmp_unprocessed_tokens )
        self.tokens = [x[:-1] for x in tmp_array]
        

        self.parsed_content = [self.__getCorrectParsedToken(token) for token in self.tokens]
        

    def __getCorrectParsedToken(self, token):
        
        if self.isReactionToken(token):
            return ParsedRxnRule( token )

        elif self.isAssignment(token):
            return ParsedAssignment( token )

        elif self.isComplexToken(token):
            return ParsedComplex(token)
        
        else:
            raise Exception()

    def getParsedComponents(self):
        return self.parsed_content


    def hasAssignment(self, variableName):
        return self.getAssignmentNdx( variableName, -1) >= 0

    def getAssignmentNdx(self, variableName, idx = -1):
        if idx < 0: idx = 0
        for x in range( idx, len(self.getParsedComponents())):
            if self.getParsedComponents()[x].isAssignment() and self.getParsedComponents()[x].getName() == variableName:
                return x
        else:
            return -1

    def getAssignment(self, variableName, idx = -1):
        num = -1
        num = self.getAssignmentNdx( variableName, idx)

        if num < 0:
            raise Exception( "Error, assignment '%s' cannot be found in line '%s'" %(variableName, self.getOriginalLine()))

        assignmentToken = self.getParsedComponents()[num]

        return assignmentToken.getValue()





