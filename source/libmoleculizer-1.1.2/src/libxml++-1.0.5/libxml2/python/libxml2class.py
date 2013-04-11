#
# Functions from module python
#

def SAXParseFile(SAX, URI, recover):
    """Interface to parse an XML file or resource pointed by an
       URI to build an event flow to the SAX object """
    libxml2mod.xmlSAXParseFile(SAX, URI, recover)

def createInputBuffer(file, encoding):
    """Create a libxml2 input buffer from a Python file """
    ret = libxml2mod.xmlCreateInputBuffer(file, encoding)
    if ret is None:raise treeError('xmlCreateInputBuffer() failed')
    return inputBuffer(_obj=ret)

def createOutputBuffer(file, encoding):
    """Create a libxml2 output buffer from a Python file """
    ret = libxml2mod.xmlCreateOutputBuffer(file, encoding)
    if ret is None:raise treeError('xmlCreateOutputBuffer() failed')
    return outputBuffer(_obj=ret)

def createPushParser(SAX, chunk, size, URI):
    """Create a progressive XML parser context to build either an
       event flow if the SAX object is not None, or a DOM tree
       otherwise. """
    ret = libxml2mod.xmlCreatePushParser(SAX, chunk, size, URI)
    if ret is None:raise parserError('xmlCreatePushParser() failed')
    return parserCtxt(_obj=ret)

def debugMemory(activate):
    """Switch on the generation of line number for elements nodes.
       Also returns the number of bytes allocated and not freed
       by libxml2 since memory debugging was switched on. """
    ret = libxml2mod.xmlDebugMemory(activate)
    return ret

def dumpMemory():
    """dump the memory allocated in the file .memdump """
    libxml2mod.xmlDumpMemory()

def htmlCreatePushParser(SAX, chunk, size, URI):
    """Create a progressive HTML parser context to build either an
       event flow if the SAX object is not None, or a DOM tree
       otherwise. """
    ret = libxml2mod.htmlCreatePushParser(SAX, chunk, size, URI)
    if ret is None:raise parserError('htmlCreatePushParser() failed')
    return parserCtxt(_obj=ret)

def htmlSAXParseFile(SAX, URI, encoding):
    """Interface to parse an HTML file or resource pointed by an
       URI to build an event flow to the SAX object """
    libxml2mod.htmlSAXParseFile(SAX, URI, encoding)

def memoryUsed():
    """Returns the total amount of memory allocated by libxml2 """
    ret = libxml2mod.xmlMemoryUsed()
    return ret

def newNode(name):
    """Create a new Node """
    ret = libxml2mod.xmlNewNode(name)
    if ret is None:raise treeError('xmlNewNode() failed')
    return xmlNode(_obj=ret)

def pythonCleanupParser():
    """Cleanup function for the XML library. It tries to reclaim
       all parsing related global memory allocated for the
       library processing. It doesn't deallocate any document
       related memory. Calling this function should not prevent
       reusing the library but one should call xmlCleanupParser()
       only when the process has finished using the library or
       XML document built with it. """
    libxml2mod.xmlPythonCleanupParser()

def setEntityLoader(resolver):
    """Set the entity resolver as a python function """
    ret = libxml2mod.xmlSetEntityLoader(resolver)
    return ret

class xmlNode(xmlCore):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlNode needs a PyCObject argument'
        self._o = _obj
        xmlCore.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlNode (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

    # accessors for xmlNode
    def ns(self):
        """Get the namespace of a node """
        ret = libxml2mod.xmlNodeGetNs(self._o)
        if ret is None:return None
        __tmp = xmlNs(_obj=ret)
        return __tmp

    def nsDefs(self):
        """Get the namespace of a node """
        ret = libxml2mod.xmlNodeGetNsDefs(self._o)
        if ret is None:return None
        __tmp = xmlNs(_obj=ret)
        return __tmp

class xmlDoc(xmlNode):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlDoc needs a PyCObject argument'
        self._o = _obj
        xmlNode.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlDoc (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

class xpathContext:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    # accessors for xpathContext
    def contextDoc(self):
        """Get the doc from an xpathContext """
        ret = libxml2mod.xmlXPathGetContextDoc(self._o)
        if ret is None:raise xpathError('xmlXPathGetContextDoc() failed')
        __tmp = xmlDoc(_obj=ret)
        return __tmp

    def contextNode(self):
        """Get the current node from an xpathContext """
        ret = libxml2mod.xmlXPathGetContextNode(self._o)
        if ret is None:raise xpathError('xmlXPathGetContextNode() failed')
        __tmp = xmlNode(_obj=ret)
        return __tmp

    def contextPosition(self):
        """Get the current node from an xpathContext """
        ret = libxml2mod.xmlXPathGetContextPosition(self._o)
        return ret

    def contextSize(self):
        """Get the current node from an xpathContext """
        ret = libxml2mod.xmlXPathGetContextSize(self._o)
        return ret

    def function(self):
        """Get the current function name xpathContext """
        ret = libxml2mod.xmlXPathGetFunction(self._o)
        return ret

    def functionURI(self):
        """Get the current function name URI xpathContext """
        ret = libxml2mod.xmlXPathGetFunctionURI(self._o)
        return ret

    def setContextDoc(self, doc):
        """Set the doc of an xpathContext """
        if doc is None: doc__o = None
        else: doc__o = doc._o
        libxml2mod.xmlXPathSetContextDoc(self._o, doc__o)

    def setContextNode(self, node):
        """Set the current node of an xpathContext """
        if node is None: node__o = None
        else: node__o = node._o
        libxml2mod.xmlXPathSetContextNode(self._o, node__o)

    #
    # xpathContext functions from module python
    #

    def registerXPathFunction(self, name, ns_uri, f):
        """Register a Python written function to the XPath interpreter """
        ret = libxml2mod.xmlRegisterXPathFunction(self._o, name, ns_uri, f)
        return ret

class xmlAttribute(xmlNode):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlAttribute needs a PyCObject argument'
        self._o = _obj
        xmlNode.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlAttribute (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

class ValidCtxt(ValidCtxtCore):
    def __init__(self, _obj=None):
        self._o = _obj
        ValidCtxtCore.__init__(self, _obj=_obj)

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlFreeValidCtxt(self._o)
        self._o = None

class xmlElement(xmlNode):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlElement needs a PyCObject argument'
        self._o = _obj
        xmlNode.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlElement (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

class xmlAttr(xmlNode):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlAttr needs a PyCObject argument'
        self._o = _obj
        xmlNode.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlAttr (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

class xmlTextReader(xmlTextReaderCore):
    def __init__(self, _obj=None):
        self.input = None
        self._o = _obj
        xmlTextReaderCore.__init__(self, _obj=_obj)

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlFreeTextReader(self._o)
        self._o = None

class xmlReg:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlRegFreeRegexp(self._o)
        self._o = None

class catalog:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlFreeCatalog(self._o)
        self._o = None

class xmlEntity(xmlNode):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlEntity needs a PyCObject argument'
        self._o = _obj
        xmlNode.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlEntity (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

class relaxNgSchema:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlRelaxNGFree(self._o)
        self._o = None

class Schema:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlSchemaFree(self._o)
        self._o = None

class Error:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    # accessors for Error
    def code(self):
        """The error code, e.g. an xmlParserError """
        ret = libxml2mod.xmlErrorGetCode(self._o)
        return ret

    def domain(self):
        """What part of the library raised this error """
        ret = libxml2mod.xmlErrorGetDomain(self._o)
        return ret

    def file(self):
        """the filename """
        ret = libxml2mod.xmlErrorGetFile(self._o)
        return ret

    def level(self):
        """how consequent is the error """
        ret = libxml2mod.xmlErrorGetLevel(self._o)
        return ret

    def line(self):
        """the line number if available """
        ret = libxml2mod.xmlErrorGetLine(self._o)
        return ret

    def message(self):
        """human-readable informative error message """
        ret = libxml2mod.xmlErrorGetMessage(self._o)
        return ret

class relaxNgValidCtxt(relaxNgValidCtxtCore):
    def __init__(self, _obj=None):
        self.schema = None
        self._o = _obj
        relaxNgValidCtxtCore.__init__(self, _obj=_obj)

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlRelaxNGFreeValidCtxt(self._o)
        self._o = None

class xpathParserContext:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    # accessors for xpathParserContext
    def context(self):
        """Get the xpathContext from an xpathParserContext """
        ret = libxml2mod.xmlXPathParserGetContext(self._o)
        if ret is None:raise xpathError('xmlXPathParserGetContext() failed')
        __tmp = xpathContext(_obj=ret)
        return __tmp

class parserCtxt(parserCtxtCore):
    def __init__(self, _obj=None):
        self._o = _obj
        parserCtxtCore.__init__(self, _obj=_obj)

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlFreeParserCtxt(self._o)
        self._o = None

    # accessors for parserCtxt
    def doc(self):
        """Get the document tree from a parser context. """
        ret = libxml2mod.xmlParserGetDoc(self._o)
        if ret is None:raise parserError('xmlParserGetDoc() failed')
        __tmp = xmlDoc(_obj=ret)
        return __tmp

    def isValid(self):
        """Get the validity information from a parser context. """
        ret = libxml2mod.xmlParserGetIsValid(self._o)
        return ret

    def lineNumbers(self, linenumbers):
        """Switch on the generation of line number for elements nodes. """
        libxml2mod.xmlParserSetLineNumbers(self._o, linenumbers)

    def loadSubset(self, loadsubset):
        """Switch the parser to load the DTD without validating. """
        libxml2mod.xmlParserSetLoadSubset(self._o, loadsubset)

    def pedantic(self, pedantic):
        """Switch the parser to be pedantic. """
        libxml2mod.xmlParserSetPedantic(self._o, pedantic)

    def replaceEntities(self, replaceEntities):
        """Switch the parser to replace entities. """
        libxml2mod.xmlParserSetReplaceEntities(self._o, replaceEntities)

    def validate(self, validate):
        """Switch the parser to validation mode. """
        libxml2mod.xmlParserSetValidate(self._o, validate)

    def wellFormed(self):
        """Get the well formed information from a parser context. """
        ret = libxml2mod.xmlParserGetWellFormed(self._o)
        return ret

class xmlDtd(xmlNode):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlDtd needs a PyCObject argument'
        self._o = _obj
        xmlNode.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlDtd (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

class xmlNs(xmlNode):
    def __init__(self, _obj=None):
        if type(_obj).__name__ != 'PyCObject':
            raise TypeError, 'xmlNs needs a PyCObject argument'
        self._o = _obj
        xmlNode.__init__(self, _obj=_obj)

    def __repr__(self):
        return "<xmlNs (%s) object at 0x%x>" % (self.name, long(pos_id (self)))

class inputBuffer(ioReadWrapper):
    def __init__(self, _obj=None):
        self._o = _obj
        ioReadWrapper.__init__(self, _obj=_obj)

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlFreeParserInputBuffer(self._o)
        self._o = None

class relaxNgParserCtxt:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlRelaxNGFreeParserCtxt(self._o)
        self._o = None

class outputBuffer(ioWriteWrapper):
    def __init__(self, _obj=None):
        self._o = _obj
        ioWriteWrapper.__init__(self, _obj=_obj)

class SchemaParserCtxt:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlSchemaFreeParserCtxt(self._o)
        self._o = None

class SchemaValidCtxt(SchemaValidCtxtCore):
    def __init__(self, _obj=None):
        self.schema = None
        self._o = _obj
        SchemaValidCtxtCore.__init__(self, _obj=_obj)

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlSchemaFreeValidCtxt(self._o)
        self._o = None

class xmlTextReaderLocator:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

class URI:
    def __init__(self, _obj=None):
        if _obj != None:self._o = _obj;return
        self._o = None

    def __del__(self):
        if self._o != None:
            libxml2mod.xmlFreeURI(self._o)
        self._o = None

    # accessors for URI
    def authority(self):
        """Get the authority part from an URI """
        ret = libxml2mod.xmlURIGetAuthority(self._o)
        return ret

    def fragment(self):
        """Get the fragment part from an URI """
        ret = libxml2mod.xmlURIGetFragment(self._o)
        return ret

    def opaque(self):
        """Get the opaque part from an URI """
        ret = libxml2mod.xmlURIGetOpaque(self._o)
        return ret

    def path(self):
        """Get the path part from an URI """
        ret = libxml2mod.xmlURIGetPath(self._o)
        return ret

    def port(self):
        """Get the port part from an URI """
        ret = libxml2mod.xmlURIGetPort(self._o)
        return ret

    def query(self):
        """Get the query part from an URI """
        ret = libxml2mod.xmlURIGetQuery(self._o)
        return ret

    def queryRaw(self):
        """Get the raw query part from an URI (i.e. the unescaped
           form). """
        ret = libxml2mod.xmlURIGetQueryRaw(self._o)
        return ret

    def scheme(self):
        """Get the scheme part from an URI """
        ret = libxml2mod.xmlURIGetScheme(self._o)
        return ret

    def server(self):
        """Get the server part from an URI """
        ret = libxml2mod.xmlURIGetServer(self._o)
        return ret

    def setAuthority(self, authority):
        """Set the authority part of an URI. """
        libxml2mod.xmlURISetAuthority(self._o, authority)

    def setFragment(self, fragment):
        """Set the fragment part of an URI. """
        libxml2mod.xmlURISetFragment(self._o, fragment)

    def setOpaque(self, opaque):
        """Set the opaque part of an URI. """
        libxml2mod.xmlURISetOpaque(self._o, opaque)

    def setPath(self, path):
        """Set the path part of an URI. """
        libxml2mod.xmlURISetPath(self._o, path)

    def setPort(self, port):
        """Set the port part of an URI. """
        libxml2mod.xmlURISetPort(self._o, port)

    def setQuery(self, query):
        """Set the query part of an URI. """
        libxml2mod.xmlURISetQuery(self._o, query)

    def setQueryRaw(self, query_raw):
        """Set the raw query part of an URI (i.e. the unescaped form). """
        libxml2mod.xmlURISetQueryRaw(self._o, query_raw)

    def setScheme(self, scheme):
        """Set the scheme part of an URI. """
        libxml2mod.xmlURISetScheme(self._o, scheme)

    def setServer(self, server):
        """Set the server part of an URI. """
        libxml2mod.xmlURISetServer(self._o, server)

    def setUser(self, user):
        """Set the user part of an URI. """
        libxml2mod.xmlURISetUser(self._o, user)

    def user(self):
        """Get the user part from an URI """
        ret = libxml2mod.xmlURIGetUser(self._o)
        return ret

