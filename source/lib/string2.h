/* Steven Andrews, 11/01.
See documentation called string2_doc.doc
Copyright 2003-2009 by Steven Andrews.  This work is distributed under the terms
of the Gnu Lesser General Public License (LGPL). */

#ifndef __string2_h
#define __string2_h

#define STRCHAR 256
#define STRCHARLONG 4000


/******************* String classification ************************/

int strisnumber(const char *str);
int okname(const char *name);
int strbegin(const char *strshort,const char *strlong,int casesensitive);
int symbolcount(const char *s,char c);

/*********************** Character locating ***********************/

char *strrpbrk(char *cs,const char *ct);
char *StrChrQuote(char *cs,char c);
int StrChrPQuote(const char *cs,char c);
int StrrChrPQuote(const char *cs,char c);
int strChrBrackets(const char *string,int n,char c,const char *delimit);
int strparenmatch(const char *string,int index);

/************************* Word operations ************************/

int wordcount(const char *s);
char *strnword(char *s,int n);
char *strnword1(char *s,int n);

/************************** String arrays *************************/

int stringfind(char **slist,int n,const char *s);

/**************** Reading sequential items from strings ***********/

int strreadni(char *s,int n,int *a,char **endp);
int strreadnf(char *s,int n,float *a,char **endp);
int strreadnd(char *s,int n,double *a,char **endp);
int strreadns(char *s,int n,char **a,char **endp);

/************ String copying with memory allocation ***************/

char *EmptyString();
char *StringCopy(const char *s);
unsigned char *PascalString(const char *s);

/************ String modifying without memory allocation **********/

char *strPreCat(char *str,const char *cat,int start,int stop);
char *strPostCat(char *str,const char *cat,int start,int stop);
char *strMidCat(char *str,int s1,int s2,const char *cat,int start,int stop);
int strchrreplace(char *str,char charfrom,char charto);
int strstrreplace(char *str,const char *strfrom,const char *strto,int max);
void strcutwhite(char *str,int end);
int strbslash2escseq(char *str);

/******************* Wildcards and enhanced wildcards *************/

int strwildcardmatch(char *pat,char *str);
int strwildcardmatchandsub(char *pat,char *str,char *dest);
int strexpandlogic(const char *pat,int start,int stop,char ***results,int top,int *nrptr,int *maxrptr);
int strEnhWildcardMatch(const char *pat,const char *str);
int strEnhWildcardMatchAndSub(const char *pat,const char *str,const char *destpat,char *dest);

#endif
