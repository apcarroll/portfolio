
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"


Heap::Heap(int maxSize){
	this->maxSize=maxSize;
	heap=new ArrayDictionaryNode[maxNumber];
	last=0;
}

bool Heap::insert(KeyType key,DataType data){
	
	if(last==maxSize){
		return false;
	}
	heap[last].key=strdup(key);
	heap[last].data=data;
	
	last++;
	int child=last-1;
	int parent=((child-1)/2);
	while(child>0){
		if(strcmp(heap[child].key,heap[parent].key)<0){
			ArrayDictionaryNode tmp;
			tmp.key = strdup(heap[child].key);
			tmp.data = heap[child].data;
			heap[child].data=heap[parent].data;
			heap[child].key=strdup(heap[parent].key);
			heap[parent].key=strdup(tmp.key);
			heap[parent].data=tmp.data;
			child=parent;
		        parent=((child-1)/2);
		}
		else{
			break;
		}	
	}
	return true;
}

bool Heap::removeMin(KeyType &key,DataType &data){
	if(last==0){
		return false;
	}
	key=strdup(heap[0].key);
	data=heap[0].data;
	heap[0]=heap[last-1];
	last--;
	int parent=0;
	int left=((2*parent)+1);
	int right=((2*parent)+2);
	while(left<last){
		int minChild=left;
		if(right<last && strcmp(heap[right].key,heap[left].key)<0){
			minChild=right;
		}
		if(strcmp(heap[minChild].key,heap[parent].key)<0){
			ArrayDictionaryNode tmp;
			tmp.key=strdup(heap[minChild].key);
			tmp.data=heap[minChild].data;
			heap[minChild].key=strdup(heap[parent].key);
			heap[minChild].data=heap[parent].data;
			heap[parent].key=strdup(tmp.key);
			heap[parent].data=tmp.data;
			parent=minChild;
			left=((2*parent)+1);
			right=((2*parent)+2);
		}
		else{
			break;
		}
		return true;
	}



}





// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;/*
	array=new ArrayDictionaryNode[maxNumber];
	
	currentNumber=0;
	maxNumber=105;
	*/
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}
	
	// Use binary search
		
		if(currentNumber==0){
			return NULL;
		}

		if(currentNumber==1){
			return array[currentNumber].data;
		}

		//int mid=currentNumber/2;
		int high=currentNumber-1;
		int low=0;
		while(high>=low){
			int mid=(high + low)/2;
			int result = strcmp(key,array[mid].key);
			if(result == 0){
				return array[mid].data;	
			}
			if(result>0){
				low = mid + 1;
			}
			else{
				high = mid - 1;
			}
		}
	// Add your implementation here

	return NULL;
}


// Sort array using heap sort
void
BinarySearchDictionary::sort()
{
	/*printf("Before\n");
	for (int i = 0; i < currentNumber; i++) {
		printf("%s\n", array[i].key);
	}
	*/
        // Add your code here





	Heap heap(currentNumber);
	for(int i=0;i<currentNumber;i++){
		heap.insert(array[i].key,array[i].data);
	
	}
	for(int i=0;i<currentNumber;i++){
		heap.removeMin(array[i].key,array[i].data);

	}








	/*printf("After\n");
	for (int i = 0; i < currentNumber; i++) {
		printf("%s\n", array[i].key);
	}
	printf("------------------------");
*/
}




