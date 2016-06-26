#include "webcrawler.h"
#include <stdio.h>
#include <string>
#include<stdlib.h>
#include<iostream>
#include<ctype.h>
#include<fstream>
// include the hashtemplate copy the file into the folder
//remember to subclass the simplehtmlparser in webcrawler.h


 WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots){
    //URLRecord record;
    //printf("got here\n");
   _headURL=0;
   _tailURL=nurlRoots;
    _maxUrls=maxUrls;
    _urlArray=(URLRecord*)malloc(sizeof(URLRecord)*maxUrls);
    _urlToUrlRecord=new HashTableTemplate<int>();   
  _wordToURLRecordList=new HashTableTemplate<URLRecordList*>();
//_wordToURLRecordList._urlRecordIndex=_headURL;
//_wordToURLRecordList->(URLRecordList*)_next=NULL;
    for(int i=0;i<nurlRoots;i++){
        //strcpy(_urlArray[i]._url,urlRoots[i]);
        _urlArray[i]._url=strdup((char*)urlRoots[i]);
      _urlToUrlRecord->insertItem((char*)urlRoots[i],i);
        //printf("%s\n",urlRoots[i]);
        _urlArray[i]._description=new char[500];
    currentWord=new char[500];//deal with it

}




}


int main(int argc,char* argv[]){

    int length=0;
    int maxUrls=1000;
    bool optional;
    if(strcmp(argv[1],"-u")==0){
        optional=true;
         length=argc-3;
        maxUrls=atoi(argv[2]);
        //printf("The new Maxurl is %d\n",maxUrls);
    }
    else{
        optional=false;
         length=argc-1;

    }
    int words[length];
    int i;
    for( i=0;i<length;i++){
        if(optional==true){
            words[i]=strlen(argv[i+3]);
        }
        else{

            words[i]=strlen(argv[i+1]);
        }

    }
        const char** theList;
    theList = (const char**)malloc(length * sizeof(const char*));
    for(int k=0;k<length;k++){

        //theList[k]=(char*)malloc(words[k]*sizeof(char));
        if(optional==true){
            //printf("this is optional %s\n",argv[k+3]);
            theList[k]=strdup(argv[k+3]);

            //printf("%s\n",theList[k]);
        }
        else{
            //printf("this isnt\n");
            theList[k]=strdup(argv[k+1]);

            //printf("%s\n",theList[k]);
        }


    }


    WebCrawler *spider=new WebCrawler(maxUrls,length,theList);
    //printf("made it to crawl\n");
        spider->crawl();
    spider->writeURLFile("url.txt");
    spider->writeWordFile("word.txt");

}


void WebCrawler::crawl(){
    int n;
    char* url;
    char* content;
    //printf("%s\n",_urlArray[_headURL]._url);
      while (_headURL <_tailURL ) {
                url=_urlArray[_headURL]._url;
                content=fetchHTML( url,&n);
		
		if(content!=NULL){
        //printf("%d\n", n);

        //if(){//document is not html
        //    continue;
        //}
        parse(content,n);
        
		}
	_headURL++;
    }



    //printf("%s\n",content);


}

void WebCrawler::writeURLFile(const char * urlFileName){
    std::ofstream myfile;
      myfile.open (urlFileName);
    for(int i=0;i<_tailURL;i++){
        myfile << i <<" " <<_urlArray[i]._url <<"\n";
        myfile <<_urlArray[i]._description <<"\n";
	myfile<<"\n";
    }
    myfile.close();
}

void WebCrawler::writeWordFile(const char *wordFileName){

    std::ofstream myfiled;
      myfiled.open (wordFileName);
    const char* key;
    URLRecordList* data= new URLRecordList;
    HashTableTemplateIterator<URLRecordList*>*iterator;
     iterator=new  HashTableTemplateIterator<URLRecordList*>(_wordToURLRecordList);
    while(iterator->next(key,data)){
        myfiled<< key<<" ";
        while(data!=NULL){
           	myfiled<< data->_urlRecordIndex <<" ";
            	data=data->_next;
        }
        myfiled<<"\n";
    }
    myfiled.close();

}

void WebCrawler::onContentFound(char c){
	int length;
	int lengthc;
	URLRecordList* record=new URLRecordList;

  
    	if(_urlArray[_headURL]._description==NULL){
     	   	length=0;

   	}
	else{
		length=strlen(_urlArray[_headURL]._description);
	}

	if(length<500){

		_urlArray[_headURL]._description[length]=c;
		_urlArray[_headURL]._description[length+1]=NULL;
		
	}
	if(isalnum(c)){
			if(currentWord==NULL){
				lengthc=0;
			}
			else{
				lengthc=strlen(currentWord);
			}
			currentWord[lengthc]=c;
			currentWord[lengthc+1]=NULL;
			}
			else{
				if((_wordToURLRecordList->find(currentWord,&record)==false)){
					record->_urlRecordIndex=_headURL;
					record ->_next = NULL;
					_wordToURLRecordList->insertItem(currentWord,record);
				}
				else{
					if(_headURL > record->_urlRecordIndex){
						URLRecordList* temp = record;   
						record = new URLRecordList;                
						record->_next=temp;
						record->_urlRecordIndex=_headURL;
						_wordToURLRecordList->insertItem(currentWord,record);
					}
				}
			currentWord[0]='\0';
			lengthc=0;
		}
	/*else{
		return;
	}*/

    //printf("length = %d\n",length);
    //printf("%c",_urlArray[_headURL]._description[length]);
    //printf("%c",currentWord[lengthc]);
}

void WebCrawler::onAnchorFound(char *url){
//if it finds and ancor <a href="website"> it calls this fxn
//onanchorfound("website")
//make sure its not in the record list already
//put it in the record list
//and the url array



    URLRecordList* anchor=new URLRecordList;
    if(_tailURL>_maxUrls-1){
        return;

    }
/*
    if((_wordToURLRecordList->find(currentWord,&anchor)==false)){
            anchor->_urlRecordIndex=_headURL;
            _wordToURLRecordList->insertItem(currentWord,anchor);
            //currentWord[lengthc]='|';
            //also insert it into the url array
            urlArray[_tailURL]
            }
            else{
                URLRecordList* second=anchor;
                while(second!=NULL&&second->_urlRecordIndex!=_headURL){

                    second=second->_next;
                }

                second->_urlRecordIndex=_headURL;
            }*/
    int n;
    if(_urlToUrlRecord->find(url,&n)==false && strncmp(url,"http://",7)==0){

      _urlArray[_tailURL]._description=new char[500];
      _urlToUrlRecord->insertItem(url,_tailURL);
        _urlArray[_tailURL]._url=strdup(url);
        _tailURL++;
    }


}
