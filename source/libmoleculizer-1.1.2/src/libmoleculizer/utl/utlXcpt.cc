// This file contains the implementation mkMsgs for a variety of Larry written
// exceptions:

// badNNIntArgXcpt
// badPosIntArgXcpt

#include "utlXcpt.hh"

namespace utl
{
    std::string
    badNNIntArgXcpt::
    mkMsg( const std::string& rTheBadArgument )
    {
        std::ostringstream msgStream;
        msgStream << "Expected non-negative integer for command-line argument; "
                  << "got `"
                  << rTheBadArgument
                  << "'.";
        return msgStream.str();
    }
    
    
    std::string
    badPosIntArgXcpt::
    mkMsg( const std::string& rTheBadArgument )
    {
        std::ostringstream msgStream;
        msgStream << "Expected positive integer for command-line argument; got `"
                  << rTheBadArgument
                  << "'.";
        return msgStream.str();
    }
    
    
    
    std::string
    insuffArgsXcpt::
    mkCountsMsg( int actualArgCount,
                 int minimumArgCount )
    {
        std::ostringstream msgStream;
        msgStream << "Expected "
                  << minimumArgCount
                  << " command line arguments; got "
                  << actualArgCount
                  << ".";
        return msgStream.str();
    }
    
    std::string
    insuffArgsXcpt::
    mkGeneralMsg( void )
    {
        return std::string( "Insufficient command-line arguments or "
                            "missing command-line argument." );
    }
    
    insuffArgsXcpt::
    insuffArgsXcpt( const std::string& rMsg ) :
        utl::xcpt( rMsg )
    {}
    
    insuffArgsXcpt
    insuffArgsXcpt::
    general( void )
    {
        return insuffArgsXcpt( mkGeneralMsg() );
    }
    
    insuffArgsXcpt
    insuffArgsXcpt::
    counts( int actualArgCount,
            int minimumArgCount )
    {
        return insuffArgsXcpt( mkCountsMsg( actualArgCount,
                                            minimumArgCount ) );
    }
    
    std::string
    badNNDoubleArgXcpt::
    mkMsg( const std::string& rTheBadArg )
    {
        std::ostringstream msgStream;
        msgStream << "Expected non-negative double for command-line argument; "
                  << "got `"
                  << rTheBadArg
                  << "'.";
        return msgStream.str();
    }
    
    std::string
    unkArgXcpt::
    mkMsg( const std::string& rTheUnrecognizedArg )
    {
        std::ostringstream msgStream;
        msgStream << "Unknown command-line argument `"
                  << rTheUnrecognizedArg
                  << "'.";
        return msgStream.str();
    }
    
}
