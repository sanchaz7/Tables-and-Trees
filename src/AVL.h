#pragma once
#include "structures.h"

template<typename TK, typename TV>
class AVLTree : public Table<TK, TV> {
	struct AVLNode {
		pair<TK, TV> KV;
		AVLNode* left;
		AVLNode* right;
		AVLNode* parent;
		int balance;

		AVLNode() : left(nullptr), right(nullptr), parent(nullptr), balance(0) {}
		AVLNode(TK k, TV v) : KV(k, v), left(nullptr), right(nullptr), parent(nullptr), balance(0) {}
	};
	AVLNode* root;

public:
	AVLTree() : root(nullptr) {}
	~AVLTree() { clear_all(root); }

protected:
	void clear_all(AVLNode* node) {
		if (node) {
			clear_all(node->left);
			clear_all(node->right);
			delete node;
		}
	}

	auto FindMin(AVLNode* subtree) const->AVLNode*;
	auto FindMax(AVLNode* subtree) const->AVLNode*;
	auto FindNext(AVLNode* x) const->AVLNode*;
	auto FindBefore(AVLNode* x) const->AVLNode*;
	auto FindNodeInTree(const TK& key) const->AVLNode*;

	void BeautifullPrintTree(AVLNode* node, string prefix = "", bool isLeft = true) const {
		if (node == nullptr) return;

		cout << prefix;
		cout << (isLeft ? "+-- " : "L-- ");
		cout << node->KV.first << "(" << node->KV.second << ")" << endl;

		string newPrefix = prefix + (isLeft ? "|   " : "    ");
		BeautifullPrintTree(node->left, newPrefix, true);
		BeautifullPrintTree(node->right, newPrefix, false);
	}

	// RR поворот (левый поворот)
	AVLNode* RR(AVLNode* a) {
		AVLNode* b = a->right;
		AVLNode* parent = a->parent;
		int balB = b->balance;

		a->right = b->left;
		if (b->left) b->left->parent = a;
		b->left = a;
		a->parent = b;
		b->parent = parent;

		if (parent) {
			if (parent->left == a) parent->left = b;
			else parent->right = b;
		}
		else {
			root = b;
		}

		// Установка балансов по случаям
		if (balB == 1) {          // b был перевешен вправо
			a->balance = 0;
			b->balance = 0;
		}
		else {                  // balB == 0
			a->balance = 1;
			b->balance = -1;
		}
		return b;
	}

	// LL поворот (правый поворот)
	AVLNode* LL(AVLNode* a) {
		AVLNode* b = a->left;
		AVLNode* parent = a->parent;
		int balB = b->balance;

		a->left = b->right;
		if (b->right) b->right->parent = a;
		b->right = a;
		a->parent = b;
		b->parent = parent;

		if (parent) {
			if (parent->left == a) parent->left = b;
			else parent->right = b;
		}
		else {
			root = b;
		}

		if (balB == -1) {         // b был перевешен влево
			a->balance = 0;
			b->balance = 0;
		}
		else {                  // balB == 0
			a->balance = -1;
			b->balance = 1;
		}
		return b;
	}

	// LR поворот (левый-правый)
	AVLNode* LR(AVLNode* p) {
		if (!p->left) return p;
		p->left = RR(p->left);
		if (p->left) p->left->parent = p;
		return LL(p);
	}

	// RL поворот (правый-левый)
	AVLNode* RL(AVLNode* p) {
		if (!p->right) return p;
		p->right = LL(p->right);
		if (p->right) p->right->parent = p;
		return RR(p);
	}

public:
	const TV& Find(const TK& Key) const override {
		AVLNode* t = FindNodeInTree(Key);
		if (t == nullptr) throw out_of_range("Нет такого узла по заданному ключу");
		return t->KV.second;
	}

	void PrintAll() const override {
		AVLNode* t = FindMin(root);
		if (t == nullptr) {
			cout << "Дерево пусто" << endl;
			return;
		}
		while (t != nullptr) {
			cout << t->KV.first << " - " << t->KV.second << " ";
			t = FindNext(t);
		}
		cout << endl;
	}

	void BeautifullPrint() const {
		if (root == nullptr) {
			cout << "Дерево пустое" << endl;
			return;
		}
		BeautifullPrintTree(root, "", true);
	}

	void Insert(const TK& key, const TV& val) override {
		AVLNode* node = new AVLNode(key, val);

		if (root == nullptr) {
			root = node;
			return;
		}

		AVLNode* t = root;
		while (true) {
			if (key < t->KV.first) {
				if (t->left == nullptr) {
					t->left = node;
					node->parent = t;
					break;
				}
				t = t->left;
			}
			else {
				if (t->right == nullptr) {
					t->right = node;
					node->parent = t;
					break;
				}
				t = t->right;
			}
		}

		BalanceAfterInsert(node->parent, key < node->parent->KV.first);
	}

	void BalanceAfterInsert(AVLNode* node, bool GoLeft) {
		AVLNode* cur = node;
		while (cur != nullptr) {
			if (GoLeft) cur->balance--;
			else cur->balance++;

			if (cur->balance == 0) return;

			if (cur->balance == 2 || cur->balance == -2) {
				AVLNode* newRoot = nullptr;
				if (cur->balance == 2) {
					if (cur->right->balance >= 0) newRoot = RR(cur);
					else newRoot = RL(cur);
				}
				else {
					if (cur->left->balance <= 0) newRoot = LL(cur);
					else newRoot = LR(cur);
				}
				if (newRoot->parent == nullptr) root = newRoot;
				return;
			}
			if (cur->parent) GoLeft = (cur->parent->left == cur);
			cur = cur->parent;
		}
	}

	void Delete(const TK& Key) override {
		AVLNode* x = FindNodeInTree(Key);
		if (x == nullptr) return;

		AVLNode* balanceStart = nullptr;
		bool GoLeft = false;

		// Случай 1: лист
		if (x->left == nullptr && x->right == nullptr) {
			balanceStart = x->parent;
			if (balanceStart) GoLeft = (balanceStart->left == x);

			if (x == root) {
				root = nullptr;
			}
			else {
				if (x->parent->left == x) x->parent->left = nullptr;
				else x->parent->right = nullptr;
			}
			delete x;
		}
		// Случай 2: только левый потомок
		else if (x->left != nullptr && x->right == nullptr) {
			balanceStart = x->parent;
			if (balanceStart) GoLeft = (balanceStart->left == x);

			if (x == root) {
				root = x->left;
				root->parent = nullptr;
			}
			else {
				AVLNode* child = x->left;
				child->parent = x->parent;
				if (x->parent->left == x) x->parent->left = child;
				else x->parent->right = child;
			}
			delete x;
		}
		// Случай 3: только правый потомок
		else if (x->left == nullptr && x->right != nullptr) {
			balanceStart = x->parent;
			if (balanceStart) GoLeft = (balanceStart->left == x);

			if (x == root) {
				root = x->right;
				root->parent = nullptr;
			}
			else {
				AVLNode* child = x->right;
				child->parent = x->parent;
				if (x->parent->left == x) x->parent->left = child;
				else x->parent->right = child;
			}
			delete x;
		}
		// Случай 4: два потомка
		else {
			AVLNode* y = FindMin(x->right);
			x->KV = y->KV;

			AVLNode* par_y = y->parent;
			AVLNode* child_y = y->right;

			if (par_y->left == y) {
				par_y->left = child_y;
			}
			else {
				par_y->right = child_y;
			}

			if (child_y) {
				child_y->parent = par_y;
			}

			// ВСЕГДА начинаем балансировку от родителя удалённого узла y
			balanceStart = par_y;
			GoLeft = (par_y->left == y);

			delete y;
		}

		// Балансировка после удаления
		if (balanceStart) {
			BalanceAfterDelete(balanceStart, GoLeft);
		}
	}

	void BalanceAfterDelete(AVLNode* start, bool GoLeft) {
		AVLNode* cur = start;
		while (cur != nullptr) {
			// Шаг 1: обновляем баланс согласно направлению удаления
			if (GoLeft)
				cur->balance++;   // левое поддерево стало ниже
			else
				cur->balance--;   // правое поддерево стало ниже

			if (cur->balance == 0) {
				// Высота уменьшилась – поднимаемся выше
				if (cur->parent) {
					GoLeft = (cur->parent->left == cur);
					cur = cur->parent;
				}
				else {
					break;
				}
			}
			else if (cur->balance == 1 || cur->balance == -1) {
				// Высота не изменилась – конец балансировки
				break;
			}
			else {
				// Нарушение баланса (2 или -2) – поворот
				AVLNode* newSubRoot = nullptr;
				if (cur->balance == 2) {
					if (cur->right->balance >= 0) {
						newSubRoot = RR(cur);
					}
					else {
						newSubRoot = RL(cur);
					}
				}
				else { // cur->balance == -2
					if (cur->left->balance <= 0) {
						newSubRoot = LL(cur);
					}
					else {
						newSubRoot = LR(cur);
					}
				}

				// После поворота проверяем, нужно ли продолжать подъём
				if (newSubRoot->balance == 0) {
					// высота поддерева уменьшилась на 1
					cur = newSubRoot->parent;
					if (cur) {
						GoLeft = (cur->left == newSubRoot);
					}
					else {
						break;
					}
				}
				else {
					break;   // высота не изменилась
				}
			}
		}
	}

	// Тестовые методы
	TK TEST_GetRoot() {
		if (!root) throw runtime_error("Tree is empty");
		return root->KV.first;
	}

	bool TEST_Tree_is_AVL(AVLNode* node) {
		if (!node) return true;
		if (abs(node->balance) > 1) return false;
		return TEST_Tree_is_AVL(node->left) && TEST_Tree_is_AVL(node->right);
	}

	bool TEST_tree_is_AVL_from_root() {
		return TEST_Tree_is_AVL(root);
	}

	vector<TK> TEST_GetKeys() {
		vector<TK> keys;
		AVLNode* t = FindMin(root);
		while (t != nullptr) {
			keys.push_back(t->KV.first);
			t = FindNext(t);
		}
		return keys;
	}

	vector<TV> TEST_GetValues() {
		vector<TV> values;
		AVLNode* t = FindMin(root);
		while (t != nullptr) {
			values.push_back(t->KV.second);
			t = FindNext(t);
		}
		return values;
	}

	bool TEST_CheckParentLinks(AVLNode* node, AVLNode* expectedParent = nullptr) {
		if (!node) return true;
		if (node->parent != expectedParent) {
			cout << "Parent link error: node " << node->KV.first
				<< " parent=" << (node->parent ? to_string(node->parent->KV.first) : "null")
				<< " expected=" << (expectedParent ? to_string(expectedParent->KV.first) : "null") << endl;
			return false;
		}
		return TEST_CheckParentLinks(node->left, node) &&
			TEST_CheckParentLinks(node->right, node);
	}

	bool TEST_CheckParentLinks() {
		if (!root) return true;
		return TEST_CheckParentLinks(root, nullptr);
	}
};

// Реализация вспомогательных методов
template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindMin(AVLNode* subtree) const -> AVLNode* {
	if (subtree == nullptr) return nullptr;
	AVLNode* t = subtree;
	while (t->left != nullptr) t = t->left;
	return t;
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindMax(AVLNode* subtree) const -> AVLNode* {
	if (subtree == nullptr) return nullptr;
	AVLNode* t = subtree;
	while (t->right != nullptr) t = t->right;
	return t;
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindNext(AVLNode* x) const -> AVLNode* {
	if (x == nullptr) return nullptr;
	if (x->right != nullptr) return FindMin(x->right);
	AVLNode* cur = x;
	AVLNode* par = x->parent;
	while (par != nullptr && par->right == cur) {
		cur = par;
		par = par->parent;
	}
	return par;
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindBefore(AVLNode* x) const -> AVLNode* {
	if (x == nullptr) return nullptr;
	if (x->left != nullptr) return FindMax(x->left);
	AVLNode* cur = x;
	AVLNode* par = x->parent;
	while (par != nullptr && par->left == cur) {
		cur = par;
		par = par->parent;
	}
	return par;
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindNodeInTree(const TK& key) const -> AVLNode* {
	AVLNode* t = root;
	while (t != nullptr && t->KV.first != key) {
		if (key < t->KV.first) t = t->left;
		else t = t->right;
	}
	return t;
}

