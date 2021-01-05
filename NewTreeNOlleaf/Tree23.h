#pragma once
#ifndef Tree23_H
#define Tree23_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Node
{
protected:
	Node* current;
	int isleaf;

public:
	Node() : current(nullptr), isleaf(0) {}
	virtual ~Node() {}

	int IsLeaf(); // ����� 1, ���� ���� �������� ������, � 0, ���� ���
	void SetParent(Node* p); // ������������� ��������
	Node* getParent(); // ���������� ��������
};

class StringKey
{
private:
	string strKey;

public:
	StringKey() {}
	StringKey(const string& strString); // �����������, ������� �������� ������
	StringKey(const StringKey& otherStringKey); // ����������� �����������
	~StringKey() {}

	int Compare(const StringKey* s)const; //���������� ��������� ����� 2-�� ��������
	string getString()const; // ���������� strKey
	static StringKey* Read(ifstream& inputFile); // ������ ����� �� �����. ������ NULL, ���� ��� ������ ��������� ������
	static StringKey* Readfromkeyboard(string new_k); // ������ ����� � ����������
};

template <typename T>
class Template
{
private:
	StringKey* key;
	T value;

public:
	Template() : key(nullptr), value(T()) {}
	Template(StringKey* key, T value) : key(key), value(value) {}
	Template(const Template<T>& otherData); // ����������� �����������. ������ ����������, ���� ���� ������� ������� �������� �������
	~Template() { delete key; }

	StringKey* GetKey()const; // ���������� ����
	T getdata(); // ���������� ��������

	static Template* Read(ifstream& inputFile); // ������ ������ �� �����. ������ NULL, ���� ��������� ������ ������
	static Template* Readfromkeyboard(string new_k, T new_value); // ������ ������ � ����������
	friend ostream& operator << (ostream& out, const Template<T>& data) // ���������� operator << ��� ������
	{
		out << data.key->getString() << "\t" << data.value;
		return out;
	}
};

template <typename T>
Template<T>::Template(const Template<T>& otherData)
{
	if (otherData.key != NULL) {
		key = new StringKey(*otherData.key);
		value = otherData.value;
	}
}
template <typename T>
StringKey* Template<T>::GetKey()const
{
	return key;
}
template <typename T>
T Template<T>::getdata()
{
	return this->value;
}
template <typename T>
Template<T>* Template<T>::Read(ifstream& inputFile)
{
	StringKey* tempSKey;
	Template<T>* outData;

	tempSKey = StringKey::Read(inputFile);

	if (!tempSKey)
	{
		return NULL;
	}

	outData = new Template<T>;
	outData->key = tempSKey;

	if (!(inputFile >> outData->value))
	{
		delete outData;
		return NULL;
	}

	return outData;
}
template <typename T>
Template<T>* Template<T>::Readfromkeyboard(string new_k, T new_value)
{
	StringKey* tempSKey = StringKey::Readfromkeyboard(new_k);
	Template<T>* outData = new Template<T>;

	outData->key = tempSKey;
	outData->value = new_value;

	return outData;
}

class NotLeafNode : public Node
{
protected:
	Node* left, * mid, * right;
	StringKey* k1, * k2, * k3;

public:
	NotLeafNode() :left(NULL), mid(NULL), right(NULL), k1(NULL), k2(NULL), k3(NULL)
	{
		SetParent(NULL);
		isleaf = false;
	}
	NotLeafNode(Node* l, Node* m, Node* r, StringKey* ik1, StringKey* ik2, StringKey* ik3);
	virtual ~NotLeafNode(void)
	{
		delete left;
		delete mid;
		delete right;
	}

	//set'� � get'�
	void setK1(StringKey* k);
	StringKey* getK1();
	void setK2(StringKey* k);
	StringKey* getK2();
	void setK3(StringKey* k);
	StringKey* getK3();

	void setLeft(Node* node);
	Node* getLeft();
	void setMid(Node* node);
	Node* getMid();
	void setRight(Node* node);
	Node* getRight();
};

class TempNode : public NotLeafNode
{
private:
	StringKey* k4; // ������� "������ ��������"
	Node* biggest; // 4-��� �������, ������� ������, ��� ��� "������ �������"

public:
	TempNode() : k4(NULL), biggest(NULL) {}
	~TempNode(void) {}

	// set'� � get'�
	void setK4(StringKey* k); // ������������� k4
	void setBiggest(Node* node); // ������������� ������ �������� �������

	StringKey* getK4(); // ���������� k4
	Node* getBiggest(); // ���������� ������ ��������
};

template <typename T>
class LeafNode : public Node
{
private:
	Template<T>* data;
	StringKey* key;

public:
	LeafNode() : data(T()), key(nullptr)
	{
		isleaf = true;
	}
	LeafNode(Template<T>* d);
	~LeafNode(void) {}

	Template<T>* Data()const; // ���������� ������ �������� ����
	StringKey* Key()const; // ���������� ���� ������� ����
};

template <typename T>
LeafNode<T>::LeafNode(Template<T>* d)
{
	key = d->GetKey();
	data = d;

	isleaf = true;
}
template <typename T>
Template<T>* LeafNode<T>::Data()const
{
	return data;
}
template <typename T>
StringKey* LeafNode<T>::Key()const
{
	return data->GetKey();
}

template <typename T>
class Dictionary
{
private:
	Node* root;

	void AddChild(TempNode* trans); // ��������� 4 ������� � ������� � ��������� ��� ���, ����� �� ����������
	TempNode* MakeTempNode(NotLeafNode* parent, LeafNode<T>* child); // ������� ��������� ���� 4

	void freeAllElementsHelper(Node* node); // ����������� ��� �������� ����������
	void Update(NotLeafNode* node); // ��������� k1, k2, k3 ���� �� �����

	NotLeafNode* FindParent(StringKey* key); // ������� �������� ����

	void print23(ofstream& outputFile, Node* node); // ���������� �������� � ���������� �������
	void print23Console(Node* node); // ���������� ������� � �������
	void counterkey(Node* node, int& count_k); // ���������� ������� ���-�� ������
	int HeightOfTree(Node* node); // ���������� ������� ������ ������

public:
	Dictionary() : root(0) {}
	~Dictionary() { delete root; }// ������ �����, ������� � ������ ���� ����������
	
	int Insert(StringKey* k, Template<T>* d); // ��������� ����� ����
	Template<T>* Delete(StringKey* k); // ������� ���� �� �����
	Template<T>* Find(StringKey* k); // ������� ���� �� �����

	int CountOfKey(); // ������� ���������� ������
	int CountOfHeight(); // ������� ������ ������

	void FreeAllElements(); // ����������� ��� �������� � �������

	void PrintDictionary(ofstream& outputFile); // �������� ������� � ���������� ������� � ����
	void PrintDictionaryConsole(); // ������� ������� � ���������� ������� � �������

	int IsEmpty(); // ���������, �������� �� ������� ������, ���� �����, ������ 1 (true), ����� ������ 0 (false)
};

template <typename T>
void Dictionary<T>::AddChild(TempNode* trans)
{
	NotLeafNode* rightMostNotLeaf = new NotLeafNode;

	rightMostNotLeaf->setLeft(trans->getRight());
	rightMostNotLeaf->setMid(trans->getBiggest());
	rightMostNotLeaf->setK1(trans->getK3());
	rightMostNotLeaf->setK2(trans->getK4());

	NotLeafNode* leftMostNotLeaf = new NotLeafNode;

	leftMostNotLeaf->setLeft(trans->getLeft());
	leftMostNotLeaf->setMid(trans->getMid());
	leftMostNotLeaf->setK1(trans->getK1());
	leftMostNotLeaf->setK2(trans->getK2());

	if (trans->getParent() == NULL)
	{
		NotLeafNode* parent = new NotLeafNode;

		parent->setLeft(leftMostNotLeaf);
		parent->setMid(rightMostNotLeaf);
		parent->setK1(leftMostNotLeaf->getK1());
		parent->setK2(rightMostNotLeaf->getK1());

		root = parent;

		trans->setLeft(NULL);
		trans->setMid(NULL);
		trans->setRight(NULL);
		trans->setBiggest(NULL);
		delete trans;
	}
	else // ���������� ���� - �� ������
	{
		NotLeafNode* parent = (NotLeafNode*)trans->getParent();

		if (parent->getRight() == NULL) // ���� �������� ����� 2-�� �����
		{
			if (trans == parent->getLeft()) // ���� 4-���� �������� ���������� �� ����
			{
				parent->setRight(parent->getMid());
				parent->setLeft(leftMostNotLeaf);
				parent->setMid(rightMostNotLeaf);
			}
			else if (trans == parent->getMid())
			{
				parent->setMid(leftMostNotLeaf);
				parent->setRight(rightMostNotLeaf);
			}

			// ���������� k1 k2 k3 �� �����
			Update(parent);
		}
		else // ���� �������� ��� ����� 3-�� �����, �� ������ 4-��� ���� - ���������� �����
		{
			TempNode* tmpFather = new TempNode;

			if (trans == parent->getLeft())
			{
				tmpFather->setLeft(leftMostNotLeaf);
				tmpFather->setMid(rightMostNotLeaf);
				tmpFather->setRight(parent->getMid());
				tmpFather->setBiggest(parent->getRight());

				tmpFather->setK1(leftMostNotLeaf->getK1());
				tmpFather->setK2(rightMostNotLeaf->getK1());
				tmpFather->setK3(((NotLeafNode*)parent->getMid())->getK1());
				tmpFather->setK4(((NotLeafNode*)parent->getRight())->getK1());
			}
			else if (trans == parent->getMid())
			{
				tmpFather->setLeft(parent->getLeft());
				tmpFather->setMid(leftMostNotLeaf);
				tmpFather->setRight(rightMostNotLeaf);
				tmpFather->setBiggest(parent->getRight());

				tmpFather->setK1(((NotLeafNode*)parent->getLeft())->getK1());
				tmpFather->setK2(leftMostNotLeaf->getK1());
				tmpFather->setK3(rightMostNotLeaf->getK1());
				tmpFather->setK4(((NotLeafNode*)parent->getRight())->getK1());
			}
			else if (trans == parent->getRight())
			{
				tmpFather->setLeft(parent->getLeft());
				tmpFather->setMid(parent->getMid());
				tmpFather->setRight(leftMostNotLeaf);
				tmpFather->setBiggest(rightMostNotLeaf);

				tmpFather->setK1(((NotLeafNode*)parent->getLeft())->getK1());
				tmpFather->setK2(((NotLeafNode*)parent->getMid())->getK1());
				tmpFather->setK3(leftMostNotLeaf->getK1());
				tmpFather->setK4(rightMostNotLeaf->getK1());
			}

			NotLeafNode* FatherOfParent = (NotLeafNode*)parent->getParent();
			tmpFather->SetParent(parent->getParent());

			if (FatherOfParent != NULL) // ���� �������� �� ������.
			{
				if (FatherOfParent->getLeft() == parent) // �������� - ����� ���
					FatherOfParent->setLeft(tmpFather);
				else if (FatherOfParent->getMid() == parent) // �������� - ������� ���
					FatherOfParent->setMid(tmpFather);
				else if (FatherOfParent->getRight() == parent) // �������� - ������ ���
					FatherOfParent->setRight(tmpFather);

				Update(FatherOfParent); // ���������� ���� ������ ����������� ���� �� �����
			}
			else // ���� �������� ��� ������
			{
				root = tmpFather;
			}

			parent->setLeft(NULL);
			parent->setMid(NULL);
			parent->setRight(NULL);
			delete parent; // �������� �������� � ����� ����� tmpFather

			trans->setLeft(NULL);
			trans->setMid(NULL);
			trans->setRight(NULL);
			trans->setBiggest(NULL);
			delete trans; // ���������� ���� ��� ������� ����� ���������

			AddChild(tmpFather);
		}
	}
}
template <typename T>
TempNode* Dictionary<T>::MakeTempNode(NotLeafNode* parent, LeafNode<T>* child)
{
	TempNode* retFourSons = new TempNode(); // ����� ������ ��������

	LeafNode<T>* right = (LeafNode<T>*)parent->getRight();
	LeafNode<T>* left = (LeafNode<T>*)parent->getLeft();
	LeafNode<T>* mid = (LeafNode<T>*)parent->getMid();

	StringKey* newChildKey = child->Key();

	// ������ #1: ����� - ����� �������
	if (newChildKey->Compare(right->Key()) > 0)
	{
		retFourSons->setLeft(left);
		retFourSons->setMid(mid);
		retFourSons->setRight(right);
		retFourSons->setBiggest(child);
		retFourSons->setK1(left->Key());
		retFourSons->setK2(mid->Key());
		retFourSons->setK3(right->Key());
		retFourSons->setK4(child->Key());
	}
	// ������ #2: ����� - ����� 3 � 4
	else if (newChildKey->Compare(mid->Key()) > 0)
	{
		retFourSons->setLeft(left);
		retFourSons->setMid(mid);
		retFourSons->setRight(child);
		retFourSons->setBiggest(right);
		retFourSons->setK1(left->Key());
		retFourSons->setK2(mid->Key());
		retFourSons->setK3(child->Key());
		retFourSons->setK4(right->Key());
	}
	// ������ #3: ����� - ����� 2 � 3
	else if (newChildKey->Compare(left->Key()) > 0)
	{
		retFourSons->setLeft(left);
		retFourSons->setMid(child);
		retFourSons->setRight(mid);
		retFourSons->setBiggest(right);
		retFourSons->setK1(left->Key());
		retFourSons->setK2(child->Key());
		retFourSons->setK3(mid->Key());
		retFourSons->setK4(right->Key());
	}
	else
	{
		retFourSons->setLeft(child);
		retFourSons->setMid(left);
		retFourSons->setRight(mid);
		retFourSons->setBiggest(right);
		retFourSons->setK1(child->Key());
		retFourSons->setK2(left->Key());
		retFourSons->setK3(mid->Key());
		retFourSons->setK4(right->Key());
	}

	if (parent != root)
	{
		NotLeafNode* FatherOfParent = (NotLeafNode*)parent->getParent(); // ���� retFourSons (= parent, �� � 4 ���������)

		retFourSons->SetParent(FatherOfParent);

		if (FatherOfParent->getLeft() == parent) // �������� - ����� ���
			FatherOfParent->setLeft(retFourSons);
		else if (FatherOfParent->getMid() == parent) // �������� - ������� ���
			FatherOfParent->setMid(retFourSons);
		else if (FatherOfParent->getRight() == parent) // �������� - ������ ���
			FatherOfParent->setRight(retFourSons);

		Update(FatherOfParent);
	}

	return retFourSons;
}
template <typename T>
void Dictionary<T>::freeAllElementsHelper(Node* node)
{
	if (node == NULL)
		return;
	if (node->IsLeaf())
	{
		LeafNode<T>* leaf = (LeafNode<T>*)node;

		delete leaf->Data(); // ���������� Template'� ������� ���� (StringKey)
		delete leaf;
	}
	else
	{
		NotLeafNode* notLeaf = (NotLeafNode*)node;

		// ������� ��� ������, �������� � ������� ������� ����������
		freeAllElementsHelper(notLeaf->getLeft());
		freeAllElementsHelper(notLeaf->getMid());
		freeAllElementsHelper(notLeaf->getRight());

		if (notLeaf == root)
		{
			root = NULL;
		}

		notLeaf->setLeft(NULL);
		notLeaf->setMid(NULL);
		notLeaf->setRight(NULL);
		delete notLeaf;
	}
}
template <typename T>
void Dictionary<T>::Update(NotLeafNode* node)
{
	if (node == NULL)
		return;
	else
	{
		if (node->getLeft()->IsLeaf()) // ���� ������� - ������
		{
			// ���������� �� ���� ��� ���� �������
			node->setK1(((LeafNode<T>*)node->getLeft())->Key());

			if (node->getMid() != NULL)
			{
				node->setK2(((LeafNode<T>*)node->getMid())->Key());
			}
			else
			{
				node->setK2(NULL);
			}

			if (node->getRight() != NULL)
			{
				node->setK3(((LeafNode<T>*)node->getRight())->Key());
			}
			else
			{
				node->setK3(NULL);
			}

			node = ((NotLeafNode*)node->getParent());
		}

		// ����������, �� �����
		while (node != NULL)
		{
			node->setK1(((NotLeafNode*)node->getLeft())->getK1());

			if (node->getMid() != NULL)
			{
				node->setK2(((NotLeafNode*)node->getMid())->getK1());
			}
			else
			{
				node->setK2(NULL);
			}

			if (node->getRight() != NULL)
			{
				node->setK3(((NotLeafNode*)node->getRight())->getK1());
			}
			else
			{
				node->setK3(NULL);
			}

			if (node == root)
			{
				node = NULL;
			}
			else
			{
				node = ((NotLeafNode*)node->getParent());
			}
		}
	}
}
template <typename T>
NotLeafNode* Dictionary<T>::FindParent(StringKey* k)
{
	Node* node = root;
	NotLeafNode* notLeaf = (NotLeafNode*)node;
	NotLeafNode* parent = NULL;

	while (!node->IsLeaf())
	{
		notLeaf = (NotLeafNode*)node;
		parent = notLeaf;

		// k >= ��� k3
		if ((notLeaf->getRight() != NULL) && (k->Compare(notLeaf->getK3()) >= 0))
		{
			node = notLeaf->getRight();
		}
		else if (k->Compare(notLeaf->getK2()) >= 0) // k ����� k2 � k3
		{
			node = notLeaf->getMid();
		}
		else // k ����� k1 � k2
			node = notLeaf->getLeft();
	}

	return parent;
}
template <typename T>
void Dictionary<T>::print23(ofstream& outputFile, Node* node)
{
	if (node == NULL)
		return;
	if (node->IsLeaf())
	{
		LeafNode<T>* leaf = (LeafNode<T>*)node;
		outputFile << *(leaf->Data()) << endl;
		outputFile.flush();
	}
	else
	{
		NotLeafNode* notLeaf = (NotLeafNode*)node;

		print23(outputFile, notLeaf->getLeft());
		print23(outputFile, notLeaf->getMid());
		print23(outputFile, notLeaf->getRight());
	}
}
template <typename T>
void Dictionary<T>::print23Console(Node* node)
{
	if (node == NULL)
		return;

	if (node->IsLeaf())
	{
		LeafNode<T>* leaf = (LeafNode<T>*)node;
		cout << *(leaf->Data()) << endl;
	}
	else
	{
		NotLeafNode* notLeaf = (NotLeafNode*)node;

		print23Console(notLeaf->getLeft());
		print23Console(notLeaf->getMid());
		print23Console(notLeaf->getRight());
	}
}
template <typename T>
void Dictionary<T>::counterkey(Node* node, int& count_k)
{
	if (node == NULL)
		return;
	if (node->IsLeaf())
		count_k++;
	else
	{
		NotLeafNode* notLeaf = (NotLeafNode*)node;

		counterkey(notLeaf->getLeft(), count_k);
		counterkey(notLeaf->getMid(), count_k);
		counterkey(notLeaf->getRight(), count_k);
	}
}
template <typename T>
int Dictionary<T>::HeightOfTree(Node* node)
{
	if (node == NULL)
		return 0;

	NotLeafNode* notLeaf = (NotLeafNode*)node;

	int left, mid, right;
	if (!node->IsLeaf()) {
		left = HeightOfTree(notLeaf->getLeft());
	}
	else
		left = -1;

	if (!node->IsLeaf()) {
		right = HeightOfTree(notLeaf->getRight());
	}
	else
		right = -1;

	if (!node->IsLeaf()) {
		mid = HeightOfTree(notLeaf->getRight());
	}
	else
		mid = -1;
	int max1 = left > mid ? left : mid;
	int max2 = mid > right ? mid : right;
	int max = max1 > max2 ? max1 : max2;

	return max + 1;
}
template <typename T>
int Dictionary<T>::Insert(StringKey* k, Template<T>* d)
{
	if (IsEmpty())
	{
		root = new LeafNode<T>(d);
	}
	else
	{
		if (Find(k) != NULL)  // ���� k ���� ������, �� ������ ��� ����������
			return 0;
		else
		{
			LeafNode<T>* newleaf = new LeafNode<T>(d); //  ����� ����

			if (root->IsLeaf()) // ���� ������ ���� ������� � ������
			{
				LeafNode<T>* leafRoot = (LeafNode<T>*)root;
				NotLeafNode* newRoot = new NotLeafNode();

				// leafRoot >= new
				if (leafRoot->Key()->Compare(newleaf->Key()) > 0)
				{
					newRoot->setLeft(newleaf);
					newRoot->setK1(newleaf->Key());
					newRoot->setMid(leafRoot);
					newRoot->setK2(leafRoot->Key());
				}
				else // new > leafRoot
				{
					newRoot->setLeft(leafRoot);
					newRoot->setK1(leafRoot->Key());
					newRoot->setMid(newleaf);
					newRoot->setK2(newleaf->Key());
				}

				root = newRoot; //��������� ������
			}
			else // ������ �� ����
			{
				NotLeafNode* parent = FindParent(newleaf->Key());

				// ���� � �������� 2-�� �����
				if (parent->getRight() == NULL)
				{
					LeafNode<T>* left = (LeafNode<T>*)parent->getLeft();
					LeafNode<T>* mid = (LeafNode<T>*)parent->getMid();

					if (newleaf->Key()->Compare(mid->Key()) > 0) // ����� ���� - ������ ���
					{
						parent->setRight(newleaf);
						parent->setK3(newleaf->Key());
						// �� ����� ��������� k2 � k3 - ����� "��������"
					}
					else if (newleaf->Key()->Compare(left->Key()) > 0) // ����� ���� - ������� ���
					{
						parent->setRight(mid);
						parent->setK3(mid->Key());
						parent->setMid(newleaf);
						parent->setK2(newleaf->Key());
						// �� ����� ��������� k2 � k3 - ����� "��������"
					}
					else if (newleaf->Key()->Compare(left->Key()) < 0) // ����� ���� - ����� ���
					{
						parent->setRight(mid);
						parent->setK3(mid->Key());
						parent->setMid(left);
						parent->setK2(left->Key());
						parent->setLeft(newleaf);
						parent->setK1(newleaf->Key());
					}

					Update(parent); // ������ ���� ����� "��������" �� �����
				}
				else // ���� � �������� 3-�� �����
				{
					TempNode* temp = MakeTempNode(parent, newleaf);

					parent->setLeft(NULL);
					parent->setMid(NULL);
					parent->setRight(NULL);
					delete parent; // �� ������� ���������� ���������� ���� �� �����

					AddChild(temp);
				}
			}
		}
	}

	return 1;
}
template <typename T>
Template<T>* Dictionary<T>::Delete(StringKey* k)
{
	Template<T>* retWorkerDeleted = Find(k);

	if (retWorkerDeleted == NULL)
	{
		return NULL;
	}
	else
	{
		NotLeafNode* parent = FindParent(k);

		if (parent == NULL) // ���� ��� ��������, ������� ������� ������
		{
			delete root;
			root = NULL;
		}
		else
		{
			LeafNode<T>* leafToDel = nullptr;

			// ���, �������� �������
			if (parent->getK1()->Compare(k) == 0)
			{
				leafToDel = (LeafNode<T>*)parent->getLeft();

				parent->setLeft(parent->getMid()); // �������� ��� �����
				if (parent->getRight() != NULL)
				{
					parent->setMid(parent->getRight());
					parent->setRight(NULL);
				}
				else
				{
					parent->setMid(NULL);
				}
			}
			else if (parent->getK2()->Compare(k) == 0)
			{
				leafToDel = (LeafNode<T>*)parent->getMid();

				//3 ����
				if (parent->getRight() != NULL)
				{
					parent->setMid(parent->getRight());
					parent->setRight(NULL);
				}
				//2 ����
				else
				{
					parent->setMid(NULL);
				}
			}
			else if ((parent->getRight() != NULL) && (parent->getK3()->Compare(k) == 0))
			{
				leafToDel = (LeafNode<T>*)parent->getRight();

				parent->setRight(NULL);
			}

			delete leafToDel;

			Update(parent);
			// ����������� ������������ �����
			while ((parent != NULL) && (!parent->IsLeaf()) && ((NotLeafNode*)parent->getMid() == NULL))
			{
				//��� ��������
				if (parent->getParent() == NULL)
				{
					root = parent->getLeft();

					parent->setLeft(NULL);
					parent->setMid(NULL);
					parent->setRight(NULL);
					delete parent;

					if (!root->IsLeaf())
					{
						parent = (NotLeafNode*)root; // ������ ��������� ��������
					}
					else
					{
						parent = NULL;
					}
				}
				else // ���� �������� ��� ��������
				{
					NotLeafNode* grandpa = (NotLeafNode*)parent->getParent();

					NotLeafNode* leftSibling = (NotLeafNode*)grandpa->getLeft();
					NotLeafNode* midSibling = (NotLeafNode*)grandpa->getMid();
					NotLeafNode* rightSibling = (NotLeafNode*)grandpa->getRight();


					if (leftSibling == parent)
					{
						//������� ������ ����
						if (midSibling->getRight() != NULL)
						{
							leftSibling->setMid(midSibling->getLeft());

							// ����� ������ �� ��������
							midSibling->setLeft(midSibling->getMid());
							midSibling->setMid(midSibling->getRight());
							midSibling->setRight(NULL);

							Update(leftSibling);
							Update(midSibling);
							Update(rightSibling);
						}
						// ������� ������ ����� ���� �������. ���� ��� ���� � ��������� ��� �����
						else
						{
							midSibling->setRight(midSibling->getMid());
							midSibling->setMid(midSibling->getLeft());
							midSibling->setLeft(leftSibling->getLeft());

							leftSibling->setLeft(NULL);
							leftSibling->setMid(NULL);
							leftSibling->setRight(NULL);
							delete leftSibling;

							grandpa->setLeft(midSibling);//����� �����
							grandpa->setMid(rightSibling);
							grandpa->setRight(NULL);

							Update(midSibling);
							Update(rightSibling);

							parent = grandpa; // ��� ��������� ��������
						}
					}
					else if (midSibling == parent)
					{
						if (leftSibling->getRight() != NULL) // ����� ����� ����� 3 ����. ������� ��� ������� ����.
						{
							midSibling->setMid(midSibling->getLeft());
							midSibling->setLeft(leftSibling->getRight());

							leftSibling->setRight(NULL);

							Update(leftSibling);
							Update(midSibling);
							Update(rightSibling);
						}
						else // ����� ������, 2-�� �������. ���� ��� ���� � ��������� ��� ������
						{
							leftSibling->setRight(midSibling->getLeft());

							midSibling->setLeft(NULL);
							midSibling->setMid(NULL);
							midSibling->setRight(NULL);
							delete midSibling;

							grandpa->setMid(rightSibling);//����� �����
							grandpa->setRight(NULL);

							Update(leftSibling);
							Update(rightSibling);
							parent = grandpa; // ��� ��������� ��������
						}
					}
					else if (rightSibling == parent)
					{
						if (midSibling->getRight() != NULL) //������� ������ ����� 3 ����. ������ ��� ������� ����.
						{
							rightSibling->setMid(rightSibling->getLeft());
							rightSibling->setLeft(midSibling->getRight());

							midSibling->setRight(NULL);

							Update(leftSibling);
							Update(midSibling);
							Update(rightSibling);
						}
						else // ������� ������ ����� 2 ����. ���� ��� ���� � ��������� ��� ������
						{
							midSibling->setRight(rightSibling->getLeft());

							rightSibling->setLeft(NULL);
							rightSibling->setMid(NULL);
							rightSibling->setRight(NULL);

							// ������ ������ ���� - ����
							delete rightSibling;

							grandpa->setRight(NULL);

							Update(midSibling);
							Update(leftSibling);

							parent = grandpa; // ��� ��������� ��������
						}
					}
				}
			}
		}
	}

	return retWorkerDeleted;
}
template <typename T>
Template<T>* Dictionary<T>::Find(StringKey* k)
{
	if (IsEmpty() || k == NULL)
	{
		return NULL;
	}

	NotLeafNode* notLeaf;
	Node* node = root;

	while (!node->IsLeaf())
	{
		notLeaf = (NotLeafNode*)node;

		// k >= k3
		if ((notLeaf->getRight() != NULL) && (k->Compare(notLeaf->getK3()) >= 0))
		{
			node = notLeaf->getRight();
		}
		// k >= k2 && k < k3
		else if (k->Compare(notLeaf->getK2()) >= 0)
		{
			node = notLeaf->getMid();
		}
		// k < k2
		else
			node = notLeaf->getLeft();
	}

	LeafNode<T>* leaf = (LeafNode<T>*)node; // ����

	if (k->Compare(leaf->Key()) == 0)
		return leaf->Data();
	else
		return NULL;
}
template <typename T>
int Dictionary<T>::CountOfKey()
{
	int count_k = 0;

	if (IsEmpty()) //���� ������� ����, �� ������� ��� ��������
		return 0;
	else
		counterkey(root, count_k);

	return count_k;
}
template <typename T>
int Dictionary<T>::CountOfHeight()
{
	int count_h = 0;

	if (IsEmpty()) //���� ������� ����, �� ������� ��� ��������
		return 0;
	else
		count_h = HeightOfTree(root);

	return count_h;
}
template <typename T>
void Dictionary<T>::FreeAllElements()
{
	freeAllElementsHelper(root); // ������ � �����
}
template <typename T>
void Dictionary<T>::PrintDictionary(ofstream& outputFile)
{
	if (IsEmpty()) // ���� ������
	{
		outputFile << "������ ������!" << endl;
		outputFile.flush();
	}
	else
		print23(outputFile, root);
}
template <typename T>
void Dictionary<T>::PrintDictionaryConsole()
{
	if (IsEmpty()) // ���� ������ 
		cout << "������ ������!" << endl;
	else
		print23Console(root);
}
template <typename T>
int Dictionary<T>::IsEmpty()
{
	return (root == NULL);
}

#endif