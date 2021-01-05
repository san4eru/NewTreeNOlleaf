#include "Tree23.h"

int Node::IsLeaf()
{
	return isleaf;
}
void Node::SetParent(Node* p)
{
	current = p;
}
Node* Node::getParent()
{
	return current;
}


StringKey::StringKey(const string& strString)
{
	strKey = strString;
}
StringKey::StringKey(const StringKey& otherStringKey)
{
	strKey = otherStringKey.strKey;
}
int StringKey::Compare(const StringKey* s)const
{
	if (atoi(strKey.c_str()) > atoi(s->strKey.c_str()))
		return -1;
	if (atoi(strKey.c_str()) < atoi(s->strKey.c_str()))
		return 1;
	if (atoi(strKey.c_str()) == atoi(s->strKey.c_str()))
		return 0;
	return 0;
	//return (strKey.compare(s->strKey));
}
string StringKey::getString()const
{
	return strKey;
}
StringKey* StringKey::Read(ifstream& inputFile)
{
	string inKey;
	StringKey* outKey = NULL;

	if (inputFile >> inKey)
	{
		outKey = new StringKey(inKey);
		return outKey;
	}

	return NULL;
}
StringKey* StringKey::Readfromkeyboard(string new_k)
{
	StringKey* outKey = new StringKey(new_k);
	return outKey;
}


NotLeafNode::NotLeafNode(Node* l, Node* m, Node* r, StringKey* ik1, StringKey* ik2, StringKey* ik3)
{
	left = l;
	mid = m;
	right = r;
	k1 = ik1;
	k2 = ik2;
	k3 = ik3;

	// установка родителя
	right->SetParent(this);
	left->SetParent(this);
	mid->SetParent(this);

	isleaf = false;
}
void NotLeafNode::setK1(StringKey* k)
{
	k1 = k;
}
StringKey* NotLeafNode::getK1()
{
	return k1;
}
void NotLeafNode::setK2(StringKey* k)
{
	k2 = k;
}
StringKey* NotLeafNode::getK2()
{
	return k2;
}
void NotLeafNode::setK3(StringKey* k)
{
	k3 = k;
}
StringKey* NotLeafNode::getK3()
{
	return k3;
}
void NotLeafNode::setLeft(Node* node)
{
	left = node;

	if (node != NULL)
		node->SetParent(this);
}
Node* NotLeafNode::getLeft()
{
	return left;
}
void NotLeafNode::setMid(Node* node)
{
	mid = node;

	if (node != NULL)
		node->SetParent(this);
}
Node* NotLeafNode::getMid()
{
	return mid;
}
void NotLeafNode::setRight(Node* node)
{
	right = node;

	if (node != NULL)
		node->SetParent(this);
}
Node* NotLeafNode::getRight()
{
	return right;
}


void TempNode::setK4(StringKey* k)
{
	k4 = k;
}
void TempNode::setBiggest(Node* node)
{
	biggest = node;

	if (node != NULL)
		node->SetParent(this);
}
StringKey* TempNode::getK4()
{
	return k4;
}
Node* TempNode::getBiggest()
{
	return biggest;
}