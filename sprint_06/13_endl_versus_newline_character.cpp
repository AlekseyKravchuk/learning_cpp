#include <iostream>
#include <string>

#include "log_duration.h"

using namespace std;

// main с аргументами означает, что программа принимает аргументы командной строки,
// при запуске из консоли их количество будет сохранено в переменную argc,
// а значения — в argv.
// Тип const char** будет обсуждаться позже в курсе, работать с ним можно
// отчасти как с вектором: argv[0] — нулевой аргумент, argv[1] — первый.
// argv[i] можно конвертировать в string
int main(int argc, char const *argv[]) {
    // Нулевой аргумент — это всегда имя программы,
    // поэтому нам нужен первый
    int arg = stoi(argv[1]);

    if (arg == 1) {
        LOG_DURATION("endl"s);
        int i;
        while (cin >> i) {
            cout << i * i << endl;
        }
    }

    if (arg == 2) {
        // Чтобы "\n" воспринималось не как перевод строки, а как
        // слэш и буква 'n', добавим перед ним ещё один слеш:
        // два слеша в строковом (и символьном) литерале воспринимаются
        // как один (\)
        LOG_DURATION("\\n"s);
        int i;
        while (cin >> i) {
            cout << i * i << "\n"s;
        }
    }

    return 0;
}

// int main(int argc, char const *argv[]) {
    
//     return 0;
// }
