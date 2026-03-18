#include "my_functions.h" 

//Неупорядоченные массивы
template <typename TK, typename TV>
void NotOrderedMas<TK, TV>::Insert(const TK& Key, const TV& Val)  {
	data.push_back(make_pair(Key, Val)); //make_pair создает пару из ключа и значения
}

template <typename TK, typename TV>
void NotOrderedMas<TK,TV>::Delete(const TK& Key)  {
	for (int i = 0; i < data.size(); i++) {
		if (data[i].first == Key)
		{
			data.erase(data.begin() + i); //удаляю элемент с индексом i
			return;
		}
	}
	cout << "Элемента с ключом " << Key << " нет" << endl;
}

template <typename TK, typename TV>
const TV& NotOrderedMas<TK, TV>::Find(const TK& Key) const  {
	for (int i = 0; i < data.size(); i++) {
		if (data[i].first == Key) return data[i].second;
	}
	throw runtime_error("Такого элемента нет"); //??
}

template <typename TK, typename TV>
void NotOrderedMas<TK,TV>::PrintAll() const  {
	for (int i = 0; i < data.size(); i++) {
		cout << data[i].first << " - " << data[i].second << " ";
	}
	cout << endl;
}


//Упорядоченные массивы
template <typename TK, typename TV>
int OrderedMas<TK, TV>::SearchPlaceBin(const TK& Key) const {
	int left = 0;
	int right = data.size() - 1;
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		if (data[mid].first == Key) return mid;
		else if (data[mid].first < Key) left = mid + 1;
		else right = mid - 1;
	}
	return left;
}

template <typename TK, typename TV>
void OrderedMas<TK, TV>::Insert(const TK& Key, const TV& Val)
{
	int pos = SearchPlaceBin(Key);
	data.resize(data.size() + 1);
	for (int i = data.size() - 1; i > pos; i--)
	{
		data[i] = data[i - 1];
	}
	data[pos] = make_pair(Key, Val);
}

template <typename TK, typename TV>
void OrderedMas<TK, TV>:: Delete(const TK& Key) 
{
	int pos = SearchPlaceBin(Key);
	if (pos < data.size() && data[pos].first == Key)
	{
		data.erase(data.begin() + pos);
		return;
	}
	cout << "Элемента с ключом " << Key << " нет" << endl;
}

template <typename TK, typename TV>
const TV& OrderedMas<TK, TV>::Find(const TK& Key) const {
	int pos = SearchPlaceBin(Key);
	if (pos < data.size() && data[pos].first == Key)
	{
		return data[pos].second;
	}
	throw runtime_error("Такого элемента нет");
}

template <typename TK, typename TV>
void OrderedMas<TK, TV>::PrintAll() const {
	for (int i = 0; i < data.size(); i++) {
		cout << data[i].first << " - " << data[i].second << " ";
	}
	cout << endl;
}


//БСТ
template <typename TK, typename TV>
auto StandartTree<TK, TV>::FindMin(Node* subtree) const -> Node* {
	Node* t = subtree;
	if (t == nullptr) return nullptr;
	while (t->left != nullptr) {
		t = t->left;
	}
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
auto StandartTree<TK, TV>::FindNext(Node* x) const -> Node* { //ищем следующий по узлу
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
auto StandartTree<TK, TV>::FindBefor(Node* x) const -> Node* { //ищем предыдущий по узлу
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

template <typename TK, typename TV>
void StandartTree<TK, TV>::clear_all(Node* node) {
	if (node) {
		clear_all(node->left);
		clear_all(node->right);
		delete node;
	}
}

template <typename TK, typename TV>
void StandartTree<TK, TV>::BeautifullPrintTree(Node* node, string prefix, bool isLeft) const {
	if (node == nullptr) return;

	cout << prefix;
	cout << (isLeft ? "+-- " : "L-- ");
	cout << node->KV.first << "(" << node->KV.second << ")" << endl;

	string newPrefix = prefix + (isLeft ? "|   " : "    ");
	BeautifullPrintTree(node->left, newPrefix, true);
	BeautifullPrintTree(node->right, newPrefix, false);
}

template <typename TK, typename TV>
const TV& StandartTree<TK, TV>::Find(const TK& Key) const {
	if (root == nullptr) throw out_of_range("Дерево пусто");

	Node* t = root;
	while (t->KV.first != Key) {
		if (Key < t->KV.first) {
			if (t->left == nullptr) throw out_of_range("Нет такого узла по заданному ключу");
			else t = t->left;
		}
		else {
			if (t->right == nullptr) throw out_of_range("Нет такого узла по заданному ключу");
			else t = t->right;
		}
	}
	return t->KV.second;
}

template <typename TK, typename TV>
void StandartTree<TK, TV>::PrintAll() const {
	Node* t = FindMin(root);
	if (t == nullptr) {
		cout << "Дерево пусто" << endl;
		return;
	}
	while (t != nullptr) {
		cout << t->KV.first << " - " << t->KV.second << "   ";
		t = FindNext(t);
	}
	cout << endl;
}

template <typename TK, typename TV>
void StandartTree<TK, TV>::BeautifullPrint() const {
	if (root == nullptr) {
		cout << "Дерево пустое" << endl;
		return;
	}
	BeautifullPrintTree(root, "", true);
}

template <typename TK, typename TV>
void StandartTree<TK, TV>::Insert(const TK& key, const TV& val) {
	Node* node = new Node;
	node->KV.first = key;
	node->KV.second = val;
	node->left = nullptr;
	node->right = nullptr;

	if (root == nullptr) {
		node->parent = nullptr;
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

template <typename TK, typename TV>
void StandartTree<TK, TV>::Delete(const TK& Key) {
	Node* x = FindNodeInTree(Key);

	if (x->left == nullptr && x->right == nullptr) {
		if (x == root) {
			root = nullptr;
			delete x;
			return;
		}
		Node* par = x->parent;
		if (par->left == x) par->left = nullptr;
		else par->right = nullptr;
		delete x;
	}
	else if (x->left == nullptr && x->right != nullptr) {
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
	else if (x->left != nullptr && x->right == nullptr) {
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
	else {
		Node* y = FindMin(x->right);
		x->KV.first = y->KV.first;
		x->KV.second = y->KV.second;

		Node* par_y = y->parent;
		Node* child_y = y->right;

		if (par_y->left == y) {
			par_y->left = child_y;
		}
		else {
			par_y->right = child_y;
		}

		if (child_y != nullptr) child_y->parent = par_y;
		delete y;
	}
}