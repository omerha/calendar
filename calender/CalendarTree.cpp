#include "CalendarTree.h"

CalendarEvent * CalendarTree::eventAt(time_t time)
{
	Node* temp = treeRoot->getMiddle();//getting the tree's root
	CalendarEvent* tempEvent;
	while(!temp->isLeaf())//going down the tree
	{
		if (temp->getMin2() > time)//case key is lower than min2 we go left.
			temp = temp->getLeft();
		else if (temp->getMin2() <= time)
		{
			if (temp->getMin3() != NULL)//In case key is higher than min 2 we check if min3 exists and choose to go middle or right.
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
	if (tempEvent->getStartTime() <= time && time <= (tempEvent->getStartTime() + tempEvent->getDuration()))//Checking if the event is during the requested time.
		return tempEvent;
	return nullptr;
}

CalendarEvent * CalendarTree::eventAfter(time_t time)
{
	Node* leafParent = nullptr;
	bool found = false;
	Node* temp = nullptr;
	leafParent = treeRoot->getMiddle()->findParent(time, treeRoot->getMiddle());//Looking for the node the event would have been, than we will search for the higher key.
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
	Node* newNode = new Node;
	Node* newNodeSplited = nullptr;
	Node* temp = nullptr;
	Node* parent = nullptr;
	time_t oldStart = NULL;
	bool inserted = false;
	newNode->setEvent(eventToInsert);//Put in the root's middle.
	if (this->isEmpty())//First node in the tree
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
				return nullptr;
		
		CalendarEvent* checkExistsEventAfter = eventAfter(eventToInsert->getStartTime());
		if (checkExistsEventAfter)//Checking that the new event doesn't hit with the after event
			if (eventToInsert->getStartTime() + eventToInsert->getDuration() > checkExistsEventAfter->getStartTime())
				return nullptr;
			
		parent = parent->findParent(timeToInsert, treeRoot->getMiddle());//Pulling the right Node the new leaf should be inserted to.
		firstParent = parent;//Getting the Node's parent in case we need to split it.
		if (!temp->insert(newNode, parent)) // Need to split node
		{
			while (!inserted)
			{
				newNodeSplited = temp->splitNodes(parent, newNode);//Splitting nodes
				temp->updateIndex(parent);//Updating index after we splited the nodes.
				if (parent != parent->getParent())//the new splited node should be inserted to the father of the node.
					parent = parent->getParent();
				else//Need to create new root
				{
					treeRoot->setMiddle(temp->newRoot(parent, newNodeSplited));//Creating new root
					return eventToInsert;
				}
				if (temp->insert(newNodeSplited, parent))//Inserting new splited node.
					inserted = true;
				else
					newNode = newNodeSplited;//Need to split parent as well.
			}
		}
		temp->updateIndex(firstParent); //Updating indexes.
		return eventToInsert;
	}
	return nullptr;
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
		parent->setLeft(nullptr);
		free(child);

		if (parent->numberOfChildren() == 2)
		{
			organize2ChildrenLeftInNode(parent);//only update min 2,3. the removed node allways will be the left. 
			updateMinUpTo(parent);									//so only min 2 bacome min 3	
																	//update min up
		}

		else//left 1 children
		{
			if (parent == treeRoot->getMiddle())
			{
				if (!parent->getMiddle()->isLeaf())
				{
					treeRoot->setMiddle(parent->getMiddle());
					free(parent);
					treeRoot->updateTheNewParentForChildren();

				}

				else//in case the middle sub tree of the root is leaf we need to set the middle child to be left child
				{
					parent->setLeft(parent->getMiddle());
					parent->setMiddle(nullptr);
				}
			}

			else if (parent->getParent()->getMiddle()->numberOfChildren() == 3)//in case the cosin have 3 children
			{
				deliverLeftChildOfCosinAndUpdateMin(parent);//deliver the left child of cosin and update min 2,3
				parent->updateTheNewParentForChildren();
				updateMinUpTo(parent);
			}

			else//cosin have 2 child
			{
				deliverChildToCosinAndUpdateMin(parent);//*TODO* maybe we need also min 1 
				parent->getParent()->getMiddle()->updateTheNewParentForChildren();//update the new child that his parent now is the cosin
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
		parent->setMin3(0);

	}

	else if (parent->getMiddle() == nullptr)
	{
		parent->setMiddle(parent->getRight());
		parent->setMin2(parent->getMin3());
		parent->setMin3(0);
	}

	else
	{
		parent->setMin3(0);
	}

	parent->setRight(nullptr);
	parent->setMin3(0);
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



void CalendarTree::deliverChildToCosinAndUpdateMin(Node*parent)//in case the cosin have 2 children we deliver 
{																//the 1 childre  that lefet from parent to cosin	
	Node*cosin = parent->getParent()->getMiddle();

	cosin->setMin3(cosin->getMin2());
	cosin->setMin2(cosin->getMin1());
	cosin->setRight(cosin->getMiddle());
	cosin->setMiddle(cosin->getLeft());
	cosin->setLeft(parent->getMiddle());



	if (parent->getMiddle()->isLeaf())
		cosin->setMin1(parent->getMiddle()->getEvent()->getStartTime());
	else
		cosin->setMin1(parent->getMiddle()->getMin1());

	parent->setMin1(0);
	parent->setMin2(0);
}

void CalendarTree::updateMinUpTo(Node * node)
{
	while (node != treeRoot)//if there is no function "get parent", need to change it 
	{
		if (node->getLeft()->isLeaf())
		{
			if (node->getLeft())
				node->setMin1(node->getLeft()->getEvent()->getStartTime());
			if (node->getMiddle())
				node->setMin2(node->getMiddle()->getEvent()->getStartTime());
			if (node->getRight())
				node->setMin3(node->getRight()->getEvent()->getStartTime());
		}

		else
		{
			if (node->getLeft())
				node->setMin1(node->getLeft()->getMin1());
			if (node->getMiddle())
				node->setMin2(node->getMiddle()->getMin1());
			if (node->getRight())
				node->setMin3(node->getRight()->getMin1());
		}
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
