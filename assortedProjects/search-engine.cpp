#include <fstream>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

#include "search-engine.h"
using namespace std;

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  // Create dictionary of the indicated type


    if(dictionaryType==ArrayDictionaryType){
        _wordToURLList=new ArrayDictionary();
	type = ArrayDictionaryType;
    }
    if(dictionaryType==HashDictionaryType){
        _wordToURLList=new HashDictionary();
	type = HashDictionaryType;
    }
        if(dictionaryType==AVLDictionaryType){
        _wordToURLList=new AVLDictionary();
	type = AVLDictionaryType;
    }
    if(dictionaryType==BinarySearchDictionaryType){
        _wordToURLList=new BinarySearchDictionary();
	type = BinarySearchDictionaryType;
    }
    FILE* URL;
    FILE* WORDS;
    URL=fopen("url.txt","r");
    WORDS=fopen("words.txt","r");
    int index;
    char* url=new char[1000];
    char* description=new char[200];
    char* temp=new char[200];
    list=new URLRecord[1000];
    while(!feof(URL)){
	fscanf(URL,"%d %s\n",&index,url);
        int length=0;
        description = new char[200];
	while(length < 200 && fscanf(URL,"%s ",temp)==1){    
            	strcat(description,temp);
                length+=strlen(temp);
            	if (length < 200){
			description[length] = ' ';
                	length++;
            		}
			
		}
	URLRecord *e=new URLRecord;
	list[index].index=index;
	list[index].url=strdup(url);
	list[index].description=(description);
	//printf("%d %s %s\n",list[index].index=index,list[index].url=strdup(url),list[index].description=(description));
	}

   
 
    int max=index;
    char  **terms = new char*[9001];
    int  **theArray=new int*[9001];
    int numFound = 0;     
    while(!feof(WORDS)){
    	char * key = new char[50];
        fscanf(WORDS,"%s",key);
    	terms[numFound] = strdup(key);
        int i=0;
        int t;
        theArray[numFound] = new int[max];
        while(!feof(WORDS) && fscanf(WORDS,"%d", &t)){
        	theArray[numFound][i] = t; 
        	i++;
     
        }
        int *newArray = new int[i+1];
    	newArray[i] = -1;
     	for (int j = 0; j < i; j++){
        	newArray[j]= theArray[numFound][j];
    	}
       delete [] theArray[numFound];
       theArray[numFound] = newArray;
       _wordToURLList->addRecord((const char*)terms[numFound],(void*) theArray[numFound]);
       numFound++;
                
    }
	

/*
    int * x = new int[max];
    void * v = _wordToURLList->findRecord("bumble");
    x = (int *)v;
    int j=0;
    while (x[j] != -1){
      j++;
    }
 
    for (int i=0;i<j;i++){
        //printf("%d\n", x[i]);
    }
 */
 
 





 

  // Populate dictionary and sort it if necessary
 
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
hrtime_t startTime;
hrtime_t endTime;
hrtime_t onewordTime;
startTime=gethrtime();
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form

    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
    return;
  }


  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
 
  //const char * wordArray=strdup(documentRequested);
  //int len=strlen(wordArray);
  //int plus=0;
 // printf("%s\n",documentRequested);
  int len=strlen(documentRequested);
  int numberOfWords=0;
  for(int i=0;i<len;i++){
    if (documentRequested[i]=='+'){
        numberOfWords++;    
    }
  }
numberOfWords++;
char finalArray[numberOfWords][len];
int wordN=0;
int position=0;
int prev =13;
for(int i=0;i<numberOfWords;i++){
    while(documentRequested[prev+position]!=NULL && documentRequested[prev + position]!='+'){
        finalArray[i][position]=documentRequested[prev+position];
        position++;
    }
    finalArray[i][position]='\0';
    //printf("%d\t%d\t%s\n",prev+position, position, finalArray[i]);    
    prev += position+1;
    position = 0;

}



int z=0;
int numberOfInts=0;
int *intersections;
int ** x = new int*[numberOfWords];
int *sizeOfIntersection=new int[numberOfWords];
for(int i=0;i<numberOfWords;i++){
  
        void * v = _wordToURLList->findRecord(finalArray[i]);
	if(v == NULL){
		fprintf( fout, " \n");
  		fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  		fprintf( fout, "<H1> <Center><em>Boiler Search</em></center></H1>\n");
  		fprintf( fout, "<H2> Search Results for \"");
  		for(int i=0;i<numberOfWords;i++){
			fprintf( fout, "%s ", finalArray[i]);
  		}
  		fprintf( fout, "\"</H2>\n");

  		for ( int i = 0; i < numberOfInts; i++ ) {
   			 fprintf( fout, "Sorry, no results :(\n");
    		}
		fprintf( fout, "<blockquote><b>Search Yielded No Results</b><p></blockquote>\n");
  		// Add search form at the end
		
  		fprintf(fout, "<HR><H2>\n");
  		fprintf(fout, "<FORM ACTION=\"search\">\n");
  		fprintf(fout, "Search:\n");
  		fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  		fprintf(fout, "</H2>\n");
  		fprintf(fout, "</FORM>\n");
		return;
		
	}
    x[i] = (int *)v;
    
      int j=0;
        while (x[i][j] != -1){
          j++;
    }
        sizeOfIntersection[i]=j;
        for (z=0;z<j;z++){
           // printf("%d\n", x[z]);
        }
}
int j=0;
for(int i=0;i<numberOfWords;i++){
   while (x[i][j] != -1){
          //printf("\n%d ",x[i][j]);
          j++;
    }
}
//printf("The number of words is %d\n",numberOfWords);
if(numberOfWords>1){
    int lowest=sizeOfIntersection[0];
    int lowIndex=0;
    for(int i=0;i<numberOfWords;i++){
        if(sizeOfIntersection[i]<lowest){
            lowest=sizeOfIntersection[i];
            lowIndex=i;
        }
    } 
    intersections=new int[lowIndex];
    int *lowArray=x[lowIndex];
    int s=0;
 
   
    for(int i=0;i<numberOfWords-1;i++){
        if(i==lowIndex){
            i++;
        } 
        for(int l=0;l<j;l++){
    
    for (int k = 0; k < lowest; k++){
          printf("one %d   the other %d\n",x[i][l],lowArray[k]);
            if(x[i][l]==lowArray[k]){
                    intersections[s]=x[i][l];
                    numberOfInts++;
		    
                    s++; 
            }
    }
       }
    }

printf("Number of ints is %d\n",numberOfInts);

for(int t=0;t<numberOfInts;t++){
    printf("\n\n\n\nThe intersections are %d ",intersections[t]);

}
}



else{
	
        printf("The one word is %s\n",finalArray[0]);
	int size=0;
	//for(int i=0;x[0][i]!=-1;i++){
	//	size++;
	//}
	//size--;
	//if(size<0){
	//	size=0;	
	//}
	intersections=new int[j];
	for(int k=0;k<j;k++){
		intersections[k]=x[0][k];
	
	}
			fprintf( fout, " \n");
  onewordTime=gethrtime();
  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></center></H1>\n");
  fprintf( fout, "<H2> Search Results for \"");
  for(int i=0;i<numberOfWords;i++){
	fprintf( fout, "%s ", finalArray[i]);
  }
  fprintf( fout, "\"</H2>\n");	
	for(int i=0;i<j;i++){

	


		//start of sanity
		fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", i+1,list[intersections[i]].url,list[intersections[i]].url);
     		fprintf( fout, "<blockquote>%s<p></blockquote>\n", list[intersections[i]].description );
	}
	fprintf(fout, "Search time %d nanoseconds<p>",abs((long)startTime - (long)onewordTime));
  if(type==ArrayDictionaryType){
        fprintf(fout, "Searched using an array\n");
    }
    if(type==HashDictionaryType){
        fprintf(fout, "Searched using a hash table\n");
    }
        if(type==AVLDictionaryType){
        fprintf(fout, "Searched using an AVL tree\n");
    }
    if(type==BinarySearchDictionaryType){
        fprintf(fout, "Searched using binary search\n");
    }
fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");	
return;
}
endTime=gethrtime();


fprintf( stderr, "Search for words: ");

fprintf( fout, " \n");
  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></center></H1>\n");

  fprintf( fout, "<H2> Search Results for \"");
  for(int i=0;i<numberOfWords;i++){
	fprintf( fout, "%s ", finalArray[i]);
  }
  fprintf( fout, "\"</H2>\n");
 
  for ( int i = 0; i < numberOfInts; i++ ) {
    //fprintf( fout, "<h3>%d. <a href=\"%d\">%s</a><h3>\n", i+1 );
    fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", i+1,list[intersections[i]].url,list[intersections[i]].url);
     fprintf( fout, "<blockquote>%s<p></blockquote>\n", list[intersections[i]].description );
    //list=list->next;
    }

  // Add search form at the end
  fprintf(fout, "Search time %d nanoseconds<p>",abs((long)startTime - (long)endTime));
  if(type==ArrayDictionaryType){
        fprintf(fout, "Searched using an array\n");
    }
    if(type==HashDictionaryType){
        fprintf(fout, "Searched using a hash table\n");
    }
        if(type==AVLDictionaryType){
        fprintf(fout, "Searched using an AVL tree\n");
    }
    if(type==BinarySearchDictionaryType){
        fprintf(fout, "Searched using binary search\n");
    }
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
 
  SearchEngine httpd(port, dictionaryType);
 
  httpd.run();

  return 0;
}

