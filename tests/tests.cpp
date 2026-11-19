#include <gtest/gtest.h>
#include <vector>
#include "../binary_tree/binary_search_tree.hpp"

namespace {
std::vector<int> Traversed(const BinarySearchTree<int, int>& tree, TraversalOrder order) {
    std::vector<int> result;
    tree.Traverse(order, [&](const int& key, const int& value) { result.push_back(value); });
    return result;
}
}

class BinarySearchTreeTest : public ::testing::Test {
    protected:
        BinarySearchTree<int, int> tree;
        void SetUp() override {
            for (int value : {4, 2, 6, 1, 3, 5, 7}) tree.Insert(value, value);
        }
};

TEST(BinarySearchTreeBasicTest, NewTreeIsEmpty) {
    BinarySearchTree<int, int> empty;
    EXPECT_TRUE(empty.IsEmpty());
    EXPECT_EQ(empty.GetCount(), 0u);
    EXPECT_FALSE(empty.Contains(1));
}

TEST_F(BinarySearchTreeTest, InsertAndContains) {
    EXPECT_EQ(tree.GetCount(), 7u);
    EXPECT_TRUE(tree.Contains(5));
    EXPECT_FALSE(tree.Contains(8));
}

TEST_F(BinarySearchTreeTest, InsertDuplicateReturnsFalse) {
    EXPECT_FALSE(tree.Insert(4, 4));
    EXPECT_EQ(tree.GetCount(), 7u);
}

TEST_F(BinarySearchTreeTest, RemoveLeaf) {
    EXPECT_TRUE(tree.Remove(1));
    EXPECT_FALSE(tree.Contains(1));
    EXPECT_EQ(tree.GetCount(), 6u);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithTwoChildren) {
    EXPECT_TRUE(tree.Remove(2));
    EXPECT_EQ(Traversed(tree, TraversalOrder::LeftRootRight), (std::vector<int>{1, 3, 4, 5, 6, 7}));
}

TEST_F(BinarySearchTreeTest, RemoveRoot) {
    EXPECT_TRUE(tree.Remove(4));
    EXPECT_EQ(Traversed(tree, TraversalOrder::LeftRootRight), (std::vector<int>{1, 2, 3, 5, 6, 7}));
}

TEST_F(BinarySearchTreeTest, RemoveMissingReturnsFalse) {
    EXPECT_FALSE(tree.Remove(100));
    EXPECT_EQ(tree.GetCount(), 7u);
}

TEST_F(BinarySearchTreeTest, FindReturnsPointerOrNull) {
    ASSERT_NE(tree.Find(3), nullptr);
    EXPECT_EQ(*tree.Find(3), 3);
    EXPECT_EQ(tree.Find(100), nullptr);
}

TEST_F(BinarySearchTreeTest, AllSixTraversals) {
    EXPECT_EQ(Traversed(tree, TraversalOrder::RootLeftRight), (std::vector<int>{4, 2, 1, 3, 6, 5, 7}));
    EXPECT_EQ(Traversed(tree, TraversalOrder::RootRightLeft), (std::vector<int>{4, 6, 7, 5, 2, 3, 1}));
    EXPECT_EQ(Traversed(tree, TraversalOrder::LeftRightRoot), (std::vector<int>{1, 3, 2, 5, 7, 6, 4}));
    EXPECT_EQ(Traversed(tree, TraversalOrder::LeftRootRight), (std::vector<int>{1, 2, 3, 4, 5, 6, 7}));
    EXPECT_EQ(Traversed(tree, TraversalOrder::RightLeftRoot), (std::vector<int>{7, 5, 6, 3, 1, 2, 4}));
    EXPECT_EQ(Traversed(tree, TraversalOrder::RightRootLeft), (std::vector<int>{7, 6, 5, 4, 3, 2, 1}));
}

TEST_F(BinarySearchTreeTest, CopyIsIndependent) {
    BinarySearchTree<int, int> copy(tree);
    copy.Remove(4);
    EXPECT_TRUE(tree.Contains(4));
    EXPECT_FALSE(copy.Contains(4));
}

TEST_F(BinarySearchTreeTest, ClearEmptiesTree) {
    tree.Clear();
    EXPECT_TRUE(tree.IsEmpty());
    EXPECT_EQ(tree.GetHeight(), 0u);
}

TEST_F(BinarySearchTreeTest, HeightIsThree) {
    EXPECT_EQ(tree.GetHeight(), 3u);
}

TEST(BinarySearchTreeRBTTest, BalancedTreeSatisfiesRedBlackProperties) {
    BinarySearchTree<int, int> rbt(true);
    for (int value : {1, 2, 3, 4, 5, 6, 7}) rbt.Insert(value, value);
    EXPECT_TRUE(rbt.IsRedBlack());
}

TEST(BinarySearchTreeRBTTest, BalanceMethodFixesUnbalancedTree) {
    BinarySearchTree<int, int> rbt;
    for (int value : {1, 2, 3, 4, 5, 6, 7}) rbt.Insert(value, value);
    EXPECT_FALSE(rbt.IsRedBlack());
    rbt.Balance();
    EXPECT_TRUE(rbt.IsRedBlack());
}

TEST(BinarySearchTreeRBTTest, BalancedTreeRemoveMaintainsProperties) {
    BinarySearchTree<int, int> rbt(true);
    for (int value : {4, 2, 6, 1, 3, 5, 7}) rbt.Insert(value, value);
    EXPECT_TRUE(rbt.IsRedBlack());
    rbt.Remove(2);
    EXPECT_TRUE(rbt.IsRedBlack());
}