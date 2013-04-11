###############################################################################
# BNGMZRConverter - A utility program for converting bngl input files to mzr
#		    input files.
# Copyright (C) 2007, 2008, 2009 The Molecular Sciences Institute
#
# Moleculizer is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# Moleculizer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Moleculizer; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Original Author:
#   Nathan Addy, Scientific Programmer	Email: addy@molsci.org
#   The Molecular Sciences Institute    Email: addy@molsci.org  
#                     
#   
###############################################################################

import pdb
import copy
import sys

class SymbolicExpressionEvaluator:

    class BadExpressionException(Exception): pass
    class MissingVariableException(Exception): pass

    class SEED:
        # Symbolic Expression Evaluator Descriptor

        CHARACTER_CONSTANTS   = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_"
        NUMBER_CONSTANTS      = "0123456789"
        PUNCTUATION_CONSTANTS = "()+-=*/^"

        def __init__(self, line, numiter = 10):
            if not iter:
                raise ValueError, "Too much iterating"
            
            self.originalLine = line
            self.curr_token_value = ""
            self.curr_token = ""
            self.curr_token_index = -1
            self.tokens = self.__parse_into_tokens(line)
            self.iter = numiter
        
        def is_alpha(self, line):
            return len(line) > 0 and line[0] in self.CHARACTER_CONSTANTS

        def is_number(self, line):
            return len(line) > 0 and line[0] in self.NUMBER_CONSTANTS

        def is_punctuation(self, line):
            return len(line) > 0 and line[0] in self.PUNCTUATION_CONSTANTS

        def is_variable(self, line):
            return self.is_alpha(line) or self.is_number(line)

        def __parse_into_tokens(self, line):
            # The tokens are ( NUMBER ) / + - * ^ VARIABLE =
            lineArray = list(line)
            tokens = []
            number = False
            character = False
            punctuation = False

            while len(lineArray) > 0:
                if self.is_alpha(lineArray[0]):
                    token, lineArray = self.__get_variable(lineArray)
                elif self.is_number(lineArray[0]):
                    token, lineArray = self.__get_number(lineArray)
                elif self.is_punctuation(lineArray):
                    token = lineArray.pop(0)
                else:
                    raise "Error: character %s not good\non line - %s" % (lineArray[0], line)
                tokens.append(token)

            return tokens

        def __get_number(self, lineArray):
            theNumber = ""
            numberOfDecimals = 0
            numberOfEs = 0
            numberOfMinInExponent = 0

            while len(lineArray):
                if self.is_number(lineArray[0]):
                    theNumber += lineArray.pop(0)

                elif lineArray[0] == "." and numberOfDecimals == 0:
                    numberOfDecimals = 1                
                    theNumber+= lineArray.pop(0)

                elif (lineArray[0] == "e" or lineArray[0] == "E") and numberOfEs == 0:
                    theNumber += lineArray.pop(0)
                    numberOfDecimals == 1
                    numberOfEs = 1

                elif (lineArray[0] == "-" and (theNumber[-1] == "E" or theNumber[-1] == "e") and numberOfMinInExponent == 0):
                    theNumber += lineArray.pop(0)
                    numberOfMinInExponent = 1

                else:
                    # I would love to remove this break, but I don't quite see how...
                    # The problem is that a number ends when you hit your first punctuation
                    # charecter.  The problem is when parsing a number like 40e-6.  It is
                    # difficult to say the "-" is an exponential and not punctuation.  It
                    # likeley would require some (relatively simple) refactoring of this class
                    # and how numbers are defined (ie changing something like "1e10" from NUM
                    # to NUM EXP NUM.
                    break

            return (theNumber, lineArray)

        def __get_variable(self, lineArray):
            variable = ""
            while self.is_variable(lineArray):
                variable += lineArray.pop(0)
            return (variable, lineArray)    
    
    def __init__(self, parameterBlock):

        # parameterBlock must be an array of lines, where each corresponds
        # to a single "KSS = 10.3" or "KSS = DIG * STE" or whatever.

	self.variableToLineDict = {}
        self.cachedVariableValues = {}

        self.installLineArray( parameterBlock )

        return



    def getVariableValue(self, variable):
        if variable in self.cachedVariableValues.keys():
            return self.cachedVariableValues[variable]
        else:
            variableValue = self.evaluateExpression(self.variableToLineDict[variable])
            self.cachedVariableValues[variable] = variableValue
            return variableValue        

    def evaluateExpression(self, expression):
        if isinstance(expression, float):
            return expression

        try:
            ans = self.processLine(expression)
        except SymbolicExpressionEvaluator.MissingVariableException, e:
            sys.stderr.write(e.message + '\n')
            raise SymbolicExpressionEvaluator.BadExpressionException("Error, expression '%s' cannot be evaluated" % expression)
        except:
            raise SymbolicExpressionEvaluator.BadExpressionException("Error, expression '%s' cannot be evaluated" % expression)
        
        return self.processLine(expression)

    def __getVariableArray(self):
        return self.variableToLineDict.keys()

    def __constructSaturationExpression(self, x, y):
        saturationExpression = "%s*%s*%s/(%s+%s)" % (x, "1", "1", y, "1")
        return saturationExpression

    def processLine(self, line):
        aSEED = self.SEED(str(line))

	self.__get_token(aSEED)
        try:
            return self.__expression(aSEED, False)
        except:
            print aSEED.originalLine
            sys.exit(1)

    def installLineArray(self, blockOfLines):
    	for line in blockOfLines:
	    self.installLine(line)
        return

    def installLine(self, line):
	if not "=" in line:
            raise ValueError, ("Error: All parameter lines must be in form 'variable_name = expression'")

        variableName, variableValue = line.split("=")
        variableName, variableValue = variableName.strip(), variableValue.strip()

        # Check for some common errors.
        if variableName in self.variableToLineDict.keys():
            raise ValueError, ("Error: variable '%s' is already defined" % variableName)
        if variableValue == "":
            raise ValueError, ("Error: variable '%s' has not been given a value")

        # Install the variableName, variableExpression pair
        self.variableToLineDict[variableName] = variableValue

    def __expression(self, aSEED, get):

	left = self.__term(aSEED, get)

        while True:
            if aSEED.curr_token == "PLUS":
                left += self.__term(aSEED, True)
            elif aSEED.curr_token == "MINUS":
                left -= self.__term(aSEED, True)
            else:
                return left

    def __term(self, aSEED, get):
	left = self.__prim(aSEED, get)
        while True:
            if aSEED.curr_token == "MUL":
                left *= self.__prim(aSEED, True)
            elif aSEED.curr_token == "DIV":
                d = self.__prim(aSEED, True)
                assert(d != 0 and "Divide by 0 error") 
                left /= d
            else:
                return left

    def __prim(self, aSEED, get):
	if get:
	    self.__get_token(aSEED)

	if aSEED.curr_token == "NUMBER":
	    v = float(aSEED.curr_token_value)
	    self.__get_token(aSEED)
	    if aSEED.curr_token == "EXP":
		v = v ** self.__prim(aSEED, True)
	    return v

	elif aSEED.curr_token == "NAME":
	    variableName = aSEED.curr_token_value
	    self.__get_token(aSEED)

            # There should be NO assignment
	    assert(aSEED.curr_token != "ASSIGN")

            if variableName in self.cachedVariableValues.keys():
                return self.cachedVariableValues[variableName]
            else:
                newSEED = self.SEED(self.variableToLineDict[variableName], aSEED.iter - 1)
                self.__get_token(newSEED)
                return self.__expression(newSEED, False)

	elif aSEED.curr_token == "MINUS":
	    return -self.__prim(aSEED, True)

	elif aSEED.curr_token == "LP":
	    val = self.__expression(aSEED, True)
	    if aSEED.curr_token != "RP":
		raise ValueError, "Right parens expected"
	    self.__get_token(aSEED)
	    return val
	else:
	    raise ValueError, "Primary Expected"

    def __get_token(self, aSEED):
	aSEED.curr_token_index += 1
	try:
	    aSEED.curr_token_value = aSEED.tokens[aSEED.curr_token_index]
	except:
	    aSEED.curr_token_value = ""
	    aSEED.curr_token = "END"
	    return

	if aSEED.is_alpha(aSEED.curr_token_value):
	    aSEED.curr_token = "NAME"
	elif aSEED.is_number(aSEED.curr_token_value):
	    aSEED.curr_token = "NUMBER"
	elif aSEED.curr_token_value == "*":
	    aSEED.curr_token = "MUL"
	elif aSEED.curr_token_value == "/":
	    aSEED.curr_token = "DIV"
	elif aSEED.curr_token_value == "+":
	    aSEED.curr_token = "PLUS"
	elif aSEED.curr_token_value == "-":
	    aSEED.curr_token = "MINUS"
	elif aSEED.curr_token_value == "(":
	    aSEED.curr_token = "LP"
	elif aSEED.curr_token_value == ")":
	    aSEED.curr_token = "RP"
	elif aSEED.curr_token_value == "=":
	    aSEED.curr_token = "ASSIGN"
	elif aSEED.curr_token_value == "^":
	    aSEED.curr_token = "EXP"
	else:
	    raise ValueError, "%s not defined" % aSEED.curr_token
	return

    RegisteredVariables = property(__getVariableArray)


class AlphaWikiSymbolicExpressionEvaluator(SymbolicExpressionEvaluator):
    def getVolume(self):
        assert("Cell_volume" in self.RegisteredVariables and "Error, volume element 'Cell_volume' has not been registered with the Symbolic Expression Evaluator.")
        return self.getVariableValue("Cell_volume")
        
