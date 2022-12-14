#include <algorithm>
#include <iostream>

using namespace std;

int MyMin(const int* l, const int* r) {
    if (l == nullptr) {
        return *r;
    }

    return min(*l, *r);
}

int main() {
    int x = 5, y = 10;
    cout << MyMin(&x, &y) << endl;
    cout << MyMin(nullptr, &y) << endl;
    cout << MyMin(&x, nullptr) << endl;
}