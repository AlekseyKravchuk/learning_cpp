#include <iostream>
#include <set>
#include <vector>

using namespace std;

/*

Часть множества

Напишите шаблонную функцию FindGreaterElements,
принимающую множество elements объектов типа T и ещё один объект border типа T
и возвращающую вектор из всех элементов множества, бо́льших border, в возрастающем порядке

template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border);

Пример кода:

int main() {
  for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
    cout << x << " ";
  }
  cout << endl;
  
  string to_find = "Python";
  cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
  return 0;
}

Вывод:
7 8
0
*/

template <typename T>
std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border) {
    auto it = elements.upper_bound(border);
    return (it != elements.end()) ? std::vector<T>(it, elements.end()) : std::vector<T>{};
}

// // менее изящное решение задачи
// template <typename T>
// std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border) {
//     auto it = elements.begin();

//     // Цель — найти первый элемент, больший border. Если итератор не достиг конца и указывает не туда, двигаем
//     while (it != elements.end() && (*it) <= border) {
//         ++it;
//     }
//     return std::vector<T>(it, elements.end());
// }

int main() {
    for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
        cout << x << " ";
    }
    cout << endl;

    string to_find = "Python";
    cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
    return 0;
}