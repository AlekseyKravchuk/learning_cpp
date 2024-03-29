// Практика 1:
//  enum class
//  initializer_list и универсальные списки инициализации
//  auto
//  decltype
//  lambda
//  template variable

#include "01_practice.h"

#include <cassert>
#include <cctype>  // std::toupper
#include <deque>
#include <initializer_list>
#include <list>

using namespace std::literals;

int main() {
    // Задание 1.
    {
        std::cout << "====================== Task #1 ======================"s << std::endl;

        enum class months : unsigned char { January,
                                            February,
                                            March /*...*/
        };

        enum class weekDays { Monday,
                              Tuesday,
                              Wednesday /*...*/ };

        months m = months::January;
        // а) уберите проблемы (это значит, что КОМПИЛЯТОР не
        // должен позволять программисту осуществлять опасные
        // сравнения!)

        if (m == months::February) { /*...*/
        }

        // б) оптимизируйте использование памяти

        months year[] = {months::January, months::February, months::March};
        size_t n = sizeof(year);
        std::cout << "size of months year[]: " << n << std::endl;

        std::cout << "=================== Task #1 ended ==================="s << std::endl
                  << std::endl;
    }

    {
        // Задание 2. Создайте и с помощью списка инициализации
        // заполните значениями вектор с элементами - string
        // С помощью range-based for преобразуйте все буквы в
        // верхний регистр а остальные символы должны остаться
        // неизменными

        std::cout << "====================== Task #2 ======================"s << std::endl;

        std::vector<std::string> words = {"a-ba"s, "c=aba"s, "ba12ba"s};
        for (auto& word : words) {
            for (auto& ch : word) {
                ch = std::toupper(ch);
            }
        }

        PrintCollection(words);

        std::cout << "=================== Task #2 ended ==================="s << std::endl
                  << std::endl;
    }

    {
        /// Задание 3. Создайте и заполните значениями
        // map двумя способами (3а и 3б) таким образом, чтобы он содержал
        // в качестве ключа букву, а в качестве значения
        // отсортированную по алфавиту совокупность слов (string),
        // начинающихся с этой буквы

        // 3а. Проинициализируйте map "вручную" посредством списка инициализации
        // С помощью range-based for и structured binding
        // распечатайте содержимое, например: A: any, apple, away

        std::cout << "====================== Task #3a ======================"s << std::endl;
        std::map<char, std::vector<std::string>> mp =
            {
                {'A', {"any"s, "apple"s, "away"s}},
                {'B', {"book"s, "boom"s, "bottom"s}},
                {'C', {"connection"s, "concise"s, "cesar"s}}};

        for (const auto& [character, words_collection] : mp) {
            std::cout << character << ": ";
            PrintCollection(words_collection);
        }
        std::cout << "=================== Task #3a ended ==================="s << std::endl
                  << std::endl;
    }

    {
        // 3б. Создайте пустой map и используйте заданный массив
        // для выполнения задания.
        // С помощью range-based for и structured binding
        // распечатайте содержимое, например: A: any, apple, away

        // дано (например):
        std::cout << "====================== Task #3b ======================"s << std::endl;
        const char* words[] = {"yong", "away", "Bar", "any", "son", "Apple", "West", "home"};
        std::map<char, std::set<std::string>> mp;

        for (const auto& word : words) {
            mp[std::toupper(word[0])].insert(word);
        }

        for (const auto& [ch, words] : mp) {
            std::cout << ch << ": "s;
            PrintCollection(words);
        }

        std::cout << "=================== Task #3b ended ==================="s << std::endl;
    }

    {
        // Задание 4. создать функцию для вывода на печать
        // элементов последовательностей, заданных ниже:
        std::cout << std::endl
                  << "******************** Task #4 ********************"s << std::endl;

        std::vector<double> vd = {1.1, 2.2, 3.3};
        // PrintAnyCont(vd);
        PrintCollection(vd);

        std::string s("abc");
        // PrintAnyCont(s);
        PrintCollection(s);

        int ar[] = {1, 2, 3};
        // PrintAnyCont(ar);
        PrintCollection(ar);

        std::initializer_list<int> il{3, 4, 5};
        // PrintAnyCont(il);
        PrintCollection(il);

        std::cout << std::endl
                  << "**************** Task #4 ended ****************"s << std::endl;
    }

    {
        /// Задание 5.
        // Cоздать функцию для "отрицания" значений, например:
        // было: {1, -2, 5}, стало: {-1, 2, -5})
        // изменение объектов типа std::string может выглядеть "aBc1" -> "AbC1"
        // элементов последовательностей, заданных ниже:

        std::cout << std::endl
                  << "******************** Task #5 ********************"s << std::endl;

        std::vector<double> vd{1.1, 2.2, 3.3};
        PrintCollection(vd, "Initial vd: "s);
        NegateAll(vd);
        PrintCollection(vd, "vd after applying NegateAll(vd): "s);
        std::cout << std::endl;

        std::list<std::string> ls{"aBc"s, "Qwerty"s, "n12"s};
        PrintCollection(ls, "Initial ls: "s);
        NegateAll(ls);
        PrintCollection(ls, "ls after applying NegateAll(ls): "s);
        std::cout << std::endl;

        int ar[]{1, 2, 3};
        PrintCollection(ar, "Initial ar: "s);
        NegateAll(ar);
        PrintCollection(ar, "ar after applying NegateAll(ar): "s);
        std::cout << std::endl;

        std::cout << std::endl
                  << "**************** Task #5 ended ****************"s << std::endl;
    }

    {
        // Задание 6. Реализовать функцию сортировки по модулю
        // элементов последовательностей, заданных ниже
        // Собственно для сортировки можно использовать обобщенный
        // алгоритм sort(), а для задания условия - лямбда-функцию
        std::cout << std::endl
                  << "******************** Task #6 ********************"s << std::endl;

        std::vector<double> vd = {-3.3, 2.2, -1.1};
        absSort(vd);
        PrintCollection(vd, "vd after absSort: "s);

        int ar[] = {-3, 2, -1};
        absSort(ar);
        PrintCollection(ar, "ar after absSort: "s);

        std::cout << std::endl
                  << "**************** Task #6 ended ****************"s << std::endl;
    }

    {
        // Задание 7.
        // Напишите функцию, которая будет формировать и
        // возвращать вектор, каждый элемент
        // которого является суммой элементов двух
        // последовательностей РАЗНОЙ длины
        // и с элементами РАЗНОГО типа.

        // Подсказка 1: так как последовательности могут быть
        // разной длины, логично сделать размер результирующего
        // вектора максимальным из двух

        // Подсказка 2: подумайте о возможности использования
        // алгоритмов copy() и transform(), в котором
        // трансформирующее действие удобно в свою очередь
        // задавать лямбда-функцией

        // например:
        // {
        // 	std::vector<int> v{ 1,2,3,4 };
        // 	std::list<double> l{ 1.1, 2.2, 3.3, 4.4, 5.5 };

        // 	//??? = SumCont(v, l);

        // 	std::list<int> ll{ 1, 2, 3, 4, 5, 6, 7, 8 };
        // 	double ar[] = { 1.1, 2.2, 3.3, 4.4, 5.5 };
        // 	//??? = SumCont(ar, ll);

        // 	std::set<std::string> s{ "abc", "qwerty", "my"};
        // 	std::deque<const char*> d{ "111", "22" };
        // 	//??? = SumCont(s, d);
        // }

        std::cout << std::endl
                  << "******************** Task #7 ********************"s << std::endl;

        std::vector<int> v{1, 2, 3, 4};
        std::list<double> l{1.1, 2.2, 3.3, 4.4, 5.5};
        auto resVector_1 = SumCont(v, l);
        PrintCollection(resVector_1);
        std::vector<double> testResult_1 = {2.1, 4.2, 6.3, 8.4, 5.5};
        assert(resVector_1 == testResult_1);
        // Desired output: 2.2, 4.2, 6.3, 8.4, 5.5

        std::list<int> ll{1, 2, 3, 4, 5, 6, 7, 8};
        double ar[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        auto resVector_2 = SumCont(ar, ll);
        PrintCollection(resVector_2);
        std::vector<double> testResult_2 = {2.1, 4.2, 6.3, 8.4, 10.5, 6, 7, 8};
        assert(resVector_2 == testResult_2);
        // Desired output: 2.1, 4.2, 6.3, 8.4, 10.5, 6, 7, 8

        std::set<std::string> s{"abc", "qwerty", "my"};
        std::deque<const char*> d{"111", "22"};
        auto resVector_3 = SumCont(s, d);
        PrintCollection(resVector_3);
        std::vector<std::string> testResult_3 = {"111abc"s, "22my"s, "qwerty"s};
        assert(resVector_3 == testResult_3);
        // Desired output: 111abc 22my qwerty

        std::cout << std::endl
                  << "**************** Task #7 ended ****************"s << std::endl;
    }

    {
        std::cout << std::endl
                  << "******************** Task #8 ********************"s << std::endl;
        // Задание 8. Реализуйте функцию, которая принимает следующие параметры:
        // сформированную последовательность любого типа с элементами любого типа,
        // два (пустых) контейнера любого типа из vector, list, deque, set
        // с элементами того же типа, что и у сформированной последовательности

        // Функция должна "разложить" значения заданной последовательности в два пустых контейнера
        // согласно заданному условию. Условие задать лябда-функцией
        // Исходная последовательность при этом не меняется

        std::vector<int> v{1, 2, 3, 4, 5};
        std::list<int> l;   // сюда четные
        std::deque<int> d;  // а сюда нечетные

        Separate(v, l, d, [](auto val) {
            return val % 2 == 0;
        });

        PrintCollection(l, "Even numbers: "s);
        PrintCollection(d, "Odd numbers: "s);

        std::cout << std::endl
                  << "******************** Task #8 ended ********************"s << std::endl;
    }

    {
        std::cout << std::endl
                  << "******************** Task #9 ********************"s << std::endl;
        // Задание 9. C помощью алгоритма for_each()!!!
        //(а не count_if()) посчитать сколько букв в верхнем
        // регистре.
        //   Использовать лямбда функцию

        char s[] = "Hello World and SoMe other UPPERCASES!";
        int count = 0;
        std::for_each(std::begin(s), std::end(s),
                      [&count](char ch) {
                          if (std::isupper(ch)) {
                              ++count;
                          }
                      });
        std::cout << "Букв в верхнем регистре: " << count << std::endl;

        std::cout << std::endl
                  << "******************** Task #9 ended ********************"s << std::endl;
    }

    {
        std::cout << std::endl
                  << "******************** Task #10 ********************"s << std::endl;
        // Задание 10. Реализовать конвертацию enum в строковое представление  - enumToString и наоборот - stringToEnum

        // ============================ Подсказки: ============================
        //***********
        // 1. Соответствующие именованным константам строки все равно нужно где-то хранить =>
        // 1.1 Именованные константы в перечислении должны быть уникальными => соответствующие строки тоже должны быть уникальными,
        // => уникальные строки могут быть использованы в качестве ключа в std::map

        // 1.2 а вот значения (соответствующие именованым константам) могут быть любыми и даже могут повторяться
        //  (упрощаем задачу, считая, что значения НЕ повторяются) => подходит контейнер std::map<string,<значение> >

        // 1.3 Согласно стандарту С++11 переменные перечислимого типа могут быть разного размера и типа,
        //  а хотелось бы обеспечить универсальную конвертацию

        //***********
        // 2.1 Так как типы перечислений разные, то enumToString и stringToEnum должны быть шаблонными
        // 2.2 Пользователь может ошибиться или злонамеренно передать в функции значения, которым
        // ничего не соответствует => защита!

        //***********
        // 3. Чтобы действия с map<string, <значение> > не зависели от типа перечисления,
        // подумайте над использованием шаблонной переменной (в частности вспомните о возможности специализации шаблонной переменной)

        std::cout << static_cast<int>(stringToEnum<State>("Walk"s)) << std::endl;
        std::cout << enumToString<State>(State::Scan) << std::endl;
        // ==================== Вариант 2 ====================

        std::cout << std::endl
                  << "******************** Task #10 ended ********************"s << std::endl;
    }
}