#pragma once

#include <iostream>
#include "../2sem_lab5/exceptions/exceptions.hpp"

struct PersonID {
    size_t number_;
    PersonID(size_t number = 0) : number_(number) {}
    bool operator==(const PersonID& other) const {
        return number_ == other.number_;
    }
    bool operator<(const PersonID& other) const {
        return number_ < other.number_;

    }
};

inline std::ostream& operator<<(std::ostream& out, const PersonID& id) {
    return out << id.number_;
}