#pragma once
#include "Node.h"
#define _CRT_SECURE_NO_WARNINGS

class CalendarTree {
	Node* treeRoot;
public:
	CalendarTree() {
		makeEmpty();//We use dummy head - our real tree starts from the middly of the dummy head
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
	void recInsert(Node* newNode,Node* parent);
	CalendarEvent* deleteFirst();
	void printSorted();
	int numBefore(time_t time);

};