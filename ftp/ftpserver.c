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
#endif	/* INADDR_NONE */

#define BUFFSIZE		256

/*-----------------------------------------------------------------------
 *
 * Program: echoserver
 * Purpose: wait for a connection from an echoclient and echo data
 * Usage:   echoserver <appnum>
 *
 *-----------------------------------------------------------------------
 */

unsigned short	portbase = 0;

int
main(int argc, char *argv[]){

    char * tmp=strdup(argv[1]);
    int port= atoi(tmp);
    char * transport = "tcp";



    struct hostent	*phe;	/* pointer to host information entry	*/
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/

	//configure socket
    s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;

    if ( pse = getservbyport(port, transport) )
		sin.sin_port = htons(ntohs((unsigned short)pse->s_port)
			+ portbase);
	else if ((sin.sin_port=htons((unsigned short)atoi(tmp))) == 0)
		printf("can't get %d service entry\n", port);
    
    ppe = getprotobyname(transport);


    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		printf("Can't bind\n");

	if ( listen(s, 1000) < 0)
		printf("Can't listen\n");
    
   
    //prepair to take commands
    char reply[100];
    char action[20];
    char filename[1024];
    while(1){

        int fd=accept(s, (struct sockaddr *)&sin, ( socklen_t *)&sin);

        recv(fd,reply,100,0);
        sscanf(reply,"%s %s\n",action,filename);




        if(strcmp(action,"GET")==0){

            if(access(filename, F_OK)!=-1){
                //file exists and you should send it

                char  succ [50]="GETRESPONSE OKAY ";
                strcat(succ,filename);
                strcat(succ,"\n");
                send(fd,succ,sizeof(succ),0);

                //open the file
                FILE * file;
			    file=fopen(filename,"r");
                //save file contents into string
                long lSize;
                char * content;
                size_t result;
			    fseek (file , 0 , SEEK_END);
			    lSize = ftell (file);
  				rewind (file);
			    content = (char*) malloc (sizeof(char)*lSize);
                fread (content,1,lSize,file);
                //convert the size
                  
                unsigned int size;
                size=htonl(lSize);

               
                //send the size

                send(fd,&size,4,0);


				int chunkSize=2048;
				//char chunk[chunkSize];				
				if(lSize<chunkSize){
					send(fd,content,lSize,0);

				}
				
            
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
		                    sendString[l]=content[index];
		                    index++;
		                }

				

						send(fd,sendString,sizeof(sendString),0);
					}
				}

			



            }
            else{
                //file does not exist and you should inform the user of this

                char fail [50]="GETRESPONSE FAILED_FILE_NOT_FOUND ";
                strcat(fail,filename);
                strcat(fail,"\n");
                send(fd,fail,sizeof(fail),0);


            }


        }


        else if(strcmp(action,"PUT")==0){
			if(access(filename, F_OK)!=-1){
				//file already exists, cannot put
				char error[100]="PUTRESPONSE FAILED_CANNOT_CREATE_FILE ";
				strcat(error,filename);
				strcat(error,"\n");
				send(fd,&error,sizeof(error),0);
				
				
			}
			else{
				//file does not exist, get from client
				//get size
				unsigned int size;
				recv(fd,&size,4,0);
				//convert it
				int realSize=ntohl(size);

				int d=realSize;
					//open the file
				   FILE * file;
				   file=fopen(filename,"w");
					int chunkSize=2048;
					int buff[chunkSize];
					int total=0;

					//easily place small messages
					if(realSize<chunkSize){
						recv(s,&buff,realSize,0);
						fwrite (buff , sizeof(char), realSize, file);
					}

					else{
							//break message into chunks and write each one
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
								//strcat(write,buff);
								d-=chunkSize;


						  }
				  }
				  fclose(file); 











			}			

        }


        else if(strcmp(action,"EXIT")==0){
            close(fd);
            exit(0);

        }
        else{
           printf("something is very wrong here \n");
        
        }


       
        memset(&reply, 0, sizeof(reply));
        memset(&action, 0, sizeof(action));
        memset(&filename, 0, sizeof(filename));


    }

}
