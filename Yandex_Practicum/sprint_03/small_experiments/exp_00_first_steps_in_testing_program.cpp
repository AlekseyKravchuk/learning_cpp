/* 
===================== ПОСТАНОВКА ЗАДАЧИ =====================
Написать программу, которая считывает со стандартного входа три строки, разделённые пробелами и состоящие из строчных латинских букв.
Программа должна вывести в стандартный вывод лексикографически минимальную из этих трёх строк.
Например:
    - для строк milk, milkshake, month должна быть выведена строка milk;
    - для строк c, a, b должна быть выведена строка a;
    - для строк fire, fog, wood должна быть выведена строка fire.
=============================================================
 */

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int main() {
    string a, b, c;

    cin >> a >> b >> c;

    /*     if (a < b && a < c) {  // Первая строка является минимальной?
        cout << a << endl;
    } else if (b < a && b < c) {  // Вторая строка является минимальной?
        cout << b << endl;
    } else if (c < a && c < b) {  // Третья строка является минимальной?
        cout << c << endl;
    } */
    cout << std::min({a, b, c}) << endl;
}