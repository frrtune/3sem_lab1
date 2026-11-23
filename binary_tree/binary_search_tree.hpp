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
    private:
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
        bool balanced_;
        bool less(const TKey& first, const TKey& second) const {
            return first < second;
        }
        bool equal_values(const TKey& first, const TKey& second) const {
            return !less(first, second) && !less(second, first);
        }
        bool equal_keys(const TKey& first, const TKey& second) const {
            return !less(first, second) && !less(second, first);
        }
        size_t get_height(const Node* node) const {
            return node == nullptr ? 0 : node->height_;
        }
        size_t max_height(size_t first, size_t second) {
            return first > second ? first : second;
        }
        void update_height(Node* node) {
            node->height_ = 1 + max_height(get_height(node->left_), get_height(node->right_));
        }

        //балансировка
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

        void destroy(Node* node) {
            if (node == nullptr) return;
            destroy(node->left_);
            destroy(node->right_);
            delete node;
        }
        Node* clone(const Node* source) {
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
            bool temporary_balanced = balanced_;
            balanced_ = other.balanced_;
            other.balanced_ = temporary_balanced;
        }

        //subtree
        size_t count_nodes(const Node* node) const {
            if (node == nullptr) return 0;
            return 1 + count_nodes(node->left_) + count_nodes(node->right_);
        }

        template <typename Result>
        Result subtree_to(const TKey& key) {
            Node* node = find(root_, key);
            if (node == nullptr) {
                throw InvalidArgumentError("key is not found");
            }
            Result result;
            Node* copy = clone(node);
            copy->red_ = false;
            result.root_ = copy;
            result.size_ = count_nodes(node);
            return result;
        }
        bool same_tree(const Node* first, const Node* second) const {
            if (first == nullptr || second == nullptr) return first == second;
            return equal_keys(first->key_, second->key_) &&
                   same_tree(first->left_, second->left_) &&
                   same_tree(first->right_, second->right_);
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
        BinarySearchTree(bool balanced = false) : root_(nullptr), size_(0), balanced_(balanced) {}
        BinarySearchTree(const BinarySearchTree& tree, bool balanced = false) : root_(clone(tree.root_)), size_(tree.size_),
        balanced_(balanced) {}
        BinarySearchTree<TKey, TValue>& operator=(const BinarySearchTree<TKey, TValue>& tree) {
            if (this != &tree) {
                BinarySearchTree<TKey, TValue> copy = tree;
                swap(copy);
            }
            return *this;
        }
        ~BinarySearchTree() {
            destroy(root_);
        }
        size_t GetCount() const {
            return size_;
        }
        size_t IsEmpty() const {
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
        bool Insert(const TKey& key, const TValue& value) {
            bool inserted = false;
            if (balanced_) {
                root_ = rb_insert(root_, key, value, inserted);
                root_->red_ = false;
            } else {
                root_ = insert(root_, key, value, inserted);
            }
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
        bool Remove(const TKey& key) {
            bool removed = false;
            if (balanced_) {
                if (find(root_, key) == nullptr) return false;
                if (!is_red(root_->left_) && !is_red(root_->right_)) root_->red_ = true;
                root_ = rb_remove(root_, key);
                if (root_ != nullptr) root_->red_ = false;
                removed = true;
            } else {
                root_ = remove(root_, key, removed);
            }
            if (removed) size_--;
            return removed;
        }
        bool IsRedBlack() const {
            return !is_red(root_) && check_red_black(root_) != -1;
        }
        void Balance() {
            BinarySearchTree<TKey, TValue> rebuilt(true);
            auto insert_value = [&](const TKey& key, const TValue& value) { rebuilt.Insert(key, value); };
            traverse(root_, TraversalOrder::LeftRootRight, insert_value);
            swap(rebuilt);
        }
        template <typename Visitor> 
        void Traverse(TraversalOrder order, Visitor visit) const {
            traverse(root_, order, visit);
        }

        //map, reduce, where
        template <typename R, typename F>
        BinarySearchTree<TKey, R> Map(F func) const {
            BinarySearchTree<TKey, R> result(balanced_);
            auto visitor = [&](const TKey& key, const TValue& value) {
                result.Insert(key, func(key, value));
            };
            traverse(root_, TraversalOrder::RootLeftRight, visitor);
            return result;
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
            BinarySearchTree<TKey, TValue> result(balanced_);
            auto visitor = [&](const TKey& key, const TValue& value) {
                if (predicate(key, value)) result.Insert(key, value);
            };
            traverse(root_, TraversalOrder::RootLeftRight, visitor);
            return result;
        }
        TreeEnumerator GetEnumerator(TraversalOrder order = TraversalOrder::LeftRootRight) const {
            TreeEnumerator enumerator;
            enumerator.items_.reserve(size_);
            auto collect = [&](const Node* node) { enumerator.items_.push_back(node); };
            traverse_nodes(root_, order, collect);
            return enumerator;
        }
        BinarySearchTree<TKey, TValue> GetSubtree(const TKey& key) {
            return subtree_to<BinarySearchTree<TKey, TValue>>(key);
        }
        bool ContainsSubtree(const BinarySearchTree<TKey, TValue>& subtree) {
            if (subtree.root_ == nullptr) return true;
            const Node* node = find(root_, subtree.root_->key_);
            return node != nullptr && same_tree(node, subtree.root_);
        }
    };