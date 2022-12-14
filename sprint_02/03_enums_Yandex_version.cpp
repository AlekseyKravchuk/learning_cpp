// search_server_s1_t1_v3.cpp

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
   public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            _word_docID_freqs[word][document_id] += inv_word_count;
        }
        _documents.emplace(document_id,
                           DocumentData{
                               ComputeAverageRating(ratings),
                               status});
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, status);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> _word_docID_freqs;
    map<int, DocumentData> _documents;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool isStopWord;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {
            text,
            is_minus,
            IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.isStopWord) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    // Existence required
    double ComputeInvertedDocumentFreq(const string& word) const {
        return log(_documents.size() * 1.0 / _word_docID_freqs.at(word).size());
    }

    vector<Document> FindAllDocuments(const Query& query, DocumentStatus status) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeInvertedDocumentFreq(word);
            for (const auto [document_id, term_freq] : _word_docID_freqs.at(word)) {
                if (_documents.at(document_id).status == status) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : _word_docID_freqs.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({document_id,
                                         relevance,
                                         _documents.at(document_id).rating});
        }
        return matched_documents;
    }
};

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating
         << " }"s << endl;
}

int main() {
    SearchServer search_server;
    search_server.SetStopWords("?? ?? ????"s);

    search_server.AddDocument(0, "?????????? ?????? ?? ???????????? ??????????????"s, DocumentStatus::ACTUAL, {8, -3});
    search_server.AddDocument(1, "???????????????? ?????? ???????????????? ??????????"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "?????????????????? ?????? ?????????????????????????? ??????????"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    search_server.AddDocument(3, "?????????????????? ?????????????? ??????????????"s, DocumentStatus::BANNED, {9});

    cout << "ACTUAL:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("???????????????? ?????????????????? ??????"s, DocumentStatus::ACTUAL)) {
        PrintDocument(document);
    }

    cout << "BANNED:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("???????????????? ?????????????????? ??????"s, DocumentStatus::BANNED)) {
        PrintDocument(document);
    }
}

// ==================== ?????? ???????????????????????? =========================

// ?????????? ???????? ?? ?????????????? ????????
SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        const string document = ReadLine();

        int status_raw;
        cin >> status_raw;

        int ratings_size;
        cin >> ratings_size;

        vector<int> ratings(ratings_size, 0);
        for (int& rating : ratings) {
            cin >> rating;
        }

        search_server.AddDocument(document_id, document, static_cast<DocumentStatus>(status_raw), ratings);
        ReadLine();
    }

    return search_server;
}

int test_main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    const DocumentStatus status = static_cast<DocumentStatus>(ReadLineWithNumber());
    for (const Document& document : search_server.FindTopDocuments(query, status)) {
        PrintDocument(document);
    }

    return 0;
}