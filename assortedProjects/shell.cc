/*
 * CS354: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include "command.h"




extern char **environ;






void ZOMBIE_SLAYER(int sig){
    while(waitpid(-1,NULL,WNOHANG)>0);
}

SimpleCommand::SimpleCommand()
{
    // Creat available space for 5 arguments
    _numberOfAvailableArguments = 5;
    _numberOfArguments = 0;
    _arguments = (char **) malloc( _numberOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
    if ( _numberOfAvailableArguments == _numberOfArguments  + 1 ) {
        // Double the available space
        _numberOfAvailableArguments *= 2;
        _arguments = (char **) realloc( _arguments,
                  _numberOfAvailableArguments * sizeof( char * ) );
    }


    int index=0;
    //int j=0;
    bool dontInc=false;
    int whereIFoundIt=0;
    int whereItEnds=0;
    bool expansion=false;
    int varlen=strlen(argument);
    int argLen=strlen(argument);
    char* var=new char[varlen];






    char* newArg = new char[100];
    int newArgindex = 0;
    for(int j=0;argument[j]!=NULL;j++){
        //printf("%c\n",argument[j]);



//variable expansion
        if(argument[j]=='$'){
            whereIFoundIt=j;
            if(argument[j+1] != NULL && argument[j+1]=='{'){
                
                index = 0;
                expansion=true;
                j=j+2;
                while(argument[j]!='}'){
                    var[index]=argument[j];
                    
                    j++;
                    dontInc=true;
                    index++;
                }
                whereItEnds=j;
                var[index]=NULL;

            }
              char* answer=getenv(var);
  
           int answerLen=strlen(answer);


           int u=0;
           int index2=0;

        for(int q=0;q<answerLen;q++){

                newArg[newArgindex]=answer[u];
 
                newArgindex++;
                u++;
        }
    

         }
/*
	else if(argument[j]=='"'){
		int counter=0;	
		argument[j]=' ';
		printf("got this far\n");
		j++;	
		char* newerArg=new char[100];	
		while(argument[j]!='"'&&argument[j]!=NULL){
			if(argument[j+1]=' '){
				j+=2;	
			}
			
			printf("got into the loop\n");
			newArg[counter]=argument[j];	
			
			counter++;	
			j++;

		}
		printf("The new argument is %s\n",newArg);
		

	}
*/
/*
	else if(argument[j]=='\'){
		int c=0;
		for(int i=0;i<strlen(argument);i++){
			if(argument[i]=='\'){
				i++;
			}
			newArg[c]=argument[i];
			c++;
		}
	}
*/

        else if (argument[j] == '~'){
         // Understanding ~
         // ~ by itself means the home directory of the current user
         // ~ followed by a word, such as ~x, means the home directory of user x
         // Either can be followed by a path, such as ~/foo, or ~x/foo

         char* home=getenv("HOME");
            int homelen=strlen(home);
            char* tilarg=new char[100];
            int Arg=strlen(argument);
            for(int qq=0;argument[qq]!=NULL;qq++){
                    if(argument[qq]=='~'&& argument[qq+1]==NULL){
                        char* temp=new char[qq];
                        char* temp2=new char[50];
                        for(int i=0;i<qq;i++){
                            temp[i]=argument[i];
                        }
                        int inc=0;
                        for(int i=qq+1;i<Arg;i++){
                            temp2[inc]=argument[i];
                        }

                        for (int k = 0 ; k < homelen; k++){
                            newArg[newArgindex] = home[k];
                            newArgindex++;
                        }
                        newArg[newArgindex] = '/';
                        newArgindex++;
			break;
                    }
		    if(argument[qq]=='~'&& argument[qq+1]=='/'){
				
				
 
		    }
		    if(argument[qq]=='~'&& argument[qq+1]!=NULL && argument[qq+1]!='/'){
				char* temp=new char[qq];
                        	char* temp2=new char[50];				
				 for(int i=0;i<qq;i++){
                        	    temp[i]=argument[i];
                        	}
                        	




				char* nameOfPath=new char[strlen(argument)];
				int counter=0;
				for(int i=qq+1;argument[i]!=NULL;i++){
					nameOfPath[counter]=argument[i];
					counter++;
				}
				nameOfPath[counter]=NULL;
				//printf("The name of the path we seek is %s\n",nameOfPath);
				struct passwd * x=getpwnam(nameOfPath);
				//printf("The thing that we extracted %s\n",x->pw_dir);
				int pathLen=strlen(x->pw_dir);
				//printf("Temp 1 is %s\n",temp);
				for(int k=0;k<pathLen;k++){
					temp2[k]=x->pw_dir[k];
				}
				//printf("New Arg is %s\n",temp2);
				
				int tempLen=strlen(temp2);
				int k;
				
				 for (k = 0 ; k < 1; k++){
	                            newArg[newArgindex] = temp2[k];
				   // printf("%c\n",newArg[newArgindex]);
	                            newArgindex++;
	                        }
				//newArg[k]=NULL;
				//newArgindex++;				
				//printf("new arg is %s\n",newArg);
				
				argument=NULL;				
				argument=strdup(temp2);
				//printf("argument is %s\n",argument);
				
		    }
            }


        }
        else {
            newArg[newArgindex] = argument[j];
            newArgindex++;
               //printf("%s\n", newArg);
        }
           /*if(dontInc==false){
            j++;
        }*/
        dontInc=false;
         //  printf("The variable to expand is %s\n",var);




    }
    newArg[newArgindex] = '\0';
  // printf("%s\n", newArg);
    _arguments[ _numberOfArguments ] = newArg;
     _arguments[ _numberOfArguments + 1] = NULL;
     _numberOfArguments++;
    //argument=getenv(var);
    //printf("The variable inside is %s\n",var);


    // Add NULL argument at the end
   /*if(expansion==false){
         _arguments[ _numberOfArguments ] = argument;
    _arguments[ _numberOfArguments + 1] = NULL;
    _numberOfArguments++;
   }*/


}

Command::Command()
{
    // Create available space for one simple command
    _numberOfAvailableSimpleCommands = 1;
    _simpleCommands = (SimpleCommand **)
        malloc( _numberOfSimpleCommands * sizeof( SimpleCommand * ) );

    _numberOfSimpleCommands = 0;
    _outFile = 0;
    _inputFile = 0;
    _errFile = 0;
    _background = 0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
    if ( _numberOfAvailableSimpleCommands == _numberOfSimpleCommands ) {
        _numberOfAvailableSimpleCommands *= 2;
        _simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
             _numberOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
    }

    _simpleCommands[ _numberOfSimpleCommands ] = simpleCommand;
    _numberOfSimpleCommands++;
}

void
Command:: clear()
{
    for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
        for ( int j = 0; j < _simpleCommands[ i ]->_numberOfArguments; j ++ ) {
            free ( _simpleCommands[ i ]->_arguments[ j ] );
        }

        free ( _simpleCommands[ i ]->_arguments );
        free ( _simpleCommands[ i ] );
    }

    if ( _outFile ) {
        free( _outFile );
    }

    if ( _inputFile ) {
        free( _inputFile );
    }

    if ( _errFile ) {
        free( _errFile );
    }

    _numberOfSimpleCommands = 0;
    _outFile = 0;
    _inputFile = 0;
    _errFile = 0;
    _background = 0;
}

void
Command::print()
{
    printf("\n\n");
    printf("              COMMAND TABLE                \n");
    printf("\n");
    printf("  #   Simple Commands\n");
    printf("  --- ----------------------------------------------------------\n");

    for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
        printf("  %-3d ", i );
        for ( int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++ ) {
            printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
        }
    }

    printf( "\n\n" );
    printf( "  Output       Input        Error        Background\n" );
    printf( "  ------------ ------------ ------------ ------------\n" );
    printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
        _inputFile?_inputFile:"default", _errFile?_errFile:"default",
        _background?"YES":"NO");
    printf( "\n\n" );

}
void printenv(char **environ){
    char **point = environ;


    int i=0;
    while (point[i]!= NULL){
        printf("%s\n",point[i]);
        i++;
    }

}

void
Command::execute()
{
    // Don't do anything if there are no simple commands
	
    
    if(strcmp(_simpleCommands[0]->_arguments[0],"exit")==0){
        exit (1);
    }
	





    int defaultin = dup( 0 );
    int defaultout = dup( 1 );
    int defaulterr = dup( 2 );
    int tmperr=dup(2);
    int ret;
    int tmpin=dup(0);
    int tmpout=dup(1);
    int fdin;
    if(_inputFile){
        fdin = open(_inputFile,O_RDONLY,S_IREAD);
    }
    else{
        fdin=dup(tmpin);
    }
    int fdout;
    int ferr;
    for(int i=0;i<_numberOfSimpleCommands; i++){
        dup2(fdin,0);
        close(fdin);
        if (i == _numberOfSimpleCommands-1){
            if(_outFile && appendage==true){
                fdout=open(_outFile,O_CREAT|O_WRONLY|O_APPEND,0664);
            }

            else if (_outFile){
                fdout=open(_outFile,O_CREAT|O_WRONLY|O_TRUNC,0664);

            }
            else{
                fdout=dup(tmpout);
            }
            if(_errFile && appendage==true){
                ferr =open(_errFile,O_CREAT|O_WRONLY|O_APPEND,0664);
            }

            else if (_errFile){
                ferr=open(_errFile,O_CREAT|O_WRONLY|O_TRUNC,0664);

            }
            else{
                ferr=dup(tmperr);
            }
        }
        else{
            int fdpipe[2];
            pipe(fdpipe);
            fdout=fdpipe[0];
            fdin=fdpipe[1];
        ferr=dup(tmperr);
        }
        dup2(fdout,1);
        close(fdout);
        dup2(ferr,2);
        close(ferr);


        if(strcmp(_simpleCommands[i]->_arguments[0],"setenv")==0){

            if(_simpleCommands[i]->_arguments[3]!=NULL){
                fprintf(stderr,"setenv: Too many arguments.\n");

            }
            else if (_simpleCommands[i]->_arguments[1]!=NULL && _simpleCommands[0]->_arguments[2]==NULL){
                putenv(_simpleCommands[i]->_arguments[1]);

            }
            else if(_simpleCommands[i]->_arguments[2] != NULL) {
                setenv(_simpleCommands[i]->_arguments[1],_simpleCommands[i]->_arguments[2],1);

            }

        }

        else if(strcmp(_simpleCommands[i]->_arguments[0],"unsetenv")==0){
            //if(_simpleCommands[i]->_arguments[1]==NULL){
            //    printf("unsetenv: Too few arguments.\n");
            //}
            //else{
                int count=1;
                while(_simpleCommands[i]->_arguments[count]!=NULL){

                    unsetenv(_simpleCommands[i]->_arguments[count]);
                    count++;
                }
        exit(1);
            //}


        }
        else if(strcmp(_simpleCommands[i]->_arguments[0],"cd")==0){
            //printf("first of all, i made it here\n");
            if(_simpleCommands[i]->_arguments[1]==NULL){
                //GO HOME!
                char* homeDirectory=getenv("HOME");
                chdir(homeDirectory);

            }
            else if(_simpleCommands[i]->_arguments[1]!=NULL && _simpleCommands[i]->_arguments[2]==NULL){
                            //printf("second of all, i made it here\n");
                int found=chdir(_simpleCommands[i]->_arguments[1]);
                if(found == -1){
                    fprintf(stderr,"%s: No such file or directory.\n", _simpleCommands[i]->_arguments[1]);

                }
            }
            else{
                fprintf(stderr,"cd: Too many arguments.\n");
            }


        }
        else{
        ret=fork();
        if(ret==0){
            //printenv(environ);
            if(strcmp(_simpleCommands[i]->_arguments[0],"printenv")==0){
                printenv(environ);
        exit(1);

            }
            else {
                execvp(_simpleCommands[i]->_arguments[0],_simpleCommands[i]->_arguments);
                perror("execvp");

                _exit(1);
            }
        }
            else if(ret<0){
            perror("fork");
            return;
        }
        }
    } //for
    dup2(tmpin,0);
    dup2(tmpout,1);
    dup2(tmperr,2);
    close(tmpin);
    close(tmpout);
    close(tmperr);
    if (!_background){
            waitpid(ret,NULL,0);
        }





    // Print contents of Command data structure
    //print();

    // Add execution here
    // For every simple command fork a new process
    // Setup i/o redirection
    // and call exec

    // Clear to prepare for next command
    clear();





    // Print new prompt

        prompt();

}

// Shell implementation

void
Command::prompt()
{
    if (isatty(0)){
        printf("myshell>");
        fflush(stdout);
    }
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

main()
{

    //printenv(environ);
    Command::_currentCommand.prompt();

	/*if (_numberOfSimpleCommands == 0 ) {
	//printf("Aint got nothing");
        Command::_currentCommand.prompt();
       // return;
    }*/



    sigignore(SIGINT);
    struct sigaction signalAction;
    signalAction.sa_handler= ZOMBIE_SLAYER;
    sigemptyset(&signalAction.sa_mask);
    signalAction.sa_flags=SA_RESTART;
    int error = sigaction(SIGCHLD,&signalAction,NULL);
    if(error){
        perror("sigaction");
        exit(-1);
    }
    yyparse();
}
