/* Steven Andrews, started 10/22/2001.
 This is a library of functions for the Smoldyn program.  See documentation
 called Smoldyn_doc1.pdf and Smoldyn_doc2.pdf.
 Copyright 2003-2011 by Steven Andrews.  This work is distributed under the terms
 of the Gnu General Public License (GPL). */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "RnSort.h"
#include "smoldyn.h"
#include "smoldynfuncs.h"
#include "string2.h"

#include "smoldynconfigure.h"

#define DEBUG 0


/******************************************************************************/
/************************* Smoldyn-Libmoleculizer interface *******************/
/******************************************************************************/

/******************************************************************************/
/****************************** Local declarations ****************************/
/******************************************************************************/

// low level utilities
int mzrSmolName2TagName(mzrssptr mzrss,char *smolname,char *tagname);
int mzrTagName2SmolName(simptr sim,const char *tagname);
int mzrIsTagNameInStream(mzrssptr mzrss,char *tagname,char *stream);
int mzrGetSpeciesSymmetry(mzrssptr mzrss,char *smolname);

// memory management
int mzrallocrules(mzrssptr mzrss,int ruleschars);
int mzrallocstreams(mzrssptr mzrss,int maxstreams);
void mzrfreestreams(char **streamname,double **displaysize,double ***color,double **strmdifc,enum MolecState *strmstate,int maxstreams);
int mzrallocnamehash(mzrssptr mzrss,unsigned int maxnamehash);
void mzrfreenamehash(char **tagname,char **smolname,int maxnamehash);
int mzrallocrxnhash(mzrssptr mzrss,int maxrxnhash);
void mzrfreerxnhash(char **mzrrxn,char **smolrxn,int maxrxnhash);
int mzrallocdefaultstate(mzrssptr mzrss,int maxspecies);
mzrssptr mzrssalloc(void);

// data structure output
void mzroutput(mzrssptr mzrss);

// structure set up
void mzrsetcondition(mzrssptr mzrss,enum StructCond cond,int upgrade);
int mzrAssignDrawingParameters(simptr sim,char *tagname,int ident);
int mzrAssignDiffCoeff(simptr sim,char *tagname,double mass,int ident);
enum MolecState mzrMolState(mzrssptr mzrss,int ident,int molint);
enum MolecState mzrDetermineProductState(mzrssptr mzrss,int ident,enum MolecState ms1,enum MolecState ms2);
int mzrAssignProductDefaultState(mzrssptr mzrss,int *reactants,int nreactants,int *products,int nproducts);
int mzraddtonamehash(mzrssptr mzrss,const char *tagname,const char *smolname);
int mzraddtorxnhash(mzrssptr mzrss,char *mzrrxn,char *smolrxn);
void mzrNextSmolrxnName(mzrssptr mzrss,char *smolrxn);
int mzrMakeNameHash(simptr sim);
int mzrssload(simptr sim,char *erstr);

// core simulation functions
#ifdef LIBMOLECULIZER
	#include <libmoleculizer/mzr/libmzr_c_interface.h>
	int mzrAddSpeciesArrayToSim(simptr sim,species **species_array,int number_species);
	int mzrAddReactionArrayToSim(simptr sim,reaction **reactionlist,int nreactions);
#endif
int mzrExpandNetwork(simptr sim);
int mzrExpandUnexpandedSpecies(simptr sim);
int mzrAddRxn(simptr sim,char *name,int order,int *reactants,int *products,int nprod,double rate);


/*
??
New API additions:

const char* getErrorMessage(moleculizer* handle);
int getErrorState( moleculizer* handle);
void setErrorMessage( moleculizer* handle, const char* error_message);
void clearErrorState( moleculizer* handle);
*/


/******************************************************************************/
/****************************** low level utilities ***************************/
/******************************************************************************/


/* mzrTagName2SmolName. */
int mzrTagName2SmolName(simptr sim,const char *tagname) {
	mzrssptr mzrss;
	int ans,itag;
	char tagnamecopy[STRCHAR];

	strncpy(tagnamecopy,tagname,STRCHAR);
	mzrss=sim->mzrss;
	itag=stringfind(mzrss->tagname,mzrss->nnamehash,tagnamecopy);
	if(itag>=0) {
		ans=stringfind(sim->mols->spname,sim->mols->nspecies,mzrss->smolname[itag]);
		if(ans>=0) return ans;
		else return -2; }
	else {
		strchrreplace(tagnamecopy,'*','$');
		ans=stringfind(sim->mols->spname,sim->mols->nspecies,tagnamecopy);
		if(ans>=0) return ans;
		else return -1; }
	return -1; }


/* mzrSmolName2TagName */
int mzrSmolName2TagName(mzrssptr mzrss,const char *smolname,char *tagname) {
#ifdef LIBMOLECULIZER
	char smolnamecopy[STRCHAR];

	strncpy(smolnamecopy,smolname,STRCHAR);
	strchrreplace(smolnamecopy,'$','*');
	return convertSomeNameToTaggedName(mzrss->mzr,smolnamecopy,tagname,STRCHAR);
#else
	return 4;
#endif
}


/* mzrReadStreamName. */
int mzrReadStreamName(char *str,char *streamname,enum MolecState *msptr) {
	char nm[STRCHAR],*pareno,*parenc;
	int itct,i;
	enum MolecState ms;

	if(!str) return -1;
	itct=sscanf(str,"%s",nm);
	if(itct!=1) return -1;			// cannot read name
	pareno=strchr(nm,'(');

	if(pareno) {
		*pareno='\0';
		pareno++;
		parenc=strrchr(pareno,')');
		if(parenc && *(parenc+1)=='\0') *parenc='\0';
		else return -2;						// improper close parenthesis
		ms=molstring2ms(pareno);
		if(ms==MSnone) return -3; }		// cannot read state
	else ms=MSsoln;

	if(!strcmp(nm,"all")) i=-5;		// all
	if(streamname) strncpy(streamname,nm,STRCHAR);
	if(msptr) *msptr=ms;
	return 0; }


/* mzrGetSpeciesStreams */
int mzrGetSpeciesStreams(mzrssptr mzrss,char ***streamnames,int *numnames) {
#ifdef LIBMOLECULIZER
	int ans;

	ans=0;
	getAllSpeciesStreams(mzrss->mzr,streamnames,numnames);
	if(*streamnames==NULL) {
		ans=1;
		*numnames=0; }
	return ans;
#else
	return 2;
#endif
}


/* mzrFreeSpeciesStreams */
void mzrFreeSpeciesStreams(char **streamnames,int numnames) {
#ifdef LIBMOLECULIZER
	freeCharPtrArray(streamnames,numnames);
	return;
#else
	return;
#endif
}


/* mzrIsTagNameInStream */
int mzrIsTagNameInStream(mzrssptr mzrss,char *tagname,char *stream) {
#ifdef LIBMOLECULIZER
	return checkSpeciesTagIsInSpeciesStream(mzrss->mzr,tagname,stream);
#else
	return -1;
#endif
}


/* mzrIsSmolNameInStream */
int mzrIsSmolNameInStream(mzrssptr mzrss,const char *smolname,char *stream) {
	char tagname[STRCHAR];

	if(mzrSmolName2TagName(mzrss,smolname,tagname)) return -1;
	return mzrIsTagNameInStream(mzrss,tagname,stream); }


/* mzrNumberOfSpecies. */
int mzrNumberOfSpecies(mzrssptr mzrss) {
#ifdef LIBMOLECULIZER
	if(!mzrss || !mzrss->mzr) return 0;
	return getNumberOfSpecies(mzrss->mzr);
#else
	return 0;
#endif
}


/* mzrNumberOfReactions */
int mzrNumberOfReactions(mzrssptr mzrss) {
#ifdef LIBMOLECULIZER
	if(!mzrss || !mzrss->mzr) return 0;
	return getNumberOfReactions(mzrss->mzr); 
#else
	return 0;
#endif
}


/* mzrGetSpeciesSymmetry */
int mzrGetSpeciesSymmetry(mzrssptr mzrss,const char *smolname) {
#ifdef LIBMOLECULIZER
	char tagname[STRCHAR];
	int er,automorph;

	if(mzrSmolName2TagName(mzrss,smolname,tagname)) return -1;
	er=getSpeciesSymmetryDegree(mzrss->mzr,tagname,&automorph);
	if(er) return -1;
	return automorph;
#else
	return -1;
#endif
}


/******************************************************************************/
/****************************** memory management *****************************/
/******************************************************************************/


/* mzrallocrules */
int mzrallocrules(mzrssptr mzrss,int ruleschars) {
	char *newrules;
	int i;

	newrules=NULL;
	if(!mzrss || ruleschars<0) return 2;
	if(ruleschars>0) {
		CHECKMEM(newrules=(char*) calloc(ruleschars,sizeof(char)));
		for(i=0;i<ruleschars;i++) newrules[i]='\0';
		if(mzrss->rules)
			strncpy(newrules,mzrss->rules,ruleschars); }
	free(mzrss->rules);
	mzrss->ruleschars=ruleschars;
	mzrss->rules=newrules;
	return 0;
failure:
	free(newrules);
	simLog(NULL,10,"Unable to allocate memory in mzrallocrules");
	return 1; }


/* mzrallocstreams */
int mzrallocstreams(mzrssptr mzrss,int maxstreams) {
	char **newstreamname;
	double **newdisplaysize,***newcolor,**newstrmdifc;
	enum MolecState *newstrmstate;
	int strm,c;
	enum MolecState ms;

	newstreamname=NULL;
	newdisplaysize=NULL;
	newcolor=NULL;
	newstrmdifc=NULL;
	newstrmstate=NULL;

	if(!mzrss || maxstreams<0) return 2;

	if(maxstreams>0) {
		CHECKMEM(newstreamname=(char**) calloc(maxstreams,sizeof(char*)));			// allocate streamname
		for(strm=0;strm<maxstreams;strm++) newstreamname[strm]=NULL;
		for(strm=0;strm<maxstreams;strm++) {
			CHECKMEM(newstreamname[strm]=EmptyString()); }

		CHECKMEM(newdisplaysize=(double**) calloc(maxstreams,sizeof(double*)));	// allocate displaysize
		for(strm=0;strm<maxstreams;strm++) newdisplaysize[strm]=NULL;
		for(strm=0;strm<maxstreams;strm++) {
			CHECKMEM(newdisplaysize[strm]=(double*) calloc(MSMAX,sizeof(double)));
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) newdisplaysize[strm][ms]=1.0; }

		CHECKMEM(newcolor=(double***) calloc(maxstreams,sizeof(double**)));		// allocate color
		for(strm=0;strm<maxstreams;strm++) newcolor[strm]=NULL;
		for(strm=0;strm<maxstreams;strm++) {
			CHECKMEM(newcolor[strm]=(double**) calloc(MSMAX,sizeof(double*)));
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) newcolor[strm][ms]=NULL;
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) {
				CHECKMEM(newcolor[strm][ms]=(double*) calloc(3,sizeof(double)));
				for(c=0;c<3;c++) newcolor[strm][ms][c]=0; }}

		CHECKMEM(newstrmdifc=(double**) calloc(maxstreams,sizeof(double*)));		// allocate strmdifc
		for(strm=0;strm<maxstreams;strm++) newstrmdifc[strm]=NULL;
		for(strm=0;strm<maxstreams;strm++) {
			CHECKMEM(newstrmdifc[strm]=(double*) calloc(MSMAX,sizeof(double)));
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) newstrmdifc[strm][ms]=-1.0; }
		
		CHECKMEM(newstrmstate=(enum MolecState*) calloc(maxstreams,sizeof(enum MolecState)));	// allocate strmstate
		for(strm=0;strm<maxstreams;strm++) newstrmstate[strm]=MSnone;
		
		for(strm=0;strm<mzrss->nstreams && strm<maxstreams;strm++) {				// copy stuff over
			strcpy(newstreamname[strm],mzrss->streamname[strm]);
			newstrmstate[strm]=mzrss->strmstate[strm];
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) {
				newdisplaysize[strm][ms]=mzrss->displaysize[strm][ms];
				for(c=0;c<3;c++)
					newcolor[strm][ms][c]=mzrss->color[strm][ms][c];
				newstrmdifc[strm][ms]=mzrss->strmdifc[strm][ms]; }}}

	mzrfreestreams(mzrss->streamname,mzrss->displaysize,mzrss->color,mzrss->strmdifc,mzrss->strmstate,mzrss->maxstreams);
	mzrss->maxstreams=maxstreams;
	if(mzrss->nstreams>maxstreams) mzrss->nstreams=maxstreams;
	mzrss->streamname=newstreamname;
	mzrss->displaysize=newdisplaysize;
	mzrss->color=newcolor;
	mzrss->strmdifc=newstrmdifc;
	mzrss->strmstate=newstrmstate;
	
	return 0;

 failure:
	mzrfreestreams(newstreamname,newdisplaysize,newcolor,newstrmdifc,newstrmstate,maxstreams);
	simLog(NULL,10,"Unable to allocate memory in mzrallocstreams");
	return 1; }


/* mzrfreestreams */
void mzrfreestreams(char **streamname,double **displaysize,double ***color,double **strmdifc,enum MolecState *strmstate,int maxstreams) {
	int strm;
	enum MolecState ms;

	if(strmstate)
		free(strmstate);
	
	if(strmdifc) {
		for(strm=0;strm<maxstreams;strm++)
			free(strmdifc[strm]);
		free(strmdifc); }
	
	if(color) {
		for(strm=0;strm<maxstreams;strm++)
			if(color[strm]) {
				for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1))
					free(color[strm][ms]);
				free(color[strm]); }
		free(color); }

	if(displaysize) {
		for(strm=0;strm<maxstreams;strm++)
			free(displaysize[strm]);
		free(displaysize); }

	if(streamname) {
		for(strm=0;strm<maxstreams;strm++)
			free(streamname[strm]);
		free(streamname); }

	return; }


/* mzrallocnamehash. */
int mzrallocnamehash(mzrssptr mzrss,unsigned int maxnamehash) {
	char **newtagname,**newsmolname;
	unsigned int i;
	
	newtagname=NULL;
	newsmolname=NULL;
	
	if(!mzrss || maxnamehash<0) return 2;
	
	if(maxnamehash>0) {
		CHECKMEM(newtagname=(char**) calloc(maxnamehash,sizeof(char*)));
		for(i=0;i<maxnamehash;i++) newtagname[i]=NULL;
		for(i=0;i<maxnamehash;i++) {
			CHECKMEM(newtagname[i]=EmptyString()); }
		
		CHECKMEM(newsmolname=(char**) calloc(maxnamehash,sizeof(char*)));
		for(i=0;i<maxnamehash;i++) newsmolname[i]=NULL;
		for(i=0;i<maxnamehash;i++) {
			CHECKMEM(newsmolname[i]=EmptyString()); }
		
		for(i=0;i<mzrss->nnamehash && i<maxnamehash;i++) {
			strcpy(newtagname[i],mzrss->tagname[i]);
			strcpy(newsmolname[i],mzrss->smolname[i]); }}
	
	mzrfreenamehash(mzrss->tagname,mzrss->smolname,mzrss->maxnamehash);
	mzrss->maxnamehash=maxnamehash;
	if(mzrss->nnamehash>maxnamehash) mzrss->nnamehash=maxnamehash;
	mzrss->tagname=newtagname;
	mzrss->smolname=newsmolname;
	
	return 0;
	
failure:
	mzrfreenamehash(newtagname,newsmolname,maxnamehash);
	simLog(NULL,10,"Unable to allocate memory in mzrallocnamehash");
	return 1; }


/* mzrfreenamehash */
void mzrfreenamehash(char **tagname,char **smolname,int maxnamehash) {
	int i;
	
	if(smolname) {
		for(i=0;i<maxnamehash;i++)
			free(smolname[i]);
		free(smolname); }
	
	if(tagname) {
		for(i=0;i<maxnamehash;i++)
			free(tagname[i]);
		free(tagname); }
	
	return; }


/* mzrallocrxnhash */
int mzrallocrxnhash(mzrssptr mzrss,int maxrxnhash) {
	char **newmzrrxn,**newsmolrxn;
	int i;

	newmzrrxn=NULL;
	newsmolrxn=NULL;

	if(!mzrss || maxrxnhash<0) return 2;

	if(maxrxnhash>0) {
		CHECKMEM(newmzrrxn=(char**) calloc(maxrxnhash,sizeof(char*)));
		for(i=0;i<maxrxnhash;i++) newmzrrxn[i]=NULL;
		for(i=0;i<maxrxnhash;i++) {
			CHECKMEM(newmzrrxn[i]=EmptyString()); }

		CHECKMEM(newsmolrxn=(char**) calloc(maxrxnhash,sizeof(char*)));
		for(i=0;i<maxrxnhash;i++) newsmolrxn[i]=NULL;
		for(i=0;i<maxrxnhash;i++) {
			CHECKMEM(newsmolrxn[i]=EmptyString()); }

		for(i=0;i<mzrss->nrxnhash && i<maxrxnhash;i++) {
			strcpy(newmzrrxn[i],mzrss->mzrrxn[i]);
			strcpy(newsmolrxn[i],mzrss->smolrxn[i]); }}

	mzrfreerxnhash(mzrss->mzrrxn,mzrss->smolrxn,mzrss->maxrxnhash);
	mzrss->maxrxnhash=maxrxnhash;
	if(mzrss->nrxnhash>maxrxnhash) mzrss->nrxnhash=maxrxnhash;
	mzrss->mzrrxn=newmzrrxn;
	mzrss->smolrxn=newsmolrxn;

	return 0;

 failure:
	mzrfreerxnhash(newmzrrxn,newsmolrxn,maxrxnhash);
	simLog(NULL,10,"Unable to allocate memory in mzrallocrxnhash");
	return 1; }


/* mzrfreerxnhash */
void mzrfreerxnhash(char **mzrrxn,char **smolrxn,int maxrxnhash) {
	int i;

	if(smolrxn) {
		for(i=0;i<maxrxnhash;i++)
			free(smolrxn[i]);
		free(smolrxn); }

	if(mzrrxn) {
		for(i=0;i<maxrxnhash;i++)
			free(mzrrxn[i]);
		free(mzrrxn); }

	return; }


/* mzrallocdefaultstate */
int mzrallocdefaultstate(mzrssptr mzrss,int maxspecies) {
	enum MolecState *newdefaultstate;
	int i;

	newdefaultstate=NULL;
	if(!mzrss || maxspecies<0) return 2;
	if(maxspecies>0) {
		CHECKMEM(newdefaultstate=(enum MolecState*) calloc(maxspecies,sizeof(enum MolecState)));
		for(i=0;i<maxspecies;i++) newdefaultstate[i]=MSnone;
		for(i=0;i<mzrss->maxspecies && i<maxspecies;i++)
			newdefaultstate[i]=mzrss->defaultstate[i]; }
	free(mzrss->defaultstate);
	mzrss->maxspecies=maxspecies;
	mzrss->defaultstate=newdefaultstate;
	return 0;
failure:
	free(newdefaultstate);
	simLog(NULL,10,"Unable to allocate memory in mzrallocdefaultstate");
	return 1; }


/* mzrssalloc */
mzrssptr mzrssalloc(void) {
	mzrssptr mzrss;
	enum MolecState ms;
	
	mzrss=(mzrssptr) malloc(sizeof(struct mzrsuperstruct));
	CHECKMEM(mzrss);

	mzrss->condition=SCinit;
	mzrss->sim=NULL;
	mzrss->mzr=NULL;
	mzrss->ruleschars=0;
	mzrss->rules=NULL;
	mzrss->maxstreams=0;
	mzrss->nstreams=0;
	mzrss->streamname=NULL;
	mzrss->displaysize=NULL;
	mzrss->color=NULL;
	mzrss->strmdifc=NULL;
	mzrss->strmstate=NULL;
	mzrss->maxNetworkSpecies=-1;
	mzrss->maxnamehash=0;
	mzrss->nnamehash=0;
	mzrss->tagname=NULL;
	mzrss->smolname=NULL;
	mzrss->maxrxnhash=0;
	mzrss->nrxnhash=0;
	mzrss->mzrrxn=NULL;
	mzrss->smolrxn=NULL;
	mzrss->maxspecies=0;
	mzrss->defaultstate=NULL;
	mzrss->refspecies=0;
	mzrss->refmass=0;
	for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) mzrss->refdifc[ms]=0;
	mzrss->expandall=0;
	return mzrss;
failure:
	simLog(NULL,10,"Unable to allocate memory in mzrssalloc");
	return NULL; }


/* mzrssfree. */
void mzrssfree(mzrssptr mzrss) {
	if(!mzrss) return;
	
#ifdef LIBMOLECULIZER
	if(mzrss->mzr) freeMoleculizerObject(mzrss->mzr);
#endif

	free(mzrss->defaultstate);
	mzrfreerxnhash(mzrss->mzrrxn,mzrss->smolrxn,mzrss->maxrxnhash);
	mzrfreenamehash(mzrss->tagname,mzrss->smolname,mzrss->maxnamehash);
	mzrfreestreams(mzrss->streamname,mzrss->displaysize,mzrss->color,mzrss->strmdifc,mzrss->strmstate,mzrss->maxstreams);
	free(mzrss->rules);
	free(mzrss);
	return; }


/******************************************************************************/
/*************************** data structure output ****************************/
/******************************************************************************/


/* mzrCheckParams. */
int mzrCheckParams(simptr sim,int *warnptr) {
	int warn,error,er,numnames,strm;
	mzrssptr mzrss;
	char **streamnames;

	error=warn=0;
	mzrss=sim->mzrss;

	if(!mzrss) {
		if(warnptr) *warnptr=warn;
		return 0; }

	if(mzrss->condition!=SCok) {warn++;simLog(sim,7,"WARNING: moleculizer not fully set up\n");}
	if(!mzrss->sim) {error++;simLog(sim,10,"BUG: moleculizer sim element undefined\n");}
	if(!mzrss->mzr) {warn++;simLog(sim,7,"WARNING: moleculizer rules not fully loaded\n");}
	if(mzrss->nstreams>mzrss->maxstreams) {error++;simLog(sim,10,"BUG: moleculizer has more streams defined than allocated\n");}
	if(mzrss->maxNetworkSpecies>=0 && !mzrss->expandall) {warn++;simLog(sim,5,"WARNING: moleculizer network expansion is limited to %i species\n",mzrss->maxNetworkSpecies);}
	if(mzrss->maxNetworkSpecies>=0 && mzrss->expandall) {warn++;simLog(sim,5,"WARNING: the rule-based network will expand fully, ignoring the species limitation that was set");}
	if(mzrss->nnamehash>mzrss->maxnamehash) {error++;simLog(sim,10,"BUG: moleculizer has more hash names defined than allocated\n");}
	if(!sim->mols) {error++;simLog(sim,10,"BUG: moleculizer defined but not Smoldyn molecules\n");}
	else {
		if(mzrss->maxspecies>0 && mzrss->maxspecies!=sim->mols->maxspecies) {
			error++;simLog(sim,10,"BUG: moleculizer maxspecies does not match mols version\n");} }

	if(mzrss->mzr) {
		er=mzrGetSpeciesStreams(mzrss,&streamnames,&numnames);
		if(er) {error++;simLog(sim,10,"ERROR: failed to allocate memory while looking up species streams\n");}
		else {
			for(strm=0;strm<mzrss->nstreams;strm++)
				if(stringfind(streamnames,numnames,mzrss->streamname[strm])<0) {
					warn++;simLog(sim,5,"WARNING: stream %s was mentioned, but not declared in rules file\n",mzrss->streamname[strm]);}
			mzrFreeSpeciesStreams(streamnames,numnames); }}
	
	if(warnptr) *warnptr=warn;
	return error; }


/* mzroutput. */
void mzroutput(mzrssptr mzrss) {
#ifdef LIBMOLECULIZER
	int i,er,i2,nspec,nstrm,strm;
	char **streamarray;
	species **speciesarray;

	if(!mzrss || !mzrss->mzr) return;
	simLog(NULL,2,"  Modifications: %i\n",getNumModificationDefs(mzrss->mzr));
	simLog(NULL,2,"  Mols: %i\n",getNumMolDefs(mzrss->mzr));
	simLog(NULL,2,"  Reaction rules: %i\n",getNumReactionRules(mzrss->mzr));
	simLog(NULL,2,"  Association reactions: %i\n",getNumDimerDecompReactionRules(mzrss->mzr));
	simLog(NULL,2,"  Transformation reactions: %i\n",getNumOmniGenReactionRules(mzrss->mzr));
	simLog(NULL,2,"  Uni-mol-gen reactions: %i\n",getNumUniMolGenReactionRules(mzrss->mzr));

	er=getAllSpecies(mzrss->mzr,&speciesarray,&nspec);
	CHECKBUG(!er,"mzroutput: getAllSpecies has unknown error");
	getAllSpeciesStreams(mzrss->mzr,&streamarray,&nstrm);

	simLog(NULL,2,"  Number of species: %i\n",nspec);
	for(i=0;i<nspec;i++) {
		i2=stringfind(mzrss->tagname,mzrss->nnamehash,speciesarray[i]->name);
		CHECKBUG(i2>=0,"mzroutput: tagname not found in namehash");
		simLog(NULL,2,"   %s, mass=%g\n",mzrss->smolname[i2],*speciesarray[i]->mass); }

	simLog(NULL,2,"  Species classes: %i\n",nstrm);
	for(strm=0;strm<nstrm;strm++) {
		simLog(NULL,2,"   %s\n",streamarray[strm]);
		for(i=0;i<nspec;i++) {
			i2=checkSpeciesTagIsInSpeciesStream(mzrss->mzr,speciesarray[i]->name,streamarray[strm]);
			CHECKBUG(i2>=0,"mzroutput: unknown error in checkSpeciesTagInSpeciesStream");
			if(i2) {
				i2=stringfind(mzrss->tagname,mzrss->nnamehash,speciesarray[i]->name);
				CHECKBUG(i2>=0,"mzroutput: tagged name not in namehash");
				simLog(NULL,2,"    %s\n",mzrss->smolname[i2]); }}}

	simLog(NULL,2,"  Number of generated reactions: %i\n",getNumberOfReactions(mzrss->mzr));

	if(streamarray) freeCharPtrArray(streamarray,nstrm);
	if(speciesarray) freeSpeciesArray(speciesarray,nspec);
	return;
failure:
	simLog(NULL,10,"%s",ErrorString);
	return;
#else
	return;
#endif
}


/* mzrssoutput. */
void mzrssoutput(simptr sim) {
	mzrssptr mzrss;
	int strm,same,i,vflag;
	enum MolecState ms,strmstate;
	char string[STRCHAR];
	double *displaysize,**color,*strmdifc;
	unsigned int ui;

	mzrss=sim->mzrss;
	if(!mzrss) return;
	vflag=strchr(sim->flags,'v')?1:0;

	simLog(sim,2,"NETWORK GENERATION PARAMETERS\n");
	if(mzrss->mzr) {
		simLog(sim,2," libmoleculizer parameters:\n");
		mzroutput(sim->mzrss); }
	else simLog(sim,2," moleculizer object has not been created yet\n");
	if(mzrss->rules)
		simLog(sim,2," rules file size: %i characters, %i lines\n",(int)strlen(mzrss->rules),symbolcount(mzrss->rules,'\n'));
	else
		simLog(sim,2," no rules file\n");

	simLog(sim,2," %i classes used, of %i allocated:\n",mzrss->nstreams,mzrss->maxstreams);
	for(strm=0;strm<mzrss->nstreams;strm++) {							// species stream display size, color, difc, state
		strmstate=mzrss->strmstate[strm];
		displaysize=mzrss->displaysize[strm];
		color=mzrss->color[strm];
		if(strmstate!=MSnone)
			simLog(sim,2,"  %s: default state %s\n",mzrss->streamname[strm],molms2string(strmstate,string));
		same=1;
		for(ms=MolecState(0);ms<MSMAX && same==1;ms=MolecState(ms + 1)) {
			if(displaysize[ms]!=displaysize[MSsoln]) same=0;
			if(color[ms][0]!=color[MSsoln][0]) same=0;
			if(color[ms][1]!=color[MSsoln][1]) same=0;
			if(color[ms][2]!=color[MSsoln][2]) same=0; }
		if(same)
			simLog(sim,2,"  %s(all): size %g, color (%g,%g,%g)\n",mzrss->streamname[strm],displaysize[MSsoln],color[MSsoln][0],color[MSsoln][1],color[MSsoln][2]);
		else {
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1))
				simLog(sim,2,"  %s(%s): size %g, color (%g,%g,%g)\n",mzrss->streamname[strm],molms2string(ms,string),displaysize[ms],color[MSsoln][0],color[MSsoln][1],color[MSsoln][2]); }
		strmdifc=mzrss->strmdifc[strm];
		same=1;
		for(ms=MolecState(0);ms<MSMAX && same==1;ms=MolecState(ms + 1))
			if(strmdifc[ms]!=strmdifc[MSsoln]) same=0;
		if(same)
			simLog(sim,2,"  %s(all): difc %g\n",mzrss->streamname[strm],strmdifc[MSsoln]);
		else {
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1))
				simLog(sim,2,"  %s(%s): difc %g\n",mzrss->streamname[strm],molms2string(ms,string),strmdifc[ms]); }}

	simLog(sim,2," %i species in hash of %i allocated\n",mzrss->nnamehash,mzrss->maxnamehash);
	if(vflag) {
		for(ui=0;ui<mzrss->nnamehash;ui++) {
			if(ui==10 && mzrss->nnamehash>11) {
				simLog(sim,2,"  ...\n");
				ui=mzrss->nnamehash-1; }
			simLog(sim,2,"  %s ~ %s\n",mzrss->tagname[ui],mzrss->smolname[ui]); }}
	
	simLog(sim,2," %i reactions in hash of %i allocated\n",mzrss->nrxnhash,mzrss->maxrxnhash);
	if(vflag) {
		for(i=0;i<mzrss->nrxnhash;i++) {
			if(i==10 && mzrss->nrxnhash>11) {
				simLog(sim,2,"  ...\n");
				i=mzrss->nrxnhash-1; }
			simLog(sim,2,"  %s ~ %s\n",mzrss->mzrrxn[i],mzrss->smolrxn[i]); }}
	
	simLog(sim,2," default states:\n");
	if(!mzrss->defaultstate)
		simLog(sim,2,"  none declared\n");
	else {
		for(i=1;i<sim->mols->nspecies;i++) {
			if(i==10 && sim->mols->nspecies>11) {
				simLog(sim,2,"  ...\n");
				i=sim->mols->nspecies-1; }
			simLog(sim,2,"  %s: %s\n",sim->mols->spname[i],molms2string(mzrss->defaultstate[i],string)); }}

	if(mzrss->refspecies>0) {
		simLog(sim,2," diff. coeff. reference: %s has mass %g\n  and diff. coeffs.",sim->mols->spname[mzrss->refspecies],mzrss->refmass);
		for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1))
			simLog(sim,2," %g",mzrss->refdifc[ms]);
		simLog(sim,2,"\n"); }

	if(mzrss->expandall)
		simLog(sim,2," network fully expanded at initialization");
	else if(mzrss->maxNetworkSpecies>=0)
		simLog(sim,2," expansion on-the-fly and limited to %i species",mzrss->maxNetworkSpecies);
	else
		simLog(sim,2," expansion on-the-fly and unlimited");

	simLog(sim,2,"\n");
	return; }


/* mzrsswrite */
void mzrsswrite(simptr sim,FILE *fptr) {
	mzrssptr mzrss;

	mzrss=sim->mzrss;
	if(!mzrss) return;
	fprintf(fptr,"# Moleculizer parameters\n");
	fprintf(fptr,"start_rules\n");
	if(mzrss->rules) fprintf(fptr,"%s\n",mzrss->rules);
	fprintf(fptr,"end_rules\n");
	fprintf(fptr,"\n");
	return; }


/******************************************************************************/
/********************************* structure set up ***************************/
/******************************************************************************/


/* mzrsetcondition. */
void mzrsetcondition(mzrssptr mzrss,enum StructCond cond,int upgrade) {
	if(!mzrss) return;
	if(upgrade==0 && mzrss->condition>cond) mzrss->condition=cond;
	else if(upgrade==1 && mzrss->condition<cond) mzrss->condition=cond;
	else if(upgrade==2) mzrss->condition=cond;
	if(mzrss->sim && mzrss->condition<mzrss->sim->condition) {
		cond=mzrss->condition;
		simsetcondition(mzrss->sim,cond==SCinit?SClists:cond,0); }
	return; }



/* mzrAssignDrawingParameters */
int mzrAssignDrawingParameters(simptr sim,char *tagname,int ident) {
#ifdef LIBMOLECULIZER
	int strm,got;
	enum MolecState ms;
	mzrssptr mzrss;

	if(!sim->mzrss || !sim->mzrss->mzr) return 2;
	mzrss=sim->mzrss;
	got=0;
	for(strm=0;strm<mzrss->nstreams && !got;strm++)
		if(checkSpeciesTagIsInSpeciesStream(mzrss->mzr,tagname,mzrss->streamname[strm])) {
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) {
				molsetcolor(sim,ident,ms,mzrss->color[strm][ms]);									// call to Smoldyn
				molsetdisplaysize(sim,ident,ms,mzrss->displaysize[strm][ms]); }		// call to Smoldyn
			got=1; }
	
	return !got;
#else
	return 2;
#endif
}


/* mzrAssignDiffCoeff */
int mzrAssignDiffCoeff(simptr sim,char *tagname,double mass,int ident) {
	mzrssptr mzrss;
	enum MolecState ms;
	int got,strm;

	if(!sim->mzrss || !sim->mols) return 2;
	mzrss=sim->mzrss;
	got=0;
	strm=0;
#ifdef LIBMOLECULIZER
	for(strm=0;strm<mzrss->nstreams && !got;strm++)
		if(checkSpeciesTagIsInSpeciesStream(mzrss->mzr,tagname,mzrss->streamname[strm]))
			got=1;
	strm--;
#endif

	for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1)) {
		if(got && mzrss->strmdifc[strm][ms]>=0)
			molsetdifc(sim,ident,ms,mzrss->strmdifc[strm][ms]);									// call to Smoldyn
		else if(mzrss->refmass>0 && mass>0)
			molsetdifc(sim,ident,ms,mzrss->refdifc[ms]*pow(mzrss->refmass/mass,1.0/3.0));		// call to Smoldyn
		else
			molsetdifc(sim,ident,ms,0); }
	return 0; }


/* mzrSetDefaultState */
int mzrSetDefaultState(simptr sim,int ident,enum MolecState ms) {
	mzrssptr mzrss;
	int er;

	if(!sim->mzrss || !sim->mols) return 2;
	mzrss=sim->mzrss;
	if(!mzrss->defaultstate || mzrss->maxspecies!=sim->mols->maxspecies) {
		er=mzrallocdefaultstate(mzrss,sim->mols->maxspecies);
		if(er) return 1; }
	mzrss->defaultstate[ident]=ms;
	return 0; }


/* mzrMolState */
enum MolecState mzrMolState(mzrssptr mzrss,int ident,int molint) {
	char *name,molname[STRCHAR],*nameend;
	int i;

	name=mzrss->sim->mols->spname[ident];
	for(i=0;i<molint && name;i++) {
		name=strchr(name,'.');
		if(name) name++; }
	if(!name) return MSnone;
	nameend=strpbrk(name,".(");
	if(nameend) {
		strncpy(molname,name,nameend-name);
		molname[nameend-name]='\0'; }
	else strcpy(molname,name);		// molname is now the molint'th mol in the ident species

	i=stringfind(mzrss->sim->mols->spname,mzrss->sim->mols->nspecies,molname);
	if(i<1) return MSnone;
	if(i<mzrss->maxspecies) return mzrss->defaultstate[i];

	return MSnone; }


/* mzrDetermineProductState */
enum MolecState mzrDetermineProductState(mzrssptr mzrss,int ident,enum MolecState ms1,enum MolecState ms2) {
	enum MolecState ms,msmol,msmolbest;
	int strm,got,i;
	
	ms=MSnone;
	if(ident<mzrss->maxspecies) ms=mzrss->defaultstate[ident];		// test 1

	got=0;
	strm=0;
#ifdef LIBMOLECULIZER
	if(ms==MSnone) {																							// test 2
		for(strm=0;strm<mzrss->nstreams && !got;strm++)
			if(checkSpeciesTagIsInSpeciesStream(mzrss->mzr,mzrss->tagname[ident],mzrss->streamname[strm]))
				got=1;
		strm--;
		if(got) ms=mzrss->strmstate[strm]; }
#endif

	if(ms==MSnone) {																							// test 3
		msmolbest=MSnone;
		for(i=0;i<=symbolcount(mzrss->sim->mols->spname[ident],'.');i++) {
			msmol=mzrMolState(mzrss,ident,i);
			if(msmol==MSfront || msmol==MSback || msmol==MSup || msmol==MSdown) msmolbest=msmol;
			else if((msmol==MSsoln || msmol==MSbsoln) && msmolbest==MSnone) msmolbest=msmol; }
		ms=msmolbest; }
																																// check consistency with ms1, ms2
	if((ms1==MSsoln || ms1==MSbsoln || ms1==MSnone) && (ms2==MSsoln || ms2==MSbsoln || ms2==MSnone) && !(ms1==MSnone && ms2==MSnone)) {
		if(ms==MSsoln || ms==MSbsoln || ms==MSnone);
		else ms=MSnone; }
	
	if(ms==MSnone) {																							// test 3
		if(ms1==MSnone)
			ms=ms2;											// ms1 is none so set to ms2
		else if(ms2==MSnone)
			ms=ms1;											// ms2 is none so set to ms1
		else if(ms1==MSsoln)
			ms=ms2;											// ms1 is soln and ms2!=none so set to ms2
		else if(ms2==MSsoln)
			ms=ms1;											// ms2 is soln and ms1!=none so set to ms1
		else if(ms1==MSbsoln)
			ms=ms2;											// ms1 is bsoln and ms2=surface so set to ms2
		else if(ms2==MSbsoln)
			ms=ms1;											// ms2 is bsoln and ms1=surface so set to ms1
		else
			ms=ms1; }										// both are surface so set to ms1

	return ms; }


/* mzrAssignProductDefaultState */
int mzrAssignProductDefaultState(mzrssptr mzrss,int *reactants,int nreactants,int *products,int nproducts) {
	int i,er;
	enum MolecState ms1,ms2,msprd;

	if(!mzrss->defaultstate) return 0;

	ms1=MSnone;
	ms2=MSnone;
	if(nreactants>=1)
		ms1=(reactants[0]<mzrss->maxspecies)?mzrss->defaultstate[reactants[0]]:MSnone;
	if(nreactants>=2)
		ms2=(reactants[1]<mzrss->maxspecies)?mzrss->defaultstate[reactants[1]]:MSnone;
	for(i=0;i<nproducts;i++) {
		msprd=mzrDetermineProductState(mzrss,products[i],ms1,ms2);
		er=mzrSetDefaultState(mzrss->sim,products[i],msprd);
		if(er) return er; }

	return 0; }


/* mzrSetStreamDisplay */
int mzrSetStreamDisplay(mzrssptr mzrss,char *streamname,enum MolecState ms,double displaysize,double *color) {
	int strm,er,c;
	enum MolecState mslo,mshi,ms1;

	strm=stringfind(mzrss->streamname,mzrss->nstreams,streamname);
	if(strm<0) {
		if(mzrss->nstreams==mzrss->maxstreams) {
			er=mzrallocstreams(mzrss,mzrss->maxstreams*2+1);
			if(er) return 1; }
		strm=mzrss->nstreams++;
		strncpy(mzrss->streamname[strm],streamname,STRCHAR); }

	if(ms==MSall) {mslo=MolecState(0);mshi=MolecState(MSMAX);}
	else if(ms==MSnone) return 0;
	else {mslo=ms;mshi=MolecState(ms+1);}
	for(ms1=mslo;ms1<mshi;ms1=MolecState(ms1 + 1)) {
		if(displaysize>=0) mzrss->displaysize[strm][ms1]=displaysize;
		if(color)
			for(c=0;c<3;c++) mzrss->color[strm][ms1][c]=color[c]; }

return 0; }


/* mzrSetStreamDifc */
int mzrSetStreamDifc(mzrssptr mzrss,char *streamname,enum MolecState ms,double difc) {
	int strm,er;
	enum MolecState mslo,mshi,ms1;

	strm=stringfind(mzrss->streamname,mzrss->nstreams,streamname);
	if(strm<0) {
		if(mzrss->nstreams==mzrss->maxstreams) {
			er=mzrallocstreams(mzrss,mzrss->maxstreams*2+1);
			if(er) return 1; }
		strm=mzrss->nstreams++;
		strncpy(mzrss->streamname[strm],streamname,STRCHAR); }

	if(ms==MSall) {mslo=MolecState(0);mshi=MolecState(MSMAX);}
	else if(ms==MSnone) return 0;
	else {mslo=ms;mshi=MolecState(ms+1);}
	for(ms1=mslo;ms1<mshi;ms1=MolecState(ms1 + 1))
		mzrss->strmdifc[strm][ms1]=difc;

return 0; }


/* mzrSetStreamState */
int mzrSetStreamState(mzrssptr mzrss,char *streamname,enum MolecState ms) {
	int strm,er;

	strm=stringfind(mzrss->streamname,mzrss->nstreams,streamname);
	if(strm<0) {
		if(mzrss->nstreams==mzrss->maxstreams) {
			er=mzrallocstreams(mzrss,mzrss->maxstreams*2+1);
			if(er) return 1; }
		strm=mzrss->nstreams++;
		strncpy(mzrss->streamname[strm],streamname,STRCHAR); }
	mzrss->strmstate[strm]=ms;

	return 0; }


/* mzraddtonamehash */
int mzraddtonamehash(mzrssptr mzrss,const char *tagname,const char *smolname) {
	int i,er;
	char smolnamecopy[STRCHAR];
	
	strncpy(smolnamecopy,smolname,STRCHAR);
	strchrreplace(smolnamecopy,'*','$');
	if(mzrss->nnamehash==mzrss->maxnamehash) {
		er=mzrallocnamehash(mzrss,mzrss->maxnamehash*2+1);
		if(er) return -1; }
	i=mzrss->nnamehash++;
	strncpy(mzrss->tagname[i],tagname,STRCHAR);
	strncpy(mzrss->smolname[i],smolnamecopy,STRCHAR);
	return i; }


/* mzraddtorxnhash */
int mzraddtorxnhash(mzrssptr mzrss,char *mzrrxn,char *smolrxn) {
	int i,er;

	if(mzrss->nrxnhash==mzrss->maxrxnhash) {
		er=mzrallocrxnhash(mzrss,mzrss->maxrxnhash*2+1);
		if(er) return -1; }
	i=mzrss->nrxnhash++;
	strncpy(mzrss->mzrrxn[i],mzrrxn,STRCHAR);
	strncpy(mzrss->smolrxn[i],smolrxn,STRCHAR);
	return i; }


/* mzrNextSmolrxnName */
void mzrNextSmolrxnName(mzrssptr mzrss,char *smolrxn) {
	static int num=0;

	sprintf(smolrxn,"mzr%i",++num);
	return; }


/* mzrMakeNameHash. */
int mzrMakeNameHash(simptr sim) {
#ifdef LIBMOLECULIZER
	mzrssptr mzrss;
	char **names;
	unsigned int numNames;
	char taggedName[STRCHAR];
	int i2,er;
	unsigned int ui;

	if(!sim->mzrss || !sim->mzrss->mzr || !sim->mols) return 3;
	mzrss=sim->mzrss;
	names=NULL;
	numNames=0;
	er=getExplicitSpeciesList(mzrss->mzr,&names,&numNames);
	if(er) return 1;
	if(mzrss->maxnamehash<numNames) {
		er=mzrallocnamehash(mzrss,numNames);
		if(er) return 1; }
	mzrss->nnamehash=0;

	for(ui=0;ui<numNames;ui++) {
		er=convertUserNameToTaggedName(mzrss->mzr,names[ui],taggedName,STRCHAR);
		if(er) return 2;
		i2=stringfind(mzrss->tagname,mzrss->nnamehash,taggedName);
		if(i2>=0) return -1-i2;
		mzraddtonamehash(mzrss,taggedName,names[ui]);

		i2=stringfind(sim->mols->spname,sim->mols->nspecies,names[ui]);
		if(i2>=0) molsetexpansionflag(sim,i2,1); }
	freeCharPtrArray(names,numNames);
	return 0;
#else
	return 4;
#endif
}


/* mzrssreadrules */
int mzrssreadrules(simptr sim,ParseFilePtr *pfpptr) {
	ParseFilePtr pfp;
	char word[STRCHAR],errstring[STRCHAR],*line2,*chptr;
	int done,pfpcode,totallength;
	mzrssptr mzrss;

	pfp=*pfpptr;
	if(!sim->mzrss) {
#ifndef LIBMOLECULIZER
		CHECKS(0,"This Smoldyn compile does not include the libmoleculizer module");
#endif
		CHECKMEM(sim->mzrss=mzrssalloc());
		sim->mzrss->sim=sim; }
	mzrss=sim->mzrss;
	if(!mzrss->rules) {
		CHECKMEM(!mzrallocrules(mzrss,STRCHAR)); }

	done=0;
	totallength=strlen(mzrss->rules);								// totallength is actual current length
	while(!done) {
		if(pfp->lctr==0 && !strchr(sim->flags,'q'))
			simLog(sim,2," Reading file: '%s'\n",pfp->fname);
		pfpcode=Parse_ReadLine(&pfp,word,&line2,errstring);
		*pfpptr=pfp;
		CHECKS(pfpcode!=3,"%s",errstring);

		if(pfpcode==0);																// already taken care of
		else if(pfpcode==2) {													// end reading
			done=1; }
		else if(!strcmp(word,"end_rules")) {					// end_rules
			CHECKS(!line2,"unexpected text following end_rules");
			return 0; }
		else {																				// a line of the rules block
			if(word[0]=='=' && (chptr=strstr(pfp->line,"Mols"))) {	// text substitute "Mols" for "Molecules" in header
				strncpy(chptr+9,chptr+4,STRCHAR-(chptr-pfp->line)-9);
				strncpy(chptr+3,"ecules",STRCHAR-(chptr-pfp->line)-3);
				pfp->line[STRCHAR-1]='\0'; }

			totallength=strlen(mzrss->rules)+strlen(pfp->line)+2;
			if(totallength>mzrss->ruleschars) {
				CHECKMEM(!mzrallocrules(mzrss,2*totallength)); }
			strcat(mzrss->rules,pfp->line);
			strcat(mzrss->rules,"\n"); }}
	
	CHECKS(0,"end of file encountered before end_rules");	// end of file
	
failure:																					// failure
	simParseError(sim,pfp);
	return 1; }


/* mzrssload.  */
int mzrssload(simptr sim) {
#ifdef LIBMOLECULIZER
	int er;
	mzrssptr mzrss;
	const char *errorstring;
	FILE *out_file=NULL;

	mzrss=sim->mzrss;
	if(mzrss->mzr) freeMoleculizerObject(mzrss->mzr);
	mzrss->mzr=NULL;
	CHECKMEM(mzrss->mzr=createNewMoleculizerObject());

#ifdef DEBUG
	out_file = fopen("./libmzr.tmp", "w");
	fprintf(out_file,"%s", mzrss->rules);
	fclose(out_file);
	out_file=NULL;
#endif

	loadCommonRulesString(mzrss->mzr,mzrss->rules);
	er=getErrorState(mzrss->mzr);
	if(er) {
		errorstring=getErrorMessage(mzrss->mzr);
		if(errorstring) {CHECKS(0,"%s",errorstring);}
		else CHECKBUG(0,"unknown libmoleculizer error"); }

	CHECKBUG(er!=1,"unknown error reading network generation rules");
	CHECKS(er!=2,"document unparsable error reading network generation rules");
	CHECKBUG(er!=3,"rules already loaded error reading network generation rules");
	CHECKS(er!=4,"file not found error reading network generation rules");

	return 0;
failure:
	clearErrorState(mzrss->mzr);
	freeMoleculizerObject(mzrss->mzr);
	mzrss->mzr=NULL;
	if(ErrorType==2) simLog(sim,8,"%s",ErrorString);
	else simLog(sim,10,"%s",ErrorString);
	return 1;
#else
	return 0;
#endif
}


/* mzrSetValue */
void mzrSetValue(mzrssptr mzrss,const char *item,int i1) {
	if(!mzrss) return;
	if(!strcmp(item,"maxNetworkSpecies"))
		mzrss->maxNetworkSpecies=i1;
	else if(!strcmp(item,"refspecies"))
		mzrss->refspecies=i1;
	else if(!strcmp(item,"expandall"))
		mzrss->expandall=i1;
	return; }


/* mzrsetupmoleculizer */
int mzrsetupmoleculizer(simptr sim) {
#ifdef LIBMOLECULIZER
	mzrssptr mzrss;
	species** new_species_array;
	reaction** new_reactions_array;
	int number_species,nreactions,er,i,flag,maxdefst;
	enum MolecState ms;
	unsigned int inm;

	if(!sim->mzrss) return 0;
	mzrss=sim->mzrss;

	if(mzrss->condition==SCinit) {
		CHECK(!mzrssload(sim));
		er=mzrMakeNameHash(sim);
		CHECKMEM(er!=1);
		CHECKBUG(!(er>1),"BUG: in mzrMakeNameHash");
		CHECKS(!(er<0),"ERROR: species %s has multiple names",mzrss->smolname[-1-er]);
		if(mzrss->refspecies>0) {
			for(ms=MolecState(0);ms<MSMAX;ms=MolecState(ms + 1))
				mzrss->refdifc[ms]=sim->mols->difc[mzrss->refspecies][ms]; }
		mzrsetcondition(mzrss,SClists,1); }

	if(mzrss->condition==SClists) {
		maxdefst=mzrss->maxspecies;
		for(i=1;i<sim->mols->nspecies;i++) {					// set default states for pre-existing species
			flag=0;
			ms=(i<maxdefst)?mzrss->defaultstate[i]:MSnone;
			if(ms!=MSnone) flag=1;
			for(ms=(MolecState)0;ms<MSMAX && !flag;ms=(MolecState)(ms+1)) {
				if(sim->mols->exist[i][ms]) {
					mzrSetDefaultState(sim,i,ms);
					flag=1; }}}

		CHECKBUG(!mzrExpandUnexpandedSpecies(sim),"BUG: in mzrExpandUnexpandedSpecies");

		for(inm=0;inm<mzrss->nnamehash;inm++) {				// set molecule exist element
			i=stringfind(sim->mols->spname,sim->mols->nspecies,mzrss->smolname[inm]);
			if(i>0) molsetexist(sim,i,MSall,1); }

		mzrsetcondition(mzrss,SCparams,1); }

	if(mzrss->condition==SCparams) {
		if(mzrss->expandall) mzrExpandNetwork(sim);

		new_species_array=NULL;
		new_reactions_array=NULL;
		number_species=0;
		nreactions=0;

		CHECKBUG(!getDeltaSpecies(mzrss->mzr,&new_species_array,&number_species),"BUG: in getDeltaSpecies");
		CHECKBUG(!getDeltaReactions(mzrss->mzr,&new_reactions_array,&nreactions),"BUG: in getDeltaReactions");
		CHECKBUG(!mzrAddSpeciesArrayToSim(sim,new_species_array,number_species),"BUG: in mzrAddSpeciesArrayToSim");
		CHECKBUG(!mzrAddReactionArrayToSim(sim,new_reactions_array,nreactions),"BUG: in mzrAddReactionArrayToSim");
		CHECKBUG(!clearDeltaState(mzrss->mzr),"BUG: in clearDeltaState");

		freeSpeciesArray(new_species_array,(unsigned int)number_species);
		freeReactionArray(new_reactions_array,(unsigned int)nreactions);
		mzrsetcondition(sim->mzrss,SCok,1); }
	
	return 0;
 failure:
	simLog(sim,10,"%s",ErrorString);
	return 1;
#else
	if(sim->mzrss) mzrsetcondition(sim->mzrss,SCok,1);
	return 0;
#endif
}

/******************************************************************************/
/*************************** core simulation functions ************************/
/******************************************************************************/


/* mzrAddSpeciesArrayToSim */
#ifdef LIBMOLECULIZER
int mzrAddSpeciesArrayToSim(simptr sim,species **species_array,int number_species) {
	mzrssptr mzrss;
	int speciesNdx,i,code;
	char smolname[STRCHAR],*tagname;
	double mass;

	mzrss=sim->mzrss;
	for(speciesNdx=0;speciesNdx<number_species;speciesNdx++) {
		tagname=species_array[speciesNdx]->name;
		mass=*species_array[speciesNdx]->mass;
		i=stringfind(mzrss->tagname,mzrss->nnamehash,tagname);
		if(i<0) {																	// add to hash between tagname and smolname if needed
			code=convertTaggedNameToUniqueID(mzrss->mzr,tagname,smolname,STRCHAR);
			if(code==1 || code==2) return 1;
			if(code==3) smolname[STRCHAR-1]='\0';		// truncate too long name and hope for the best
			i=mzraddtonamehash(mzrss,tagname,smolname);
			if(i<0) return 1; }

#ifdef DEBUG
		printf("lmzr adding species: %s\n",mzrss->smolname[i]);
#endif
		code=moladdspecies(sim,mzrss->smolname[i]);																	// call to Smoldyn
		if(code>=0) {
			molsetexpansionflag(sim,code,1);																					// call to Smoldyn
			mzrAssignDiffCoeff(sim,tagname,mass,code);
			mzrAssignDrawingParameters(sim,tagname,code); }
		else if(code==-5) {
			if(!strcmp(mzrss->smolname[i],sim->mols->spname[mzrss->refspecies]))
				mzrss->refmass=mass; }
		else return 1; }
	return 0; }
#endif


/* mzrCompareReactions */
int mzrCompareReactions(void *rxn1void,void *rxn2void) {
#ifdef LIBMOLECULIZER
	reaction *rxn1,*rxn2;
	double mass,mass1lo,mass1hi,mass2lo,mass2hi;
	int dot,dot1lo,dot1hi,dot2lo,dot2hi;
	int n1,n2;

	rxn1=(reaction*) rxn1void;
	rxn2=(reaction*) rxn2void;
	n1=rxn1->numberReactants;
	n2=rxn2->numberReactants;
	if(n1>n2) return -1;							// first test: rxn1<rxn2 if n1>n2
	if(n1<n2) return 1;

	dot1lo=dot1hi=dot2lo=dot2hi=-1;
	if(n1>=1) dot1lo=symbolcount(rxn1->reactantVector[0]->name,'.');
	if(n1>=2) dot1hi=symbolcount(rxn1->reactantVector[1]->name,'.');
	if(dot1lo>=0 && dot1hi>=0 && dot1lo>dot1hi) {
		dot=dot1lo;
		dot1lo=dot1hi;
		dot1hi=dot; }
	if(n2>=1) dot2lo=symbolcount(rxn2->reactantVector[0]->name,'.');
	if(n2>=2) dot2hi=symbolcount(rxn2->reactantVector[1]->name,'.');
	if(dot2lo>=0 && dot2hi>=0 && dot2lo>dot2hi) {
		dot=dot2lo;
		dot2lo=dot2hi;
		dot2hi=dot; }

	if(dot1lo < dot2lo) return -1;		// second test: rxn1<rxn2 if dot1lo < dot2lo
	if(dot2lo < dot1lo) return 1;
	
	if(dot1hi < dot2hi) return -1;		// third test: rxn1<rxn2 if dot1hi < dot2hi
	if(dot2hi < dot1hi) return 1;

	mass1lo=mass1hi=mass2lo=mass2hi=-1;
	if(n1>=1) mass1lo=*rxn1->reactantVector[0]->mass;
	if(n1>=2) mass1hi=*rxn1->reactantVector[1]->mass;
	if(mass1lo>=0 && mass1hi>=0 && mass1lo>mass1hi) {
		mass=mass1lo;
		mass1lo=mass1hi;
		mass1hi=mass; }
	if(n2>=1) mass2lo=*rxn2->reactantVector[0]->mass;
	if(n2>=2) mass2hi=*rxn2->reactantVector[1]->mass;
	if(mass2lo>=0 && mass2hi>=0 && mass2lo>mass2hi) {
		mass=mass2lo;
		mass2lo=mass1hi;
		mass2hi=mass; }

	if(mass1lo < mass2lo) return -1;	// fourth test: rxn1<rxn2 if mass1lo < mass2lo
	if(mass2lo < mass1lo) return 1;

	if(mass1hi < mass2hi) return -1;	// fifth test: rxn1<rxn2 if mass1hi < mass2hi
	if(mass2hi < mass1hi) return 1;
	return 0;
#else
	return 0;
#endif
}


/* mzrAddReactionArrayToSim */
#ifdef LIBMOLECULIZER
int mzrAddReactionArrayToSim(simptr sim,reaction **reactionlist,int nreactions) {
	int r,numReactants,numProducts;
	int reactants[2],products[MAXPRODUCT];
	int rct,prd,i,er;
	double rate;

//	sortVoid((void**) reactionlist,nreactions,mzrCompareReactions);	// sort so association rxns are first, and small reactants are first
// ?? prior line commented out for debugging ??
#ifdef DEBUG
	printf("lmzr adding reactions:\n");
#endif
	for(r=0;r<nreactions;r++) {
		rate=*reactionlist[r]->rate;
		numReactants=reactionlist[r]->numberReactants;
		numProducts=reactionlist[r]->numberProducts;

		for(rct=0;rct<numReactants;rct++) {
			i=mzrTagName2SmolName(sim,reactionlist[r]->reactantVector[rct]->name);
#ifdef DEBUG
			printf("%s %s ",sim->mols->spname[i],rct==numReactants-1?"->":"+");
#endif
			if(i<0) return 1;
			reactants[rct]=i; }

		for(prd=0;prd<numProducts;++prd) {
			i=mzrTagName2SmolName(sim,reactionlist[r]->productVector[prd]->name);
#ifdef DEBUG
			printf("%s %s",sim->mols->spname[i],prd==numProducts-1?"\n":"+");
#endif
			if(i<0) return 1;
			products[prd]=i; }

		er=mzrAssignProductDefaultState(sim->mzrss,reactants,numReactants,products,numProducts);
		if(er) return er;
		if(rate>0)
			er=mzrAddRxn(sim,reactionlist[r]->name,numReactants,reactants,products,numProducts,rate);
		if(er) return 1; }
#ifdef DEBUG
	printf("lmzr done adding reactions.\n");
#endif
	return 0; }
#endif


/* mzrExpandSpecies */
int mzrExpandSpecies(simptr sim,int ident) {
#ifdef LIBMOLECULIZER
	mzrssptr mzrss;
	int er,size1,size2;
	char tagged_name[STRCHAR];
	
	mzrss=sim->mzrss;
	er=convertSomeNameToTaggedName(mzrss->mzr,sim->mols->spname[ident],tagged_name,STRCHAR);
	if(er) return 1;

	size1=getNumberOfSpecies(mzrss->mzr)*getNumberOfReactions(mzrss->mzr);
	if(mzrss->maxNetworkSpecies<0 || mzrss->maxNetworkSpecies>getNumberOfSpecies(mzrss->mzr))
		expandSpeciesByTag(mzrss->mzr,tagged_name);
	molsetexpansionflag(sim,ident,0);																					// call to Smoldyn
	size2=getNumberOfSpecies(mzrss->mzr)*getNumberOfReactions(mzrss->mzr);
	
	if(size1!=size2) {						// If the network grew because of expansion. 
		mzrsetcondition(sim->mzrss,SCparams,0); }
	
	return 0;
#else
	return 2;
#endif
}


/* mzrExpandNetwork */
int mzrExpandNetwork(simptr sim) {
#ifdef LIBMOLECULIZER
	mzrssptr mzrss;
	int er,size1,size2;
	
	mzrss=sim->mzrss;
	
	size1=getNumberOfSpecies(mzrss->mzr)*getNumberOfReactions(mzrss->mzr);
	er=expandNetwork(mzrss->mzr);
	if(er) return 1;
	molsetexpansionflag(sim,-1,0);																					// call to Smoldyn
	size2=getNumberOfSpecies(mzrss->mzr)*getNumberOfReactions(mzrss->mzr);
	
	if(size1!=size2) {						// If the network grew because of expansion. 
		mzrsetcondition(sim->mzrss,SCparams,0); }
	
	return 0;
#else
	return 2;
#endif
}


/* mzrExpandUnexpandedSpecies. */
int mzrExpandUnexpandedSpecies(simptr sim) {
	int i;
	molssptr mols;

	mols=sim->mols;
	for(i=0;i<mols->nspecies;i++) {
		if(mols->expand[i]) {
			if(mzrExpandSpecies(sim,i)) {
				return 1; }}}
	return 0; }


/* mzrAddRxn */
int mzrAddRxn(simptr sim,char *name,int order,int *reactants,int *products,int nprod,double rate) {
	mzrssptr mzrss;
	enum MolecState rctstate[MAXORDER],prdstate[MAXPRODUCT],ms;
	rxnptr rxn;
	int rallsoln,pallsoln,i;
	char smolrxn[STRCHAR];

	mzrss=sim->mzrss;
	rallsoln=1;
	for(i=0;i<order;i++) {
		ms=(mzrss->defaultstate)?mzrss->defaultstate[reactants[i]]:MSsoln;
		if(ms==MSnone) ms=MSsoln;
		if(ms!=MSsoln) rallsoln=0;
		rctstate[i]=ms; }

	pallsoln=1;
	for(i=0;i<nprod;i++) {
		ms=(mzrss->defaultstate)?mzrss->defaultstate[products[i]]:MSsoln;
		if(ms==MSnone) ms=MSsoln;
		if(ms!=MSsoln) pallsoln=0;
		prdstate[i]=ms; }

	mzrNextSmolrxnName(mzrss,smolrxn);
	
#ifdef DEBUG
	printf("   reaction name: %s\n",smolrxn);
#endif
	
	i=mzraddtorxnhash(mzrss,name,smolrxn);
	if(i<0) return 1;
	rxn=RxnAddReactionCheck(sim,smolrxn,order,reactants,rctstate,nprod,products,prdstate,NULL,NULL);		// call to Smoldyn
	if(!rxn) {
		simLog(sim,10,"%s\n",ErrorString);
		return 1; }
	RxnSetValue(sim,"rate",rxn,rate);

	if(rallsoln && !pallsoln) {				// all reactants in soln but not all products in soln
		for(i=0;i<order;i++) rctstate[i]=MSall;
		RxnSetPermit(sim,rxn,order,rctstate,1);
		for(i=0;i<order;i++) rctstate[i]=MSsoln;
		RxnSetPermit(sim,rxn,order,rctstate,0); }

	return 0; }




