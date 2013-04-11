/* Generated */

#include <Python.h>
#include <libxml/xmlversion.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#include "libxml_wrap.h"
#include "libxml2-py.h"

PyObject *
libxml_xmlErrorGetLine(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlErrorPtr Error;
    PyObject *pyobj_Error;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlErrorGetLine", &pyobj_Error))
        return(NULL);
    Error = (xmlErrorPtr) PyError_Get(pyobj_Error);

    c_retval = Error->line;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURISetOpaque(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * opaque;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetOpaque", &pyobj_URI, &opaque))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->opaque != NULL) xmlFree(URI->opaque);
    URI->opaque = (char *)xmlStrdup((const xmlChar *)opaque);
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlErrorGetFile(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlErrorPtr Error;
    PyObject *pyobj_Error;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlErrorGetFile", &pyobj_Error))
        return(NULL);
    Error = (xmlErrorPtr) PyError_Get(pyobj_Error);

    c_retval = Error->file;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURIGetScheme(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetScheme", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->scheme;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathSetContextDoc(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;
    xmlDocPtr doc;
    PyObject *pyobj_doc;

    if (!PyArg_ParseTuple(args, (char *)"OO:xmlXPathSetContextDoc", &pyobj_ctxt, &pyobj_doc))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);
    doc = (xmlDocPtr) PyxmlNode_Get(pyobj_doc);

    ctxt->doc = doc;
    Py_INCREF(Py_None);
    return(Py_None);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlURISetPort(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    int port;

    if (!PyArg_ParseTuple(args, (char *)"Oi:xmlURISetPort", &pyobj_URI, &port))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    URI->port = port;
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlURIGetFragment(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetFragment", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->fragment;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURISetScheme(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * scheme;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetScheme", &pyobj_URI, &scheme))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->scheme != NULL) xmlFree(URI->scheme);
    URI->scheme = (char *)xmlStrdup((const xmlChar *)scheme);
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlURIGetOpaque(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetOpaque", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->opaque;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlErrorGetLevel(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlErrorPtr Error;
    PyObject *pyobj_Error;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlErrorGetLevel", &pyobj_Error))
        return(NULL);
    Error = (xmlErrorPtr) PyError_Get(pyobj_Error);

    c_retval = Error->level;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathGetContextNode(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    xmlNodePtr c_retval;
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlXPathGetContextNode", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);

    c_retval = ctxt->node;
    py_retval = libxml_xmlNodePtrWrap((xmlNodePtr) c_retval);
    return(py_retval);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlParserSetPedantic(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;
    int pedantic;

    if (!PyArg_ParseTuple(args, (char *)"Oi:xmlParserSetPedantic", &pyobj_ctxt, &pedantic))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    ctxt->pedantic = pedantic;
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlParserSetLoadSubset(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;
    int loadsubset;

    if (!PyArg_ParseTuple(args, (char *)"Oi:xmlParserSetLoadSubset", &pyobj_ctxt, &loadsubset))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    ctxt->loadsubset = loadsubset;
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlURISetFragment(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * fragment;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetFragment", &pyobj_URI, &fragment))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->fragment != NULL) xmlFree(URI->fragment);
    URI->fragment = (char *)xmlStrdup((const xmlChar *)fragment);
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlURISetUser(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * user;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetUser", &pyobj_URI, &user))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->user != NULL) xmlFree(URI->user);
    URI->user = (char *)xmlStrdup((const xmlChar *)user);
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlParserGetWellFormed(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlParserGetWellFormed", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    c_retval = ctxt->wellFormed;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURIGetPort(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetPort", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->port;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlParserGetIsValid(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlParserGetIsValid", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    c_retval = ctxt->valid;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURISetAuthority(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * authority;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetAuthority", &pyobj_URI, &authority))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->authority != NULL) xmlFree(URI->authority);
    URI->authority = (char *)xmlStrdup((const xmlChar *)authority);
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlURIGetQuery(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetQuery", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->query;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathGetContextSize(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlXPathGetContextSize", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);

    c_retval = ctxt->contextSize;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlErrorGetDomain(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlErrorPtr Error;
    PyObject *pyobj_Error;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlErrorGetDomain", &pyobj_Error))
        return(NULL);
    Error = (xmlErrorPtr) PyError_Get(pyobj_Error);

    c_retval = Error->domain;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURISetQueryRaw(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * query_raw;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetQueryRaw", &pyobj_URI, &query_raw))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->query_raw != NULL) xmlFree(URI->query_raw);
    URI->query_raw = (char *)xmlStrdup((const xmlChar *)query_raw);
    Py_INCREF(Py_None);
    return(Py_None);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathParserGetContext(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    xmlXPathContextPtr c_retval;
    xmlXPathParserContextPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlXPathParserGetContext", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlXPathParserContextPtr) PyxmlXPathParserContext_Get(pyobj_ctxt);

    c_retval = ctxt->context;
    py_retval = libxml_xmlXPathContextPtrWrap((xmlXPathContextPtr) c_retval);
    return(py_retval);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlURIGetPath(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetPath", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->path;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlParserSetReplaceEntities(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;
    int replaceEntities;

    if (!PyArg_ParseTuple(args, (char *)"Oi:xmlParserSetReplaceEntities", &pyobj_ctxt, &replaceEntities))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    ctxt->replaceEntities = replaceEntities;
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlURISetServer(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * server;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetServer", &pyobj_URI, &server))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->server != NULL) xmlFree(URI->server);
    URI->server = (char *)xmlStrdup((const xmlChar *)server);
    Py_INCREF(Py_None);
    return(Py_None);
}

#if defined(LIBXML_HTML_ENABLED)
#endif
PyObject *
libxml_xmlURISetQuery(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * query;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetQuery", &pyobj_URI, &query))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->query != NULL) xmlFree(URI->query);
    URI->query = (char *)xmlStrdup((const xmlChar *)query);
    Py_INCREF(Py_None);
    return(Py_None);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathSetContextNode(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;
    xmlNodePtr node;
    PyObject *pyobj_node;

    if (!PyArg_ParseTuple(args, (char *)"OO:xmlXPathSetContextNode", &pyobj_ctxt, &pyobj_node))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);
    node = (xmlNodePtr) PyxmlNode_Get(pyobj_node);

    ctxt->node = node;
    Py_INCREF(Py_None);
    return(Py_None);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlErrorGetMessage(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlErrorPtr Error;
    PyObject *pyobj_Error;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlErrorGetMessage", &pyobj_Error))
        return(NULL);
    Error = (xmlErrorPtr) PyError_Get(pyobj_Error);

    c_retval = Error->message;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

#if defined(LIBXML_XPATH_ENABLED)
#endif
#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathGetContextPosition(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlXPathGetContextPosition", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);

    c_retval = ctxt->proximityPosition;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlParserSetValidate(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;
    int validate;

    if (!PyArg_ParseTuple(args, (char *)"Oi:xmlParserSetValidate", &pyobj_ctxt, &validate))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    ctxt->validate = validate;
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlErrorGetCode(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    int c_retval;
    xmlErrorPtr Error;
    PyObject *pyobj_Error;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlErrorGetCode", &pyobj_Error))
        return(NULL);
    Error = (xmlErrorPtr) PyError_Get(pyobj_Error);

    c_retval = Error->code;
    py_retval = libxml_intWrap((int) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURISetPath(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlURIPtr URI;
    PyObject *pyobj_URI;
    char * path;

    if (!PyArg_ParseTuple(args, (char *)"Oz:xmlURISetPath", &pyobj_URI, &path))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    if (URI->path != NULL) xmlFree(URI->path);
    URI->path = (char *)xmlStrdup((const xmlChar *)path);
    Py_INCREF(Py_None);
    return(Py_None);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathGetFunction(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const xmlChar * c_retval;
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlXPathGetFunction", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);

    c_retval = ctxt->function;
    py_retval = libxml_xmlCharPtrConstWrap((const xmlChar *) c_retval);
    return(py_retval);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlURIGetUser(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetUser", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->user;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathGetFunctionURI(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const xmlChar * c_retval;
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlXPathGetFunctionURI", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);

    c_retval = ctxt->functionURI;
    py_retval = libxml_xmlCharPtrConstWrap((const xmlChar *) c_retval);
    return(py_retval);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
PyObject *
libxml_xmlParserSetLineNumbers(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;
    int linenumbers;

    if (!PyArg_ParseTuple(args, (char *)"Oi:xmlParserSetLineNumbers", &pyobj_ctxt, &linenumbers))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    ctxt->linenumbers = linenumbers;
    Py_INCREF(Py_None);
    return(Py_None);
}

PyObject *
libxml_xmlURIGetServer(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetServer", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->server;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

#if defined(LIBXML_XPATH_ENABLED)
PyObject *
libxml_xmlXPathGetContextDoc(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    xmlDocPtr c_retval;
    xmlXPathContextPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlXPathGetContextDoc", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlXPathContextPtr) PyxmlXPathContext_Get(pyobj_ctxt);

    c_retval = ctxt->doc;
    py_retval = libxml_xmlDocPtrWrap((xmlDocPtr) c_retval);
    return(py_retval);
}

#endif /* defined(LIBXML_XPATH_ENABLED) */
#if defined(LIBXML_HTML_ENABLED)
#endif
PyObject *
libxml_xmlParserGetDoc(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    xmlDocPtr c_retval;
    xmlParserCtxtPtr ctxt;
    PyObject *pyobj_ctxt;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlParserGetDoc", &pyobj_ctxt))
        return(NULL);
    ctxt = (xmlParserCtxtPtr) PyparserCtxt_Get(pyobj_ctxt);

    c_retval = ctxt->myDoc;
    py_retval = libxml_xmlDocPtrWrap((xmlDocPtr) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURIGetAuthority(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetAuthority", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->authority;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

PyObject *
libxml_xmlURIGetQueryRaw(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    const char * c_retval;
    xmlURIPtr URI;
    PyObject *pyobj_URI;

    if (!PyArg_ParseTuple(args, (char *)"O:xmlURIGetQueryRaw", &pyobj_URI))
        return(NULL);
    URI = (xmlURIPtr) PyURI_Get(pyobj_URI);

    c_retval = URI->query_raw;
    py_retval = libxml_charPtrConstWrap((const char *) c_retval);
    return(py_retval);
}

