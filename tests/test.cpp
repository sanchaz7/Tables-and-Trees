#include "pch.h"
#include "C:\Users\Master\source\repos\Tables-and-Trees\src\structures.h"
#include <string>

using namespace std;

TEST(AVLTreeTest, Find_5) {
    AVLTree<int, string> tree;
    tree.Insert(5, "five");
    tree.Insert(3, "three");
    tree.Insert(7, "seven");

    EXPECT_EQ(tree.Find(5), "five");
}