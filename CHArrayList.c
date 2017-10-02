
#include "CHArrayList.h"


SPArrayList* spArrayListCreate(int maxSize){
	if (maxSize <= 0) //checks if the size is valid
		return NULL;
	SPArrayList* arr = (SPArrayList*) malloc(sizeof(SPArrayList)); //allocates space for the arraylist
	if(!arr) //checks if allocation is fine
		return NULL;
	elem** element = (elem**) malloc(maxSize*(sizeof(elem*)));
	if(!element){ //checks allocation
		free(arr);
		return NULL;
	}
	for(int i=0;i<maxSize;i++){
		element[i] = (elem*) malloc(sizeof(elem));
		if(!element[i]){
			for(int j=0;j<i;j++)
				free(element[j]);
			free(element);
			free(arr);
			return NULL;
		}
	}
	arr->maxSize = maxSize;
	arr->elements = element;
	arr->actualSize = 0;
	return arr;
}


SPArrayList* spArrayListCopy(SPArrayList* src){
	if(src == NULL) //checks allocation
		return NULL;
	SPArrayList* arr = spArrayListCreate(src->maxSize); //creates new arraylist
	if(!arr) //checks allocation
		return NULL;
	arr->actualSize = src->actualSize;
	copyParams(src,arr);
	return arr;
}


void copyParams(SPArrayList* src,SPArrayList* arr){
	for (int i=0;i<src->actualSize;i++){ //copies elements - each element represents one move
		arr->elements[i]->dstCol=src->elements[i]->dstCol;
		arr->elements[i]->dstRow=src->elements[i]->dstRow;
		arr->elements[i]->startCol=src->elements[i]->startCol;
		arr->elements[i]->startRow=src->elements[i]->startRow;
		arr->elements[i]->fig=src->elements[i]->fig;
		arr->elements[i]->prevFig=src->elements[i]->prevFig;
		arr->elements[i]->gameStatus=src->elements[i]->gameStatus;
		arr->elements[i]->player=src->elements[i]->player;
	}
}

void spArrayListDestroy(SPArrayList* src){
	if(src != NULL){  // destroying struct if needed
		for(int i=0; i<src->maxSize;i++)
			free(src->elements[i]);
		free(src->elements);
		free(src);
	}
}


SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if(!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize=0; //changes the actual size of the array - lazy clean
	return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, elem* element, int index){ //gets index between zero to actualSize-1
	if(!src || (index<0 || index>(src->maxSize)-1) || index>(src->actualSize)) //checks validity of the index
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(src->actualSize == src->maxSize) //checks if the array is full
		return SP_ARRAY_LIST_FULL;
    for (int i = src->actualSize; i > index ; --i) { //copies the elements
    	src->elements[i]->dstCol=src->elements[i-1]->dstCol;
    	src->elements[i]->dstRow=src->elements[i-1]->dstRow;
   		src->elements[i]->startCol=src->elements[i-1]->startCol;
    	src->elements[i]->startRow=src->elements[i-1]->startRow;
    	src->elements[i]->fig=src->elements[i-1]->fig;
    	src->elements[i]->prevFig=src->elements[i-1]->prevFig;
    	src->elements[i]->gameStatus=src->elements[i-1]->gameStatus;
    	src->elements[i]->player=src->elements[i-1]->player;
    }
    src->elements[index]->dstCol=element->dstCol; //inserts the element
    src->elements[index]->dstRow=element->dstRow;
  	src->elements[index]->startCol=element->startCol;
   	src->elements[index]->startRow=element->startRow;
   	src->elements[index]->fig=element->fig;
   	src->elements[index]->prevFig=element->prevFig;
   	src->elements[index]->gameStatus=element->gameStatus;
   	src->elements[index]->player=element->player;
	src->actualSize = src->actualSize + 1;
	return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, elem* element){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListAddAt(src,element,0);
}


SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, elem* element){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(spArrayListIsFull(src))
			return SP_ARRAY_LIST_FULL;
	return spArrayListAddAt(src,element,src->actualSize);
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){ //gets index between zero to actualSize-1
	if(!src || (index<0 || index>(src->actualSize)-1)) //checks validity of the index
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(src->actualSize == 0) //if the array is empty
		return SP_ARRAY_LIST_EMPTY;
    for (int i = index; i < src->actualSize-1; ++i) {//copies elements
    	src->elements[i]->dstCol=src->elements[i+1]->dstCol;
    	src->elements[i]->dstRow=src->elements[i+1]->dstRow;
   		src->elements[i]->startCol=src->elements[i+1]->startCol;
    	src->elements[i]->startRow=src->elements[i+1]->startRow;
    	src->elements[i]->fig=src->elements[i+1]->fig;
    	src->elements[i]->prevFig=src->elements[i+1]->prevFig;
    	src->elements[i]->gameStatus=src->elements[i+1]->gameStatus;
    	src->elements[i]->player=src->elements[i+1]->player;
    }
	src->actualSize = src->actualSize - 1;
	return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(spArrayListIsEmpty(src))
		return SP_ARRAY_LIST_EMPTY;
	return spArrayListRemoveAt(src,0);
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(spArrayListIsEmpty(src))
		return SP_ARRAY_LIST_EMPTY;
	return spArrayListRemoveAt(src,(src->actualSize)-1);
}


elem* spArrayListGetAt(SPArrayList* src, int index){
	if (index<0 || index>src->actualSize-1 || (!src)) //checks validity of the input
		return NULL;
	else
		return src->elements[index];
}


elem* spArrayListGetFirst(SPArrayList* src){
	if (!src)
		return NULL;
	return spArrayListGetAt(src,0);
}


elem* spArrayListGetLast(SPArrayList* src){
	if (!src)
		return NULL;
	return spArrayListGetAt(src,(src->actualSize)-1);
}


int spArrayListMaxCapacity(SPArrayList* src){
	if (!src)
		return -1;
	return src->maxSize;
}


int spArrayListSize(SPArrayList* src){
	if (!src)
		return -1;
	return src->actualSize;
}


bool spArrayListIsFull(SPArrayList* src){
	if ((!src) || ((src->actualSize)<(src->maxSize)))
		return false;
	return true;
}


bool spArrayListIsEmpty(SPArrayList* src){
	if ((!src) || ((src->actualSize)!=0))
		return false;
	return true;
}
