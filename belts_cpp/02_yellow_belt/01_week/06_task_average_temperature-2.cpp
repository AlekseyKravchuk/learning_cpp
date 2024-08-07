#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

/*
Средняя температура-2
Решите задачу «Средняя температура» в других ограничениях на входные данные.

Даны значения температуры, наблюдавшиеся в течение N подряд идущих дней.
Найдите номера дней (в нумерации с нуля) со значением температуры выше среднего арифметического за все N дней.

Гарантируется, что среднее арифметическое значений температуры является целым числом.

Формат ввода:
Вводится число N, затем N целых чисел — значения температуры в 0-й, 1-й, ... (N−1)-й день.
Гарантируется, что N не превышает миллиона (10^6), а значения температуры,
измеряющиеся в миллионных долях градусов по Цельсию, лежат в диапазоне от −10^8 до 10^8.
Проверять соблюдение этих ограничений не нужно: вы можете ориентироваться на них при выборе наиболее подходящих типов для переменных.

Формат вывода:
Первое число K — количество дней, значение температуры в которых выше среднего арифметического. Затем K целых чисел — номера этих дней.
Пример

Ввод:
5
5 4 1 -2 7

Вывод:
3
*/

int main() {
    int n;
    std::cin >> n;
    std::vector<int> t(n);

    double average = 0.0;
    for (int i = 0; i < n; ++i) {
        std::cin >> t[i];
        // используем формулу для "накопленного среднего", CMA - Cumulative Moving Average
        // CMA[n+1] = CMA[n] + (x[n+2] - CMA[]) / (n + 1)
        average += (t[i] - average) / (i + 1);
    }

    std::vector<int> indices;
    for (int i = 0; i < n; ++i) {
        if (t[i] > average) {
            indices.push_back(i);
        }
    }

    std::cout << indices.size() << std::endl;
    bool isFirst = true;

    for (const auto& index: indices) {
        if (isFirst) {
            std::cout << index;
            isFirst = false;
        } else {
            std::cout << ' ' << index;
        }
    }
    std::cout << std::endl;

    return 0;
}
