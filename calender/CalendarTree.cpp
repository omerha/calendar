#include "CalendarTree.h"
int check = 0;
int test = 0;
CalendarEvent * CalendarTree::eventAt(time_t time)
{
	Node* temp = treeRoot->getMiddle();
	CalendarEvent* tempEvent;
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

CalendarEvent * CalendarTree::eventAfter(time_t time)
{
	Node* leafParent = nullptr;
	bool found = false;
	Node* temp = nullptr;
	leafParent = leafParent->findParent(time, treeRoot->getMiddle());
	if (leafParent->getLeft()->getEvent()->getStartTime() >= time)
		return leafParent->getLeft()->getEvent();
	else if (leafParent->getLeft()->getEvent()->getStartTime() <= time)
	{
		if (leafParent->getMiddle()->getEvent()->getStartTime() <= time)
		{
			if (leafParent->getMiddle()->getEvent()->getStartTime() == time)
				return leafParent->getMiddle()->getEvent();
			else if (leafParent->getRight() != NULL)
				if (leafParent->getRight()->getEvent()->getStartTime() > time)
					return leafParent->getRight()->getEvent();
		}
		else 
			return leafParent->getMiddle()->getEvent();
	}
	//assuming the afterevent is in the right leaf or in the right node
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
	if (test < timeToInsert)
		test = timeToInsert;
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
		CalendarEvent* checkExistsEvent = eventAt(eventToInsert->getStartTime());
		if (checkExistsEvent)//checking if there is no other event at the same time.
			if (checkExistsEvent->getDuration() + checkExistsEvent->getStartTime() != eventToInsert->getStartTime())
			{
				check++;
				return nullptr;
			}
		CalendarEvent* checkExistsEventAfter = eventAfter(eventToInsert->getStartTime());
		if (checkExistsEventAfter)
			if (eventToInsert->getStartTime() + eventToInsert->getDuration() > checkExistsEventAfter->getStartTime())
			{
				check++;
					return nullptr;
			}
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
	Node * parent = treeRoot->getMiddle();
	Node* child = treeRoot->getMiddle();
	CalendarEvent*res = nullptr;

	if (!this->isEmpty())
	{
		while (!child->isLeaf())
		{
			parent = child;
			child = child->getLeft();
		}

		res = child->getEvent();
		free(child);
		parent->setLeft(nullptr);

		if (parent->numberOfChildren() == 2)
		{
			organize2ChildrenLeftInNode(parent);//only update min 2,3. the removed node allways will be the left. 
												//so only min 2 bacome min 3	
		}

		else//left 1 children
		{
			if (parent == treeRoot)
			{
				if (!parent->getMiddle()->isLeaf())
				{
					treeRoot = parent->getMiddle();
					free(parent);
				}

				else//in case the middle sub tree in the middle of the root is leaf we need to set the middle child to be left child
				{
					parent->setLeft(parent->getMiddle());
					parent->setMiddle(nullptr);
				}
			}
			else if (parent->getParent()->getMiddle()->numberOfChildren() == 3)//in case the cosin have 3 children
			{
				deliverLeftChildOfCosinAndUpdateMin(parent);//deliver the left child of cosin and update min 2,3
				updateMinUpTo(parent);
			}

			else
			{
				deliverChildToCosinAndUpdateMin(child);//*TODO* maybe we need also min 1 
				parent->setMiddle(nullptr);
				deleteFirst();
			}
		}
	}
	return res;
}

void CalendarTree::organize2ChildrenLeftInNode(Node* parent)
{
	if (parent->getLeft() == nullptr)
	{
		parent->setLeft(parent->getMiddle());
		parent->setMiddle(parent->getRight());
		parent->setMin1(parent->getMin2());
		parent->setMin2(parent->getMin3());
		parent->setMin3(-1);

	}

	else if (parent->getMiddle() == nullptr)
	{
		parent->setMiddle(parent->getRight());
		parent->setMin2(parent->getMin3());
		parent->setMin3(-1);
	}

	else
	{
		parent->setMin3(-1);
	}

	parent->setRight(nullptr);
}

void CalendarTree::deliverLeftChildOfCosinAndUpdateMin(Node * parent)
{
	Node * nodeThatGetNewChild = parent;
	Node * cosin = parent->getParent()->getMiddle();
	Node*ChildToTake = cosin->getLeft();

	nodeThatGetNewChild->setRight(ChildToTake);


	organize2ChildrenLeftInNode(cosin);
	organize2ChildrenLeftInNode(nodeThatGetNewChild);
}



void CalendarTree::deliverChildToCosinAndUpdateMin(Node*child)
{
	Node*parent = child->getParent();
	Node*cosin = parent->getMiddle();

	cosin->setMin3(cosin->getMin2());
	cosin->setMin2(cosin->getMin1());
	cosin->setRight(cosin->getMiddle());
	cosin->setMiddle(cosin->getLeft());
	cosin->setLeft(child);

	if (child->isLeaf())
		cosin->setMin1(child->getEvent()->getStartTime());

	else
		cosin->setMin1(child->getMin1());
}

void CalendarTree::updateMinUpTo(Node * node)
{
	while (node != nullptr)//if there is no function "get parent", need to change it 
	{
		node->setMin1(node->getLeft()->getMin1());
		node->setMin2(node->getMiddle()->getMin1());
		node->setMin3(node->getRight()->getMin1());

		node = node->getParent();
	}
}

void CalendarTree::printSorted()
{
	treeRoot->print(treeRoot->getMiddle());
}

int CalendarTree::numBefore(time_t time)
{
	return 0;
}
