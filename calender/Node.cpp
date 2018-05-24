#include "Node.h"

Node * Node::findParent(time_t time,Node* root)
{
	Node* parent = nullptr;
	time_t tempMin1 = NULL;
	bool foundChild = false;
	while (root->left != nullptr || root->right != nullptr || root->middle != nullptr)
	{
		foundChild = false;
		if (root->min2 > time)
		{
			tempMin1 = root->getMiddle()->getMin1();
			if(tempMin1!=NULL)
				if (tempMin1 < time)
				{
					parent = root;
					root = root->middle;
					foundChild = true;
				}
			if (!foundChild)
			{
				parent = root;
				root = root->left;
			}
		}
		else if (root->min2 <= time && (root->min3 > time || root->min3 == 0))
		{
			parent = root;
			root = root->middle;
		}
		else
		{
			parent = root;
			root = root->right;
		}
		if (root->isLeaf())
			return parent;
	}

	return nullptr;
}

int Node::numberOfChildren()
{
	int res = 0;

	if (this->getRight() != nullptr)
		res = 3;

	else if (this->getMiddle() != nullptr)
		res = 2;
	else
		res = 1;

	return res;

}

Node * Node::splitNodes(Node * fullNode, Node * newNode)
{
	Node* splitNode = new Node;
	splitNode->parent = fullNode->parent;
	time_t newEventTime;
	bool isNewNodeLeaf = newNode->isLeaf();
	if (isNewNodeLeaf)
		newEventTime = newNode->key->getStartTime();
	else
		newEventTime = newNode->min2;
	if (fullNode->min1 > newEventTime)
	{
		splitNode->min2 = fullNode->min1;
		splitNode->middle = fullNode->left;
		splitNode->min1 = newEventTime;
		splitNode->left = newNode;
		fullNode->min1 = fullNode->min2;
		fullNode->min2 = fullNode->min3;
		fullNode->left = fullNode->middle;
		fullNode->middle = fullNode->right;
	}
	else if (fullNode->min2 > newEventTime)
	{
		splitNode->min1 = fullNode->min1;
		splitNode->left = fullNode->left;
		splitNode->min2 = newEventTime;
		splitNode->middle = newNode;
		fullNode->min2 = fullNode->min3;
		fullNode->left = fullNode->middle;
		fullNode->middle = fullNode->right;
	}
	else if (fullNode->min3 > newEventTime)
	{
		splitNode->min1 = fullNode->min1;
		splitNode->left = fullNode->left;
		splitNode->min2 = fullNode->min2;
		splitNode->middle = fullNode->middle;
		fullNode->min2 = fullNode->min3;
		fullNode->left = newNode;
		fullNode->min1 = newEventTime;
		fullNode->middle = fullNode->right;
	}
	else if (fullNode->min3 < newEventTime)
	{
		splitNode->min1 = fullNode->min1;
		splitNode->left = fullNode->left;
		splitNode->min2 = fullNode->min2;
		splitNode->middle = fullNode->middle;
		fullNode->min1 = fullNode->min3;
		fullNode->left = fullNode->right;
		fullNode->min2 = newEventTime;
		fullNode->middle = newNode;
	}
	splitNode->left->parent = splitNode;
	splitNode->middle->parent = splitNode;
	fullNode->left->parent = fullNode;
	fullNode->right->parent = fullNode;
	fullNode->min3 = 0;
	fullNode->right = nullptr;
	return splitNode;
}

Node * Node::newRoot(Node * first, Node * second)
{
	Node* newRoot = new Node;
	if (first->min1 < second->min1)
	{
		newRoot->min1 = first->min1;
		newRoot->left = first;
		newRoot->min2 = second->min1;
		newRoot->middle = second;
	}
	else if (first->min1 > second->min1)
	{
		newRoot->min2 = first->min1;
		newRoot->middle = first;
		newRoot->min1 = second->min1;
		newRoot->left = second;
	}
	first->parent = second->parent = newRoot;
	newRoot->parent = newRoot;
	return newRoot;
}

void Node::updateIndex(Node * newNode)
{
	Node* temp = newNode;
	Node* parent = newNode->parent;
	do
	{
		if (!temp->isLeaf())
		{
			temp->min1 = findMin(temp->left);
			temp->min2 = findMin(temp->middle);
			if (temp->min3 != 0)
				temp->min3 = findMin(temp->right);
		}
		temp = parent;
		parent = parent->parent; 
	} while (temp != parent);
	parent->min1 = findMin(parent->left);
	parent->min2 = findMin(parent->middle);
	if (parent->min3 != 0)
		parent->min3 = findMin(parent->right);
}

time_t Node::findMin(Node * nodeToSearch)
{
	Node* temp = nodeToSearch;
	while (!temp->isLeaf())
	{
		temp = temp->left;
	}
	return temp->key->getStartTime();
}

void Node::print(Node * nodeToPrint)
{
	if (nodeToPrint->isLeaf())
	{
		nodeToPrint->key->print();
		return;
	}
	print(nodeToPrint->left);
	print(nodeToPrint->middle);
	if (nodeToPrint->right != nullptr)
		print(nodeToPrint->right);
}



bool Node::insert(Node * newNode, Node * parentNode)
{
	time_t newEventMin1 = -1;
	time_t newEventTime;
	if (newNode->isLeaf())
		newEventTime = newNode->key->getStartTime();
	else
	{
		newEventTime = newNode->min2;
		newEventMin1 = newNode->min1;
	}
	if (parentNode->right != nullptr)//Node is full
		return false;
	else if (newEventTime < parentNode->min2)
	{
		parentNode->right = parentNode->middle;
		parentNode->min3 = parentNode->min2;
		if (newEventTime < parentNode->min1)
		{
			parentNode->min2 = parentNode->min1;
			parentNode->min1 = newEventTime;
			parentNode->middle = parentNode->left;
			parentNode->left = newNode;
		}
		else
		{
			parentNode->min2 = newEventTime;
			parentNode->middle = newNode;
		}
	
		return true;
	}
	else
	{
		parentNode->right = newNode;
		parentNode->min3 = newEventTime;
		return true;
	}
	return false;
}

bool Node::isLeaf()
{

	if (this->min1 == 0 && this->min2 == 0 && this->min3 == 0)
	{
		return true;
	}
	return false;
}
