# These are things that shoud be defined in Main.

class StdoutCatcher:
    def __init__(self):
        self.data = ''

    def write(self, stuff):
        self.data = self.data + stuff

    def get(self):
        return self.data
    
    def getLastLine(self):
        lines = self.data.split("\n")
        lines = [l.strip() for l in lines if l.strip() != ""]
        if len(lines) == 0:
            return ""
        else:
            return lines[-1]


def And( x, y):
    return x and y

def DataUnifier( anArray):
    # This function takes an array that is a bunch of comma ended and semicolon ended 
    # lines, and combines lines so that there is one statement per line.

    # ['GDP,', 'mass = 100.0;', 'GTP,', 'mass = 110.0;', 'Ste2(to-Ste4,  to-alpha),', 'mass = 100.0,', 'preferred_state = top, front;', 'Ste4(to-Ste2, to-Gpa1, to-Ste5 {default, obstructed}, to-Fus3, *ModSite {None, Phosphorylated, DoublyPhosphorylated} ),', 'mass = 100.0;', 'Ste5( to-Ste4, to-Ste11, to-Ste7, to-Fus3),', 'mass = 100.0;', 'Gpa1( to-Ste4 { default, GTP-bound-shape}, to-GXP { default, bound-shape} ),', 'mass = 100.0;', 'alpha,', 'mass = 100.0;']
    if (len(anArray) <= 1): return anArray

    newArray = []
    current_string = ""

    for ndx in range(len(anArray)):
        current_string += anArray[ndx].strip()
        current_string = current_string.strip()
        if current_string[-1] == ";":
            newArray.append(current_string)
            current_string = ""

    return newArray

def getBalancedArray( arrayToBalance ):
    if not EachEntryIsParenBalanced( arrayToBalance ):
    # Combine the ..., ndx_i, ndx_(i+1) where ndx_i is the smallest i not balanced
        return getBalancedArray( GetIncrementallyBetterArray( arrayToBalance ) )
    else:
        return arrayToBalance

def GetIncrementallyBetterArray( anArray ):

    values = [ StringIsParenBalenced(x) for x in anArray]

    # This is correct: this function should only be used if the array does not pass
    # EachEntryIsParenBalanced.
    assert( False in values)

    badNdx = values.index( False )
    
    combinedTokens = anArray[badNdx] + anArray[badNdx + 1]

    returnArray = anArray[ : badNdx]
    returnArray.append( combinedTokens )
    returnArray.extend( anArray[badNdx + 2 : ] )
    
    return returnArray
    
def EachEntryIsParenBalanced( array ):
    entries = [ StringIsParenBalenced(x) for x in array ]
    
    returnVal = True
    for val in entries:
        returnVal = returnVal and val

    return returnVal

def StringIsParenBalenced(line):
    return ( line.count("(") == line.count(")") and 
             line.count("[") == line.count("]") and 
             line.count("{") == line.count("}") )

        


