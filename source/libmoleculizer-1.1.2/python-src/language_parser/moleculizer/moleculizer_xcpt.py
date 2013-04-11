###############################################################################
# Copyright (C) 2007, 2008, 2009 The Molecular Sciences Institute
# Original Author:
#   Nathan Addy, Scientific Programmer	Email: addy@molsci.org
#   The Molecular Sciences Institute    
#
###############################################################################

def makeCustomException(class_name, xcpt_msg, \
                          base_xcpt_cls = Exception):
    """Returns an instantiation of a factory-created exception class
    with user-specified message.

    Example
    -------
    makeCustomException("MissingParameterXcpt", "You missed a param.")
      returns an object of type MissingParameterXcpt which derives from
      base_xcpt_cls, and has meesage 'You missed a param'
    """

    # a light-weight exception class factory.
    try:
      obj = base_xcpt_cls()
      assert(isinstance(obj, Exception))
    except:
      error_msg = \
          "Base_Xcpt_Cls '%s' should be of type Exception but is not." % \
          base_xcpt_cls
      makeCustomException( "FunctionParamException", error_msg, \
                             base_xcpt_cls = Exception )

    # Define the new exception class, which has name 'class_name', has 
    # parent class base_xcpt_class, and an initialization function which 
    # simply passes its params onto the __init__ method of the base class.
    def new_exception_init(self, xcpt_msg):
      base_xcpt_cls.__init__(self, xcpt_msg )

    new_exception_type = type( class_name, (base_xcpt_cls,), \
                                 {"__init__": new_exception_init})

    exception = new_exception_type( xcpt_msg )

    return exception


def makeCustomExceptionType(class_name, base_xcpt_cls = Exception):
  # This is an extention of the makeCustomException function. 

    """
    Returns an instantiation of a factory-created exception class
    type.

    Example
    -------
    makeCustomException("MissingParameterXcpt")
      returns a class of type MissingParameterXcpt.
    """
    try:
      assert(isinstance(base_xcpt_cls, Exception))
    except:
      error_msg = \
          "Base_Xcpt_Cls '%s' should be of type Exception but is not." % \
          base_xcpt_cls
      makeCustomException( "FunctionParamException", error_msg, \
                             base_xcpt_cls = Exception )

    # Define the new exception class, which has name 'class_name', has 
    # parent class base_xcpt_class, and an initialization function which 
    # simply passes its params onto the __init__ method of the base class.
    def new_exception_init(self, xcpt_msg):
      base_xcpt_cls.__init__(self, xcpt_msg )

    new_exception_type = type( class_name, (base_xcpt_cls,), \
                                 {"__init__": new_exception_init})

    return new_exception_type

class InsaneBlockOnTheLooseException( Exception ):
    def __init__(self, badBlock):
        Exception.__init__(self, "Error, an insane block on the loose! Call animal control, and tell them to bring a shotgun and a net, and have them search the following, offending block!\n Insane Block:\n\t " + str(badBlock))
        self.badBlock = badBlock[:]
        
    
