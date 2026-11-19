#pragma once


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
            Node(const T& key, const T& value, bool red = false) 
            : key_(key), value_(value), left_(nullptr), right_(nullptr), height_(1), red_(red) {}
        };
        Node* root_;
        size_t size_;
        bool balanced_;
        bool less(const TKey& first, const TKey& second) const {
            return first < second;
        }
        bool equal(const TKey& first, const TKey& second) const {
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
        template <typename Visitor>
        void traverse(const Node* node, TraversalOrder order, Visitor& visit) const {
            if (node == nullptr) return;
            switch (order) {
                case TraversalOrder::RootLeftRight:
                    visit(node->value_);
                    traverse(node->left_, order, visit);
                    traverse(node->right_, order, visit);
                    break;
                case TraversalOrder::RootRightLeft:
                    visit(node->value_);
                    traverse(node->right_, order, visit);
                    traverse(node->left_, order, visit);
                    break;
                case TraversalOrder::LeftRightRoot:
                    traverse(node->left_, order, visit);
                    traverse(node->right_, order, visit);
                    visit(node->value_);
                    break;
                case TraversalOrder::LeftRootRight:
                    traverse(node->left_, order, visit);
                    visit(node->value_);
                    traverse(node->right_, order, visit);
                    break;
                case TraversalOrder::RightLeftRoot:
                    traverse(node->right_, order, visit);
                    traverse(node->left_, order, visit);
                    visit(node->value_);
                    break;
                case TraversalOrder::RightRootLeft:
                    traverse(node->right_, order, visit);
                    visit(node->value_);
                    traverse(node->left_, order, visit);
                    break;
            }
        }
        void swap(BinarySearchTree<T, Compare>& other) {
            Node* temporary_root = root_;
            root_ = other.root_;
            other.root_ = temporary_root;
            size_t temporary_size = size_;
            size_ = other.size_;
            other.size_ = temporary_size;
            Compare temporary_compare = less;
            less = other.less;
            other.less = temporary_compare;
            bool temporary_balanced = balanced_;
            balanced_ = other.balanced_;
            other.balanced_ = temporary_balanced;
        }
    public:
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
            BinarySearchTree<T, Compare> rebuilt(less, true);
            auto insert_value = [&](const T& value) { rebuilt.Insert(value); };
            traverse(root_, TraversalOrder::LeftRootRight, insert_value);
            swap(rebuilt);
        }
        template <typename Visitor> 
        void Traverse(TraversalOrder order, Visitor visit) const {
            traverse(root_, order, visit);
        }
    };