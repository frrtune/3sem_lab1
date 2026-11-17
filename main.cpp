#include <iostream>
#include <string>
#include <windows.h>
#include "binary_tree/binary_search_tree.hpp"
#include "types/complex.hpp"
#include "types/person.hpp"
#include "types/student.hpp"
#include "types/teacher.hpp"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    bool balanced = false;
    int type = 1;

    BinarySearchTree<int> t_int;
    BinarySearchTree<Complex> t_complex;
    BinarySearchTree<Teacher> t_teacher;
    BinarySearchTree<Student> t_student;

    int choice = -1;
    while (choice != 0) {
        cout << "\n1. Новое дерево (тип + режим)\n2. Добавить\n3. Удалить\n4. Найти\n5. Статистика\n0. Выход\nВыбор: ";
        if (!(cin >> choice)) break;

        switch (choice) {
            case 0:
                break;
            case 1: {
                cout << "Тип (1-int, 2-Complex, 3-Teacher, 4-Student): ";
                cin >> type;
                cout << "Режим (0 - обычное, 1 - красно-черное): ";
                int b;
                cin >> b;
                balanced = (b == 1);

                switch (type) {
                    case 1:
                        t_int = BinarySearchTree<int>(Less<int>(), balanced);
                        break;
                    case 2:
                        t_complex = BinarySearchTree<Complex>(Less<Complex>(), balanced);
                        break;
                    case 3:
                        t_teacher = BinarySearchTree<Teacher>(Less<Teacher>(), balanced);
                        break;
                    case 4:
                        t_student = BinarySearchTree<Student>(Less<Student>(), balanced);
                        break;
                }
                break;
            }
            case 2: {
                switch (type) {
                    case 1: {
                        int val;
                        cin >> val;
                        cout << (t_int.Insert(val) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                    case 2: {
                        double r, i;
                        cin >> r >> i;
                        cout << (t_complex.Insert(Complex(r, i)) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                    case 3: {
                        size_t id;
                        string name, dept;
                        cin >> id >> name >> dept;
                        cout << (t_teacher.Insert(Teacher(PersonID(id), name, "M", "L", 0, dept)) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                    case 4: {
                        size_t id;
                        string name, group;
                        cin >> id >> name >> group;
                        cout << (t_student.Insert(Student(PersonID(id), name, "M", "L", 0, group)) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                }
                break;
            }
            case 3: {
                switch (type) {
                    case 1: {
                        int val;
                        cin >> val;
                        cout << (t_int.Remove(val) ? "Удалено\n" : "Нет\n");
                        break;
                    }
                    case 2: {
                        double r, i;
                        cin >> r >> i;
                        cout << (t_complex.Remove(Complex(r, i)) ? "Удалено\n" : "Нет\n");
                        break;
                    }
                    case 3: {
                        size_t id;
                        cin >> id;
                        cout << (t_teacher.Remove(Teacher(PersonID(id), "A", "B", "C", 0, "D")) ? "Удалено\n" : "Нет\n");
                        break;
                    }
                    case 4: {
                        size_t id;
                        cin >> id;
                        cout << (t_student.Remove(Student(PersonID(id), "A", "B", "C", 0, "D")) ? "Удалено\n" : "Нет\n");
                        break;
                    }
                }
                break;
            }
            case 4: {
                switch (type) {
                    case 1: {
                        int val;
                        cin >> val;
                        cout << (t_int.Contains(val) ? "Есть\n" : "Нет\n");
                        break;
                    }
                    case 2: {
                        double r, i;
                        cin >> r >> i;
                        cout << (t_complex.Contains(Complex(r, i)) ? "Есть\n" : "Нет\n");
                        break;
                    }
                    case 3: {
                        size_t id;
                        cin >> id;
                        cout << (t_teacher.Contains(Teacher(PersonID(id), "A", "B", "C", 0, "D")) ? "Есть\n" : "Нет\n");
                        break;
                    }
                    case 4: {
                        size_t id;
                        cin >> id;
                        cout << (t_student.Contains(Student(PersonID(id), "A", "B", "C", 0, "D")) ? "Есть\n" : "Нет\n");
                        break;
                    }
                }
                break;
            }
            case 5: {
                switch (type) {
                    case 1:
                        cout << "Количество: " << t_int.GetCount() << ", Высота: " << t_int.GetHeight() << "\n";
                        break;
                    case 2:
                        cout << "Количество: " << t_complex.GetCount() << ", Высота: " << t_complex.GetHeight() << "\n";
                        break;
                    case 3:
                        cout << "Количество: " << t_teacher.GetCount() << ", Высота: " << t_teacher.GetHeight() << "\n";
                        break;
                    case 4:
                        cout << "Количество: " << t_student.GetCount() << ", Высота: " << t_student.GetHeight() << "\n";
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
    return 0;
}