// ******************************** Задание 1 ********************************
// У вас есть каркас шаблонного класса ScopedPtr. Это умный указатель, в котором частично реализован такой функционал:

//     - Конструктор по умолчанию инициализирует указатель нулевым значением;
//     - Заготовка метода GetRawPtr. Метод возвращает адрес объекта, на который ссылается умный указатель;
//     - Заготовка конвертирующего конструктора для создания ScopedPtr из сырого указателя;
//     - Удалённый конструктор копирования. В этом случае создать копию умного указателя будет нельзя.
//       Как следствие, не будет проблем, когда два умных указателя владеют одним объектом.

// =========================== ЗАГОТОВКА ===========================
// template <typename T>
// class ScopedPtr {
// public:
//     ScopedPtr() = default;
//     explicit ScopedPtr(T* raw_ptr) noexcept {...}
//     ScopedPtr(const ScopedPtr&) = delete;
//     ~ScopedPtr() {...}
//     T* GetRawPtr() const noexcept {...}
//     T* Release() noexcept {...}

// private:
//     T* ptr_ = nullptr;
// };
// ==================================================================

// Объект класса ScopedPtr<T> может находиться в двух состояниях: владеть объектом типа T или не владеть.
// При этом должны выполняться следующие правила:

//     - Конструктор без параметров создаёт умный указатель, не владеющий объектом.
//     - Конструктор ScopedPtr(T* raw_ptr) создаёт умный указатель, который владеет объектом,
//       если и только если переданный указатель не был равен nullptr.
//     - Метод GetRawPtr не влияет на владение.
//     - Метод Release отменяет владение, если оно было.

// Цель умного указателя — удалить объект, которым он владеет.
// Он должен делать это в своём деструкторе, который вам предстоит реализовать вместе с недостающими методами.
// Проверки в заготовке кода упростят реализацию конструкторов и деструкторов.

// Требования
// Реализуйте недостающие методы класса ScopedPtr:

//     - Недостающий код конструктора ScopedPtr из «сырого» указателя на объект в динамической памяти. Созданный ScopedPtr должен владеть переданным объектом. Допустимо значение указателя nullptr. В этом случае владения не возникает.
//     - Тело метода GetRawPtr.
//     - Удаление объекта, которым владеет ScopedPtr, в деструкторе.
//     - Тело метода Release.

// Ограничения
// Сохраните сигнатуры всех публичных методов класса ScopedPtr неизменными, чтобы код скомпилировался без ошибок.

#include <cassert>
#include <utility>  // std::exchange

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ScopedPtr {
public:
    // Конструктор по умолчанию создаёт нулевой указатель,
    // так как поле "_ptr" имеет значение по умолчанию "nullptr"
    ScopedPtr() = default;

    // Создаёт указатель, ссылающийся на переданный "rawPtr".
    // "rawPtr" ссылается либо на объект, созданный в куче при помощи new,
    // либо является нулевым указателем
    // Спецификатор noexcept обозначает, что метод не бросает исключений
    explicit ScopedPtr(T* rawPtr) noexcept {
        _ptr = rawPtr;
    }

    // Удаляем у класса конструктор копирования
    ScopedPtr(const ScopedPtr&) = delete;

    // Деструктор. Удаляет объект, на который ссылается умный указатель.
    ~ScopedPtr() {
        // Операции "delete" можно безопасно передавать указатель, равный "nullptr", — в этом случае она ничего не делает.
        delete _ptr;
    }

    // Возвращает указатель, хранящийся внутри ScopedPtr
    T* GetRawPtr() const noexcept {
        return _ptr;
    }

    // Прекращает владение объектом, на который ссылается умный указатель.
    // Возвращает прежнее значение "сырого" указателя и устанавливает поле "_ptr" в null
    T* Release() noexcept {
        return std::exchange(_ptr, nullptr);
    }

private:
    T* _ptr = nullptr;
};

// Этот main тестирует класс ScopedPtr
int main() {
    // Вспомогательный "шпион", позволяющий узнать о своём удалении
    struct DeletionSpy {
        explicit DeletionSpy(bool& is_deleted) : _isDeleted(is_deleted) { }

        ~DeletionSpy() {
            _isDeleted = true;
        }

        bool& _isDeleted;
    };

    // Проверяем автоматическое удаление
    {
        bool is_deleted = false;
        {
            // настраиваем "шпион", чтобы при своём удалении он выставил is_deleted в true
            DeletionSpy* raw_ptr = new DeletionSpy(is_deleted);
            ScopedPtr<DeletionSpy> p(raw_ptr);
            assert(p.GetRawPtr() == raw_ptr);
            assert(!is_deleted);
            // При выходе из блока деструктор p должен удалить "шпиона"
        }
        // Если деструктор умного указателя работает правильно, шпион перед своей "смертью"
        // должен выставить is_deleted в true
        assert(is_deleted);
    }

    // Проверяем работу метода Release
    {
        bool is_deleted = false;
        DeletionSpy* raw_ptr = new DeletionSpy(is_deleted);
        {
            ScopedPtr<DeletionSpy> scoped_ptr(raw_ptr);
            assert(scoped_ptr.Release() == raw_ptr);
            assert(scoped_ptr.GetRawPtr() == nullptr);
            // После Release умный указатель не ссылается на объект и не удаляет его при своём удалении
        }
        assert(!is_deleted);
        delete raw_ptr;
        assert(is_deleted);
    }
}