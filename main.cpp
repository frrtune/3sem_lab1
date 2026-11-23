#include <iostream>
#include <string>
#include <limits>
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
        cout << "\n1. Новое дерево (тип + режим)\n2. Добавить\n3. Удалить (по ключу)\n4. Найти (по ключу)\n5. Статистика\n"
             << "6. Обход (итератор)\n7. Map (значения * k)\n8. Where (фильтр)\n9. Reduce (свёртка)\n"
             << "10. Извлечь поддерево (по ключу)\n11. Поиск вхождения поддерева\n0. Выход\nВыбор: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод\n";
            continue;
        }

        try {
            switch (choice) {
                case 0:
                    break;
                case 1: {
                    cout << "Тип (1-int, 2-Complex, 3-Teacher, 4-Student): ";
                    if (!(cin >> type)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
                    cout << "Режим (0 - обычное, 1 - красно-черное): ";
                    int b;
                    if (!(cin >> b)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
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
                            if (!(cin >> key >> val)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            cout << (t_int.Insert(key, val) ? "Добавлено\n" : "Уже есть\n");
                            break;
                        }
                        case 2: {
                            int key;
                            double r, i;
                            cout << "Введите ключ, затем Re и Im комплексного числа: ";
                            if (!(cin >> key >> r >> i)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            cout << (t_complex.Insert(key, Complex(r, i)) ? "Добавлено\n" : "Уже есть\n");
                            break;
                        }
                        case 3: {
                            int key;
                            size_t id;
                            string name, dept;
                            cout << "Введите ключ, ID, имя и кафедру: ";
                            if (!(cin >> key >> id >> name >> dept)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            cout << (t_teacher.Insert(key, Teacher(PersonID(id), name, "M", "L", 0, dept)) ? "Добавлено\n" : "Уже есть\n");
                            break;
                        }
                        case 4: {
                            int key;
                            size_t id;
                            string name, group;
                            cout << "Введите ключ, ID, имя и группу: ";
                            if (!(cin >> key >> id >> name >> group)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            cout << (t_student.Insert(key, Student(PersonID(id), name, "M", "L", 0, group)) ? "Добавлено\n" : "Уже есть\n");
                            break;
                        }
                    }
                    break;
                }
                case 3: {
                    int key;
                    cout << "Введите ключ: ";
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
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
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
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
                case 6: {
                    int ord;
                    cout << "Порядок (1 - возрастание, 2 - убывание, 3 - прямой, 4 - обратный): ";
                    if (!(cin >> ord)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
                    TraversalOrder order = TraversalOrder::LeftRootRight;
                    if (ord == 2) order = TraversalOrder::RightRootLeft;
                    else if (ord == 3) order = TraversalOrder::RootLeftRight;
                    else if (ord == 4) order = TraversalOrder::RootRightLeft;

                    switch (type) {
                        case 1: {
                            auto it = t_int.GetEnumerator(order);
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 2: {
                            auto it = t_complex.GetEnumerator(order);
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 3: {
                            auto it = t_teacher.GetEnumerator(order);
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 4: {
                            auto it = t_student.GetEnumerator(order);
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                    }
                    break;
                }
                case 7: {
                    int factor;
                    cout << "Множитель для значений: ";
                    if (!(cin >> factor)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
                    switch (type) {
                        case 1: {
                            BinarySearchTree<int, int> mapped = t_int.Map<int>(
                                [factor](const int&, const int& value) { return value * factor; });
                            auto it = mapped.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 2: {
                            BinarySearchTree<int, Complex> mapped = t_complex.Map<Complex>(
                                [factor](const int&, const Complex& value) { return value * factor; });
                            auto it = mapped.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 3: {
                            BinarySearchTree<int, string> mapped = t_teacher.Map<string>(
                                [](const int&, const Teacher& value) { return value.GetFullName(); });
                            auto it = mapped.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 4: {
                            BinarySearchTree<int, string> mapped = t_student.Map<string>(
                                [](const int&, const Student& value) { return value.GetGroup(); });
                            auto it = mapped.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                    }
                    break;
                }
                case 8: {
                    switch (type) {
                        case 1: {
                            int threshold;
                            cout << "Порог (оставить значения > порога): ";
                            if (!(cin >> threshold)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            BinarySearchTree<int, int> filtered = t_int.Where(
                                [threshold](const int&, const int& value) { return value > threshold; });
                            auto it = filtered.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 2: {
                            int threshold;
                            cout << "Порог (оставить Re > порога): ";
                            if (!(cin >> threshold)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            BinarySearchTree<int, Complex> filtered = t_complex.Where(
                                [threshold](const int&, const Complex& value) { return value.GetReal() > threshold; });
                            auto it = filtered.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 3: {
                            string dept;
                            cout << "Кафедра: ";
                            if (!(cin >> dept)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            BinarySearchTree<int, Teacher> filtered = t_teacher.Where(
                                [dept](const int&, const Teacher& value) { return value.GetDepartment() == dept; });
                            cout << "Оставлено преподавателей: " << filtered.GetCount() << "\n";
                            break;
                        }
                        case 4: {
                            string group;
                            cout << "Группа: ";
                            if (!(cin >> group)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Некорректный ввод\n";
                                break;
                            }
                            BinarySearchTree<int, Student> filtered = t_student.Where(
                                [group](const int&, const Student& value) { return value.GetGroup() == group; });
                            cout << "Оставлено студентов: " << filtered.GetCount() << "\n";
                            break;
                        }
                    }
                    break;
                }
                case 9: {
                    switch (type) {
                        case 1: {
                            int sum = t_int.Reduce<int>(
                                [](int acc, const int&, const int& value) { return acc + value; }, 0);
                            cout << "Сумма значений: " << sum << "\n";
                            break;
                        }
                        case 2: {
                            Complex sum = t_complex.Reduce<Complex>(
                                [](Complex acc, const int&, const Complex& value) { return acc + value; }, Complex(0, 0));
                            cout << "Сумма значений: " << sum << "\n";
                            break;
                        }
                        case 3: {
                            int count = t_teacher.Reduce<int>(
                                [](int acc, const int&, const Teacher&) { return acc + 1; }, 0);
                            cout << "Всего преподавателей: " << count << "\n";
                            break;
                        }
                        case 4: {
                            int count = t_student.Reduce<int>(
                                [](int acc, const int&, const Student&) { return acc + 1; }, 0);
                            cout << "Всего студентов: " << count << "\n";
                            break;
                        }
                    }
                    break;
                }
                case 10: {
                    int key;
                    cout << "Введите ключ корня поддерева: ";
                    if (!(cin >> key)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
                    switch (type) {
                        case 1: {
                            auto sub = t_int.GetSubtree(key);
                            auto it = sub.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 2: {
                            auto sub = t_complex.GetSubtree(key);
                            auto it = sub.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 3: {
                            auto sub = t_teacher.GetSubtree(key);
                            auto it = sub.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                        case 4: {
                            auto sub = t_student.GetSubtree(key);
                            auto it = sub.GetEnumerator();
                            while (it.Next()) {
                                cout << it.GetCurrentKey() << " -> " << it.GetCurrentValue() << "\n";
                            }
                            break;
                        }
                    }
                    break;
                }
                case 11: {
                    int n;
                    cout << "Сколько узлов в поддереве: ";
                    if (!(cin >> n) || n < 0) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Некорректный ввод\n";
                        break;
                    }
                    switch (type) {
                        case 1: {
                            BinarySearchTree<int, int> sub(balanced);
                            for (int i = 0; i < n; ++i) {
                                int k, v;
                                cout << "Ключ и значение: ";
                                if (!(cin >> k >> v)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Некорректный ввод\n";
                                    break;
                                }
                                sub.Insert(k, v);
                            }
                            cout << (t_int.ContainsSubtree(sub) ? "Входит\n" : "Не входит\n");
                            break;
                        }
                        case 2: {
                            BinarySearchTree<int, Complex> sub(balanced);
                            for (int i = 0; i < n; ++i) {
                                int k;
                                double r, im;
                                cout << "Ключ, Re и Im: ";
                                if (!(cin >> k >> r >> im)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Некорректный ввод\n";
                                    break;
                                }
                                sub.Insert(k, Complex(r, im));
                            }
                            cout << (t_complex.ContainsSubtree(sub) ? "Входит\n" : "Не входит\n");
                            break;
                        }
                        case 3: {
                            BinarySearchTree<int, Teacher> sub(balanced);
                            for (int i = 0; i < n; ++i) {
                                int k;
                                size_t id;
                                string name, dept;
                                cout << "Ключ, ID, имя и кафедра: ";
                                if (!(cin >> k >> id >> name >> dept)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Некорректный ввод\n";
                                    break;
                                }
                                sub.Insert(k, Teacher(PersonID(id), name, "M", "L", 0, dept));
                            }
                            cout << (t_teacher.ContainsSubtree(sub) ? "Входит\n" : "Не входит\n");
                            break;
                        }
                        case 4: {
                            BinarySearchTree<int, Student> sub(balanced);
                            for (int i = 0; i < n; ++i) {
                                int k;
                                size_t id;
                                string name, group;
                                cout << "Ключ, ID, имя и группа: ";
                                if (!(cin >> k >> id >> name >> group)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Некорректный ввод\n";
                                    break;
                                }
                                sub.Insert(k, Student(PersonID(id), name, "M", "L", 0, group));
                            }
                            cout << (t_student.ContainsSubtree(sub) ? "Входит\n" : "Не входит\n");
                            break;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        } catch (const std::exception& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
    return 0;
}