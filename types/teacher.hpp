#pragma once

#include <ostream>
#include <string>
#include "person.hpp"

class Teacher : public Person {
    private:
        std::string department_;
    public:
        Teacher() = default;
        Teacher(const PersonID& id, const std::string& first_name, const std::string& middle_name,
                const std::string& last_name, time_t birth_date, const std::string& department)
            : Person(id, first_name, middle_name, last_name, birth_date), department_(department) {
            if (department.empty()) throw InvalidArgumentError("department is empty");
        }
        const std::string& GetDepartment() const {
            return department_;
        }
};

inline std::ostream& operator<<(std::ostream& out, const Teacher& teacher) {
    return out << static_cast<const Person&>(teacher) << ", department " << teacher.GetDepartment();
}