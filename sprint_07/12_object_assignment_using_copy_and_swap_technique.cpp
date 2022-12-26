class Object {
public:
    // копирующий конструктор
    Object(const Object& other);

    // копирующая операция присваивания
    Object& operator=(const Object& rhs) {
        if (this != &rhs) {
            // Реализация операции присваивания с помощью идиомы Copy-and-swap.
            // Если исключение будет выброшено, то на текущий объект оно не повлияет.
            auto rhs_copy(rhs);

            // rhs_copy содержит копию правого аргумента.
            // Обмениваемся с ним данными.
            swap(rhs_copy);

            // Теперь текущий объект содержит копию правого аргумента,
            // а rhs_copy - прежнее состояние текущего объекта, которое при выходе
            // из блока будет разрушено.
        }

        return *this;
    }

    // обменивает состояние текущего объекта с other без выбрасывания исключений
    void swap(Object& other) noexcept;

    ~Object();
};