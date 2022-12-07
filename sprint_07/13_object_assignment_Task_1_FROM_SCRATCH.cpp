// ******************************** Задание 1 ********************************
// Реализуйте операцию присваивания в классе PtrVector.
// Операция должна обеспечивать строгую гарантию безопасности исключений.
// Если в процессе присваивания будет выброшено исключение, PtrVector должен остаться в состоянии, в котором он был до выполнения этой операции.
// В заготовке кода вы видите тесты, которые упрощают проверку задачи.
// Также там приведена допустимая реализация PtrVector из предыдущей задачи. Вы можете заменить её на свою.
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

using namespace std;

// Используйте эту заготовку PtrVector или замените её на свою реализацию
template <typename T>
class PtrVector {
   public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Резервируем место в vector-е для хранения нужного количества элементов
        // Благодаря этому при push_back не будет выбрасываться исключение
        _items.reserve(other._items.size());

        try {
            for (auto p : other._items) {
                // Копируем объект, если указатель на него ненулевой
                auto p_copy = p ? new T(*p) : nullptr;  // new может выбросить исключение

                // Не выбросит исключение, т. к. в vector память уже зарезервирована
                _items.push_back(p_copy);
            }
        } catch (...) {
            // удаляем элементы в векторе и перевыбрасываем пойманное исключение
            DeleteItems();
            throw;
        }
    }

    // Оператор присваивания класса PtrVector - обеспечивает строгую гарантию безопасности исключений.
    // Если в процессе присваивания будет выброшено исключение, PtrVector должен остаться в состоянии,
    // в котором он был до выполнения этой операции.
    PtrVector& operator=(const PtrVector& rhs) {
        // Обеспечить строгую гарантию безопасности исключений (strong exception guarantee) в пользовательском
        // операторе присваивания позволяет идиома “copy-and-swap”.
        // В ней оператор присваивания переиспользует функционал конструктора копирования.

        // если применяем операцию взятия адреса "&" к ссылке, то получаем адрес объекта
        // если адреса равны, возвращаем ссылку на текущий объект
        if (this == &rhs) {
            return *this;
        }

        // вызываем конструктор копирования, создаем копию вектора "rhs"
        PtrVector rhsCopied(rhs);

        // осталось только поменять содержимое двух векторов - текущего (*this) и вектора-копии ("rhsCopied")
        // метод "swap" класса "vector" обменивает содержимое двух контейнеров за время O(1) и не выбрасывает исключений.
        _items.swap(rhsCopied._items);

        return *this;
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе _items
    ~PtrVector() {
        DeleteItems();
    }

    // Возвращает ссылку на вектор указателей
    vector<T*>& GetItems() noexcept {
        return _items;
    }

    // Возвращает константную ссылку на вектор указателей
    vector<T*> const& GetItems() const noexcept {
        return _items;
    }

   private:
    void DeleteItems() noexcept {
        for (auto p : _items) {
            delete p;
        }
    }

    vector<T*> _items;
};

// Эта функция main тестирует шаблон класса PtrVector
int main() {
    struct CopyingSpy {
        CopyingSpy(int& copy_count, int& deletion_count)
            : copy_count_(copy_count), deletion_count_(deletion_count) {
        }
        CopyingSpy(const CopyingSpy& rhs)
            : copy_count_(rhs.copy_count_)  // счётчик копирований
              ,
              deletion_count_(rhs.deletion_count_)  // счётчик удалений
        {
            if (rhs.throw_on_copy_) {
                throw runtime_error("copy construction failed"s);
            }
            ++copy_count_;
        }
        ~CopyingSpy() {
            ++deletion_count_;
        }
        void ThrowOnCopy() {
            throw_on_copy_ = true;
        }

       private:
        int& copy_count_;
        int& deletion_count_;
        bool throw_on_copy_ = false;
    };

    // Проверка присваивания
    {
        int item0_copy_count = 0;
        int item0_deletion_count = 0;
        {
            PtrVector<CopyingSpy> v;

            v.GetItems().push_back(new CopyingSpy(item0_copy_count, item0_deletion_count));
            v.GetItems().push_back(nullptr);
            {
                PtrVector<CopyingSpy> v_copy;
                v_copy = v;
                assert(v_copy.GetItems().size() == v.GetItems().size());
                assert(v_copy.GetItems().at(0) != v.GetItems().at(0));
                assert(v_copy.GetItems().at(1) == nullptr);
                assert(item0_copy_count == 1);
                assert(item0_deletion_count == 0);
            }
            assert(item0_deletion_count == 1);
        }
        assert(item0_deletion_count == 2);
    }

    // Проверка корректности самоприсваивания
    {
        int item0_copy_count = 0;
        int item0_deletion_count = 0;

        PtrVector<CopyingSpy> v;
        v.GetItems().push_back(new CopyingSpy(item0_copy_count, item0_deletion_count));
        CopyingSpy* first_item = v.GetItems().front();

        v = v;
        assert(v.GetItems().size() == 1);
        // При самоприсваивании объекты должны быть расположены по тем же адресам
        assert(v.GetItems().front() == first_item);
        assert(item0_copy_count == 0);
        assert(item0_deletion_count == 0);
    }

    // Проверка обеспечения строгой гарантии безопасности исключений при присваивании
    {
        int item0_copy_count = 0;
        int item0_deletion_count = 0;

        int item1_copy_count = 0;
        int item1_deletion_count = 0;

        // v хранит 2 элемента
        PtrVector<CopyingSpy> v;
        v.GetItems().push_back(new CopyingSpy(item0_copy_count, item0_deletion_count));
        v.GetItems().push_back(new CopyingSpy(item1_copy_count, item1_deletion_count));

        int other_item0_copy_count = 0;
        int other_item0_deletion_count = 0;
        // other_vector хранит 1 элемент, при копировании которого будет выброшено исключение
        PtrVector<CopyingSpy> other_vector;
        other_vector.GetItems().push_back(new CopyingSpy(other_item0_copy_count, other_item0_deletion_count));
        other_vector.GetItems().front()->ThrowOnCopy();

        // Сохраняем массив указателей
        auto v_items(v.GetItems());

        try {
            v = other_vector;
            // Операция должна выбросить исключение
            assert(false);
        } catch (const runtime_error&) {
        }

        // Элементы массива должны остаться прежними
        assert(v.GetItems() == v_items);
        assert(item0_copy_count == 0);
        assert(item1_copy_count == 0);
        assert(other_item0_copy_count == 0);
    }
}