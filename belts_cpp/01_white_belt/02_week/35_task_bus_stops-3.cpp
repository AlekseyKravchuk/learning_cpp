#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Автобусные остановки — 3»

В этой задаче вам нужно присваивать номера автобусным маршрутам.

А именно, для каждого маршрута, заданного множеством названий остановок,
нужно либо выдать новый номер (первому маршруту — 1, второму — 2 и т. д.),
либо вернуть номер существующего маршрута, которому соответствует такое множество остановок.

В отличие от задачи «Автобусные остановки — 2», наборы остановок, которые можно получить друг из друга перестановкой элементов или добавлением/удалением повторяющихся, следует считать одинаковыми.
Формат ввода

Сначала вводится количество запросов Q, затем Q описаний запросов.

Каждый запрос представляет собой положительное количество остановок N, за которым следуют разделённые пробелом N названий остановок соответствующего маршрута (не обязательно различных). Названия остановок состоят лишь из латинских букв и символов подчёркивания.
Формат вывода

Выведите ответ на каждый запрос в отдельной строке.

Если маршрут с данным набором остановок уже существует, в ответ на соответствующий запрос выведите Already exists for i, где i — номер маршрута с таким набором остановок. В противном случае нужно выделить введённому набору остановок новый номер i и вывести его в формате New bus i.
Пример

Ввод

5
2 Marushkino Kokoshkino
1 Kokoshkino
2 Marushkino Kokoshkino
2 Kokoshkino Marushkino
2 Kokoshkino Kokoshkino

Вывод

New bus 1
New bus 2
Already exists for 1
Already exists for 1
Already exists for 2

*/

void ProcessOperations(int n) {
    std::map<std::set<std::string>, int> routes;
    int route_number{};

    for (int i = 0; i < n; ++i) {
        int num_stops{};
        std::cin >> num_stops;
        std::string stop_name{};
        std::set<std::string> stops;

        for (int k = 0; k < num_stops; ++k) {
            std::cin >> stop_name;
            stops.insert(stop_name);
        }

        if (!routes.count(stops)) {
            routes[stops] = ++route_number;
            std::cout << "New bus "s << routes[stops] << std::endl;
        } else {
            std::cout << "Already exists for "s << routes[stops] << std::endl;
        }
    }
}

int main() {
    // std::ifstream in("35_task_input.txt");      // configuring input from the file "35_task_input.txt"
    // std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    // std::cin.rdbuf(in.rdbuf());                 // redirect std::cin to "35_task_input.txt"!

    int n{};
    std::cin >> n;
    ProcessOperations(n);

    // std::cin.rdbuf(cinbuf);  // reset to standard input again

    return 0;
}
