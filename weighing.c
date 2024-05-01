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
    
    //create node
	tWeighingNode* node = weighingList_createNode(weighing);
    
    //check if node is properly created
    if (node == NULL){
        return E_MEMORY_ERROR;
    }
    
    //check if node with the same date and code already exists. In case it does add the new node weight to it
    tWeighingNode* existingNode;
    existingNode = weighingList_findNode(*list, node->elem.code, node->elem.harvestDay);
    
    if(existingNode !=NULL){
        existingNode->elem.weight += node->elem.weight;
        return E_SUCCESS;
    }
    
    //add node as the first and last element if the list is empty
    if (list->first == NULL && list->last == NULL){
        list->first = node;
        list->last = node; 
        return E_SUCCESS;
    }
    
    //find the previous node where insertion of the new one must be done
    tWeighingNode* previousNode;
    previousNode = weighingList_findPrev(*list, node->elem.code, node->elem.harvestDay);
    
    //if pervious node is NULL, it means the node should be inserted in the first position of the list
    if (previousNode == NULL){
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    } else {
        //insert node in the middle of the list
        node->next = previousNode->next;
        if (previousNode->next != NULL) {
            previousNode->next->prev = node;
        } else {
            //insert node at the end of the list
            list->last = node;
        }
        previousNode->next = node;
        node->prev = previousNode;
    }
    
    return E_SUCCESS;
}

// Return the weight until the day received as a parameter according to a code
double weighingList_getWeight(tWeighingList list, const char* code, tDate day)
{
	// PR2 EX 1d
	double totalWeight;
    totalWeight = 0.0;
    
    tWeighingNode* currentNode;
    currentNode = list.first;
    
    while(currentNode != NULL){
        
        if (date_cmp(currentNode->elem.harvestDay, day) == -1){
            totalWeight = totalWeight + currentNode->elem.weight;
        }
        
        if (strcmp(currentNode->elem.code, code) == 0){
            totalWeight = totalWeight + currentNode->elem.weight;
        }
        
        
        currentNode = currentNode->next;
    }

    /////////////////////////////////
    return totalWeight;
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
    
    tWeighingNode* currentNode = list.first;
    
    while(currentNode != NULL){
        if ((date_cmp(currentNode->elem.harvestDay, harvestDay) == 0) && (strcmp(currentNode->elem.code, code) == 0)){
            return currentNode; 
        }
           
        currentNode = currentNode->next;
    }
    
    return NULL;
    
}

// Find the previous node to insert a new node by its harvest day and its code
tWeighingNode* weighingList_findPrev(tWeighingList list, const char* code, tDate harvestDay){
    
    // set as a currentNode the last element of the list

    tWeighingNode* currentNode = list.last;
    
    //if the weighing to add has a newest date than the current node, return the current node 
    while(currentNode!= NULL){
        if(date_cmp(harvestDay, currentNode->elem.harvestDay) == 1){
            return currentNode;
        }
        //if the weighing to add has the same date but smaller code, return the current node
        if((date_cmp(harvestDay, currentNode->elem.harvestDay) == 0) && (strcmp(code, currentNode->elem.code) > 0)){
            return currentNode;
        }
        currentNode = currentNode->prev;
    }
    
    //if none of the nodes in the list meets the previous requirements return NULL. 
    return NULL;

}
