#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ============ БАЗОВЫЙ АБСТРАКТНЫЙ КЛАСС ============
template <typename TK, typename TV>
class Table
{
public:
    virtual const TV& Find(const TK& Key) const = 0;
    virtual void Delete(const TK& Key) = 0;
    virtual void Insert(const TK& Key, const TV& Val) = 0;
    virtual void PrintAll() const = 0;
    virtual ~Table() {} // Виртуальный деструктор
};

// ============ КЛАСС НЕУПОРЯДОЧЕННОГО МАССИВА ============
template <typename TK, typename TV>
class NotOrderedMas : public Table<TK, TV>
{
private:
    vector<pair<TK, TV>> data;
public:
    NotOrderedMas() : data() {}
    void Insert(const TK& Key, const TV& Val) override;
    void Delete(const TK& Key) override;
    const TV& Find(const TK& Key) const override;
    void PrintAll() const override;
};

template <typename TK, typename TV>
class OrderedMas : public Table<TK, TV>  //упорядоченный динакмический массив
{
private:
    vector <pair<TK, TV>> data;
public:
    OrderedMas() : data() {}

    int SearchPlaceBin(const TK& Key) const;
    void Insert(const TK& Key, const TV& Val) override;
    void Delete(const TK& Key) override;
    const TV& Find(const TK& Key) const override;
    void PrintAll() const override;
};


// ============ КЛАСС ОБЫЧНОГО БИНАРНОГО ДЕРЕВА ============
template <typename TK, typename TV>
class StandartTree : public Table<TK, TV>
{
private:
    struct Node {
        pair<TK, TV> KV;
        Node* left;
        Node* right;
        Node* parent;
        Node() : left(nullptr), right(nullptr), parent(nullptr) {}
        Node(TK k, TV v, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
            : KV(k, v), left(l), right(r), parent(p) {}
    };

    Node* root;

    // Приватные вспомогательные методы
    Node* FindMin(Node* subtree) const;
    Node* FindMax(Node* subtree) const;
    Node* FindNext(Node* x) const;
    Node* FindBefor(Node* x) const;
    void clear_all(Node* node);
    Node* FindNodeInTree(const TK& key) const;
    void BeautifullPrintTree(Node* node, string prefix = "", bool isLeft = true) const;

public:
    StandartTree() : root(nullptr) {}
    ~StandartTree() { clear_all(root); }

    const TV& Find(const TK& Key) const override;
    void Delete(const TK& Key) override;
    void Insert(const TK& Key, const TV& Val) override;
    void PrintAll() const override;
    void BeautifullPrint() const;
};

// ============ КЛАСС АВЛ ДЕРЕВА ============
template <typename TK, typename TV>
class AVLTree : public Table<TK, TV>
{
private:
    struct AVLNode {
        pair<TK, TV> KV;
        AVLNode* left;
        AVLNode* right;
        AVLNode* parent;
        int balance;

        AVLNode() : left(nullptr), right(nullptr), parent(nullptr), balance(0) {}
        AVLNode(TK k, TV v, AVLNode* l = nullptr, AVLNode* r = nullptr,
            AVLNode* p = nullptr)
            : KV(k, v), left(l), right(r), parent(p), balance(0) {}
    };

    AVLNode* root;

    // Приватные вспомогательные методы
    void clear_all(AVLNode* node);
    AVLNode* FindMin(AVLNode* subtree) const;
    AVLNode* FindMax(AVLNode* subtree) const;
    AVLNode* FindNext(AVLNode* x) const;
    AVLNode* FindPrev(AVLNode* x) const;
    AVLNode* FindNodeInTree(const TK& key) const;
    void BeautifullPrintTree(AVLNode* node, string prefix = "", bool isLeft = true) const;

    // Методы балансировки
    AVLNode* RR(AVLNode* p);
    AVLNode* LL(AVLNode* p);
    AVLNode* RL(AVLNode* p);
    AVLNode* LR(AVLNode* p);
    void BalanceAfterInsert(AVLNode* node, bool GoLeft);
    void BalanceAfterDelete(AVLNode* balanceStart, bool GoLeft);
    void updateBalancesAfterRL(AVLNode* p, AVLNode* x, AVLNode* y, int OriginalBalY);
    void updateBalancesAfterLR(AVLNode* p, AVLNode* x, AVLNode* y, int OriginalBalY);

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear_all(root); }

    const TV& Find(const TK& Key) const override;
    void Delete(const TK& Key) override;
    void Insert(const TK& Key, const TV& Val) override;
    void PrintAll() const override;
    void BeautifullPrint() const;
};
