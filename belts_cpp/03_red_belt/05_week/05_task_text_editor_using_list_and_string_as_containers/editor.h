#include <algorithm>
#include <iterator>  // std::inserter, std::distance, std::make_move_iterator
#include <list>

class Editor {
   public:
    Editor() = default;                      // конструктор по умолчанию
    void Left();                             // сдвинуть курсор влево
    void Right();                            // сдвинуть курсор вправо
    void Insert(char token);                 // вставить символ token
    void Cut(size_t number_of_tokens = 1);   // вырезать не более number_of_tokens символов, начиная с текущей позиции курсора
    void Copy(size_t number_of_tokens = 1);  // cкопировать не более number_of_tokens символов, начиная с текущей позиции курсора
    void Paste();                            // вставить содержимое буфера в текущую позицию курсора
    std::string GetText() const;             // получить текущее содержимое текстового редактора

   private:
    std::list<char> _text{};
    std::string _buf{};
    uint64_t _pos{};
    std::list<char>::iterator _it_pos{std::begin(_text)};
};

void TypeText(Editor& editor, const std::string& text);
