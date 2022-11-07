#include <iostream>

using namespace std;

// Функция для рекурсивного вычисления факториала:
// 0! = 1
// n! = n*(n-1)!
int Factorial(int n) {
    cout << "  Factorial("s << n << "): &n="s << &n << endl;
    return n > 0 ? n * Factorial(n - 1) : 1;
}

int main() {
    for (int i = 0; i < 4; ++i) {
        cout << "=== Calculating factorial of "s << i << " ==="s << endl;
        int f = Factorial(i);
        cout << "Result is: "s << f << endl
             << endl;
    }
}