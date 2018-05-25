#pragma once
#include "Node.h"
#define _CRT_SECURE_NO_WARNINGS

class CalendarTree {
	Node* treeRoot;
public:
	CalendarTree() {
		makeEmpty();//We use dummy head - our real tree starts from the middle of the dummy head
		treeRoot->setMiddle(new Node);
	}
	void makeEmpty() {
		treeRoot = new Node;
	}
	int isEmpty() {
		return treeRoot->getMiddle()->getMiddle() == nullptr;
	}
	CalendarEvent* eventAt(time_t time);
	CalendarEvent* eventAfter(time_t time);
	CalendarEvent* insert(CalendarEvent* eventToInsert);
	CalendarEvent* deleteFirst();
	void printSorted();
	void organize2ChildrenLeftInNode(Node* parent);
	void deliverLeftChildOfCosinAndUpdateMin(Node * parent);
	void deliverChildToCosinAndUpdateMin(Node*child);
	void updateMinUpTo(Node * node);
	int numBefore(time_t time);

};