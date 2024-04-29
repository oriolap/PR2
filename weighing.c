#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "error.h"
#include "weighing.h"

// Initialize a weighing
tApiError weighing_init(tWeighing* weighing, const char* code, float weight, tDate harvestDay) {
	
	// Preconditions
	assert(weighing != NULL);
	assert(code != NULL);
	
	// Allocate the memory for the code
	weighing->code = (char*)malloc(sizeof(char) * (strlen(code) + 1));
	
	if (weighing->code == NULL)
	{
		return E_MEMORY_ERROR;
	}
	
	// Assign all weighing properties
	strcpy(weighing->code, code);
	weighing->weight = weight;
	date_cpy(&(weighing->harvestDay), harvestDay);
	
	return E_SUCCESS;
}

// Copy a weighing from src to dst
tApiError weighing_cpy(tWeighing* dst, tWeighing src)
{
	// Preconditions
	assert(dst != NULL);
	
	// Initialize all values with init function
	return weighing_init(dst, src.code, src.weight, src.harvestDay);
}

// Release a weighing
void weighing_free(tWeighing* weighing)
{
	// Preconditions
	assert(weighing != NULL);
	
	// Release the memory allocated in init function
	if (weighing->code != NULL) {
		free(weighing->code);
		weighing->code = NULL;
	}
}

// Initialize a weighing list
tApiError weighingList_init(tWeighingList* list)
{

    assert(list != NULL);
	// PR2 EX 1a
	list->first = NULL;
    list->last = NULL;
    /////////////////////////////////
    
    return E_SUCCESS;
    
}

// Check if a weighing list is empty
bool weighingList_empty(tWeighingList list)
{
	// PR2 EX 1b
	if (list.first == NULL){
        return true;
    }
    /////////////////////////////////
    return false;
}

// Insert new weighing data
tApiError weighingList_add(tWeighingList* list, tWeighing weighing)
{
	// PR2 EX 1c
	tWeighingNode* node;
    node = weighingList_createNode(weighing);
    
    if (node == NULL){
        return E_MEMORY_ERROR;
    }
    
    if (list->first == NULL && list->last == NULL){
        list->first = node;
        list->last = node; 
        return E_SUCCESS;
    }
    
    tWeighingNode* previousNode;
    previousNode = weighingList_findPrev(*list, node->elem.code, node->elem.harvestDay);
    
    if (previousNode == NULL){
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    } else {
        // Insert in the middle or at the end
        node->next = previousNode->next;
        if (previousNode->next != NULL) {
            previousNode->next->prev = node;
        } else {
            // New node becomes the last node
            list->last = node;
        }
        previousNode->next = node;
        node->prev = previousNode;
    }
    
    /*if (previousNode != NULL){
        node->prev = previousNode;
        node->next = previousNode->next;
        previousNode->next->prev = node;
        previousNode->next = node;
        
    }*/
    /////////////////////////////////
    return E_SUCCESS;
}

// Return the weight until the day received as a parameter according to a code
double weighingList_getWeight(tWeighingList list, const char* code, tDate day)
{
	// PR2 EX 1d
	
    /////////////////////////////////
    return 0.0;
}

// Release the list
void weighingList_free(tWeighingList* list)
{
	// PR2 EX 1e
	
	////////////////
}


// Create a new node for weighing list
tWeighingNode* weighingList_createNode(tWeighing weighing)
{
    tWeighingNode* node;
    node = (tWeighingNode*)malloc(sizeof(tWeighingNode));
    if (node == NULL){
        return NULL;
    }
    weighing_cpy(&node->elem, weighing);
    node->prev = NULL;
    node->next = NULL;
    
    return node;

}

// Find a node by harvest date and code
tWeighingNode* weighingList_findNode(tWeighingList list, const char* code, tDate harvestDay){
    
}

// Find the previous node to insert a new node by its harvest day and its code
tWeighingNode* weighingList_findPrev(tWeighingList list, const char* code, tDate harvestDay){
    // set as a currentNode the first element of the list
    tWeighingNode* currentNode = list.first;
    
    //iterate through the list while the current node is not null, and it's not the last element of the list. 
    while(currentNode != NULL && currentNode->next != NULL){
        if (date_cmp(currentNode->elem.harvestDay, harvestDay) == -1 && (date_cmp(currentNode->next->elem.harvestDay, harvestDay) == 1)){
            return currentNode;
        }
        if (date_cmp(currentNode->elem.harvestDay, harvestDay) == 0 && (strcmp(currentNode->elem.code, code) > 0)){
            return currentNode; 
        }
        currentNode = currentNode->next;
    }

    //in case the new node has the newest date, return the last node of the list.      
    if(date_cmp(list.last->elem.harvestDay, harvestDay) == -1){
        currentNode = list.last;
        return currentNode; 
    }
    
    return NULL;
   
}
