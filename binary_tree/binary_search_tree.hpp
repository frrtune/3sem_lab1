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
            return node == nullptr ? 0 : node->height_;
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
};