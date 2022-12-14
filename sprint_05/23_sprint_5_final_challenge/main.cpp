#include <iostream>
#include <vector>
#include <string>

#include "document.h"
#include "paginator.h"
#include "read_input_functions.h"
#include "request_queue.h"
#include "search_server.h"
#include "string_processing.h"

//using namespace std::literals;
using namespace std;

int main() {
    SearchServer search_server("and in at"s);
    RequestQueue requestQueue(search_server);

    search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, {1, 2, 3});
    search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, {1, 2, 8});
    search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, {1, 3, 2});
    search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, {1, 1, 1});

    // 1439 запросов с нулевым результатом
    for (int i = 0; i < 1439; ++i) {
        requestQueue.AddFindRequest("empty request"s);
    }

    // все еще 1439 запросов с нулевым результатом
    requestQueue.AddFindRequest("curly dog"s);

    // новые сутки, первый запрос удален, 1438 запросов с нулевым результатом
    requestQueue.AddFindRequest("big collar"s);

    // первый запрос удален, 1437 запросов с нулевым результатом
    requestQueue.AddFindRequest("sparrow"s);

    std::cout << "Total empty requests: "s << requestQueue.GetNoResultRequests() << std::endl;
    return 0;
}
