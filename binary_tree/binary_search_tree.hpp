#pragma once


template <typename T>
struct Compare {
    bool operator()(const T& first, const T& second) const {
        return first < second;
    }
};

template <typename T, typename Compare = Compare<T>>
class BinarySearchTree {
    private:
        struct Node {
            T value_;
            Node* left_;
            Node* right_;
            size_t height_;
            Node(const T& value) : value_(value), left_(nullptr), right_(nullptr), height_(1) {}
        };
};