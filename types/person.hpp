#pragma once

#include <iostream>
#include <string>
#include <ctime>
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

class Person {
    private:
        PersonID id_;
        std::string first_name_;
        std::string middle_name_;
        std::string last_name_;
        time_t birth_date_;
    public:
        Person() : id_(), first_name_(), middle_name_(), last_name_(), birth_date_(0) {}
        Person(const PersonID& id, const std::string& first_name,
            const std::string& middle_name, const std::string& last_name, time_t birth_date) 
            : id_(id), first_name_(first_name), middle_name_(middle_name), last_name_(last_name), birth_date_(birth_date) {
                if (first_name_.empty()) throw InvalidArgumentError("first name is empty");
                if (last_name_.empty()) throw InvalidArgumentError("last name is empty");
            }
            PersonID GetID() const {
                return id_;
            }
            const std::string& GetFirstName() const {
                return first_name_;
            }
            const std::string& GetMiddleName() const {
                return middle_name_;
            }
            const std::string& GetLastName() const {
                return last_name_;
            }
            time_t GetBirthDate() const {
                return birth_date_;
            }
            std::string GetFullName() const {
                if (middle_name_.empty()) return first_name_ + " " + last_name_;
                return first_name_ + " " + middle_name_ + " " + last_name_;
            }
            bool operator==(const Person& other) const {
                return id_ == other.id_;
            }
            bool operator<(const Person& other) const {
                return id_ < other.id_;
            }
};