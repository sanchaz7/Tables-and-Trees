#pragma once
#include "structures.h"

using namespace std;

template <typename TK, typename TV>

class HashTableChain : public Table<TK,TV> {
	
	struct Node {
		pair <TK, TV> KV;
		size_t hash_v;
		Node* next;

		Node(const TK& key,const TV& value, size_t hash_value) : KV(make_pair(key, value)), hash_v(hash_value), next(nullptr) {}
	};

	Node** mas;
	size_t M;
	size_t count;

	size_t GetHash(const TK& x) const { return hash<TK>{}(x) % M; }

	

public:
	HashTableChain(size_t size_mas = 10) : M(size_mas), count(0)
	{
		mas = new Node*[size_mas](); // из-за скобок в конце все эл-ты массива - nullptr
	}
	~HashTableChain() 
	{
		Clear();
		delete[] mas;
	}
private:
	void Clear() {
		for (int i = 0; i < M; i++) {
			Node* cur = mas[i];
			while (cur != nullptr) {
				Node* delete_el = cur;
				cur = cur->next;
				delete delete_el;
			}
			mas[i] = nullptr;
		}
		count = 0;
	}
	
	Node* FindNode(const TK& Key) const { //если элемента нет, возвращает nullptr
		int index = GetHash(Key);
		Node* Head = mas[index];

		if (Head) {
			while (Head != nullptr) {
				if ((Head->KV).first == Key) {
					return Head;
				}
				Head = Head->next;
			}
		}
		return nullptr;
	}

public:
	const TV& Find(const TK& Key) const override {
		Node* node = FindNode(Key);
		if (!node) {
			throw out_of_range("Элемента по заданному ключу нет");
		}
		else {
			return node->KV.second;
		}
	}

	void Delete(const TK& Key) override {
		Node* x = FindNode(Key);
		if (!x) throw out_of_range("Нет элемента с указанным ключем");

		int index = GetHash(Key);
		Node* Head = mas[index];

		if (Head == x) {
			mas[index] = x->next;
			delete x;
			count--;
			return;
		}
		else {
			while (Head != nullptr && Head->next != x) {
				Head = Head->next;
			}
			Head->next = x->next;
			delete x;
			count--;
			return;
		}
	}

	void Insert(const TK& Key, const TV& Val) override {
		//обновляю значение, если уже есть такой ключ
		Node* repeat = FindNode(Key);
		if (repeat) {
			repeat->KV.second = Val;
			return;
		}

		int index = GetHash(Key);
		Node* Head = mas[index];

		Node* x = new Node(Key, Val, GetHash(Key));

		x->next = Head;
		mas[index] = x;

		count++;
	}

	void PrintAll() const override {
		cout << "Hash Table:" << endl;
		for (int i = 0; i < M; i++) {
			cout << "[" << i << "] ";
			
			Node* Head = mas[i];
			while (Head != nullptr) {
				cout << "(" << Head->KV.first << "-" << Head->KV.second << ") ";
				Head = Head->next;
			}
			cout << endl;
		}
	} 

public:
	size_t TEST_GetCount() const { return count; }

};