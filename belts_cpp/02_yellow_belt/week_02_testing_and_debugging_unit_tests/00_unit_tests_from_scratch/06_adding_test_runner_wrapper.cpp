#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

using Synonyms = std::map<std::string, std::set<std::string>>;

// выполняем декомпозицию задачи "Синонимы"

void AddSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word) {
    synonyms[first_word].insert(second_word);
    synonyms[second_word].insert(first_word);

    // // код с преднамеренно внесенной ошибкой
    // synonyms[first_word].insert(second_word);
    // synonyms[second_word].insert(second_word);
}

size_t GetSynonymsCount(Synonyms& synonyms,
                        const std::string& word) {
    return synonyms[word].size();

    // // // код с преднамеренно внесенной ошибкой
    // return synonyms[word].size() + 1;
}

bool AreSynonyms(Synonyms& synonyms,
                 const std::string& first_word,
                 const std::string& second_word) {
    return synonyms[first_word].count(second_word);

    // // код с преднамеренно внесенной ошибкой
    // return synonyms[first_word].count(second_word) + 1;
}

// =================================== Testing Framework ===================================

/*
=========== ХОЧЕТСЯ добиться следующего поведения нашего фреймворка: ===========
- Если хоть один из тестов упал, то после выполнения оставшихся тестов, программа должна завершиться.
- Если все тесты прошли, то должна отработать основная программа

=> Для этого обернем шаблон RunTest в класс TestRunner: если количество упавших тестов больше 0, то нужно завершить программу.
*/

class TestRunner {
   public:
    // Шаблон "RunTest" позволяет указывать блок "try-catch" только в одном месте и передавать в него произвольные функции для тестирования
    template <typename TestFunc>
    void RunTest(TestFunc func, const std::string& test_name) {
        try {
            if (_times_runtest_called == 0) {
                std::cerr << "======================== Beginning of TESTS ========================" << std::endl;
            }

            func();
            ++_times_runtest_called;  // учитываем очередной запуск функции-теста

            std::cerr << _times_runtest_called << ") "
                      << std::setfill(' ') << std::setw(30) << std::left
                      << test_name << " OK" << std::endl;
        } catch (const std::runtime_error& e) {
            ++_fail_count;            // увеличиваем счетчик упавших тестов
            ++_times_runtest_called;  // учитываем упавший тест в общем количестве запущенных тестов
            std::cerr << test_name << " fail: " << e.what() << std::endl;
        }
    }

    // В деструкторе оцениваем количество упавших тестов и общее количество запусков тестов
    ~TestRunner() {
        if (_fail_count) {
            std::cerr << "============ ATTENTION! " << _fail_count << " test(s) failed. Terminate. ============" << std::endl;
            exit(1);
        } else {
            std::cerr << "================= ALL " << _times_runtest_called << " TEST(S) SUCCESSFULLY PASSED =================" << std::endl;
        }
    }

   private:
    size_t _fail_count = 0;            // счетчик числа упавших тестов
    size_t _times_runtest_called = 0;  // счетчик количества запусков метода "RunTest"
};

template <typename Collection>
std::string Join(const Collection& collection, const std::string& separator = " ") {
    std::ostringstream oss;
    bool isFirst = true;

    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << separator;
        }

        isFirst = false;
        oss << elm;
    }

    return oss.str();
}

template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<FirstType, SecondType>& p) {
    os << "{" << p.first << " : " << p.second << "}";

    return os;
}

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    os << "{" << Join(m, ", ") << "}";

    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{" << Join(s, ", ") << "}";
}

template <typename T, typename U>
void AssertEqual(const T& t, const U& u, const std::string& hint) {
    if (t != u) {
        std::ostringstream oss;
        oss << "Assertion failed: " << t << " != " << u
            << " Hint: " << hint;

        // вылет этого исключения за пределы main  приводит к аварийному завершению программы
        // а хочется выполнять ВСЕ тесты, независимо от того, упал ли какой-то и них или нет
        throw std::runtime_error(oss.str());
    }
}

// для упрощения проверки булевских значений
void Assert(bool bool_value, const std::string& hint) {
    AssertEqual(bool_value, true, hint);
}
// ================== END of Testing Framework ==================

// =============================== TESTS ===============================
void TestAddSynonyms() {
    {
        Synonyms empty;
        AddSynonyms(empty, "a", "b");

        const Synonyms expected = {
            {"a", {"b"}},
            {"b", {"a"}}};

        AssertEqual(empty, expected, "add synonyms empty");
    }

    {
        Synonyms synonyms = {
            {"a", {"b"}},
            {"b", {"a", "c"}},
            {"c", {"b"}}};

        AddSynonyms(synonyms, "a", "c");
        Synonyms expected = {
            {"a", {"b", "c"}},
            {"b", {"a", "c"}},
            {"c", {"a", "b"}}};

        AssertEqual(synonyms, expected, "add synonyms NOT empty");
    }
}

void TestCount() {
    {
        Synonyms empty;
        AssertEqual(GetSynonymsCount(empty, "a"), 0u, "count for empty");
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        AssertEqual(GetSynonymsCount(synonyms, "a"), 2u, "count for a");
        AssertEqual(GetSynonymsCount(synonyms, "b"), 1u, "count for b");
        AssertEqual(GetSynonymsCount(synonyms, "z"), 0u, "count for z");
    }
}

void TestAreSynonyms() {
    {
        Synonyms emtpy;

        Assert(!AreSynonyms(emtpy, "a", "b"), "are synonyms a, b");
        Assert(!AreSynonyms(emtpy, "b", "a"), "are synonyms b, a");
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        Assert(AreSynonyms(synonyms, "a", "b"), "are synonyms: a, b");
        Assert(AreSynonyms(synonyms, "b", "a"), "are synonyms: b, a");
        Assert(AreSynonyms(synonyms, "a", "c"), "are synonyms: a, c");
        Assert(AreSynonyms(synonyms, "c", "a"), "are synonyms: c, a");
        Assert(!AreSynonyms(synonyms, "b", "c"), "are synonyms: b, c");
        Assert(!AreSynonyms(synonyms, "c", "b"), "are synonyms: c, b");
    }
}

void TestAll() {
    TestRunner test_runner;
    test_runner.RunTest(TestAddSynonyms, "TestAddSynonyms");
    test_runner.RunTest(TestCount, "TestCount");
    test_runner.RunTest(TestAreSynonyms, "TestAreSynonyms");
}  // здесь вызывается деструктор класса "TestRunner"

// ============================ END of TESTS ============================

int main() {
    TestAll();

    int querries_number;
    std::cin >> querries_number;  // считываем количество запросов, которое нам даны

    // словарь, который хранит для данной строки множество её синонимов
    Synonyms synonyms;

    for (int i = 0; i < querries_number; ++i) {
        std::string operation_code;
        std::cin >> operation_code;

        if (operation_code == "ADD") {
            std::string first_word, second_word;
            std::cin >> first_word >> second_word;
            AddSynonyms(synonyms, first_word, second_word);

        } else if (operation_code == "COUNT") {
            std::string word;
            std::cin >> word;
            std::cout << GetSynonymsCount(synonyms, word) << std::endl;
        } else if (operation_code == "CHECK") {
            std::string first_word, second_word;
            std::cin >> first_word >> second_word;
            AreSynonyms(synonyms, first_word, second_word);
        }
    }

    return 0;
}
