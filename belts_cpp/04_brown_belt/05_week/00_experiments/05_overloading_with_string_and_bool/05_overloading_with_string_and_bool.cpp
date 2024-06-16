#include <iostream>
#include <string>

using namespace std;

// В следующей задаче вам понадобится перегружать некоторую функцию по разным типам, в числе которых есть string и bool.
// Упрощённо эту ситуацию можно представить так:

void PrintType(const string& s) {
    cout << "string " << s << endl;
}

void PrintType(bool b) {
    cout << "bool " << b << endl;
}

int main() {
    // В большинстве случаев перегрузка срабатывает вполне ожидаемо:
    PrintType(false); // выведет bool 0

    string s(5, '+');
    PrintType(s); // выведет string +++++

    // Но есть особый случай, в котором перегрузка ведёт себя, на первый взгляд, непредсказуемо:
    PrintType("+++");  // выведет bool 1

    // Разберёмся, почему так происходит и как проще всего решить проблему.
    // 1. Символы в кавычках — это не string
    // Если бы выражение "+++" имело тип string, никаких проблем бы не возникло.
    // Дело в том, что возможность так задавать строки пришла в C++ из C, где никакого string не было; "+++",
    // как и любой другой набор символов в кавычках, называется строковым литералом.

    // // Его тип легко определить, сложив два строковых литерала и вызвав тем самым ошибку компиляции:
    // PrintType("+++" + " ");

    // Для вызова PrintType(string) достаточно при вызове создать объект класса string по строковому литералу.
    // Простейший способ сделать это — явно вызвать нужный конструктор:
    PrintType(string("+++"));  // выведет string +++

    // Использование строкового литерала с суффиксом s позволяет сразу получить объект класса string:
    PrintType("+++"s);  // выведет string +++


    return 0;
}
