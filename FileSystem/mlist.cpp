#include <iostream>
#include "mlist.h"
using namespace std;
//------------------------------------------------------------------------------
//IMPORTANT: You are not allowed to remove or add any parameters to any functions.
//NOTE: There can only be at most 2 loops in this file
//------------------------------------------------------------------------------

//Constructor, construct an empty doubly linked list.
MList::MList()
{
	ntop = NULL;
	nbottom = NULL;
}

//Destructor, properly clears and deallocates everything of current list.
//simply call the clear function if you have already implemented that.
MList::~MList()
{	
	clear();
}

//Note: RECURSION --- This function should be implemented using recursion.
//this function properly clears and deallocates everything of current list.
//there should be no loop in this function
void MList::clear()
{
	if (!isEmpty()) {
		Node* temp = ntop;
		if (ntop->next != NULL) {
			ntop = ntop->next;
			delete temp;
			clear();
		} else {
			delete ntop;
            ntop = NULL;
            nbottom = NULL;
		}
	}
}

//returns a boolean true if the list is empty, false if otherwise.
bool MList::isEmpty()
{   
	if (ntop == NULL && nbottom == NULL) return true;
	return false;
}

/*
	Add or insert a new node with d_item as its data at the top of the list.
	You need to dynamically allocate memory for the new node.
*/
void MList::push_top(Data* d_item)
{ 
	Node* temp = new Node;
	temp->nodeData = d_item;
	temp->prev = NULL;

	if (isEmpty()) {
		ntop = temp;
		nbottom = temp;
		temp->next = NULL;
	} else {
		ntop->prev = temp;
		temp->next = ntop;
		ntop = temp;
	}
}

/*
	Add or insert a new node with d_item as its data at the bottom of the list.
	You need to dynamically allocate memory for the new node.
*/
void MList::push_bottom(Data* d_item)
{
	Node* temp = new Node;
	temp->nodeData = d_item;
	temp->next = NULL;

	if (isEmpty()) {
		ntop = temp;
		nbottom = temp;
		temp->prev = NULL;
	} else {
		nbottom->next = temp;
		temp->prev = nbottom;
		nbottom = temp;
	}
}

/*
	Delete or remove the top node of the list.
	Once pop, you need to properly deallocate the memory for the node (not the data).
	If your list is currently empty, you don't have to do anything.
*/
void MList::pop_top()
{ 
	if (!isEmpty()) {
		Node* temp;
		if (ntop->next == NULL) {
			delete ntop;
			ntop = NULL;
			nbottom = NULL;
		} else {
			temp = ntop->next;
			temp->prev = NULL;
			delete ntop;
			ntop = temp;
		}
	}
}

/*
	Delete or remove the bottom node of the list.
	Once pop, you need to properly deallocate the memory for the node (not the data).
	If your list is currently empty, you don't have to do anything.
*/
void MList::pop_bottom()
{
	if (!isEmpty()) {
		Node* temp;
		if (ntop->next == NULL) {
			delete nbottom;
			ntop = NULL;
			nbottom = NULL;
		} else {
			temp = nbottom->prev;
			delete nbottom;
			nbottom = temp;
			nbottom->next = NULL;
		}
	}	
}

/*
	Note: RECURSION --- This function should be implemented using recursion.
	Search a node in the list that has data with the same name as the parameter.
	If one is found, return the pointer to the node.
	If one is not found, return NULL.
*/
Node* MList::search(Node* start, string name)
{
	if (start == NULL) return start;
	if (start->nodeData->name == name) return start;
	return search(start->next, name);
}

//Swapping node a with node b.
void MList::swapNode(Node* a, Node* b)
{	
	if (a == b) return;
	Node* temp;
	if (ntop == b) { //fubar
		temp = a;
		a = b;
		b = temp;
	} else if (nbottom == a) { //""
		temp = b;
		b = a;
		a = temp;
	}
	if (ntop == a && nbottom == b && a->next == b) {
		ntop = b;
		b->next = a;
		a->prev = b;
		nbottom = a;
		b->prev = NULL;
		a->next = NULL;
	} else if (ntop != a && nbottom == b && a->next != b) {
		temp = a->prev;
		temp->next = b;
		b->next = a->next;
		b->prev->next = a;
		a->next->prev = b;
		a->prev = b->prev;
		b->prev = temp;
		nbottom = a;
		a->next = NULL;
	} else if (ntop != a && nbottom == b && a->next == b) {
		temp = a->prev;
		temp->next = b;
		b->next = a;
		a->prev = b;
		b->prev = temp;
		a->next = NULL;
		nbottom = a;
	} else if (ntop == a && nbottom != b && a->next != b) {
		temp = b->next;
		b->next = a->next;
		temp->prev = a;
		a->next = temp;
		a->prev = b->prev;
		a->prev->next = a;
		b->next->prev = b;
		ntop = b;
		b->prev = NULL;
	} else if (ntop == a && nbottom != b && a->next == b) {
		temp = b->next;
		ntop = b;
		b->next = a;
		a->next = temp;
		temp->prev = a;
		a->prev = b;
		b->prev = NULL;
	} else if (a->next == b) {
		temp = a->prev;
		a->prev = b;
		a->next = b->next;
		a->next->prev = a;
		b->next = a;
		b->prev = temp;
		temp->next = b;
	} else if (ntop == a && nbottom == b && a->next != NULL) {
		b->next = a->next;
		a->prev = b->prev;
		b->next->prev = b;
		a->prev->next = a;
		ntop = b;
		nbottom = a;
		b->prev = NULL;
	} else if (a->prev != NULL && b->next != NULL) {
		temp =  b->next;
		a->prev->next = b;
		b->next->prev = a;
		a->next->prev = b;
		b->prev->next = a;
		b->next = a->next;
		temp->prev = a;
		a->next = temp;
		temp = a->prev;
		a->prev = b->prev;
		b->prev = temp;
	}
}

/*
	This function PERMANENTLY deletes a node from the list given a pointer to that node.
	You can assume that the node exists in the list - no need to search for the node.
*/
void MList::deleteNode(Node* a)
{   
	Node* temp = a;
	if (ntop == nbottom) {
		ntop = NULL;
		nbottom = NULL;
	} else if (ntop == a) {
		ntop = ntop->next;
		ntop->prev = NULL;
	} else if (nbottom == a) {
		nbottom = nbottom->prev;
		nbottom->next = NULL;
	} else {
		a->next->prev = a->prev;
		a->prev->next = a->next;
		nbottom->next = NULL;
	}
	delete temp;
}

/*
	Unlike the delete function above, this function does NOT permanently delete the node.
	What you need to do at the last step is to set the node's prev and next pointers to NULL.
	Again, you can assume that the node exists in the list - no need to search for the node.
	Note: this function will be useful
		when we want to insert the given node later at another position after you remove it from the list.
*/
void MList::removeNode(Node* a)
{
	Node* temp = a;
	if (ntop == nbottom) {
		ntop = NULL;
		nbottom = NULL;
	} else if (ntop == a) {
		ntop = ntop->next;
		ntop->prev = NULL;
	} else if (nbottom == a) {
		nbottom = nbottom->prev;
		nbottom->next = NULL;
	} else {
		a->next->prev = a->prev;
		a->prev->next = a->next;
		nbottom->next = NULL;
	}
	temp->next = NULL;
	temp->prev = NULL;
}

/*
	Insert node a after node b.
	Note: b can be NULL, Q: what does that mean?
	otherwise, you can assume that b exists in the list.
*/
void MList::insertAfter(Node *a, Node* b)
{
    if (a == NULL) return;
	else if (isEmpty()) {
		ntop = a;
		nbottom = a;
        a->next = NULL;
        a->prev = NULL;
	} else if (b == NULL) {
		a->next = ntop;
		ntop->prev = a;
		ntop = a;
        a->prev = NULL;
	} else if (b->next == NULL) {
		b->next = a;
		a->prev = b;
		nbottom = a;
        a->next = NULL;
	} else if (b->next != NULL) {
		a->next = b->next;
		a->prev = b;
		a->next->prev = a;
		b->next = a;
	}
}

/*
	Note: RECURSION --- This function should be implemented using recursion.
	Implement a SELECTION SORT algorithm using recursion.
	The function takes in a start node.
	Things to keep in mind:
		1). sort should NOT be run if the list is empty.
		2). if mode = true, sort by name in alphabetical order
			(i.e., A-Za-z) based on ASCII number if mode = true
		3). if mode = false, sort by size in ascending order (low->high)
	You can search any online resources available to you (i.e. search 'selection sort')
	to understand the basic idea behind the selection sort algorithm.
	Note: 
		1). there can only be at most one loop in this function
		2). this function is private
			see sortByNameSelection and sortBySizeSelection for the calls to this function
*/
void MList::sortSelection(Node* start, bool mode)
{
	if (!isEmpty() && start != NULL) {
		Node* temp = start;
		Node* swap = start;
		while (temp != NULL) {
			if (mode && swap->nodeData->name > temp->nodeData->name) {
				swap = temp;
			} else if (!mode && swap->nodeData->size > temp->nodeData->size) {
				swap = temp;
			}
			temp = temp->next;
		}
		swapNode(start, swap);
		sortSelection(swap->next, mode);
	}

}

/*
	Note: RECURSION --- This function should be implemented using recursion.
	Implement an INSERTION SORT algorithm using recursion.
	The function takes in a start node.
	Things to keep in mind:
		1). sort should NOT be run if the list is empty.
		2). we are trying to sort by name in alphabetical order
			(i.e., A-Za-z) based on ASCII number.
	You can search any online resources available to you (i.e. search 'insertion sort')
	to understand the basic idea behind the insertion sort algorithm.
	The gif from the wikipedia page of this algorithm is the easiest to understand in my opinion
	Link: https://en.wikipedia.org/wiki/Insertion_sort
	Note: 
		1). there can only be at most one loop in this function
		2). this function is private, see sortByNameInsertion for the call to this function
*/
void MList::sortInsertion(Node* start)
{
	if (isEmpty() || ntop == nbottom || start == NULL) return;
	Node* temp;
	if (start != nbottom) temp = start->next;
	else temp = start;
	while (temp != NULL && temp->prev != NULL) {
		Node* swap = temp->prev;
		if (temp->nodeData->name < swap->nodeData->name) {
			swapNode(swap, temp);
			swap = temp->prev;
		} else temp = temp->prev;
	}
	sortInsertion(start->next);
}

/*
	Note: RECURSION --- This function should be implemented using recursion.
	Traverse and print our list from node n to the top.
	we will seperate each node printed by the passed-in delimiter.
	If a node you encounter is a folder type, do

		cout<<....name.....<<delim;

	If a node you encounter is a file type, do

		cout<<....name.....<<"("<<..size..<<")"<<delim;

	Note: please do NOT include <<endl; (aka newline)
	Example output (assume that the delimiter is a single space):
	
		folder1 file1(0) folder2 file2(0)

	There should be no loop in this function
	See printBtT function for the call to this function.
	This function is private
*/
void MList::traverseToTop(Node* n, string delim)
{
	if (n != NULL) {
		if (n->nodeData->isFolder) {
			cout << n->nodeData->name << delim;
		} else {
			cout << n->nodeData->name << "(" << n->nodeData->size << ")" << delim;
		}
		traverseToTop(n->prev, delim);
	}
}

/*
	Note: RECURSION --- This function should be implemented using recursion.
	Traverse and print our list from node n to the bottom.
	we will seperate each node printed by the passed-in delimiter.
	If a node you encounter is a folder type, do

		cout<<....name.....<<delim;

	If a node you encounter is a file type, do

		cout<<....name.....<<"("<<..size..<<")"<<delim;

	Note: please do NOT include <<endl; (aka newline)
	Example output (assume that the delimiter is a single space):
	
		folder1 file1(0) folder2 file2(0)

	There should be no loop in this function
	See printTtB function for the call to this function.
	This function is private
*/
void MList::traverseToBottom(Node* n, string delim)
{
	if (n != NULL) {
		if (n->nodeData->isFolder) {
			cout << n->nodeData->name << delim;
		} else {
			cout << n->nodeData->name << "(" << n->nodeData->size << ")" << delim;
		}
		traverseToBottom(n->next, delim);
	}
}

//------------------------------------------------------------------------------
// FUNCTIONS BELOW ARE COMPLETE, PLEASE DON'T CHANGE ANYTHING HERE
//------------------------------------------------------------------------------

//getting the pointer to the top node.
Node* MList::top(){
	return ntop;
}

//getting the pointer to the bottom node.
Node* MList::bottom(){
	return nbottom;
}

//call traverseToBottom to print all item in the list from bottom node to top node
//the list of items is separated by a given delimiter
void MList::printBtT(string delim){
	//create a temp pointer to hold bottom
	Node* tmp = nbottom;
	traverseToTop(tmp,delim);
}

//call traverseToBottom to print all item in the list from top node to bottom node
//the list of items is separated by a given delimiter
void MList::printTtB(string delim){
	Node* tmp = ntop;
	traverseToBottom(tmp,delim);
}

//call sortSelection function, mode = true = sort by name
//public member
void MList::sortByNameSelection(){
	bool mode = true;
	Node *tmp = ntop;
	sortSelection(tmp,mode);
}

//call sortSelection function, mode = false = sort by size
//public member
void MList::sortBySizeSelection(){
	bool mode = false;
	Node *tmp = ntop;
	sortSelection(tmp,mode);
}

//call sortInsertion function
//public member
void MList::sortByNameInsertion(){
	Node *tmp = ntop;
	sortInsertion(tmp);
}