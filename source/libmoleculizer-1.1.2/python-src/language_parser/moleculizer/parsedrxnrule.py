from parsedcomplex import *

# This is something that looks like "complex+complex->complex+complex"
class ParsedRxnRule( ParsedMzrToken ):

    def RxnRuleTokenPassesSanityCheck(line):
        return "->" in line and not "=" in line

    RxnRuleTokenPassesSanityCheck = staticmethod( RxnRuleTokenPassesSanityCheck )

    def __init__(self, rxnRuleLine ):
        ParsedMzrToken.__init__(self, rxnRuleLine)

        self.parsed_reactants = []
        self.parsed_products = []
        
        self.parse()
        
    def parse(self):
        rxnRuleLine = self.getOriginalLine()

        if not self.RxnRuleTokenPassesSanityCheck( rxnRuleLine ):
            raise Exception()

        reactantsLine = rxnRuleLine.split("->")[0]
        productsLine = rxnRuleLine.split("->")[1]

        reactantTokens = reactantsLine.split("+")
        productTokens = productsLine.split("+")

        self.parsed_reactants = [ParsedComplex(token) for token in reactantTokens]
        self.parsed_products = [ParsedComplex(token) for token in productTokens]

    def getReactants(self):
        return self.parsed_reactants

    def getProducts(self):
        return self.parsed_products

    def isDecomposition(self):
        return len(self.parsed_reactants) == 1 and len(self.parsed_products) == 2 

    def isDimerization(self):
        return len(self.parsed_reactants) == 2 and len(self.parsed_products) == 1

    def isReaction(self):
        return True

