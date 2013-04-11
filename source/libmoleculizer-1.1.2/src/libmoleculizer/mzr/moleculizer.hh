//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//        This file is part of Libmoleculizer
//
//        Copyright (C) 2001-2009 The Molecular Sciences Institute.
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// Moleculizer is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Moleculizer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Moleculizer; if not, write to the Free Software Foundation
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307,  USA
//
// END HEADER
//
// Original Author:
//   Larry Lok, Research Fellow, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//


#ifndef MOLECULIZER_H
#define MOLECULIZER_H

#include "utl/defs.hh"
#include "mzr/mzrException.hh"
#include "mzr/unit.hh"
#include "fnd/reactionNetworkDescription.hh"
#include "mzr/mzrSpecies.hh"
#include "mzr/mzrReaction.hh"
#include "mzr/pythonRulesManager.hh"


namespace mzr
{
		class unitsMgr;
    
		// The main bulk of this class can be found in ReactionNetworkDescription.
		class moleculizer :
				public fnd::ReactionNetworkDescription<mzrSpecies, mzrReaction>
		{
        
		public:
				// Moleculizer only has basic constructors and destructors.  
				moleculizer( void );
				~moleculizer( void );

		public:

				//////////////////////////////////////////////////
				// 
				// Functions for working with Input
				//
				//////////////////////////////////////////////////

				void loadCommonRulesFileName(const std::string& aFileName );
				void loadCommonRulesString(const std::string& commonRulesAsString);
				void loadXmlFileName( const std::string& aFileName );
				void loadXmlString( const std::string& documentAsString );
				void loadGeneratedNetwork( xmlpp::Element* pGeneratedNetworkElmt);
				void loadParsedDocument( xmlpp::Document* pDoc );
				void writeInternalData(const std::string& data );
				bool getModelHasBeenLoaded() const;

				// These functions are used for reading in rules statements, one at a time.
				void addParameterStatement(const std::string& statement );
				void addModificationStatement( std::string& statement);
				void addMolsStatement( std::string& statement);
				void addAllostericPlexStatement( std::string& statement);
				void addAllostericOmniStatement( std::string& statement);
				void addDimerizationGenStatement( std::string& statement);
				void addOmniGenStatement( std::string& statement);
				void addUniMolGenStatement( std::string& statement);
				void addSpeciesStreamStatement( std::string& statement);


				// NEWWORK hoho This is the new function I'm editing in. I am
				// creating a seperate .cc file to compile the code for it.
				unsigned int getSizeOfAutomorphismGroup(const mzrSpecies& mzrSpec) const;

				//////////////////////////////////////////////////
				// 
				// Functions for working with Output
				//
				//////////////////////////////////////////////////

				xmlpp::Document* makeDomOutput( bool verbose ) throw( std::exception );
				xmlpp::Document* makeDomOutput( bool verboseXML, CachePosition networkSizeRange ) 
						throw( std::exception );

				void writeOutputFile( const std::string& fileName, bool verbose = false);
				void writeOutputFile( const std::string& fileName, bool verbose, CachePosition pos);


				//////////////////////////////////////////////////
				// 
				// Functions for Setup
				//
				//////////////////////////////////////////////////

				unsigned int getGenerationDepth( void ) const;
				void setGenerateDepth( unsigned int generateDepth );
        
				void setRateExtrapolation( bool rateExtrapolation );
				bool getRateExtrapolation() const;


				//////////////////////////////////////////////////
				// 
				// Functions for expanding the nework
				//
				//////////////////////////////////////////////////

				void generateCompleteNetwork();
				CachePosition generateCompleteNetwork(long maxNumSpecies, long maxNumRxns = -1);


				//////////////////////////////////////////////////
				// 
				// Functions for working with names.
				//
				//////////////////////////////////////////////////

				void 
				getUserNames(std::vector<std::string>& refVector) const;
        
				void recordUserNameToSpeciesIDPair( const std::string& userName,
																																								const std::string& speciesID );
        
				bool nameIsUserName(const std::string& possibleUserName) const;



				// I don't know if these two functions are good or bad, but they
				// are intended for Smoldyn and others that may have a name,
				// but don't want to keep track of how they got it.
				std::string convertSomeNameToTaggedName(const std::string& name) const;
				const mzrSpecies* getSpeciesWithSomeName(const std::string& name) const;

				mzrSpecies* getSpeciesWithTaggedName( SpeciesTagCref taggedName) throw( fnd::NoSuchSpeciesXcpt );
				const mzrSpecies* getSpeciesWithTaggedName( SpeciesTagCref taggedName) const throw( fnd::NoSuchSpeciesXcpt );

				mzrSpecies* getSpeciesWithUniqueID( SpeciesIDCref uniqueID ) throw( mzr::IllegalNameXcpt );
				const mzrSpecies* getSpeciesWithUniqueID( SpeciesIDCref uniqueID ) const throw( mzr::IllegalNameXcpt );

				std::string convertUserNameToSpeciesID(const std::string& possibleUserName) const 
						throw( utl::xcpt );

				std::string convertUserNameToTaggedName( const std::string& possibleUserName) const
						throw( utl::xcpt);





				//////////////////////////////////////////////////
				// 
				// Functions for working with state
				//
				//////////////////////////////////////////////////

				int getNumberOfDefinedModifications() const;
				int getNumberOfDefinedMols() const;
				int getNumberOfDefinedRules() const;

				int getNumberOfDimerReactionRules() const;
				int getNumberOfOmniGenReactionRules() const;
				int getNumberOfUniMolReactionRules() const;
				int getNumberOfReactionRules() const;

				int getNumberOfPlexFamilies() const;


				//////////////////////////////////////////////////
				// 
				// Functions for working with species streams
				//
				//////////////////////////////////////////////////

				void getSpeciesStreams( std::vector<std::string>& speciesStreamNames) const;
				int getNumberOfSpeciesStreams() const;
				int getNumberOfSpeciesInSpeciesStream(const std::string& streamName) const;
				void getSpeciesInSpeciesStream(const std::string& streamName, std::vector<const mzr::mzrSpecies*>& speciesVector) const;

				bool speciesWithTagIsInSpeciesStream(const std::string speciesTag, const std::string& speciesStream ) const;
				bool speciesWithUniqueIDIsInSpeciesStream(const std::string speciesTag, const std::string& speciesStream ) const;


				//////////////////////////////////////////////////
				// 
				// Overloads of fnd::reactionNetworkDescription
				//
				//////////////////////////////////////////////////

				// These overload the base reaction network description names.
				virtual bool
				recordSpecies( mzrSpecies*);

				virtual bool
				recordSpecies( mzrSpecies*, SpeciesID&);

                                virtual bool
                                recordReaction( mzrReaction*);


				//////////////////////////////////////////////////
				// 
				// Functions for making structural queries about species.
				//
				//////////////////////////////////////////////////
	
				int getMolCountInSpecies(const std::string& molName, const mzr::mzrSpecies* pSpec) const;
				int getMolCountInTaggedSpecies(const std::string& molName, const std::string& taggedName) const;

		protected:
				void setModelHasBeenLoaded( bool value );
        
				void insertGeneratedNetwork( xmlpp::Element* generatedNetworkElt, CachePosition pos, bool verbose );
				void insertGeneratedNetwork( xmlpp::Element* generatedNetworkElement, bool verbose );
        
		public:

				////////////////////////////////////////////////
				// Units loaded by the user, waiting for destruction.
				//
				// This class is the manager for units, and the place that new units
				// can be installed.  It's public because units need to get to
				// each other.
				unitsMgr* pUserUnits;
        


		protected:
				void
				constructorPrelude( void );
        
				void
				verifyInput( const xmlpp::Element* const pRootElt,
																	const xmlpp::Element* const pModelElt,
																	const xmlpp::Element* const pStreamElt ) const
						throw( std::exception );
        
				void
				constructorCore( xmlpp::Element* pRootElt,
																					xmlpp::Element* pModelElt,
																					xmlpp::Element* pStreamElt )
						throw( std::exception );
        
				void
				moleculizerParseDomInput( xmlpp::Element* pRootElt,
																														xmlpp::Element* pModelElt,
																														xmlpp::Element* pStreamElt )
						throw( std::exception );
        
				std::map<std::string, std::string> userNameToSpeciesIDChart;



		private:

        
				// Codes the input capabilities of moleculizer, including its parsing
				// routine.
				inputCapabilities inputCap;
        
				static int DEFAULT_GENERATION_DEPTH;
        
        
				bool modelLoaded;
				bool extrapolationEnabled;

				PythonRulesManager rulesManager;

				// Now we store a copy of the parser, so that people can get a copy of the rules, at any time.
				xmlpp::DomParser* theParser;

		};

		class restoreGeneratedSpecies
		{
      
		public:
				restoreGeneratedSpecies( mzr::moleculizer& refMolzer )
						:
						rMolzer( refMolzer )
				{}
        
				void operator()( const xmlpp::Node* pNode );

		private:
				moleculizer& rMolzer;
        
		};
}

#endif
