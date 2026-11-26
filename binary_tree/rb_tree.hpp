#pragma once

#include "binary_search_tree.hpp"


template <typename TKey, typename TValue>
class RedBlackTree : public BinarySearchTree<TKey, TValue> {
    private:
        typedef BinarySearchTree<TKey, TValue> Base;
        typedef typename Base::Node Node;
        using Base::root_;
        using Base::size_;
        using Base::less;
        using Base::equal_keys;
        using Base::update_height;
        using Base::min_node;
        using Base::find;

        bool is_red(const Node* node) const {
            return node != nullptr && node->red_;
        }
        Node* rotate_left(Node* node) {
            Node* pivot = node->right_;
            node->right_ = pivot->left_;
            pivot->left_ = node;
            pivot->red_ = node->red_;
            node->red_ = true;
            update_height(node);
            update_height(pivot);
            return pivot;
        }
        Node* rotate_right(Node* node) {
            Node* pivot = node->left_;
            node->left_ = pivot->right_;
            pivot->right_ = node;
            pivot->red_ = node->red_;
            node->red_ = true;
            update_height(node);
            update_height(pivot);
            return pivot;
        }
        void flip_colors(Node* node) {
            node->red_ = !node->red_;
            node->left_->red_ = !node->left_->red_;
            node->right_->red_ = !node->right_->red_;
        }
        Node* move_red_left(Node* node) {
            flip_colors(node);
            if (is_red(node->right_->left_)) {
                node->right_ = rotate_right(node->right_);
                node = rotate_left(node);
                flip_colors(node);
            }
            return node;
        }
        Node* move_red_right(Node* node) {
            flip_colors(node);
            if (is_red(node->left_->left_)) {
                node = rotate_right(node);
                flip_colors(node);
            }
            return node;
        }
        Node* fix_up(Node* node) {
            if (is_red(node->right_) && !is_red(node->left_)) node = rotate_left(node);
            if (is_red(node->left_) && is_red(node->left_->left_)) node = rotate_right(node);
            if (is_red(node->left_) && is_red(node->right_)) flip_colors(node);
            update_height(node);
            return node;
        }
        Node* rb_insert(Node* node, const TKey& key, const TValue& value, bool& inserted) {
            if (node == nullptr) {
                inserted = true;
                return new Node(key, value, true);
            }
            if (less(key, node->key_)) {
                node->left_ = rb_insert(node->left_, key, value, inserted);
            } else if (less(node->key_, key)) {
                node->right_ = rb_insert(node->right_, key, value, inserted);
            } else {
                return node;
            }
            return fix_up(node);
        }
        Node* rb_detach_min(Node* node) {
            if (node->left_ == nullptr) return nullptr;
            if (!is_red(node->left_) && !is_red(node->left_->left_)) node = move_red_left(node);
            node->left_ = rb_detach_min(node->left_);
            return fix_up(node);
        }
        Node* rb_remove(Node* node, const TKey& key) {
            if (less(key, node->key_)) {
                if (!is_red(node->left_) && !is_red(node->left_->left_)) node = move_red_left(node);
                node->left_ = rb_remove(node->left_, key);
            } else {
                if (is_red(node->left_)) node = rotate_right(node);
                if (equal_keys(key, node->key_) && node->right_ == nullptr) {
                    delete node;
                    return nullptr;
                }
                if (!is_red(node->right_) && !is_red(node->right_->left_)) node = move_red_right(node);
                if (equal_keys(key, node->key_)) {
                    Node* successor = min_node(node->right_);
                    Node* new_right = rb_detach_min(node->right_);
                    successor->left_ = node->left_;
                    successor->right_ = new_right;
                    successor->red_ = node->red_;
                    delete node;
                    node = successor;
                } else {
                    node->right_ = rb_remove(node->right_, key);
                }
            }
            return fix_up(node);
        }
        int check_red_black(const Node* node) const {
            if (node == nullptr) return 1;
            if (is_red(node->right_)) return -1;
            if (is_red(node) && is_red(node->left_)) return -1;
            int left = check_red_black(node->left_);
            int right = check_red_black(node->right_);
            if (left == -1 || right == -1 || left != right) return -1;
            return left + (is_red(node) ? 0 : 1);
        }
    public:
        RedBlackTree() : Base() {}
        RedBlackTree(const RedBlackTree& tree) : Base(tree) {}

        explicit RedBlackTree(const BinarySearchTree<TKey, TValue>& tree) : Base() {
            auto insert_pair = [&](const TKey& key, const TValue& value) { this->Insert(key, value); };
            tree.Traverse(TraversalOrder::LeftRootRight, insert_pair);
        }
        RedBlackTree& operator=(const RedBlackTree& tree) = default;

        bool Insert(const TKey& key, const TValue& value) override {
            bool inserted = false;
            root_ = rb_insert(root_, key, value, inserted);
            root_->red_ = false;
            if (inserted) size_++;
            return inserted;
        }
        bool Remove(const TKey& key) override {
            if (find(root_, key) == nullptr) return false;
            if (!is_red(root_->left_) && !is_red(root_->right_)) root_->red_ = true;
            root_ = rb_remove(root_, key);
            if (root_ != nullptr) root_->red_ = false;
            size_--;
            return true;
        }
        bool IsRedBlack() const {
            return !is_red(root_) && check_red_black(root_) != -1;
        }

        template <typename R, typename F>
        RedBlackTree<TKey, R> Map(F func) const {
            return this->template map_to<RedBlackTree<TKey, R>>(func);
        }
        template <typename Predicate>
        RedBlackTree<TKey, TValue> Where(Predicate predicate) const {
            return this->template where_to<RedBlackTree<TKey, TValue>>(predicate);
        }
        RedBlackTree<TKey, TValue> GetSubtree(const TKey& key) const {
            return this->template subtree_to<RedBlackTree<TKey, TValue>>(key);
        }
};