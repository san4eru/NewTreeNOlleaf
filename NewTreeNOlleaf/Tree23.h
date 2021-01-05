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

	int IsLeaf(); // вернёт 1, если узел является листом, и 0, если нет
	void SetParent(Node* p); // устанавливает родителя
	Node* getParent(); // возвращает родителя
};

class StringKey
{
private:
	string strKey;

public:
	StringKey() {}
	StringKey(const string& strString); // Конструктор, который получает строку
	StringKey(const StringKey& otherStringKey); // Конструктор копирования
	~StringKey() {}

	int Compare(const StringKey* s)const; //алфавитное сравнение между 2-мя строками
	string getString()const; // Возвращвет strKey
	static StringKey* Read(ifstream& inputFile); // Чтение ключа из файла. Вернет NULL, если при чтении произошла ошибка
	static StringKey* Readfromkeyboard(string new_k); // Чтение ключа с клавиатуры
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
	Template(const Template<T>& otherData); // Конструктор копирования. Выдает исключение, если ключ другого объекта является нулевым
	~Template() { delete key; }

	StringKey* GetKey()const; // Возвращает ключ
	T getdata(); // Возвращает значение

	static Template* Read(ifstream& inputFile); // Чтение ячейки из файла. Вернет NULL, если произошла ошибка чтения
	static Template* Readfromkeyboard(string new_k, T new_value); // Чтение ячейки с клавиатуры
	friend ostream& operator << (ostream& out, const Template<T>& data) // перегрузка operator << для вывода
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

	//set'ы и get'ы
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
	StringKey* k4; // минимум "самого большого"
	Node* biggest; // 4-тый ребенок, который больше, чем его "правый ребенок"

public:
	TempNode() : k4(NULL), biggest(NULL) {}
	~TempNode(void) {}

	// set'ы и get'ы
	void setK4(StringKey* k); // устанавливает k4
	void setBiggest(Node* node); // устанавливает самого большого ребенка

	StringKey* getK4(); // возвращает k4
	Node* getBiggest(); // возвращает самого большого
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

	Template<T>* Data()const; // Возвращает данные текущего узла
	StringKey* Key()const; // Возвращает ключ текущий узла
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

	void AddChild(TempNode* trans); // добавляет 4 словарь в словарь и разделяет его так, чтобы он действовал
	TempNode* MakeTempNode(NotLeafNode* parent, LeafNode<T>* child); // создает временный узел 4

	void freeAllElementsHelper(Node* node); // освобождает все элементы рекурсивно
	void Update(NotLeafNode* node); // обновляет k1, k2, k3 узел до корня

	NotLeafNode* FindParent(StringKey* key); // находит родителя узла

	void print23(ofstream& outputFile, Node* node); // рекурсивно печатает в правильном порядке
	void print23Console(Node* node); // рекурсивно выводит в консоль
	void counterkey(Node* node, int& count_k); // рекурсивно считает кол-во ключей
	int HeightOfTree(Node* node); // рекурсивно считает высоту дерева

public:
	Dictionary() : root(0) {}
	~Dictionary() { delete root; }// удалит левый, средний и правый узлы рекурсивно
	
	int Insert(StringKey* k, Template<T>* d); // Вставляет новый узел
	Template<T>* Delete(StringKey* k); // Удаляет узел по ключу
	Template<T>* Find(StringKey* k); // Находит узел по ключу

	int CountOfKey(); // Находит количество ключей
	int CountOfHeight(); // Находит высоту дерева

	void FreeAllElements(); // Освобождает все элементы в словаре

	void PrintDictionary(ofstream& outputFile); // Печатает словарь в алфавитном порядке в файл
	void PrintDictionaryConsole(); // Выводит словарь в алфавитном порядке в консоль

	int IsEmpty(); // Проверяет, является ли словарь пустым, если пусто, вернет 1 (true), иначе вернет 0 (false)
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
	else // переходной узел - не корень
	{
		NotLeafNode* parent = (NotLeafNode*)trans->getParent();

		if (parent->getRight() == NULL) // если родитель имеет 2-ух детей
		{
			if (trans == parent->getLeft()) // узел 4-сына является наименьшим из двух
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

			// обновление k1 k2 k3 до корня
			Update(parent);
		}
		else // если родитель уже имеет 3-ех детей, то делаем 4-ого сына - переходным узлом
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

			if (FatherOfParent != NULL) // если родитель не корень.
			{
				if (FatherOfParent->getLeft() == parent) // родитель - левый сын
					FatherOfParent->setLeft(tmpFather);
				else if (FatherOfParent->getMid() == parent) // родитель - средний сын
					FatherOfParent->setMid(tmpFather);
				else if (FatherOfParent->getRight() == parent) // родитель - правый сын
					FatherOfParent->setRight(tmpFather);

				Update(FatherOfParent); // обновление отца нашего переходного узла до корня
			}
			else // если родитель был корнем
			{
				root = tmpFather;
			}

			parent->setLeft(NULL);
			parent->setMid(NULL);
			parent->setRight(NULL);
			delete parent; // поменяли родителя с нашим новым tmpFather

			trans->setLeft(NULL);
			trans->setMid(NULL);
			trans->setRight(NULL);
			trans->setBiggest(NULL);
			delete trans; // переходной узла был заменен двумя сыновьями

			AddChild(tmpFather);
		}
	}
}
template <typename T>
TempNode* Dictionary<T>::MakeTempNode(NotLeafNode* parent, LeafNode<T>* child)
{
	TempNode* retFourSons = new TempNode(); // будет вместо родителя

	LeafNode<T>* right = (LeafNode<T>*)parent->getRight();
	LeafNode<T>* left = (LeafNode<T>*)parent->getLeft();
	LeafNode<T>* mid = (LeafNode<T>*)parent->getMid();

	StringKey* newChildKey = child->Key();

	// случай #1: Новый - самый большой
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
	// случай #2: Новый - между 3 и 4
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
	// случай #3: Новый - между 2 и 3
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
		NotLeafNode* FatherOfParent = (NotLeafNode*)parent->getParent(); // отец retFourSons (= parent, но с 4 сыновьями)

		retFourSons->SetParent(FatherOfParent);

		if (FatherOfParent->getLeft() == parent) // родитель - левый сын
			FatherOfParent->setLeft(retFourSons);
		else if (FatherOfParent->getMid() == parent) // родитель - средний сын
			FatherOfParent->setMid(retFourSons);
		else if (FatherOfParent->getRight() == parent) // родитель - правый сын
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

		delete leaf->Data(); // деструктор Template'а удаляет ключ (StringKey)
		delete leaf;
	}
	else
	{
		NotLeafNode* notLeaf = (NotLeafNode*)node;

		// удаляем его левого, среднего и правого сыновей рекурсивно
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
		if (node->getLeft()->IsLeaf()) // если сыновья - листья
		{
			// Обновление на один шаг выше листьев
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

		// обновление, до корня
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

		// k >= чем k3
		if ((notLeaf->getRight() != NULL) && (k->Compare(notLeaf->getK3()) >= 0))
		{
			node = notLeaf->getRight();
		}
		else if (k->Compare(notLeaf->getK2()) >= 0) // k между k2 и k3
		{
			node = notLeaf->getMid();
		}
		else // k между k1 и k2
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
		if (Find(k) != NULL)  // Если k ключ найден, то ячейка уже существует
			return 0;
		else
		{
			LeafNode<T>* newleaf = new LeafNode<T>(d); //  Новый лист

			if (root->IsLeaf()) // если только один элемент в дереве
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

				root = newRoot; //обновляем корень
			}
			else // корень не лист
			{
				NotLeafNode* parent = FindParent(newleaf->Key());

				// если у родителя 2-ое детей
				if (parent->getRight() == NULL)
				{
					LeafNode<T>* left = (LeafNode<T>*)parent->getLeft();
					LeafNode<T>* mid = (LeafNode<T>*)parent->getMid();

					if (newleaf->Key()->Compare(mid->Key()) > 0) // новый лист - правый сын
					{
						parent->setRight(newleaf);
						parent->setK3(newleaf->Key());
						// не нужно обновлять k2 и k3 - отцов "родителя"
					}
					else if (newleaf->Key()->Compare(left->Key()) > 0) // новый лист - средний сын
					{
						parent->setRight(mid);
						parent->setK3(mid->Key());
						parent->setMid(newleaf);
						parent->setK2(newleaf->Key());
						// не нужно обновлять k2 и k3 - отцов "родителя"
					}
					else if (newleaf->Key()->Compare(left->Key()) < 0) // новый лист - левый сын
					{
						parent->setRight(mid);
						parent->setK3(mid->Key());
						parent->setMid(left);
						parent->setK2(left->Key());
						parent->setLeft(newleaf);
						parent->setK1(newleaf->Key());
					}

					Update(parent); // Обнови всех отцов "родителя" до корня
				}
				else // если у родителя 3-ое детей
				{
					TempNode* temp = MakeTempNode(parent, newleaf);

					parent->setLeft(NULL);
					parent->setMid(NULL);
					parent->setRight(NULL);
					delete parent; // мы сделали временного четвертого сына из этого

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

		if (parent == NULL) // лист был корневым, сделать словарь пустым
		{
			delete root;
			root = NULL;
		}
		else
		{
			LeafNode<T>* leafToDel = nullptr;

			// сын, которого удалили
			if (parent->getK1()->Compare(k) == 0)
			{
				leafToDel = (LeafNode<T>*)parent->getLeft();

				parent->setLeft(parent->getMid()); // сдвигаем все влево
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

				//3 сына
				if (parent->getRight() != NULL)
				{
					parent->setMid(parent->getRight());
					parent->setRight(NULL);
				}
				//2 сына
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
			// исправление неправильных узлов
			while ((parent != NULL) && (!parent->IsLeaf()) && ((NotLeafNode*)parent->getMid() == NULL))
			{
				//нет родителя
				if (parent->getParent() == NULL)
				{
					root = parent->getLeft();

					parent->setLeft(NULL);
					parent->setMid(NULL);
					parent->setRight(NULL);
					delete parent;

					if (!root->IsLeaf())
					{
						parent = (NotLeafNode*)root; // начать следующую итерацию
					}
					else
					{
						parent = NULL;
					}
				}
				else // есть родитель для родителя
				{
					NotLeafNode* grandpa = (NotLeafNode*)parent->getParent();

					NotLeafNode* leftSibling = (NotLeafNode*)grandpa->getLeft();
					NotLeafNode* midSibling = (NotLeafNode*)grandpa->getMid();
					NotLeafNode* rightSibling = (NotLeafNode*)grandpa->getRight();


					if (leftSibling == parent)
					{
						//возьмем левого сына
						if (midSibling->getRight() != NULL)
						{
							leftSibling->setMid(midSibling->getLeft());

							// сдвиг левого на середину
							midSibling->setLeft(midSibling->getMid());
							midSibling->setMid(midSibling->getRight());
							midSibling->setRight(NULL);

							Update(leftSibling);
							Update(midSibling);
							Update(rightSibling);
						}
						// средний справа имеет двух сыновей. дать ему один и поставить его слева
						else
						{
							midSibling->setRight(midSibling->getMid());
							midSibling->setMid(midSibling->getLeft());
							midSibling->setLeft(leftSibling->getLeft());

							leftSibling->setLeft(NULL);
							leftSibling->setMid(NULL);
							leftSibling->setRight(NULL);
							delete leftSibling;

							grandpa->setLeft(midSibling);//сдвиг влево
							grandpa->setMid(rightSibling);
							grandpa->setRight(NULL);

							Update(midSibling);
							Update(rightSibling);

							parent = grandpa; // для следующей итерации
						}
					}
					else if (midSibling == parent)
					{
						if (leftSibling->getRight() != NULL) // Левый слева имеет 3 сына. возьмем его правого сына.
						{
							midSibling->setMid(midSibling->getLeft());
							midSibling->setLeft(leftSibling->getRight());

							leftSibling->setRight(NULL);

							Update(leftSibling);
							Update(midSibling);
							Update(rightSibling);
						}
						else // Левый налево, 2-ое сыновей. дать ему один и поставить его справа
						{
							leftSibling->setRight(midSibling->getLeft());

							midSibling->setLeft(NULL);
							midSibling->setMid(NULL);
							midSibling->setRight(NULL);
							delete midSibling;

							grandpa->setMid(rightSibling);//сдвиг влево
							grandpa->setRight(NULL);

							Update(leftSibling);
							Update(rightSibling);
							parent = grandpa; // для следующей итерации
						}
					}
					else if (rightSibling == parent)
					{
						if (midSibling->getRight() != NULL) //Средний налево имеет 3 сына. возьми его правого сына.
						{
							rightSibling->setMid(rightSibling->getLeft());
							rightSibling->setLeft(midSibling->getRight());

							midSibling->setRight(NULL);

							Update(leftSibling);
							Update(midSibling);
							Update(rightSibling);
						}
						else // Средний налево имеет 2 сына. дать ему один и поставить его справа
						{
							midSibling->setRight(rightSibling->getLeft());

							rightSibling->setLeft(NULL);
							rightSibling->setMid(NULL);
							rightSibling->setRight(NULL);

							// сейчас правый брат - пуст
							delete rightSibling;

							grandpa->setRight(NULL);

							Update(midSibling);
							Update(leftSibling);

							parent = grandpa; // для следующей итерации
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

	LeafNode<T>* leaf = (LeafNode<T>*)node; // лист

	if (k->Compare(leaf->Key()) == 0)
		return leaf->Data();
	else
		return NULL;
}
template <typename T>
int Dictionary<T>::CountOfKey()
{
	int count_k = 0;

	if (IsEmpty()) //если словарь пуст, то возврат без подсчета
		return 0;
	else
		counterkey(root, count_k);

	return count_k;
}
template <typename T>
int Dictionary<T>::CountOfHeight()
{
	int count_h = 0;

	if (IsEmpty()) //если словарь пуст, то возврат без подсчета
		return 0;
	else
		count_h = HeightOfTree(root);

	return count_h;
}
template <typename T>
void Dictionary<T>::FreeAllElements()
{
	freeAllElementsHelper(root); // Начать с корня
}
template <typename T>
void Dictionary<T>::PrintDictionary(ofstream& outputFile)
{
	if (IsEmpty()) // если пустое
	{
		outputFile << "Дерево пустое!" << endl;
		outputFile.flush();
	}
	else
		print23(outputFile, root);
}
template <typename T>
void Dictionary<T>::PrintDictionaryConsole()
{
	if (IsEmpty()) // Если пустое 
		cout << "Дерево пустое!" << endl;
	else
		print23Console(root);
}
template <typename T>
int Dictionary<T>::IsEmpty()
{
	return (root == NULL);
}

#endif