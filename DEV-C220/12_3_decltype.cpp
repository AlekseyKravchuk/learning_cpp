#include <iostream>
#include <string>
#include <vector>

#include "my_print.h"

int main() {
    const std::vector<int> v(5);
    // PrintCollection(v);

    // оператор индексирования вернет константную ссылку на int (const int&)
    // но при инициализации по значению с помощью "auto" ссылка (&) и константность (const)
    // будут проигнорированы, поэтому в результате "a" будет иметь тип "int"
    auto a = v[0];

    // при использовании "decltype" тип почти всегда будет таким же, как и тип выражения в скобках
    // то есть "const int&" => "b" будет иметь тип "const int&", при этом ссылка "b" должна быть проинициализирована
    // причем, поскольку ссылка КОНСТАНТНАЯ, она может быть проинициализирована абсолютным значением
    decltype(v[0]) b = 1;

    // а пример ниже НЕ скомпилируется, потому что ссылка должна быть проинициализирована при создании
    // decltype(v[0]) c;  // ОШИБКА!!!

    // используем обычный, НЕ константный вектор
    std::vector<int> vNonConst(5);

    // НЕконстантную ссылку нельзя проинициализировать абсолютным значением
    // такую ссылку можно проинициализировать только "lvalue"
    // decltype(vNonConst[0]) d = 5;  // ОШИБКА, инициализировать неконстантную ссылку можно только "lvalue"

    int y = 20;
    decltype(vNonConst[0]) d = y;  // а так уже можно, "d" будет иметь тип "int&"



    return 0;
}
