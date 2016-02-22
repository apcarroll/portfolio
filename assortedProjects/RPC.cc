//
// CS354: Implementaiton of the interprocess RPC
//
#include <iostream>
#include "RPC.h"
#include <stdio.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <thread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;
RPCServer::RPCServer( char * sharedFileName, int maxAvailableThreads )
{
	// 1. Create a file sharedFileName for RPCShared of the size we need for maxAvailableThreads
	
	int fileDes=open(sharedFileName, O_RDWR|O_CREAT|O_TRUNC,0660);
	
	int sizeShared= sizeof(RPCShared)+(maxAvailableThreads-1)*sizeof(RPCThread);
	lseek(fileDes,sizeShared,SEEK_SET);
	char a=0;
	write(fileDes,&a,1);
	_shared=(RPCShared*)mmap(NULL, sizeShared,PROT_READ|PROT_WRITE,MAP_SHARED,fileDes,0);
_shared->_maxAvailableThreads=maxAvailableThreads;
	// 3. Initialze semaphores and mutex. Use the USYNC_PROCESS flags since semaphores and mutexs
	//    will be used to synchroinize threads across processes.
	// 
	sema_init(&_shared-> _sem_available_threads, maxAvailableThreads,USYNC_PROCESS,NULL);
	mutex_init(&_shared->_mutex,USYNC_PROCESS,NULL);
		
	


	
	int i=0;
	while(i<maxAvailableThreads){

		_shared->_rpcThread[i]._in_use=false;
		sema_init(&_shared->_rpcThread[i]._sem_request,0,USYNC_PROCESS,NULL);
		sema_init(&_shared->_rpcThread[i]._sem_result,0,USYNC_PROCESS,NULL);
		i++;
	}
	
}

void
RPCServer::registerRPC( char * name, RPCProcedure rpcProcedure, void * cookie )
{
	// Add an entry to the _rpcTable with name, rpcProcedure and cookie.
	RPCTableEntry  entry;
	strcpy(entry._rpcName,name);
	entry._rpcProcedure=rpcProcedure;
	entry._cookie=cookie;

	_rpcTable[name]=entry;

}

void
RPCServer::runServer()
{
	// 1. For each maxAvailableThreads-1 call pthread_create to
	// start a server thread calling threadServer  with the corresponding index 

	ThreadServerArgs * args;
	//pthread_t threads[_shared->_maxAvailableThreads];
	for(int T=0;T<_shared->_maxAvailableThreads;T++){

		args =new ThreadServerArgs; 
		args->s=this;
		args->threadIndex = T;

		 pthread_create(0, NULL, (void*(*)(void*))threadServer, (void *)args);

	}
	// 2. then call threadServer
	args = new ThreadServerArgs;
	args->s=this;
	args->threadIndex = _shared->_maxAvailableThreads;

	threadServer( args );
	delete args;
}


void 
RPCServer::threadServer( ThreadServerArgs * args ) 
{
	while (1) {
		// 1. wait for an incoming request for this thread
		sema_wait(&args->s->_shared->_rpcThread[args->threadIndex]._sem_request);		
		// 2. lookup the procedure in the rpctable
		//cookie,argument,result
		RPCProcedure prods = args->s->_rpcTable[args->s->_shared->_rpcThread[args->threadIndex]._rpcName]._rpcProcedure;
		prods(args->s->_rpcTable[args->s->_shared->_rpcThread[args->threadIndex]._rpcName]._cookie,args->s->_shared->_rpcThread[args->threadIndex]._argument,args->s->_shared->_rpcThread[args->threadIndex]._result);
		// 3. Invoke procedure through a pointer
		


		// 4. sema_post that the call is complete. 
		sema_post(&args->s->_shared->_rpcThread[args->threadIndex]._sem_result);		
	}
}

RPCClient::RPCClient( char * sharedFileName )
{

	
	int fd=open(sharedFileName,O_RDWR,0660);	
	int sizeShared = lseek(fd, 0, SEEK_END);
	_shared = (RPCShared *)mmap(NULL, sizeShared, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	

}

int
RPCClient::call( char * rpcName, RPCArgument argument, RPCResult result )
{

	// 1. Wait if there are no threads available
	sema_wait(&_shared-> _sem_available_threads);
 	// 2. Get mutex
	mutex_lock(&_shared->_mutex);
	// 3. Find the index of an available thread    find one that is not inuse
	int j;	
	for( j=0;j<_shared->_maxAvailableThreads;j++){
		if(_shared->_rpcThread[j]._in_use==false){
			break;
		}
	}
	_shared->_rpcThread[j]._in_use = true;
	// 4. release mutex
		mutex_unlock(&_shared->_mutex);
	// 4. Copy argument into the RPCThread record aka the rpctable for that index
	
	strcpy(_shared->_rpcThread[j]._rpcName, rpcName);	
	memcpy(_shared->_rpcThread[j]._argument,argument, sizeof(RPCArgument));
	
	
	// 5. Wake up that server thread  `
	sema_post(&_shared->_rpcThread[j]._sem_request);
	// 6.. wait until results are ready.
	sema_wait(&_shared->_rpcThread[j]._sem_result);
	memcpy(result, _shared->_rpcThread[j]._result, sizeof(RPCResult));
	sema_post(&_shared-> _sem_available_threads);
	_shared->_rpcThread[j]._in_use = false;
	// 7. return
  	return 0;
}

int 
RPCClient::maxSimultaneousCalls()
{
}

int
RPCClient::totalCalls()
{
}




