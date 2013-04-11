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

class XmlObject:
    def __init__(self, elementName, parent = 0):
        self.elementName = elementName
        self.attributeList = []
        self.subelementList = []

        if parent:
            self.attachToParent( parent )

    def attachToParent(self, parentElement):
        parentElement.addSubElement(self)
        return parentElement

    def addAttribute(self, attributeName, value):
        self.attributeList.append( (str(attributeName), str(value)) )

    def addSubFile(self, fileName):
        f = open(fileName).read()
        self.addSubElement(f)

    def addSubElement(self, xmlElement):
        assert(isinstance(xmlElement, XmlObject) or isinstance(xmlElement, str))
        self.subelementList.append( xmlElement)

    def getNumberOfChildren(self):
        return len(self.subelementList)

    def writeall(self, outputstream):
        outputstream.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        self.write(outputstream)

    def write(self, outputstream):
        outputstream.write("<")
        outputstream.write(self.elementName)
        for attribute, value in self.attributeList:
            outputstream.write(" ")
            outputstream.write(attribute + "=")
            outputstream.write('"')
            outputstream.write(value)
            outputstream.write('"')

        if len(self.subelementList) == 0:
            outputstream.write(' />')
        else:
            outputstream.write('>')

            for subElement in self.subelementList:
                if isinstance(subElement, XmlObject):
                    subElement.write(outputstream)
                elif isinstance(subElement, str):
                    outputstream.write(subElement)
                else:
                    raise ValueError
            outputstream.write("</" + self.elementName + ">")
