#include <iostream>
#include <string>
#include <vector>

using namespace std;
using WhiteElephant = string;

// функция, которая умеет делать очень больших слонов
WhiteElephant BuyElephant() {
    return WhiteElephant(100000000, 'a');
}

int main() {
    // в начале создаётся ПУСТАЯ толпа слонов
    vector<WhiteElephant> crowd_of_elephants;

    // Пользователь покупает слона и сохраняет его в переменную "heavy_elephant"
    WhiteElephant heavy_elephant = BuyElephant();

    // После этого слон добавляется в толпу
    // Но в момент копирования heavy_elephant в толпу слонов станет двое.
    // Один теперь в толпе с другими слонами, а второй остался в переменной heavy_elephant.
    // Вряд ли покупатель слонов хотел копировать животных.
    // crowd_of_elephants.push_back(heavy_elephant);

    // В языке С++ есть способ, позволяющий не размножать слонов и другие объекты там, где не нужно.
    // Этот метод использует move-семантику.
    // Вместо семантики копирования — один слон в толпе, второй у хозяина — можно использовать семантику перемещения:
    // забрать слона у хозяина и поместить в толпу.

    // Если изменим код так, чтобы купленный слон не сохранялся в переменную heavy_elephant,
    // а напрямую передавался в метод push_back, временный слон, вернувшийся после BuyElephant будет напрямую перемещён в толпу.
    // Компилятор видит, что имеет дело с временным объектом, и понимает, что объект будет уничтожен сразу после переноса в вектор.
    // Поэтому компилятор не копирует объект, а переносит его.
    crowd_of_elephants.push_back(BuyElephant());
}