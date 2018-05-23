#pragma once
#include "CalendarEvent.h"

class Node {
	Node *left, *right, *middle;
	Node * parent;
	time_t min1,min2, min3;
	CalendarEvent* key;
public:
	Node() 
	{
		left=nullptr;
		middle = nullptr;
		right = nullptr;
		min2 = NULL;
		min3 = NULL;
		min1 = NULL;
	}


	CalendarEvent* getEvent() { return key; }
	void setEvent(CalendarEvent* event) { key = event; }
	Node* getLeft() { return left; }
	Node* getMiddle() { return middle; }
	Node* getRight() { return right; }
	Node* getParent() { return parent; }
	void setParent(Node* nParent) { parent = nParent; }
	void setLeft(Node* nLeft) { left = nLeft; }
	void setMiddle(Node* nMiddle) { middle = nMiddle; }
	void setRight(Node* nRight) { right = nRight; }
	time_t getMin2() { return min2; }
	time_t getMin3() { return min3; }
	time_t getMin1() { return min1; }
	void setMin1(time_t tMin1) { min1 = tMin1; }
	void setMin2(time_t tMin2) { min2 = tMin2; }
	void setMin3(time_t tMin3) { min3 = tMin3; }
	Node* findParent(time_t time,Node* root);
	Node* splitNodes(Node* fullNode, Node* newNode);
	Node* newRoot(Node* first, Node* second);
	void updateIndex(Node* newNode);
	time_t findMin(Node* nodeToSearch);
	void print(Node* nodeToPrint);
	int  numberOfChildren();


	//Node* find();
	bool insert(Node* newNode,Node* parentNode);
	//void delete();
	bool isLeaf();
};