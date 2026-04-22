#include "pch.h"
#include "C:\Users\Master\source\repos\Tables-and-Trees\src\structures.h"
#include "C:\Users\Master\source\repos\Tables-and-Trees\src\hash_chain.h"
#include <string>

using namespace std;
//=====================================ТЕСТЫ ДЛЯ НЕУПОРЯДОЧЕННОГО МАССИВА==================================
TEST(NotOrderedMas, Mas_is_Empty) {
    NotOrderedMas<int, string> m;
    EXPECT_EQ(m.TEST_GetSize(), 0);

    vector<string> empty = {};
    EXPECT_EQ(m.TEST_GetValues(), empty);
}

TEST(NotOrderedMas, Insert_first) {
    NotOrderedMas<int, string> m;
    m.Insert(1, "a");
    EXPECT_EQ(m.TEST_GetValues(), vector<string>{"a"});
    EXPECT_EQ(m.TEST_GetKeys(), vector<int>{1});
}

TEST(NotOrderedMas, Insert_more) {
    NotOrderedMas<int, string> m;
    m.Insert(1, "a");
    m.Insert(5, "b");
    m.Insert(11, "c");
    m.Insert(3, "d");
    m.Insert(18, "a");

    EXPECT_EQ(m.TEST_GetSize(), 5);

    vector<int> keys = { 1 , 5, 11, 3, 18 };
    EXPECT_EQ(m.TEST_GetKeys(), keys);

    vector<string> values = { "a", "b", "c", "d", "a" };
    EXPECT_EQ(m.TEST_GetValues(), values);
}

TEST(NotOrderedMas, Delete_middle_element) {
    NotOrderedMas<int, string> m;
    m.Insert(7, "a");
    m.Insert(14, "b");
    m.Insert(21, "c");
    m.Insert(28, "d");
    m.Insert(35, "e");

    m.Delete(21);

    EXPECT_EQ(m.TEST_GetSize(), 4);

    EXPECT_EQ(m.Find(7), "a");
    EXPECT_EQ(m.Find(14), "b");
    EXPECT_EQ(m.Find(28), "d");
    EXPECT_EQ(m.Find(35), "e");
    EXPECT_THROW(m.Find(21), runtime_error);

    vector<int> Keys = { 7, 14, 28, 35 };
    EXPECT_EQ(m.TEST_GetKeys(), Keys);
}

TEST(NotOrderedMas, Delete_all_elements_one_by_one) {
    NotOrderedMas<int, string> m;
    m.Insert(1, "one");
    m.Insert(2, "two");
    m.Insert(3, "three");

    m.Delete(1);
    EXPECT_EQ(m.TEST_GetSize(), 2);
    EXPECT_EQ(m.Find(2), "two");
    EXPECT_EQ(m.Find(3), "three");
    EXPECT_THROW(m.Find(1), runtime_error);

    m.Delete(2);
    EXPECT_EQ(m.TEST_GetSize(), 1);
    EXPECT_EQ(m.Find(3), "three");
    EXPECT_THROW(m.Find(2), runtime_error);

    m.Delete(3);
    EXPECT_EQ(m.TEST_GetSize(), 0);
    EXPECT_THROW(m.Find(3), runtime_error);

    vector<int> empty;
    EXPECT_EQ(m.TEST_GetKeys(), empty);
}

TEST(NotOrderedMas, Find_after_delete_and_reinsert) { //поиск после удаления и новой вставки
    NotOrderedMas<int, string> m;
    m.Insert(42, "a");
    m.Insert(99, "b");

    EXPECT_EQ(m.Find(42), "a");

    m.Delete(42);
    EXPECT_THROW(m.Find(42), runtime_error);

    m.Insert(42, "c"); //вставляю тот же ключ, но новое значение

    EXPECT_EQ(m.Find(42), "c");
    EXPECT_EQ(m.TEST_GetSize(), 2);

    vector<int> keys = { 99, 42 };
    EXPECT_EQ(m.TEST_GetKeys(), keys);
}



//===========================ТЕСТЫ ДЛЯ АВЛ-ДЕРЕВА======================================


TEST(AVLTreeTest, Empty_Tree_is_AVL) {
    AVLTree<int, string> tree;
    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());

    vector<int> keys = {};
    vector<string> values = {};
    EXPECT_EQ(tree.TEST_GetKeys(), keys);
    EXPECT_EQ(tree.TEST_GetValues(), values);
}

TEST(AVLTreeTest, One_element_in_tree) {
    AVLTree<int, string> tree;
    tree.Insert(1, "a");
    
    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());
    vector<int> keys = {1};
    vector<string> values = {"a"};

    EXPECT_EQ(tree.TEST_GetKeys(), keys);
    EXPECT_EQ(tree.TEST_GetValues(), values);
    EXPECT_EQ(tree.Find(1), "a");
}

TEST(AVLTreeTest, RR_insert) {
    AVLTree<int, string> tree;
    tree.Insert(10, "a");
    tree.Insert(20, "b");
    tree.Insert(30, "c");  // тут RR будет

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());

    vector<int> Keys = { 10, 20, 30 }; 
    vector<string> Values = { "a", "b", "c" };

    EXPECT_EQ(tree.TEST_GetKeys(), Keys);
    EXPECT_EQ(tree.TEST_GetValues(), Values);

    EXPECT_EQ(tree.TEST_GetRoot(), 20);
}

TEST(AVLTreeTest, LL_insert) {
    AVLTree<int, string> tree;
    tree.Insert(30, "a");
    tree.Insert(20, "b");
    tree.Insert(10, "c");  // LL поворот

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());

    vector<int> expectedKeys = { 10, 20, 30 };
    vector<string> expectedValues = { "c", "b", "a" };

    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);
    EXPECT_EQ(tree.TEST_GetValues(), expectedValues);

    EXPECT_EQ(tree.TEST_GetRoot(), 20);
}

TEST(AVLTreeTest, RR_DeepInTree) {
    AVLTree<int, string> tree;

    tree.Insert(50, "root");
    tree.Insert(30, "left");
    tree.Insert(80, "right");

    tree.Insert(90, "right-right1");
    tree.Insert(100, "right-right2");  // должен вызвать RR относительно 80

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());

    EXPECT_EQ(tree.Find(50), "root");
    EXPECT_EQ(tree.Find(30), "left");
    EXPECT_EQ(tree.Find(80), "right");
    EXPECT_EQ(tree.Find(90), "right-right1");
    EXPECT_EQ(tree.Find(100), "right-right2");

    vector<int> expectedKeys = { 30, 50, 80, 90, 100 };
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    // Визуально проверяем структуру
    cout << "Tree after RR deep:" << endl;
    tree.BeautifullPrint();
}

TEST(AVLTreeTest, LL_DeepInTree) {
    AVLTree<int, string> tree;

    tree.Insert(50, "root");
    tree.Insert(30, "left");
    tree.Insert(80, "right");
    tree.Insert(20, "left-left1");
    tree.Insert(10, "left-left2");  // должен вызвать LL относительно 30

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());

    EXPECT_EQ(tree.Find(50), "root");
    EXPECT_EQ(tree.Find(30), "left");
    EXPECT_EQ(tree.Find(80), "right");
    EXPECT_EQ(tree.Find(20), "left-left1");
    EXPECT_EQ(tree.Find(10), "left-left2");

    vector<int> expectedKeys = { 10, 20, 30, 50, 80 };
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    cout << "Tree after LL deep:" << endl;
    tree.BeautifullPrint();
}

TEST(AVLTreeTest, LR_DeepInTree) {
    AVLTree<int, string> tree;

    tree.Insert(50, "50");
    tree.Insert(30, "30");
    tree.Insert(80, "80");
    tree.Insert(20, "20");
    tree.Insert(40, "40");
    tree.Insert(90, "90");
    tree.Insert(35, "35");
    tree.Insert(38, "38");

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());

    EXPECT_TRUE(tree.TEST_CheckParentLinks());

    vector<int> expectedKeys = { 20, 30, 35, 38, 40, 50, 80, 90 };
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    // Проверяем значения
    EXPECT_EQ(tree.Find(38), "38");
    EXPECT_EQ(tree.Find(40), "40");
    EXPECT_EQ(tree.Find(35), "35");
}

TEST(AVLTreeTest, RL_DeepInTree) {
    AVLTree<int, string> tree;

    // Строим дерево, где RL поворот произойдёт в глубине (не в корне)
    tree.Insert(50, "root");
    tree.Insert(30, "left");
    tree.Insert(80, "right");

    // Добавляем в правое поддерево элементы, которые вызовут RL поворот
    // относительно узла 80 (не в корне)
    tree.Insert(90, "right-right");
    tree.Insert(70, "right-left");
    tree.Insert(75, "right-left-right");  // ← вызывает RL на узле 80

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());
    EXPECT_TRUE(tree.TEST_CheckParentLinks());

    // Проверяем, что все элементы на месте
    EXPECT_EQ(tree.Find(50), "root");
    EXPECT_EQ(tree.Find(30), "left");
    EXPECT_EQ(tree.Find(80), "right");
    EXPECT_EQ(tree.Find(90), "right-right");
    EXPECT_EQ(tree.Find(70), "right-left");
    EXPECT_EQ(tree.Find(75), "right-left-right");

    // Ожидаемый порядок ключей (LTR обход)
    vector<int> expectedKeys = { 30, 50, 70, 75, 80, 90 };
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    // После RL поворота корнем поддерева должен стать 75
    // Проверяем это через parent-связи или структуру
    cout << "Tree after RL deep rotation:" << endl;
    tree.BeautifullPrint();
}

TEST(AVLTreeTest, Delete_Root_OnlyNode) {
    AVLTree<int, string> tree;
    tree.Insert(10, "ten");

    tree.Delete(10);

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());
    EXPECT_TRUE(tree.TEST_CheckParentLinks());

    vector<int> expectedKeys = {};
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    EXPECT_THROW(tree.Find(10), out_of_range);
}

TEST(AVLTreeTest, Delete_Root_TwoChildren_left_and_right) {
    AVLTree<int, string> tree;
    tree.Insert(20, "twenty");
    tree.Insert(10, "ten");
    tree.Insert(30, "thirty");

    tree.Delete(20);

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());
    EXPECT_TRUE(tree.TEST_CheckParentLinks());

    // После удаления корня на его место должен встать следующий по порядку (30)
    // или минимальный из правого поддерева
    vector<int> expectedKeys = { 10, 30 };
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    EXPECT_THROW(tree.Find(20), out_of_range);
    EXPECT_EQ(tree.Find(10), "ten");
    EXPECT_EQ(tree.Find(30), "thirty");
}

TEST(AVLTreeTest, Delete_and_RR) {
    AVLTree<int, string> tree;

    // Строим дерево, где после удаления из левого поддерева
    // у корня возникает balance = 2, и правый ребёнок имеет balance >= 0
    tree.Insert(10, "10");
    tree.Insert(5, "5");
    tree.Insert(15, "15");
    tree.Insert(12, "12");
    tree.Insert(18, "18");

    // Удаляем из левого поддерева (узел 5)
    // У корня 10: левое поддерево пусто, правое высоты 2 → balance = 2
    // Правый ребёнок 15 имеет balance = 0 → нужен RR поворот
    tree.Delete(5);

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());
    EXPECT_TRUE(tree.TEST_CheckParentLinks());

    // После RR поворота новым корнем должен стать 15
    EXPECT_EQ(tree.TEST_GetRoot(), 15);

    vector<int> expectedKeys = { 10, 12, 15, 18 };
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    // Проверяем значения
    EXPECT_EQ(tree.Find(10), "10");
    EXPECT_EQ(tree.Find(12), "12");
    EXPECT_EQ(tree.Find(15), "15");
    EXPECT_EQ(tree.Find(18), "18");

    cout << "Tree after delete causing RR rotation:" << endl;
    tree.BeautifullPrint();
}

TEST(AVLTreeTest, Delete_and_LL) {
    AVLTree<int, string> tree;

    // Строим дерево, где после удаления из правого поддерева
    // у корня возникает balance = -2, и левый ребёнок имеет balance <= 0
    tree.Insert(20, "20");
    tree.Insert(10, "10");
    tree.Insert(30, "30");
    tree.Insert(5, "5");
    tree.Insert(15, "15");

    // Удаляем из правого поддерева (узел 30)
    // У корня 20: правое поддерево пусто, левое высоты 2 → balance = -2
    // Левый ребёнок 10 имеет balance = 0 → нужен LL поворот
    tree.Delete(30);

    EXPECT_TRUE(tree.TEST_tree_is_AVL_from_root());
    EXPECT_TRUE(tree.TEST_CheckParentLinks());

    // После LL поворота новым корнем должен стать 10
    EXPECT_EQ(tree.TEST_GetRoot(), 10);

    vector<int> expectedKeys = { 5, 10, 15, 20 };
    EXPECT_EQ(tree.TEST_GetKeys(), expectedKeys);

    // Проверяем значения
    EXPECT_EQ(tree.Find(20), "20");
    EXPECT_EQ(tree.Find(10), "10");
    EXPECT_EQ(tree.Find(5), "5");
    EXPECT_EQ(tree.Find(15), "15");

    cout << "Tree after delete causing LL rotation:" << endl;
    tree.BeautifullPrint();
}



//======================Тесты для хэш таблицы===============
TEST(HashTableChainTest, Create_empty_table) {
    HashTableChain<int, string> table;
    EXPECT_EQ(table.TEST_GetCount(), 0);
    EXPECT_THROW(table.Find(12), out_of_range);
    table.PrintAll();
}

TEST(HashTableChainTest, Insert_1) {
    HashTableChain<int, string> table;
    table.Insert(5, "a");
    table.Insert(10, "b");
    table.Insert(192, "c");

    EXPECT_EQ(table.TEST_GetCount(), 3);
    EXPECT_EQ(table.Find(5), "a");
    EXPECT_EQ(table.Find(10), "b");
    EXPECT_EQ(table.Find(192), "c");
    EXPECT_THROW(table.Find(12), out_of_range);
    table.PrintAll();
}

TEST(HashTableChainTest, Delete_1) {
    HashTableChain<int, string> table;
    table.Insert(5, "a");
    table.Insert(10, "b");
    table.Insert(192, "c");

    EXPECT_EQ(table.TEST_GetCount(), 3);

    table.Delete(10);

    EXPECT_EQ(table.TEST_GetCount(), 2);
    EXPECT_EQ(table.Find(5), "a");
    EXPECT_EQ(table.Find(192), "c");
    EXPECT_THROW(table.Find(10), out_of_range);
    table.PrintAll();
}
TEST(HashTableChainTest, Delete_First_Element_In_Chain) {
    HashTableChain<int, string> table;
    table.Insert(5, "a");
    table.Insert(10, "b");    // если 5 и 10 попадают в одну корзину
    table.Insert(15, "c");

    table.Delete(5);  // удаляем голову списка

    EXPECT_EQ(table.TEST_GetCount(), 2);
    EXPECT_THROW(table.Find(5), out_of_range);
    EXPECT_EQ(table.Find(10), "b");
    EXPECT_EQ(table.Find(15), "c");
    table.PrintAll();
}

TEST(HashTableChainTest, Delete_Last_Element_In_Chain) {
    HashTableChain<int, string> table;
    table.Insert(5, "a");
    table.Insert(10, "b");
    table.Insert(15, "c");

    table.Delete(15);  // удаляем хвост списка

    EXPECT_EQ(table.TEST_GetCount(), 2);
    EXPECT_EQ(table.Find(5), "a");
    EXPECT_EQ(table.Find(10), "b");
    EXPECT_THROW(table.Find(15), out_of_range);
    table.PrintAll();
}
TEST(HashTableChainTest, Delete_All_Elements_One_By_One) {
    HashTableChain<int, string> table;
    table.Insert(1, "a");
    table.Insert(2, "b");
    table.Insert(3, "c");

    table.Delete(2);
    EXPECT_EQ(table.TEST_GetCount(), 2);
    EXPECT_THROW(table.Find(2), out_of_range);

    table.Delete(1);
    EXPECT_EQ(table.TEST_GetCount(), 1);
    EXPECT_THROW(table.Find(1), out_of_range);

    table.Delete(3);
    EXPECT_EQ(table.TEST_GetCount(), 0);
    EXPECT_THROW(table.Find(3), out_of_range);

    table.PrintAll();
}

TEST(HashTableChainTest, Complex_Operations_With_Small_M) {
    // Создаём таблицу с M=5, чтобы было много коллизий
    HashTableChain<int, string> table(5);

    // Много вставок (все ключи будут иметь коллизии, т.к. 5,10,15,20... % 5 = 0)
    for (int i = 1; i <= 20; i++) {
        table.Insert(i, "value_" + to_string(i));
    }
    EXPECT_EQ(table.TEST_GetCount(), 20);

    // Проверяем, что все элементы на месте
    for (int i = 1; i <= 20; i++) {
        EXPECT_EQ(table.Find(i), "value_" + to_string(i));
    }

    // Удаляем каждый второй элемент
    for (int i = 2; i <= 20; i += 2) {
        table.Delete(i);
    }
    EXPECT_EQ(table.TEST_GetCount(), 10);

    // Проверяем, что чётные удалены, нечётные остались
    for (int i = 1; i <= 20; i++) {
        if (i % 2 == 0) {
            EXPECT_THROW(table.Find(i), out_of_range);
        }
        else {
            EXPECT_EQ(table.Find(i), "value_" + to_string(i));
        }
    }

    // Обновляем значения у оставшихся
    for (int i = 1; i <= 20; i += 2) {
        table.Insert(i, "updated_" + to_string(i));
    }

    // Проверяем обновление
    for (int i = 1; i <= 20; i += 2) {
        EXPECT_EQ(table.Find(i), "updated_" + to_string(i));
    }

    // Вставляем новые элементы с другими ключами
    table.Insert(100, "hundred");
    table.Insert(101, "one_o_one");
    table.Insert(102, "one_o_two");
    EXPECT_EQ(table.TEST_GetCount(), 13);

    // Удаляем всё подряд
    for (int i = 1; i <= 20; i += 2) {
        table.Delete(i);
    }
    table.Delete(100);
    table.Delete(101);
    table.Delete(102);

    EXPECT_EQ(table.TEST_GetCount(), 0);
    EXPECT_THROW(table.Find(1), out_of_range);

    table.PrintAll();
}
