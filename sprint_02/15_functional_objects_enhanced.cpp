#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Animal {
    string name;
    int age;
    double weight;
};

template <typename Container, typename KeyMapper>
void SortBy(Container& container, KeyMapper key_mapper, bool reverse = false) {
    // теперь можно сортировать контейнер с компаратором key_mapper(lhs) < key_mapper(rhs)

    if (reverse) {
        // лямбда-компаратор для сортировки по убыванию
        auto cmpDescending = [key_mapper](const auto& lhs, const auto& rhs) { return key_mapper(lhs) > key_mapper(rhs); };
        sort(container.begin(), container.end(), cmpDescending);
    } else {
        // лямбда-компаратор для сортировки по возрастанию
        auto cmpAscending = [key_mapper](const auto& lhs, const auto& rhs) { return key_mapper(lhs) < key_mapper(rhs); };
        sort(container.begin(), container.end(), cmpAscending);
    }
}

void PrintNames(const vector<Animal>& animals) {
    for (const Animal& animal : animals) {
        cout << animal.name << ' ';
    }
    cout << endl;
}

int main() {
    vector<Animal> animals = {
        {"Мурка"s, 10, 5},
        {"Белка"s, 5, 1.5},
        {"Георгий"s, 2, 4.5},
        {"Рюрик"s, 12, 3.1},
    };

    // без сортировки: Мурка Белка Георгий Рюрик
    PrintNames(animals);

    // сортировка по имени: Белка Георгий Мурка Рюрик
    SortBy(
        animals, [](const Animal& animal) { return animal.name; }, true);
    PrintNames(animals);

    // сортировка по убыванию веса: Мурка Георгий Рюрик Белка
    SortBy(animals, [](const Animal& animal) { return animal.weight; });
    PrintNames(animals);

    return 0;
}