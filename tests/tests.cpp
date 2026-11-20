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

TEST_F(BinarySearchTreeTest, MapTransformsValues) {
    auto mapped = tree.Map<int>([](const int&, const int& value) { return value * 10; });
    EXPECT_EQ(Traversed(mapped, TraversalOrder::LeftRootRight), (std::vector<int>{10, 20, 30, 40, 50, 60, 70}));
}

TEST_F(BinarySearchTreeTest, MapKeepsKeysAndOrder) {
    auto mapped = tree.Map<int>([](const int& key, const int&) { return key + 1; });
    EXPECT_EQ(Traversed(mapped, TraversalOrder::LeftRootRight), (std::vector<int>{2, 3, 4, 5, 6, 7, 8}));
}

TEST_F(BinarySearchTreeTest, ReduceSumsValues) {
    int sum = tree.Reduce<int>([](int acc, const int&, const int& value) { return acc + value; }, 0);
    EXPECT_EQ(sum, 28);
}

TEST_F(BinarySearchTreeTest, ReduceWithInitial) {
    int sum = tree.Reduce<int>([](int acc, const int&, const int& value) { return acc + value; }, 100);
    EXPECT_EQ(sum, 128);
}

TEST_F(BinarySearchTreeTest, WhereKeepsMatching) {
    auto filtered = tree.Where([](const int&, const int& value) { return value % 2 == 0; });
    EXPECT_EQ(Traversed(filtered, TraversalOrder::LeftRootRight), (std::vector<int>{2, 4, 6}));
}

TEST_F(BinarySearchTreeTest, WhereKeepsNothing) {
    auto filtered = tree.Where([](const int&, const int&) { return false; });
    EXPECT_TRUE(filtered.IsEmpty());
}

TEST_F(BinarySearchTreeTest, EnumeratorInOrder) {
    auto it = tree.GetEnumerator(TraversalOrder::LeftRootRight);
    std::vector<int> result;
    while (it.Next()) result.push_back(it.GetCurrentValue());
    EXPECT_EQ(result, (std::vector<int>{1, 2, 3, 4, 5, 6, 7}));
}

TEST_F(BinarySearchTreeTest, EnumeratorHasNextAndTryGetCurrent) {
    auto it = tree.GetEnumerator();
    EXPECT_TRUE(it.HasNext());
    EXPECT_TRUE(it.Next());
    int key = 0, value = 0;
    EXPECT_TRUE(it.TryGetCurrent(key, value));
    EXPECT_EQ(key, 1);
    EXPECT_EQ(value, 1);
}

TEST_F(BinarySearchTreeTest, EnumeratorThrowsBeforeStart) {
    auto it = tree.GetEnumerator();
    EXPECT_THROW(it.GetCurrentKey(), RangeError);
    EXPECT_THROW(it.GetCurrentValue(), RangeError);
}

TEST_F(BinarySearchTreeTest, EnumeratorThrowsAfterEnd) {
    auto it = tree.GetEnumerator();
    while (it.Next()) {}
    EXPECT_THROW(it.GetCurrentKey(), RangeError);
}

TEST_F(BinarySearchTreeTest, EnumeratorReset) {
    auto it = tree.GetEnumerator();
    while (it.Next()) {}
    it.Reset();
    EXPECT_TRUE(it.HasNext());
    EXPECT_TRUE(it.Next());
    EXPECT_EQ(it.GetCurrentValue(), 1);
}