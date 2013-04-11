#include "mzrException.hh"
#include "moleculizer.hh"

#include <libxml++/libxml++.h>

namespace mzr
{

    std::string
    ModelNotLoadedXcpt::mkMsg(const std::string& functionName)
    {            
	std::ostringstream oss;
	oss << "Function '" 
	    << functionName 
	    << "' could not be completed because a model has not yet been loaded." ;
	return oss.str();
    }


    ModelNotLoadedXcpt::ModelNotLoadedXcpt(const std::string& funcName)
	:
	xcpt( mkMsg(funcName))
    {}


    std::string
    BadRulesDefinitionXcpt::mkMsg()
    {
	std::ostringstream oss;
	oss << "Can't parse a bad ruleset.";
	return oss.str();
    }
        
    BadRulesDefinitionXcpt::BadRulesDefinitionXcpt()
	:
	xcpt( mkMsg() )
    {}

    
    std::string
    mzrPythonXcpt::getPythonMessage() const
    {
        return this->python_error;
    }


    std::string
    mzrPythonXcpt::getLongMessage() const
    {
        std::ostringstream xcptMsg;
        xcptMsg<< this->getMessage() << '\n' \
               << "Error Message from Python:\n\t" << this->getPythonMessage();

        return xcptMsg.str();
    }

    std::string
    IllegalNameXcpt::mkMsg( const std::string& illegalName )
    {
	std::ostringstream oss;
	oss << "Illegal Name: '" << illegalName << "'";
	return oss.str();
    }
        
    IllegalNameXcpt::IllegalNameXcpt( const std::string& illegalName )
	:
	xcpt( mkMsg( illegalName ) )
    {}









    
    std::string
    unknownUserNameXcpt::
    mkMsg(const std::string& unknownUserName)
    {
        std::ostringstream oss;
        oss << utl::xcpt::mkMsg() 
            << "User name '" 
            << unknownUserName
            << "' cannot be found in the database of user names to species.";
        return oss.str();
    }
    
    std::string
    dumpableNotSpeciesStreamXcpt::
    mkMsg( const std::string& rDumpableName )
    {
        std::ostringstream msgStream;
        msgStream << utl::xcpt::mkMsg()
                  << "Dumpable '"
                  << rDumpableName
                  << "' is not a species stream, but "
                  << "was registered as such.";
        return msgStream.str();
    }
    
    std::string
    illegalSpeciesNameXcpt::
    mkMsg( const std::string& rSpeciesName,
           const std::string& rMessage )
    {
        return ( rSpeciesName + " is an illegal name" );
    }

    illegalSpeciesNameXcpt::illegalSpeciesNameXcpt( const std::string& rSpeciesName, const std::string& rMessage ) :
	utl::xcpt( mkMsg( rSpeciesName,
			  rMessage ) )
    {}




    std::string
    unhandledModelContentXcpt::mkMsg( xmlpp::Node* pOffendingModelContentNode )
    {
	std::ostringstream msgStream;
	msgStream << utl::dom::xcpt::mkMsg( pOffendingModelContentNode )
		  << "No unit claims to handle this "
		  << pOffendingModelContentNode->get_name()
		  << " node in the model section.";
	return msgStream.str();
    }


    std::string
    unhandledExplicitSpeciesContentXcpt::mkMsg( xmlpp::Node* pBadExplicitSpeciesContentNode )
    {
	std::ostringstream msgStream;
	msgStream << utl::dom::xcpt::mkMsg( pBadExplicitSpeciesContentNode )
		  << "No unit claims to handle this "
		  << pBadExplicitSpeciesContentNode->get_name()
		  << " node in the explicit species section.";
	return msgStream.str();
    }




    std::string
    unhandledSpeciesStreamsContentXcpt::mkMsg( xmlpp::Node* pBadSpeciesStreamsContentNode )
    {
	std::ostringstream msgStream;
	msgStream << utl::dom::xcpt::mkMsg( pBadSpeciesStreamsContentNode )
		  << "No unit claims to handle this "
		  << pBadSpeciesStreamsContentNode->get_name()
		  << " node in the species streams section.";
	return msgStream.str();
    }


    std::string
    unhandledEventsContentXcpt::mkMsg( xmlpp::Node* pOffendingEventsContentNode )
    {
	std::ostringstream msgStream;
	msgStream << utl::dom::xcpt::mkMsg( pOffendingEventsContentNode )
		  << "No unit claims to handle this "
		  << pOffendingEventsContentNode->get_name()
		  << " node in the events section.";
	return msgStream.str();
    }


    std::string
    unhandledReactionGenXcpt::mkMsg( xmlpp::Node* pBadReactionGenNode )
    {
	std::ostringstream msgStream;
	msgStream << utl::dom::xcpt::mkMsg( pBadReactionGenNode )
		  << "No unit claims to handle this "
		  << pBadReactionGenNode->get_name()
		  << " node in the reaction generators section.";
	return msgStream.str();
    }


    
    std::string
    unkDumpableXcpt::
    mkMsg( const std::string& rDumpableName,
           xmlpp::Node* pOffendingNode )
    {
        std::ostringstream msgStream;
        msgStream << utl::dom::xcpt::mkMsg( pOffendingNode )
                  << "Unknown dumpable `"
                  << rDumpableName
                  << "'.";
        return msgStream.str();
    }
    
    std::string
    dupDumpableNameXcpt::
    mkMsg( const std::string& rDumpableName,
           xmlpp::Node* pOffendingNode )
    {
        std::ostringstream msgStream;
        msgStream << utl::dom::xcpt::mkMsg( pOffendingNode )
                  << "There is already a dumpable named "
                  << rDumpableName
                  << ".";
        return msgStream.str();
    }
    
    std::string
    unkSpeciesXcpt::
    mkMsg( const std::string& rSpeciesName,
           xmlpp::Node* pOffendingNode )
    {
        std::ostringstream msgStream;
        msgStream << utl::dom::xcpt::mkMsg( pOffendingNode )
                  << "Unknown species `"
                  << rSpeciesName
                  << "'.";
        return msgStream.str();
    }
    
    std::string
    dupSpeciesNameXcpt::
    mkMsg( const std::string& rSpeciesName,
           xmlpp::Node* pRequestingNode )
    {
        std::ostringstream msgStream;
        msgStream << utl::dom::xcpt::mkMsg( pRequestingNode )
                  << "There is already a species named "
                  << rSpeciesName
                  << ".";
        return msgStream.str();
    }
    
    std::string
    stopEventInPastXcpt::
    mkMsg( double now,
           double badEventTime )
    {
        std::ostringstream msgStream;
        msgStream << "Stop scheduled at time "
                  << badEventTime
                  << ", which is in the past at simulation time "
                  << now
                  << ".";
        return msgStream.str();
    }
    
    
    std::string
    unkStatStreamXcpt::
    mkMsg( const std::string& rBadStreamName,
           xmlpp::Node* pOffendingNode )
    {
        std::ostringstream msgStream;
        msgStream << utl::dom::xcpt::mkMsg( pOffendingNode )
                  << "Unknown stat-stream `"
                  << rBadStreamName
                  << "'.";
        return msgStream.str();
    }
    
    std::string
    missingExtrapolationParameter::
    mkMsg( const std::string& rSpeciesName,
           const std::string& missingParamName )
    {
        std::ostringstream msgStream;
        msgStream << "Error: Species '"
                  << rSpeciesName
                  << "' expected having parameter "
                  << missingParamName
                  << ", which was not found.";
        return msgStream.str();
    }
		
		std::string
		SpeciesIsNotPlexType::mkMsg(const std::string& specName)
		{
				std::ostringstream msgStream;
				msgStream << "Error: Species '"
														<< specName
														<< "' could not be cast to a plex-type.";
				return msgStream.str();
		}

    
}

