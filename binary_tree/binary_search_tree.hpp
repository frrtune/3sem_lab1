#pragma once


template <typename T>
struct Less {
    bool operator()(const T& first, const T& second) const {
        return first < second;
    }
};

template <typename T, typename Compare = Less<T>>
class BinarySearchTree {
    private:
        struct Node {
            T value_;
            Node* left_;
            Node* right_;
            size_t height_; //можно ли хранить явно?
            Node(const T& value) : value_(value), left_(nullptr), right_(nullptr), height_(1) {}
        };
        Node* root_;
        size_t size_;
        Compare less_;
        size_t get_height(const Node* node) {
            return node == nullptr ? 0 : node->height_;
        }
        size_t max_height(size_t first, size_t second) {
            return first > second ? first : second;
        }
        void update_height(Node* node) {
            node->height_ = 1 + max_height(get_height(node->left_), get_height(node->right_));
        }
        void destroy(Node* node) {
            if (node == nullptr) destroy;
            destroy(node->left_);
            destroy(node->right_);
            delete node;
        }
        Node* clone(const Node* source) {
            if (source == nullptr) return nullptr;
            Node* copy = new Node(source->value_);
            copy->height_ = source->height_;
            try {
                copy->left_ = clone(source->left_);
                copy->right_ = clone(source->right_);
            } catch(...) {
                destroy(copy);
                throw;
            }
            return copy;
        }
        Node* insert(Node* node, const T& value, bool& inserted) {
            if (node == nullptr) {
                inserted = true;
                return new Node(value);
            }
            if (less_(value, node->value)) {
                node->left_ = insert(node->left_, value, inserted);
            } else if (less_(node->value, value)) {
                node->right = insert(node->right_, value, inserted);
            } else {
                return node;
            }
            update_height(node);
            return node;
        }
        Node* find(Node* node, const T& value) const {
            if (node == nullptr) return nullptr;
            if (less_(value, node->value_)) return find(node->left_, value);
            if (less_(node->value_, value)) return find(node->right_, value);
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
        Node* remove(Node* node, const T& value, bool& removed) {
            if (node == nullptr) return nullptr;
            if (less_(value, node->value_)) {
                node->left_ = remove(node->left_, value, removed);
            } else if (less_(node->value_, value)) {
                node->right_ = remove(node->right_, value, removed);
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
        void in_order(const Node* node, Visitor& visit) {
            if (node == nullptr) return;
            in_order(node->left_, visit);
            visit(node->value_);
            in_order(node->right_, visit);
        }
        void swap(BinarySearchTree<T, Compare>& other) {
            Node* temporary_root = root_;
            root_ = other.root_;
            other.root_ = temporary_root;
            size_t temporary_size = size_;
            size_ = other.size_;
            other.size_ = temporary_size;
            Compare temporary_compare = compare;
            compare_ = other.compare_;
            other.compare_ = temporary_compare;
        }
    public:
        BinarySearchTree(Compare compare = Compare()) : root_(nullptr), size_(0), compare_(compare) {}
        BinarySearchTree(const BinarySearchTree<T, Compare>& tree) : root_(clone(tree.root_), size_(tree.size_), compare_(tree).compare_) {}
        BinarySearchTree<T, Compare>& operator=(const BinarySearchTree<T, Compare>& tree) {
            if (this != &tree) {
                BinarySearchTree<T, Compare> copy = tree;
                swap(copy);
            }
            return *this;
        }
    };