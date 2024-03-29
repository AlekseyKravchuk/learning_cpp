

/*
=================== «Максимальный элемент по предикату» ===================
Условие задачи

Реализуйте шаблонную функцию

template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(
  ForwardIterator first, ForwardIterator last, UnaryPredicate pred);

возвращающую итератор на максимальный элемент в диапазоне [first, last), для которого предикат pred возвращает true. Если диапазон содержит несколько подходящих элементов с максимальным значением, то результатом будет первое вхождение такого элемента. Если диапазон не содержит подходящих элементов, то функция должна вернуть last.

Гарантируется, что:
    - pred принимает аргумент того типа, который имеют элементы диапазона
    - для ForwardIterator определены операторы ++, ==, !=, * (разыменование)
    - для типа, на который указывает итератор, определён оператор < («меньше»)
*/

#include <algorithm>
#include <forward_list>
#include <iterator>  // std::distance
#include <list>
#include <numeric>
#include <string>
#include <vector>

#include "test_runner.h"

using namespace std;

template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
    if (first == last) {
        return last;
    }

    // отдельно рассматриваем случай, когда в контейнере ровно 1 элемент
    if (std::distance(first, last) == 1) {
        return (pred(*first)) ? first : last;
    }

    auto it = first;
    auto largest = (pred(*first) ? first : last);
    
    while (++it != last) {
        if (pred(*it)) {
            if (largest != last) {
                largest = (*largest < *it) ? it : largest;
            } else {
                largest = it;
            }
        }
    }

    return largest;
}

void TestUniqueMax() {
    auto IsEven = [](int x) {
        return x % 2 == 0;
    };

    const list<int> hill{2, 4, 8, 9, 6, 4, 2};
    auto max_iterator = hill.begin();
    advance(max_iterator, 2);

    vector<int> numbers(10);
    iota(numbers.begin(), numbers.end(), 1);

    /*
      Мы не используем AssertEqual, потому что для итераторов отсутствует перегрузка оператора вывода в поток ostream.
      Разыменование здесь также недопустимо, так как оно может повлечь неопределенное поведение, если функция max_element_if, к примеру,
      вернула итератор, указывающий на конец контейнера.
    */
    Assert(
        max_element_if(numbers.begin(), numbers.end(), IsEven) == --numbers.end(),
        "Expect the last element");

    Assert(
        max_element_if(hill.begin(), hill.end(), IsEven) == max_iterator,
        "Expect the maximal even number");
}

void TestSeveralMax() {
    struct IsCapitalized {
        bool operator()(const string &s) {
            return !s.empty() && isupper(s.front());
        }
    };

    const forward_list<string> text{"One", "two", "Three", "One", "Two",
                                    "Three", "one", "Two", "three"};
    auto max_iterator = text.begin();
    advance(max_iterator, 4);

    Assert(
        max_element_if(text.begin(), text.end(), IsCapitalized()) == max_iterator,
        "Expect thr first \"Two\"");
}

void TestNoMax() {
    const vector<int> empty;
    const string str = "Non-empty string";

    auto AlwaysTrue = [](int) {
        return true;
    };

    Assert(
        max_element_if(empty.begin(), empty.end(), AlwaysTrue) == empty.end(),
        "Expect end for empty container");

    auto AlwaysFalse = [](char) {
        return false;
    };

    Assert(
        max_element_if(str.begin(), str.end(), AlwaysFalse) == str.end(),
        "Expect end for AlwaysFalse predicate");
}

void TestEqualValues() {
    vector<int> numbers = {5, 5, 5, 5, 5};

    auto IsOdd = [](int x) {
        return x % 2 != 0;
    };

    Assert(
        max_element_if(numbers.begin(), numbers.end(), IsOdd) == numbers.begin(),
        "max_element_if() returned end of the container but there is at least one suitable element");
}

void TestDescendingOrderWithRepetitions() {
    vector<int> numbers = {5, 5, 5, 5, 5, 3, 1};

    auto IsOdd = [](int x) {
        return x % 2 != 0;
    };

    Assert(
        max_element_if(numbers.begin(), numbers.end(), IsOdd) == numbers.begin(),
        "TestDescendingOrderWithRepetitions");
}

void TestExtra() {
    vector<int> numbers = {5, 5, 5, 7, 5, 5, 8, 5, 5};

    auto IsOdd = [](int x) {
        return x % 2 != 0;
    };

    Assert(
        max_element_if(numbers.begin(), numbers.end(), IsOdd) == (numbers.begin() + 3),
        "TestExtra");
}

void Test_1() {
    vector<int> numbers = {5};

    auto IsOdd = [](int x) {
        return x % 2 != 0;
    };

    Assert(
        max_element_if(numbers.begin(), numbers.end(), IsOdd) == (numbers.begin()),
        "Test_1");
}

void Test_2() {
    vector<int> numbers = {5, 5};

    auto IsOdd = [](int x) {
        return x % 2 != 0;
    };

    Assert(
        max_element_if(numbers.begin(), numbers.end(), IsOdd) == (numbers.begin()),
        "Test_2");
}

int main() {
    TestRunner tr;
    tr.RunTest(TestUniqueMax, "TestUniqueMax");
    tr.RunTest(TestSeveralMax, "TestSeveralMax");
    tr.RunTest(TestNoMax, "TestNoMax");
    tr.RunTest(TestEqualValues, "TestEqualValues");
    tr.RunTest(TestDescendingOrderWithRepetitions, "TestDescendingOrderWithRepetitions");
    tr.RunTest(TestExtra, "TestExtra");
    tr.RunTest(Test_1, "Test_1");
    tr.RunTest(Test_2, "Test_2");
    return 0;
}