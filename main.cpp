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

    BinarySearchTree<int, int> t_int(false);
    BinarySearchTree<int, Complex> t_complex(false);
    BinarySearchTree<int, Teacher> t_teacher(false);
    BinarySearchTree<int, Student> t_student(false);

    int choice = -1;
    while (choice != 0) {
        cout << "\n1. Новое дерево (тип + режим)\n2. Добавить\n3. Удалить (по ключу)\n4. Найти (по ключу)\n5. Статистика\n0. Выход\nВыбор: ";
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
                        t_int = BinarySearchTree<int, int>(balanced);
                        break;
                    case 2:
                        t_complex = BinarySearchTree<int, Complex>(balanced);
                        break;
                    case 3:
                        t_teacher = BinarySearchTree<int, Teacher>(balanced);
                        break;
                    case 4:
                        t_student = BinarySearchTree<int, Student>(balanced);
                        break;
                }
                break;
            }
            case 2: {
                switch (type) {
                    case 1: {
                        int key, val;
                        cout << "Введите ключ и значение: ";
                        cin >> key >> val;
                        cout << (t_int.Insert(key, val) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                    case 2: {
                        int key;
                        double r, i;
                        cout << "Введите ключ, затем Re и Im комплексного числа: ";
                        cin >> key >> r >> i;
                        cout << (t_complex.Insert(key, Complex(r, i)) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                    case 3: {
                        int key;
                        size_t id;
                        string name, dept;
                        cout << "Введите ключ, ID, имя и кафедру: ";
                        cin >> key >> id >> name >> dept;
                        cout << (t_teacher.Insert(key, Teacher(PersonID(id), name, "M", "L", 0, dept)) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                    case 4: {
                        int key;
                        size_t id;
                        string name, group;
                        cout << "Введите ключ, ID, имя и группу: ";
                        cin >> key >> id >> name >> group;
                        cout << (t_student.Insert(key, Student(PersonID(id), name, "M", "L", 0, group)) ? "Добавлено\n" : "Уже есть\n");
                        break;
                    }
                }
                break;
            }
            case 3: {
                int key;
                cout << "Введите ключ: ";
                cin >> key;
                switch (type) {
                    case 1:
                        cout << (t_int.Remove(key) ? "Удалено\n" : "Нет\n");
                        break;
                    case 2:
                        cout << (t_complex.Remove(key) ? "Удалено\n" : "Нет\n");
                        break;
                    case 3:
                        cout << (t_teacher.Remove(key) ? "Удалено\n" : "Нет\n");
                        break;
                    case 4:
                        cout << (t_student.Remove(key) ? "Удалено\n" : "Нет\n");
                        break;
                }
                break;
            }
            case 4: {
                int key;
                cout << "Введите ключ для поиска: ";
                cin >> key;
                switch (type) {
                    case 1:
                        cout << (t_int.Contains(key) ? "Есть\n" : "Нет\n");
                        break;
                    case 2:
                        cout << (t_complex.Contains(key) ? "Есть\n" : "Нет\n");
                        break;
                    case 3:
                        cout << (t_teacher.Contains(key) ? "Есть\n" : "Нет\n");
                        break;
                    case 4:
                        cout << (t_student.Contains(key) ? "Есть\n" : "Нет\n");
                        break;
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