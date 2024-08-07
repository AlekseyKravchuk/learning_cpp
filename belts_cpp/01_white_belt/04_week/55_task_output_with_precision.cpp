#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std::literals;

/*

Задание по программированию «Вывод с точностью»

В файле input.txt записаны вещественные числа, по одному на строчку.
На стандартный вывод напечатайте эти числа в том же порядке, по одному на строке, но сделайте так,
чтобы у каждого из них было ровно три знака после десятичной точки.

Пример ввода 1
5
0.34567
10.4

Пример вывода 1
5.000
0.346
10.400
 
Пример ввода 2
3.33353
3.333
3.3

Пример вывода 2
3.334
3.333
3.300

Примечание:
Воспользуйтесь стандартными манипуляторами потоков вывода.
*/

int main() {
    std::string path{"input.txt"s};
    std::ifstream input(path);

    double value{};
    if (input) {
        while (input >> value) {
            std::cout << std::fixed << std::setprecision(3) << value << std::endl;
        }
    }

    return 0;
}
