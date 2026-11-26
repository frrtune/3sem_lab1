#pragma once


#include <vector>
#include "../2sem_lab5/exceptions/exceptions.hpp"


enum class TraversalOrder {
    RootLeftRight,
    RootRightLeft,
    LeftRightRoot,
    LeftRootRight,
    RightLeftRoot,
    RightRootLeft
};

template <typename TKey, typename TValue>
class BinarySearchTree {
    protected:
        struct Node {
            TKey key_;
            TValue value_;
            Node* left_;
            Node* right_;
            size_t height_;
            bool red_;
            Node(const TKey& key, const TValue& value, bool red = false) 
            : key_(key), value_(value), left_(nullptr), right_(nullptr), height_(1), red_(red) {}
        };
        Node* root_;
        size_t size_;

        bool less(const TKey& first, const TKey& second) const {
            return first < second;
        }
        bool equal_keys(const TKey& first, const TKey& second) const {
            return !less(first, second) && !less(second, first);
        }
        size_t get_height(const Node* node) const {
            return node == nullptr ? 0 : node->height_;
        }
        size_t max_height(size_t first, size_t second) const {
            return first > second ? first : second;
        }
        void update_height(Node* node) {
            node->height_ = 1 + max_height(get_height(node->left_), get_height(node->right_));
        }
        void destroy(Node* node) const {
            if (node == nullptr) return;
            destroy(node->left_);
            destroy(node->right_);
            delete node;
        }
        Node* clone(const Node* source) const {
            if (source == nullptr) return nullptr;
            Node* copy = new Node(source->key_, source->value_);
            copy->height_ = source->height_;
            copy->red_ = source->red_;
            try {
                copy->left_ = clone(source->left_);
                copy->right_ = clone(source->right_);
            } catch (...) {
                destroy(copy);
                throw;
            }
            return copy;
        }
        Node* find(Node* node, const TKey& key) const {
            if (node == nullptr) return nullptr;
            if (less(key, node->key_)) return find(node->left_, key);
            if (less(node->key_, key)) return find(node->right_, key);
            return node;
        }
        Node* min_node(Node* node) {
            while (node->left_ != nullptr) {
                node = node->left_;
            }
            return node;
        }
        template <typename NodeVisitor>
        void traverse_nodes(const Node* node, TraversalOrder order, NodeVisitor& visit) const {
            if (node == nullptr) return;
            const Node* left = node->left_;
            const Node* right = node->right_;
            switch (order) {
                case TraversalOrder::RootLeftRight:
                    visit(node);
                    traverse_nodes(left, order, visit);
                    traverse_nodes(right, order, visit);
                    break;
                case TraversalOrder::RootRightLeft:
                    visit(node);
                    traverse_nodes(right, order, visit);
                    traverse_nodes(left, order, visit);
                    break;
                case TraversalOrder::LeftRightRoot:
                    traverse_nodes(left, order, visit);
                    traverse_nodes(right, order, visit);
                    visit(node);
                    break;
                case TraversalOrder::LeftRootRight:
                    traverse_nodes(left, order, visit);
                    visit(node);
                    traverse_nodes(right, order, visit);
                    break;
                case TraversalOrder::RightLeftRoot:
                    traverse_nodes(right, order, visit);
                    traverse_nodes(left, order, visit);
                    visit(node);
                    break;
                case TraversalOrder::RightRootLeft:
                    traverse_nodes(right, order, visit);
                    visit(node);
                    traverse_nodes(left, order, visit);
                    break;
            }
        }
        template <typename Visitor>
        void traverse(const Node* node, TraversalOrder order, Visitor& visit) const {
            auto adapter = [&](const Node* current) { visit(current->key_, current->value_); };
            traverse_nodes(node, order, adapter);
        }
        void swap(BinarySearchTree<TKey, TValue>& other) {
            Node* temporary_root = root_;
            root_ = other.root_;
            other.root_ = temporary_root;
            size_t temporary_size = size_;
            size_ = other.size_;
            other.size_ = temporary_size;
        }

        template <typename Result, typename F>
        Result map_to(F func) const {
            Result result;
            auto visitor = [&](const TKey& key, const TValue& value) {
                result.Insert(key, func(key, value));
            };
            traverse(root_, TraversalOrder::RootLeftRight, visitor);
            return result;
        }
        template <typename Result, typename Predicate>
        Result where_to(Predicate predicate) const {
            Result result;
            auto visitor = [&](const TKey& key, const TValue& value) {
                if (predicate(key, value)) result.Insert(key, value);
            };
            traverse(root_, TraversalOrder::RootLeftRight, visitor);
            return result;
        }
        size_t count_nodes(const Node* node) const {
            if (node == nullptr) return 0;
            return 1 + count_nodes(node->left_) + count_nodes(node->right_);
        }
        bool same_tree(const Node* first, const Node* second) const {
            if (first == nullptr || second == nullptr) return first == second;
            return equal_keys(first->key_, second->key_) &&
                   same_tree(first->left_, second->left_) &&
                   same_tree(first->right_, second->right_);
        }
        void set_content(BinarySearchTree& tree, Node* root, size_t size) {
            tree.root_ = root;
            tree.size_ = size;
        }

        template <typename Result>
        Result subtree_to(const TKey& key) const {
            Node* node = find(root_, key);
            if (node == nullptr) {
                throw InvalidArgumentError("key is not found");
            }
            Result result;
            Node* copy = clone(node);
            copy->red_ = false;
            set_content(result, copy, count_nodes(node));
            return result;
        }
    private:
        Node* insert(Node* node, const TKey& key, const TValue& value, bool& inserted) {
            if (node == nullptr) {
                inserted = true;
                return new Node(key, value);
            }
            if (less(key, node->key_)) {
                node->left_ = insert(node->left_, key, value, inserted);
            } else if (less(node->key_, key)) {
                node->right_ = insert(node->right_, key, value, inserted);
            } else {
                return node;
            }
            update_height(node);
            return node;
        }
        Node* detach_min(Node* node) {
            if (node->left_ == nullptr) return node->right_;
            node->left_ = detach_min(node->left_);
            update_height(node);
            return node;
        }
        Node* remove(Node* node, const TKey& key, bool& removed) {
            if (node == nullptr) return nullptr;
            if (less(key, node->key_)) {
                node->left_ = remove(node->left_, key, removed);
            } else if (less(node->key_, key)) {
                node->right_ = remove(node->right_, key, removed);
            } else {
                removed = true;
                if (node->right_ == nullptr) {
                    Node* left = node->left_;
                    delete node;
                    return left;
                }
                if (node->left_ == nullptr) {
                    Node* right = node->right_;
                    delete node;
                    return right;
                }
                Node* target = node;
                node = min_node(target->right_);
                node->right_ = detach_min(target->right_);
                node->left_ = target->left_;
                delete target;
            }
            update_height(node);
            return node;
        }
    public:
        class TreeEnumerator {
            private:
                std::vector<const Node*> items_;
                size_t position_;
                friend class BinarySearchTree<TKey, TValue>;
                TreeEnumerator() : items_(), position_(0) {}
                bool is_inside() const {
                    return position_ >= 1 && position_ <= items_.size();
                }
            public:
                bool HasNext() const {
                    return position_ < items_.size();
                }
                bool Next() {
                    if (position_ < items_.size()) {
                        position_++;
                        return true;
                    }
                    position_ = items_.size() + 1;
                    return false;
                }
                const TKey& GetCurrentKey() const {
                    if (!is_inside()) throw RangeError(position_, items_.size());
                    return items_[position_ - 1]->key_;
                }
                const TValue& GetCurrentValue() const {
                    if (!is_inside()) throw RangeError(position_, items_.size());
                    return items_[position_ - 1]->value_;
                }
                bool TryGetCurrent(TKey& key, TValue& value) const {
                    if (!is_inside()) return false;
                    key = items_[position_ - 1]->key_;
                    value = items_[position_ - 1]->value_;
                    return true;
                }
                void Reset() {
                    position_ = 0;
                }
        };
        BinarySearchTree() : root_(nullptr), size_(0) {}
        BinarySearchTree(const BinarySearchTree& tree) : root_(clone(tree.root_)), size_(tree.size_) {}
        BinarySearchTree& operator=(const BinarySearchTree& tree) {
            if (this != &tree) {
                BinarySearchTree<TKey, TValue> copy = tree;
                swap(copy);
            }
            return *this;
        }
        virtual ~BinarySearchTree() {
            destroy(root_);
        }
        size_t GetCount() const {
            return size_;
        }
        bool IsEmpty() const {
            return size_ == 0;
        }
        size_t GetHeight() const {
            return get_height(root_);
        }
        void Clear() {
            destroy(root_);
            root_ = nullptr;
            size_ = 0;
        }
        virtual bool Insert(const TKey& key, const TValue& value) {
            bool inserted = false;
            root_ = insert(root_, key, value, inserted);
            if (inserted) size_++;
            return inserted;
        }
        const TValue* Find(const TKey& key) const {
            Node* node = find(root_, key);
            return node == nullptr ? nullptr : &node->value_;
        }
        bool Contains(const TKey& key) const {
            return find(root_, key) != nullptr;
        }
        virtual bool Remove(const TKey& key) {
            bool removed = false;
            root_ = remove(root_, key, removed);
            if (removed) size_--;
            return removed;
        }
        template <typename Visitor> 
        void Traverse(TraversalOrder order, Visitor visit) const {
            traverse(root_, order, visit);
        }
        TreeEnumerator GetEnumerator(TraversalOrder order = TraversalOrder::LeftRootRight) const {
            TreeEnumerator enumerator;
            enumerator.items_.reserve(size_);
            auto collect = [&](const Node* node) { enumerator.items_.push_back(node); };
            traverse_nodes(root_, order, collect);
            return enumerator;
        }

        //map, reduce, where
        template <typename R, typename F>
        BinarySearchTree<TKey, R> Map(F func) const {
            return map_to<BinarySearchTree<TKey, R>>(func);
        }
        template <typename R, typename Reducer>
        R Reduce(Reducer reducer, R initial) const {
            auto visitor = [&](const TKey& key, const TValue& value) {
                initial = reducer(initial, key, value);
            };
            traverse(root_, TraversalOrder::LeftRootRight, visitor);
            return initial;
        }
        template <typename Predicate>
        BinarySearchTree<TKey, TValue> Where(Predicate predicate) const {
            return where_to<BinarySearchTree<TKey, TValue>>(predicate);
        }

        // поддеревья
        BinarySearchTree<TKey, TValue> GetSubtree(const TKey& key) const {
            return subtree_to<BinarySearchTree<TKey, TValue>>(key);
        }
        bool ContainsSubtree(const BinarySearchTree<TKey, TValue>& subtree) const {
            if (subtree.root_ == nullptr) return true;
            const Node* node = find(root_, subtree.root_->key_);
            return node != nullptr && same_tree(node, subtree.root_);
        }
};