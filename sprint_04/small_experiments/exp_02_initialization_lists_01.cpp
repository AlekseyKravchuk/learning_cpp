#include <iostream>
#include <string>

using namespace std;

/*
Список инициализации конструктора служит для инициализации полей класса до выполнения тела конструктора.
Старайтесь всегда задавать значения полей класса в списке инициализации конструктора, а не в его теле.
Тогда программа не будет выполнять двойную работу: вызывать у поля конструктор по умолчанию, а потом перезаписывать его значение через присваивание.
В списке инициализации сразу вызовется нужный параметризованный конструктор.
*/

class Cat {
   public:
    // задаём значение поля name_, копируя в него аргумент конструктора
    Cat(const string& name) : _name(name) {}

    string GetName() const {
        return _name;
    }

   private:
    string _name;
};

class Witch {
   public:
    // Передаём параметр конструктору поля cat_
    Witch(const string& catName) : _cat(catName) {
        cout << "Моего кота зовут "s << _cat.GetName() << endl;
    }

   private:
    Cat _cat;
};

int main() {
    Witch hermione{"Живоглот"s};
}