// C++ function for illustration
// map::lower_bound() function

// std::map::lower_bound возвращает ИТЕРАТОР, указывающий на первый элемент, который НЕ МЕНЬШЕ (т.е. больше или равен) ключа KEY
// 1) Если KEY есть в мапе, возвращается итератор на этот элемент (на пару <key, value>);
// 2) Если ключ KEY в мапе НЕТ, НО существует ключ с бОльшим чем KEY значением, возвращается итератор на этот (следующий) элемент;
// 3) Если ключа KEY в мапе НЕТ И KEY превышает значение максимального ключа в мапе, то возвращается итератор "std::map::end()"
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    // initialize container
    map<int, string> mp;

    // insert elements in random order
    mp.insert({1971, "Marina"s});
    mp.insert({1967, "Polina"s});
    mp.insert({1980, "Olga"s});
    mp.insert({1961, "Oksana"s});
    for (auto it = mp.begin(); it != mp.end(); it++) {
        cout << (*it).first << " " << (*it).second << endl;
    }

    // если ключ 1967 присутствует в мапе, возвращается итератор на пару с этим ключом
    int key_present = 1967;
    auto it = mp.lower_bound(1967);
    std::cout << "Key is present: ";
    cout << "The lower bound of key "s << key_present << " is ";
    cout << (*it).first << " " << (*it).second << endl;

    // если ключ 1975 ОТСУТСТВУЕТ в мапе и принадлежит диапазону ключей мапы,
    // возвращается итератор, который указывает на следующую пару в контейнере
    int key_NOT_present = 1975;
    std::cout << "Key is NOT present, but in range: ";
    it = mp.lower_bound(key_NOT_present);
    cout << "The lower bound of key "s << key_NOT_present << " is ";
    cout << (*it).first << " " << (*it).second;

    // когда ключ 1990 больше максимального ключа в мапе, возвращается итератор map::end()
    // при этом "*(it).first = количетво_элементов_в_словаре", а "*(it).second = ANY_value",
    it = mp.lower_bound(1990);
    if (it == mp.end()) {
        std::cout << "Ooops"s << std::endl;
    }
    cout << "\nThe lower bound of key 1990 is ";
    cout << (*it).first << " " << (*it).second;
    std::cout << std::endl;

    return 0;
}
