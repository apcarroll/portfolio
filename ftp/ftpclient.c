#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#ifndef	INADDR_NONE
#define	INADDR_NONE	0xffffffff

#define INPUT_PROMPT		"Input command > "
#define RECEIVED_PROMPT		"Response > "


#endif	/* INADDR_NONE */




/*-----------------------------------------------------------------------
 *
 * Program: echoclient
 * Purpose: contact echoserver, send user input and print server response
 * Usage:   echoclient <compname> [appnum]
 * Note:    Appnum is optional. If not specified the standard echo appnum
 *          (7) is used.
 *
 *-----------------------------------------------------------------------
 */
int
main(int argc, char *argv[]){

    char * host=strdup(argv[1]);
    char * transport="tcp";
    char * tmp=strdup(argv[2]);
    int port=atoi(tmp);

    struct hostent	*phe;	/* pointer to host information entry	*/
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/


	//configure socket
    s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;

    if ( phe = gethostbyname(host) )
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		printf("can't get %s host entry\n", host);

    if ( pse = getservbyport(port, transport) )
		sin.sin_port = pse->s_port;
	else if ((sin.sin_port=htons((unsigned short)atoi(tmp))) == 0)
		printf("can't get %d service entry\n", port);

    if ( phe = gethostbyname(host) )
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		printf("can't get %s host entry\n", host);

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        printf("Could not connect \n");
        exit(0);
    }

	//prepair for stdin commands
    char str[1050];
    char action[20];
    char filename[1024];

    memset(&str, 0, sizeof(str));
    memset(&action, 0, sizeof(action));
    memset(&filename, 0, sizeof(filename));
    
   

    while(strcmp(action,"EXIT")!=0){
        printf("Input command > ");

        memset(&str, 0, sizeof(str));
        memset(&action, 0, sizeof(action));
        memset(&filename, 0, sizeof(filename)); 

        gets(str);
        sscanf(str,"%s %s\n",action,filename);

       
        if(strcmp(action,"GET")==0){

            //check to make sure file does not already exist
            if(access(filename, F_OK)!=-1){
				printf("RESPONSE > ");
                printf("FILE_ALREADY_EXIST: %s\n",filename);
            }
            else{
            //FILE NOT FOUND, GET IT FROM THE SERVER

                //send the request
                send(s, str, sizeof(str), 0);//you may have to change this
                //recieve a response



                char reply[100];
                recv(s,reply,100,0);
                char proto[100]="GETRESPONSE FAILED_FILE_NOT_FOUND ";
                strcat(proto,filename);
                strcat(proto,"\n");

                if(strcmp(reply,proto)==0){
					printf("RESPONSE > ");
                    printf("%s\n",reply);
                }

                else{
                    //file found, write the file
					printf("RESPONSE > ");
                    printf("%s\n",reply);

                    unsigned int size;


                   int chunkSize=2048;
				   char buff[chunkSize]; 
				   int writeSize=10000;
				   char write[writeSize];
				   int total=0;
				   
				   recv(s,&size,4,0);
				   int realSize=ntohl(size);

				   int d=realSize;
					//write to the file
				   FILE * file;
				   file=fopen(filename,"w");
					//allow for very small files
					if(realSize<chunkSize){
						recv(s,&buff,realSize,0);
						fwrite (buff , sizeof(char), realSize, file);
					}
					//break into chunks and write each to the file
					else{
						   while(d>-chunkSize){

								total+=chunkSize;
								

								int place;
								if(d<chunkSize){
									place=d;
								}	
								else{
									place=chunkSize;
								}
								recv(s,&buff,place,0);
								int Len=strlen(buff);
								strcmp(buff,NULL);

								fwrite (buff , sizeof(char), Len, file);

								memset(&buff, 0, sizeof(buff));	

								d-=chunkSize;


						  }
				  }
				  fclose(file); 
				

            }

        }
    
        }
        else if(strcmp(action,"PUT")==0){

			if(access(filename, F_OK)!=-1){
				//if the file exists read its contents
				FILE * file;
  				long lSize;
  				char * buffer;
				file=fopen(filename,"r");
				fseek (file , 0 , SEEK_END);
			    lSize = ftell (file);
  				rewind (file);
				buffer = (char*) malloc (sizeof(char)*lSize);
				fread (buffer,1,lSize,file);
				
				unsigned int size;
				size=htonl(lSize);

				send(s, str, sizeof(str), 0);

				//send(s,size,4,0);


				int chunkSize=2048;
				int total=0;

				//send the file if it's small
				if(lSize<chunkSize){
					send(s,buffer,lSize,0);

				}
				//send it in pieces if it's big
            
              	else{
		            //send the contents
		            int numSends=lSize/chunkSize;
		            numSends++;
		            int index=0;
		            int k;
		            int l;
		            int chunk;
		            int remaining=lSize;
		            char sendString[chunkSize+1];
		            for(k=0;k<numSends;k++){
		                if(remaining>=chunkSize){
		                    chunk=chunkSize;
		                }
		                else{
		                    chunk=remaining;
		                }
		                for(l=0;l<chunk;l++){
		                    sendString[l]=buffer[index];
		                    index++;
		                }
		                //index=0;
				

						send(s,sendString,sizeof(sendString),0);
					}

			}






				char reply[50];
				recv(s,&reply,sizeof(reply),0);
				printf("RESPONSE > ");
				printf("%s\n",reply);
			}

			else{
				char error[100];
				printf("RESPONSE > ");
				printf("PUTRESPONSE FAILED_FILE_ALREADY_EXIST\n");
				strcat(error,filename);
				strcat(error,"\n");

			}







        }

        else if(strcmp(action,"EXIT")==0){
            printf("RECIEVED EXIT\n");            
            close(s);


        }
        
        else{
            printf("Usage: GET|PUT|EXIT filename\n");
    
        }       

        



    }

    exit(0);


}
