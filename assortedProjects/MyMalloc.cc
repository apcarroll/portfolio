//
// CS354: MyMalloc Project
//
// The current implementation gets memory from the OS
// every time memory is requested and never frees memory.
//
// You will implement the allocator as indicated in the handout.
//
// Also you will need to add the necessary locking mechanisms to
// support multi-threaded programs.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <thread.h>

mutex_t lock;


//take the free object header and then cast it as an object header and then return it the way the others did



/*
this is how you use GDB
lore 7 % setenv LD_LIBRARY_PATH "."
lore 8 % gdb test1



*/























struct freeObjectHeader{
     int _flags;            
  size_t _objectSize;
    freeObjectHeader* next;
    freeObjectHeader* prev;
};




freeObjectHeader * freelist[65]={NULL};




enum {
  ObjFree = 0,
  ObjAllocated = 1
};

// Header of an object. Used both when the object is allocated and freed
class ObjectHeader {
 public:
  int _flags;              // flags == ObjFree or flags = ObjAllocated
  size_t _objectSize;         // Size of the object. Used both when allocated
                  // and freed.
};




  








class Allocator {
  // State of the allocator

  // Size of the heap
  size_t _heapSize;

  // True if heap has been initialized
  int _initialized;

  // Verbose mode
  int _verbose;

  // # malloc calls
  int _mallocCalls;

  // # free calls
  int _freeCalls;

  // # realloc calls
  int _reallocCalls;
 
  // # realloc calls
  int _callocCalls;
 
public:
  // This is the only instance of the allocator.
  static Allocator TheAllocator;

  //Initializes the heap
  void initialize();

  // Allocates an object
  void * allocateObject( size_t size );

  // Frees an object
  void freeObject( void * ptr );

  // Returns the size of an object
  size_t objectSize( void * ptr );

  // At exit handler
  void atExitHandler();

  //Prints the heap size and other information about the allocator
  void print();

  // Gets memory from the OS
  void * getMemoryFromOS( size_t size );

  void increaseMallocCalls() { _mallocCalls++; }

  void increaseReallocCalls() { _reallocCalls++; }

  void increaseCallocCalls() { _callocCalls++; }

  void increaseFreeCalls() { _freeCalls++; }

};

Allocator Allocator::TheAllocator;

extern "C" void
atExitHandlerInC()
{
  Allocator::TheAllocator.atExitHandler();
}

void
Allocator::initialize()
{
  // Environment var VERBOSE prints stats at end and turns on debugging
  // Default is on
  _verbose = 1;

  const char * envverbose = getenv( "MALLOCVERBOSE" );
  if ( envverbose && !strcmp( envverbose, "NO") ) {
    _verbose = 0;
  }
//	mutex_init(&lock,USYNC_THREAD,NULL);
  // In verbose mode register also printing statistics at exit
  atexit( atExitHandlerInC );

  _initialized = 1;
}







void *
Allocator::allocateObject( size_t size )
{

  //Make sure that allocator is initialized
  if ( !_initialized ) {
    initialize();
  }
 // int ret = mutex_lock(&lock);
//	printf("Allocate mutex lock ret = %d \n", ret);

  bool isIn=false;   
  // Add the ObjectHeader to the size and round the total size up to a
  // multiple of 8 bytes for alignment.
  size_t totalSize = (size + 2* sizeof(ObjectHeader) + 7) & ~7;
  int list = totalSize >= 512? 64:((totalSize)>>3);//one of the 65 lists
    int i;
    freeObjectHeader * tmp;
	if(freelist[list]!=NULL){	
		if(freelist[list]->_objectSize==size){
			//return the freelist object
			freelist[list]->_flags=ObjAllocated;
			freelist[list]->_objectSize=totalSize;
			Allocator::freeObject(freelist[list]);
			return (void*)((ObjectHeader*)freelist[list]+1);
		}
	}
    for( i=list;i<65;i++){
        if(freelist[i]!=NULL){
            isIn=true;  
            //tmp = freelist[i];
			Allocator::freeObject(freelist[i]);
			return (void*)freelist[i];
            break;
        }
       
    }
	if(isIn==false){
		
	}
    if(isIn==true){
        if(freelist[list]!=NULL){

            if(i != list){
                    //split into s+size ofheader+size of footer goes to user
                //second chunk is just what remains  goes to freelist
                //freelist[i].next=size+
				
				
            }
          
            //update redirect the pointers to get the node out
             void * mem = freelist[list];
                //do the other stuff
          
          
              ObjectHeader * o = (ObjectHeader *) mem;//first pointer int the list if it doesnt fit send it to next
              o->_objectSize = totalSize;
              o->_flags = ObjAllocated;
            freeObjectHeader * temp=(freeObjectHeader*)o;
             if(o->_objectSize != totalSize){
                temp=temp -> next;
                while(freelist[list]->_objectSize!=totalSize){
                    list++;
                  
                }
                if(list==65){
                //split into s+size ofheader+size of footer goes to user
                //second chunk is just what remains  goes to freelist
                     void * mem = getMemoryFromOS( 16384);
                //do the other stuff
              ObjectHeader * o = (ObjectHeader *) mem;
				o->_flags=42;
					((ObjectHeader*)((char*)o + 16384 - sizeof(ObjectHeader)))->_flags = 42;
//				(ObjectHeader*)((char*)o+(16384-sizeof(ObjectHeader)))->_flags=42;
              o->_objectSize = totalSize;
              o->_flags = ObjAllocated;
		
                    temp=temp -> next;
			  int remainder = 16384- totalSize;
			  freeObjectHeader * sendBack=(freeObjectHeader*)((char *)mem+totalSize);
			  sendBack->_objectSize=remainder-sizeof(ObjectHeader);
			  sendBack->_flags=ObjFree;
			  Allocator::freeObject(sendBack);
              return (void *) (o + 1);
                }
            }
            return (void *)(temp + 1);            
            // return (void *) (o + 1);
             //return freelist[list].next;
        }
        else{
          
        }
    }
    else{
  // You should get memory from the OS only if the memory in the free list could not
  // satisfy the request.
     if(totalSize>16384){
  // Simple allocator always gets memory from the OS.
             void * mem = getMemoryFromOS( totalSize );

              // Get a pointer to the object header
            //void * mem = freelist[list];
              ObjectHeader * o = (ObjectHeader *) mem;

              // Store the totalSize. We will need it in realloc() and in free()
              o->_objectSize = totalSize;

              // Set object as allocated
              o->_flags = ObjAllocated;

              // Return the pointer after the object header.
              return (void *) ((char*)mem + sizeof(ObjectHeader));
            }
     else{
             void * mem = getMemoryFromOS( 16384);
                //do the other stuff
			  int keep=16384-totalSize;
			  
              ObjectHeader * o = (ObjectHeader *) mem;
              o->_objectSize = totalSize;
              o->_flags = ObjAllocated;	 
			  ObjectHeader * giveAway=(ObjectHeader *)((char*)o+totalSize);
			  freelist[64]=(freeObjectHeader*)giveAway;
			  Allocator::freeObject(giveAway);
              return (void *) (o + 1);
        }
    }
 //mutex_unlock(&lock);

}


































void
Allocator::freeObject( void * ptr )
{
  // Here you will return the object to the free list sorted by address and you will coalesce it
  // if possible.

//mutex_lock(&lock);
bool coalAbove=false;
bool coalBelow=false;


//printf("\nThe object size is %d\n",test->_objectSize);
//use gdb to figure out the pointers

    ObjectHeader * realHead=(ObjectHeader*)(((char *)ptr)-sizeof(ObjectHeader)); //this is the correct thing do not change it pls 
	if(realHead->_flags==ObjFree){
		return;
	} 
	ObjectHeader* upperFooter=(ObjectHeader*)(((char*)ptr)-2*sizeof(ObjectHeader));
	ObjectHeader* lowerHeader=(ObjectHeader*)(((char*)realHead)+(2*sizeof(ObjectHeader))+realHead->_objectSize);
  
    if (upperFooter->_flags==0 && upperFooter->_objectSize >=0 ){
        coalAbove=true;
    }
	if(lowerHeader->_flags==0 && upperFooter->_objectSize >=0 ){
		coalBelow=true;
	}
   
    int* sizeAddr=(int*)ptr-1;
    int size=*sizeAddr;
    int* flagaddr=(int*)ptr-2;
    int flag=*flagaddr;
    int index = size >= 512? 64:((size)>>3);
   // void* ptr2=freelist[index];
  
	if(coalAbove==true && coalBelow==true){
		size=size+upperFooter->_objectSize+lowerHeader->_objectSize;
		  
	   upperFooter->_objectSize=size;
		     freelist[index]=(freeObjectHeader*)ptr;
        freelist[index]->_objectSize=size;
        //freelist[index]->_flags=ObjAllocated; this is what i had before
        freelist[index]->_flags=ObjFree;       
        freelist[index]->next=NULL;
		
	}
    if(coalAbove==true){
       size+=upperFooter->_objectSize;
	   upperFooter->_flags==1;
	   upperFooter->_objectSize=size;
		     freelist[index]=(freeObjectHeader*)ptr;
        freelist[index]->_objectSize=size;
        //freelist[index]->_flags=ObjAllocated; this is what i had before
        freelist[index]->_flags=ObjFree;       
        freelist[index]->next=NULL;	
    }
	if(coalBelow==true){
		size+=lowerHeader->_objectSize;
		     freelist[index]=(freeObjectHeader*)ptr;
        freelist[index]->_objectSize=size;
        //freelist[index]->_flags=ObjAllocated; this is what i had before
        freelist[index]->_flags=ObjFree;       
        freelist[index]->next=NULL;
		
	}


// Here is my attempt to coalesce

   
//    ObjectHeader * higherFooter=myHead-1;
    //higherFooter=(ObjectHeader*)head-2;
    /*
    int* sizeAboveIndex=head-1;
    int* higherFooterIndex=head-2;
    int sizeAbove=*sizeAboveIndex;
    int higherFooter=*higherFooterIndex;
      */ 




    if(freelist[index]==NULL){//attention point
        freelist[index]=(freeObjectHeader*)ptr;
        freelist[index]->_objectSize=size;
        //freelist[index]->_flags=ObjAllocated; this is what i had before
        freelist[index]->_flags=ObjFree;       
        freelist[index]->next=NULL;
  
    }
    else{

    freeObjectHeader * adder=(freeObjectHeader*)ptr;
    adder->_objectSize=size;
    adder->_flags=ObjAllocated;
    adder->prev=NULL;  
    adder->next=freelist[index];      







    }





//mutex_unlock(&lock);




}

size_t
Allocator::objectSize( void * ptr )
{
  // Return the size of the object pointed by ptr. We assume that ptr is a valid obejct.
  ObjectHeader * o =
    (ObjectHeader *) ( (char *) ptr - sizeof(ObjectHeader) );

  // Substract the size of the header
  return o->_objectSize - sizeof(ObjectHeader);
}

void
Allocator::print()
{
  printf("\n-------------------\n");

  printf("HeapSize:\t%d bytes\n", _heapSize );
  printf("# mallocs:\t%d\n", _mallocCalls );
  printf("# reallocs:\t%d\n", _reallocCalls );
  printf("# callocs:\t%d\n", _callocCalls );
  printf("# frees:\t%d\n", _freeCalls );

  printf("\n-------------------\n");
}

void *
Allocator::getMemoryFromOS( size_t size )
{
  // Use sbrk() to get memory from OS
  _heapSize += size;
 
  return sbrk( size );
}

void
Allocator::atExitHandler()
{
  // Print statistics when exit
  if ( _verbose ) {
    print();
  }
}

//
// C interface
//

extern "C" void *
malloc(size_t size)
{
  Allocator::TheAllocator.increaseMallocCalls();
 
  return Allocator::TheAllocator.allocateObject( size );
}

extern "C" void
free(void *ptr)
{
  Allocator::TheAllocator.increaseFreeCalls();
 
  if ( ptr == 0 ) {
    // No object to free
    return;
  }
 
  Allocator::TheAllocator.freeObject( ptr );
}

extern "C" void *
realloc(void *ptr, size_t size)
{
  Allocator::TheAllocator.increaseReallocCalls();
  
  // Allocate new object
  void * newptr = Allocator::TheAllocator.allocateObject( size );

  // Copy old object only if ptr != 0
  if ( ptr != 0 ) {
  
    // copy only the minimum number of bytes
    size_t sizeToCopy =  Allocator::TheAllocator.objectSize( ptr );
    if ( sizeToCopy > size ) {
      sizeToCopy = size;
    }
  
    memcpy( newptr, ptr, sizeToCopy );

    //Free old object
    Allocator::TheAllocator.freeObject( ptr );
  }

  return newptr;
}

extern "C" void *
calloc(size_t nelem, size_t elsize)
{
  Allocator::TheAllocator.increaseCallocCalls();
  
  // calloc allocates and initializes
  size_t size = nelem * elsize;

  void * ptr = Allocator::TheAllocator.allocateObject( size );

  if ( ptr ) {
    // No error
    // Initialize chunk with 0s
    memset( ptr, 0, size );
  }

  return ptr;
}

extern "C" void
checkHeap()
{
    // Verifies the heap consistency by iterating over all objects
    // in the free lists and checking that the next, previous pointers
    // size, and boundary tags make sense.
    // The checks are done by calling assert( expr ), where "expr"
    // is a condition that should be always true for an object.
    //
    // assert will print the file and line number and abort
    // if the expression "expr" is false.
    //
    // checkHeap() is required for your project and also it will be
    // useful for debugging.
}
