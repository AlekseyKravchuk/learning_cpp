#include <iostream>
#include <string>
#include <utility>  // std::as_const
#include <vector>

using namespace std::literals;

class A {
   public:
    virtual void F(int n = 1) {
        std::cout << "Virtual A::F()."s << std::endl;
        std::cout << "Accepted default parameter with value = "s << n << std::endl;
    }
};

class B : public A {
   private:
    void F() /* override */ {
        std::cout << "Virtual B::F()."s << std::endl;
    }
};

int main() {
    A* p = new B;
    p->F();

    return 0;
}
