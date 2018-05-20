#include "CalendarTree.h"

CalendarEvent * CalendarTree::eventAt(time_t time)
{
	Node* temp = treeRoot;
	CalendarEvent* tempEvent;
	//while (!(temp->getLeft()->isLeaf) || !(temp->getMiddle()->isLeaf) || !(temp->getRight()->isLeaf))
	while(!temp->isLeaf())
	{
		if (temp->getMin2() > time)
			temp = temp->getLeft();
		else if (temp->getMin2() <= time)
		{
			if (temp->getMin3() != NULL)
			{
				if (time < temp->getMin3())
					temp = temp->getMiddle();
				else
					temp = temp->getRight();
			}
			else
				temp = temp->getMiddle();
		}	
	}
	tempEvent = temp->getEvent();
	if (tempEvent->getStartTime() <= time && time <= (tempEvent->getStartTime() + tempEvent->getDuration()))
		return tempEvent;
	return nullptr;
}

//CalendarEvent * CalendarTree::eventAfter(time_t time)
//{	
//	Node* temp = treeRoot;
//	Node* parent = nullptr;
//	CalendarEvent* tempEvent;
//	bool foundParent = false;
//	bool found = false;
//	while (!found)
//	{
//		while (!temp->isLeaf())
//		{
//			foundParent = false;
//			if (temp->getMin2() > time)
//				temp = temp->getLeft();
//			else if (temp->getMin2() <= time)
//			{
//				if (temp->getMin3() != NULL)
//				{
//					if (time < temp->getMin3())
//						temp = temp->getMiddle();
//					else
//						temp = temp->getRight();
//				}
//				else
//					temp = temp->getMiddle();
//			}
//		}
//		tempEvent = temp->getEvent();
//		if (tempEvent->getStartTime() >= time)
//			return tempEvent;
//		else if (tempEvent->getStartTime() == NULL)
//			return nullptr;
//		else
//		{
//			while (!foundParent)
//			{
//				if (temp->isLeaf())
//					parent = temp->findParent(tempEvent->getStartTime(), treeRoot);
//				else
//					parent = parent->findParent(temp->getMin2(), treeRoot);
//				if (parent == nullptr)
//					return nullptr;
//				else if (parent->getLeft() == temp)
//				{
//					if (parent->getMiddle() != NULL)
//					{
//						temp = parent->getMiddle();
//						foundParent = true;
//					}
//					else
//						temp = parent;
//				}
//				else if (parent->getMiddle() == temp)
//				{
//					if (parent->getRight() != NULL)
//					{
//						temp = parent->getRight();
//						foundParent = true;
//					}
//					else
//						temp = parent;
//				}
//				else
//					temp = parent;
//			}
//		}
//	}
//	return nullptr;
//}
CalendarEvent * CalendarTree::eventAfter(time_t time)
{
	Node* leafParent = nullptr;
	bool found = false;
	Node* temp = nullptr;
	leafParent = leafParent->findParent(time, treeRoot->getMiddle());
	if (leafParent->getLeft()->getEvent()->getStartTime() == time)
		return leafParent->getMiddle()->getEvent();
	else if (leafParent->getMiddle()->getEvent()->getStartTime() == time)
	{
		if (leafParent->getRight() != NULL)
			return leafParent->getRight()->getEvent();
	}//assuming the afterevent is in the right leaf or in the right node
	temp = leafParent->getParent();
	while (!found)
	{
		if (temp->getLeft() == leafParent)
			return eventAt(temp->getMiddle()->getMin1());
		else if (temp->getMiddle() == leafParent)
		{
			if (temp->getRight() != NULL)
				return eventAt(temp->getRight()->getMin1());
		}
		leafParent = temp;
		temp = leafParent->getParent();
		if (temp == leafParent)//we are looking in the root and there is no right event
			found = true;
	}
	return nullptr;
}
CalendarEvent* CalendarTree::insert(CalendarEvent* eventToInsert)
{
	time_t timeToInsert = eventToInsert->getStartTime();
	Node* firstParent = nullptr;
	Node* newNode = new Node;
	Node* newNodeSplited = nullptr;
	Node* temp = nullptr;
	Node* parent = nullptr;
	time_t oldStart = NULL;
	bool inserted = false;
	newNode->setEvent(eventToInsert);//Put in the root's middle.
	if (this->isEmpty())
	{
		temp = treeRoot->getMiddle();
		temp->setMiddle(newNode);
		temp->setParent(temp);//Root's parent is root
		newNode->setParent(temp);
		return eventToInsert;
	}
	else if (treeRoot->getMiddle()->getLeft() == nullptr)//Second items in the tree
	{
		temp = treeRoot->getMiddle();
		oldStart = temp->getMiddle()->getEvent()->getStartTime();
		newNode->setParent(temp);
		if (temp->getMiddle()->getEvent()->getDuration() + oldStart < timeToInsert)
		{
			temp->setLeft(temp->getMiddle());
			temp->setMiddle(newNode);
			temp->setMin2(timeToInsert);
			temp->setMin1(oldStart);
			return eventToInsert;
		}
		else if (oldStart > timeToInsert + eventToInsert->getDuration())
		{
			temp->setLeft(newNode);
			temp->setMin2(oldStart);
			temp->setMin1(timeToInsert);
			return eventToInsert;
		}
		else
			return nullptr;
	}
	else
	{
		parent = parent->findParent(timeToInsert, treeRoot->getMiddle());
		firstParent = parent;
		if (!temp->insert(newNode, parent)) // Need to split node
		{
			while (!inserted)
			{
				newNodeSplited = temp->splitNodes(parent, newNode);
				temp->updateIndex(parent);
				if (parent != parent->getParent())
					parent = parent->getParent();
				else//Need to create new root
				{
					treeRoot->setMiddle(temp->newRoot(parent, newNodeSplited));
					return eventToInsert;
				}
				if (temp->insert(newNodeSplited, parent))
					inserted = true;
				else
					newNode = newNodeSplited;
			}
		}
		temp->updateIndex(firstParent);
		return eventToInsert;
	}
	return nullptr;
}

void CalendarTree::recInsert(Node * newNode, Node * parent)
{
}

CalendarEvent* CalendarTree::deleteFirst()
{
	return nullptr;
}

void CalendarTree::printSorted()
{
	treeRoot->print(treeRoot->getMiddle());
}

int CalendarTree::numBefore(time_t time)
{
	return 0;
}
