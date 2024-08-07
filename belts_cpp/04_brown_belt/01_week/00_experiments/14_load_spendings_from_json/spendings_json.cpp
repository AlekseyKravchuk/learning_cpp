#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "json.h"
#include "test_runner.h"

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

vector<Spending> LoadFromJson(istream& input) {
    using namespace Json;
    Document doc = Load(input);

    vector<Spending> spendings;
    for (const Node& node : doc.GetRoot().AsArray()) {
        spendings.push_back({node.AsMap().at("category").AsString(),
                             node.AsMap().at("amount").AsInt()});
    }

    return spendings;
}

void TestLoadFromJson() {
    istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 5780, "category": "restaurants"},
    {"amount": 7500, "category": "clothes"},
    {"amount": 23740, "category": "travel"},
    {"amount": 12000, "category": "sport"}
  ])");

    const vector<Spending> spendings = LoadFromJson(json_input);

    const vector<Spending> expected = {
        {"food", 2500},
        {"transport", 1150},
        {"restaurants", 5780},
        {"clothes", 7500},
        {"travel", 23740},
        {"sport", 12000}};
    ASSERT_EQUAL(spendings, expected);
}

void TestJsonLibrary() {
    using namespace Json;

    istringstream json_input(R"([
    {"amount": 2500, "category": "food"},
    {"amount": 1150, "category": "transport"},
    {"amount": 12000, "category": "sport"}
  ])");

    Document doc = Load(json_input);
    const vector<Node>& root = doc.GetRoot().AsArray();
    ASSERT_EQUAL(root.size(), 3u);

    const map<string, Node>& food = root.front().AsMap();
    ASSERT_EQUAL(food.at("category").AsString(), "food");
    ASSERT_EQUAL(food.at("amount").AsInt(), 2500);

    const map<string, Node>& sport = root.back().AsMap();
    ASSERT_EQUAL(sport.at("category").AsString(), "sport");
    ASSERT_EQUAL(sport.at("amount").AsInt(), 12000);

    Node transport(map<string, Node>{{"category", Node("transport")}, {"amount", Node(1150)}});
    Node array_node(vector<Node>{transport});
    ASSERT_EQUAL(array_node.AsArray().size(), 1u);
}

int main() {
    string fname = "input.json";
    ifstream json_input(fname);
    
    if (!json_input) {
        cerr << "File \"" << fname << "\" is not opened. Exiting." << endl;
        return -1;
    }

    Json::Document doc = Json::Load(json_input);

    cout << "After LoadFromJson" << endl;

    // TestRunner tr;
    // RUN_TEST(tr, TestJsonLibrary);
    // RUN_TEST(tr, TestLoadFromJson);

    return 0;
}