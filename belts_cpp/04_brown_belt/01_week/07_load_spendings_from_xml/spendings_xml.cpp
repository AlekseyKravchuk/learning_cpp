#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "test_runner.h"
#include "xml.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

bool operator==(const Spending& lhs, const Spending& rhs) {
    return lhs.category == rhs.category && lhs.amount == rhs.amount;
}

ostream& operator<<(ostream& os, const Spending& s) {
    return os << '(' << s.category << ": " << s.amount << ')';
}

int CalculateTotalSpendings(
    const vector<Spending>& spendings) {
    int result = 0;
    for (const Spending& s : spendings) {
        result += s.amount;
    }
    return result;
}

string MostExpensiveCategory(
    const vector<Spending>& spendings) {
    auto compare_by_amount =
        [](const Spending& lhs, const Spending& rhs) {
            return lhs.amount < rhs.amount;
        };
    return max_element(begin(spendings), end(spendings),
                       compare_by_amount)
        ->category;
}

vector<Spending> LoadFromXml(istream& input) {
    // Реализуйте эту функцию с помощью библиотеки xml.h
    if (!input) {
        return vector<Spending>{};
    }

    Document doc = Load(input);
    const Node& root = doc.GetRoot();
    for (const auto& node : root.Children()) {
        std::cout << "In LoadFromXml function" << std::endl;
    }

    std::cout << "In LoadFromXml function" << std::endl;

    return vector<Spending>{};
}

void TestLoadFromXml() {
    istringstream xml_input(R"(<july>
    <spend amount="2500" category="food"></spend>
    <spend amount="1150" category="transport"></spend>
    <spend amount="5780" category="restaurants"></spend>
    <spend amount="7500" category="clothes"></spend>
    <spend amount="23740" category="travel"></spend>
    <spend amount="12000" category="sport"></spend>
  </july>)");

    const vector<Spending> spendings = LoadFromXml(xml_input);

    const vector<Spending> expected = {
        {"food", 2500},
        {"transport", 1150},
        {"restaurants", 5780},
        {"clothes", 7500},
        {"travel", 23740},
        {"sport", 12000}};
    ASSERT_EQUAL(spendings, expected);
}

void TestXmlLibrary() {
    // Тест демонстрирует, как пользоваться библиотекой из файла xml.h

    istringstream xml_input(R"(<july>
    <spend amount="2500" category="food"></spend>
    <spend amount="23740" category="travel"></spend>
    <spend amount="12000" category="sport"></spend>
    </july>)");

    Document doc = Load(xml_input);
    const Node& root = doc.GetRoot();
    ASSERT_EQUAL(root.Name(), "july");
    ASSERT_EQUAL(root.Children().size(), 3u);

    const Node& food = root.Children().front();
    ASSERT_EQUAL(food.AttributeValue<string>("category"), "food");
    ASSERT_EQUAL(food.AttributeValue<int>("amount"), 2500);

    const Node& sport = root.Children().back();
    ASSERT_EQUAL(sport.AttributeValue<string>("category"), "sport");
    ASSERT_EQUAL(sport.AttributeValue<int>("amount"), 12000);

    Node july("july", {});
    Node transport("spend", {{"category", "transport"}, {"amount", "1150"}});
    july.AddChild(transport);
    ASSERT_EQUAL(july.Children().size(), 1u);
}

int main() {
    std::string file_name = "input.txt";
    std::ifstream xml_input(file_name);
    vector<Spending> spendings = LoadFromXml(xml_input);

    std::cout << "Finished" << std::endl;

    // std::string file_name = "input.txt";
    // std::string file_name = "input_extended.txt";
    // std::ifstream xml_input(file_name);
    // if (!xml_input) {
    //     std::cerr << "file \"" << file_name << "\" is not opened" << std::endl;
    //     return -1;
    // }

    // Document doc = Load(xml_input);
    // const Node& root = doc.GetRoot();
    // std::cout << "Finished" << std::endl;

    // std::vector<Node> nodes;
    // while (xml_input.peek() != '\n' && xml_input.peek() != EOF) {
    //     nodes.push_back(Load(xml_input).GetRoot());
    // }

    // std::cout << "Finished" << std::endl;

    // TestRunner tr;
    // RUN_TEST(tr, TestXmlLibrary);
    // RUN_TEST(tr, TestLoadFromXml);
}
