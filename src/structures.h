#pragma once

#include <iostream>
#include <vector>

using namespace std;

template <typename TK, typename TV>

class Table
{
public:
	virtual const TV& Find(const TK& Key) const = 0; //только чтение
	virtual void Delete(const TK& Key) = 0;
	virtual void Insert(const TK& Key, const TV& Val) = 0;
	virtual void PrintAll() const = 0;

};

template <typename TK, typename TV>
class NotOrderedMas : public Table<TK,TV>  //неупорядоченный динакмический массив
{
private:
	vector <pair<TK, TV>> data;
public:
	NotOrderedMas() : data() {}
	void Insert(const TK& Key, const TV& Val) override {
		data.push_back(make_pair(Key, Val)); //make_pair создает пару из ключа и значения
	}

	void Delete(const TK& Key) override {
		for (int i = 0; i < data.size(); i++) {
			if (data[i].first == Key)
			{
				data.erase(data.begin() + i); //удаляю элемент с индексом i
				return;
			}
		}
		cout << "Элемента с ключом " << Key << " нет" << endl;
	}

	const TV& Find(const TK& Key) const override {
		for (int i = 0; i < data.size(); i++) {
			if (data[i].first == Key) return data[i].second;
		}
		throw runtime_error("Такого элемента нет"); //??
	}

	void PrintAll() const override {
		for (int i = 0; i < data.size(); i++) {
			cout << data[i].first << " - " << data[i].second << " ";
		}
		cout << endl;
	}

	//Функции для тестов:
	vector<TV> TEST_GetValues() { //вернуть вектор значений
		vector<TV> values;
		values.reserve(data.size());
		for (int i = 0; i < data.size(); i++) values.push_back(data[i].second);
		return values;
	}

	vector<TK> TEST_GetKeys() { //вернуть вектор ключей
		vector<TK> keys;
		keys.reserve(data.size());
		for (int i = 0; i < data.size(); i++) keys.push_back(data[i].first);
		return keys;
	}
	int TEST_GetSize() { return data.size(); }
};

template <typename TK, typename TV>
class StandartTree : public Table<TK, TV> {
	struct Node {
		pair <TK, TV> KV; //ключ-значение
		Node* left;
		Node* right;
		Node* parent;
		Node() : left(nullptr), right(nullptr), parent(nullptr) {}
		Node(TK k, TV v, Node* l, Node* r, Node* p) : KV(k, v), left(l), right(r), parent(p) {} //добавил конструктор с параметрами в класс BST
	};
protected: 
	Node* root; //корень
	
public:
	StandartTree() : root(nullptr) {}
	~StandartTree() { clear_all(root); } //дописать 
protected:
	auto FindMin(Node* subtree) const-> Node*;
	auto FindMax(Node* subtree) const -> Node*;

	auto FindNext(Node* x) const -> Node*;
	auto FindBefor(Node* x) const -> Node*;
	void clear_all(Node* node) {
		if (node) {
			clear_all(node->left);
			clear_all(node->right);
			delete node;
		}
	}

	auto FindNodeInTree(const TK& key) const->Node*;

	void BeautifullPrintTree(Node* node = root, string prefix = "", bool isLeft = true) const  {
		if (node == nullptr) return;

		cout << prefix;
		cout << (isLeft ? "+-- " : "L-- ");
		cout << node->KV.first << "(" << node->KV.second << ")" << endl;

		// Формируем префикс для детей
		string newPrefix = prefix + (isLeft ? "|   " : "    ");

		BeautifullPrintTree(node->left, newPrefix, true);
		BeautifullPrintTree(node->right, newPrefix, false);
	}

public:
	const TV& Find(const TK& Key) const override {
		if (root == nullptr) throw out_of_range("Дерево пусто");

		Node* t = root;
		while ((t->KV).first != Key) {
			if (Key < (t->KV).first) { //КЛЮЧ < Текущего
				if (t->left == nullptr) throw out_of_range("Нет такого узла по заданному ключу");//нет того, что ищем 
				else t = t->left;
			}
			else {
				if (t->right == nullptr) throw out_of_range("Нет такого узла по заданному ключу"); //нет того, что ищем
				else t = t->right;
			}
		}
		return (t->KV).second;
	}

	void PrintAll() const override {
		Node* t = this->FindMin(root);

		if (t == nullptr) { cout << "Дерево пусто" << endl; return; }

		while (t != nullptr) {
			cout << t->KV.first << " - " << t->KV.second << "   ";
			t = this->FindNext(t);
		}
		cout << endl;
	} // до сюда вроде норм проверено

	void BeautifullPrint() const {
		if (root == nullptr) { cout << "Дерево пустое" << endl; return; }
		else BeautifullPrintTree(root, "", true);
	}

	void Insert(const TK& key, const TV& val) override {
		Node* node = new Node;
		node->KV.first = key;
		node->KV.second = val;

		if (root == nullptr) {
			node->parent = nullptr; //родитель корня нулевой или ссылается на себя?
			root = node;
		}
		else {
			Node* t = root;
			while (true) {
				if (node->KV.first < t->KV.first) {
					if (t->left == nullptr) {
						t->left = node;
						node->parent = t;
						break;
					}
					else t = t->left;
				}
				else {
					if (t->right == nullptr) {
						t->right = node;
	                    node->parent = t;
						break;
			    	}
					else t = t->right;
				}
			}
		}
	}

	void Delete(const TK& Key) override {
		Node* x = FindNodeInTree(Key);

		if (x->left == nullptr && x->right == nullptr) { // это лист
			if (x == root) { root = nullptr; delete x; return; }
			Node* par = x->parent;
			if (par->left == x) par->left = nullptr; // удаляю левый
			else par->right = nullptr; //удаляю правый лист
			delete x;
		}
		else if(x->left == nullptr && x->right != nullptr) { // удаляю узел с правым потомком
			if (x == root) {
				Node* child = x->right;
				child->parent = nullptr;
				root = child;
				delete x;
				return;
			}

			Node* par = x->parent;
			Node* child = x->right;
			child->parent = par;
			if (par->left == x) par->left = child;
			else par->right = child;
			delete x;
		}
		else if(x->left != nullptr && x->right == nullptr) { //удаляю только с левым потомком
			if (x == root) {
				Node* child = x->left;
				child->parent = nullptr;
				root = child;
				delete x;
				return;
			}

			Node* par = x->parent;
			Node* child = x->left;
			child->parent = par;
			if (par->left == x) par->left = child;
			else par->right = child;
			delete x;
		}
		else { // удаляю узел с двумя потомками
			Node* y = FindMin(x->right); //новый элемент на месте удаляемого

			x->KV.first = y->KV.first;   //
			x->KV.second = y->KV.second; //в х сохранил значения y
			
			Node* par_y = y->parent;
			Node* child_y = y->right;

			if (par_y->left == y) { // y слева
				par_y->left = child_y;
			}
			else { // y справа
				par_y->right = child_y;
			}

			if (child_y != nullptr) child_y->parent = par_y;

			delete y;
		}
	}
};

template<typename TK, typename TV>
auto StandartTree<TK,TV>::FindMin(Node* subtree) const -> Node* { //поиск минимума в поддеревее subtree
	Node* t = subtree;
	if (t == nullptr) return nullptr;

	while (t->left != nullptr) { t = t->left; }
	return t;
}

template<typename TK, typename TV>
auto StandartTree<TK, TV>::FindMax(Node* subtree) const -> Node* {
	Node* t = subtree;
	if (t == nullptr) return nullptr;

	while (t->right != nullptr) t = t->right;
	return t;
}

template<typename TK, typename TV>
auto StandartTree<TK,TV>::FindNext(Node* x) const -> Node* { //ищем следующий по узлу
	if (x == nullptr) return nullptr;

	if (this->FindMax(root) == x) return nullptr; //нет больше максимума

	if (x->right != nullptr) return this->FindMin(x->right); //если есть правый потомок, ищу в после него минимум

	else { // если нет правого потомка
		Node* cur = x; Node* par = x->parent;
		while (par != nullptr && par->left != cur) { //пока не пришли к ролителю слева
			cur = cur->parent; par = par->parent;
		}
		return par;
	}
}

template<typename TK, typename TV>
auto StandartTree<TK,TV>::FindBefor(Node* x) const -> Node* { //ищем предыдущий по узлу
	if (x == nullptr) return nullptr;

	if (x == this->FindMin(root)) return nullptr;

	if (x->left != nullptr) return this->FindMax(x->left);

	else {
		Node* cur = x; Node* par = x->parent;
		while (par->right != cur) {
			cur = cur->parent; par = par->parent;
		}
		return par;
	}
}

template<typename TK, typename TV>
auto StandartTree<TK, TV>::FindNodeInTree(const TK& key) const -> Node* { //поиск в дереве, только возращает узел
	if (root == nullptr) return nullptr;
	Node* t = root;
	while (t->KV.first != key) {
		if (key < t->KV.first) {
			if (t->left == nullptr) throw out_of_range("Нет такого узла по заданному ключу");
			else t = t->left;
		}
		else {
			if (t->right == nullptr) throw out_of_range("Нет такого узла по заданному ключу"); //нет того, что ищем
			else t = t->right;
		}
	}
	return t;
}







