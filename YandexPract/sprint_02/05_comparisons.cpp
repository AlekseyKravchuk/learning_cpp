#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    double relevance;
    int rating;
};

void SortDocuments(vector<Document>& matched_documents) {
    sort(matched_documents.begin(), matched_documents.end(),
         [](const Document& lhs, const Document& rhs) {
             if (lhs.rating != rhs.rating) {
                 return lhs.rating > rhs.rating;
             } else {
                 return lhs.relevance > rhs.relevance;
             }
         });
    // Second way to impolement comparator in sort() function
    // sort(matched_documents.begin(), matched_documents.end(),
    //      [](const Document& lhs, const Document& rhs) {
    //          return lhs.rating > rhs.rating
    //          || (lhs.rating == rhs.rating && lhs.relevance > rhs.relevance);
    //      });
}

int main() {
    vector<Document> documents = {{100, 0.5, 4}, {101, 1.2, 4}, {102, 0.3, 5}};
    SortDocuments(documents);
    for (const Document& document : documents) {
        cout << document.id << ' ' << document.relevance << ' ' << document.rating << endl;
    }

    return 0;
}