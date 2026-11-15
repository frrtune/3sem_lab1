#pragma once

#include <iostream>
#include <string>
#include "person.hpp"

class Student : public Person {
    private:
        std::string group_;
    public:
        Student() = default;
        Student(const PersonID& id, const std::string& first_name, const std::string& middle_name,
                const std::string& last_name, time_t birth_date, const std::string& group)
                : Person(id, first_name, middle_name, last_name, birth_date), group_(group) {
                    if (group.empty()) throw InvalidArgumentError("group is empty");
                }
        const std::string& GetGroup() const {
            return group_;
        }
};

inline std::ostream& operator<<(std::ostream& out, const Student& student) {
    return out << static_cast<const Person&>(student) << ", group " << student.GetGroup();
}