#include <vector>

#include "test_runner.h"

using namespace std;

/*
================== Задание по программированию «Таблица» ==================
Вам надо написать шаблонный класс Table для электронной таблицы.
Для простоты будем считать, что все ячейки таблицы имеют один и тот же тип данных T.
Таблица должна уметь менять свой размер по требованию пользователя.
Вновь созданные ячейки должны заполняться значениями по умолчанию типа T.

Требования к классу такие:
    Класс должен называться Table.
    - У класса должен быть шаблонный параметр T - тип элемента в ячейке.
    - У класса должен быть конструктор, получающий на входе два числа типа size_t, - количество строк и столбцов соответственно.
    - У класса должны быть константная и неконстантная версии оператора [], возвращающего нечто такое, к чему снова можно было бы применить оператор [].
      То есть, должны работать конструкции вида cout << table[i][j];и table[i][j] = value;. 
      Проверять корректность индексов при этом не нужно.
    - У класса должна быть функция Resize, получающая на вход два параметра типа size_t и меняющая размер таблицы.
      Старые данные, умещающиеся в новый размер, должны при этом сохраниться.
    - У класса должен быть константный метод Size, возвращающий pair<size_t, size_t> размер таблицы
      (в том же порядке, в котором эти аргументы передавались в конструктор).
*/

template <typename T>
class Table {
   public:
    Table(size_t row_count, size_t column_count)
        : _table(row_count, std::vector<T>(column_count, T{})) {}

    const std::vector<T>& operator[](size_t idx) const {
        return _table[idx];
    }

    std::vector<T>& operator[](size_t idx) {
        return _table[idx];
    }

    void Resize(size_t new_row_count, size_t new_column_count) {
        // _table.resize(new_row_count, std::vector<T>(new_column_count, T{}));
        _table.resize(new_row_count);
        for (auto& row : _table) {
            row.resize(new_column_count, T{});
        }
    }

    std::pair<size_t, size_t> Size() const {
        return {_table.size(), _table[0].size()};
    }

   private:
    std::vector<std::vector<T>> _table;
};

void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}
