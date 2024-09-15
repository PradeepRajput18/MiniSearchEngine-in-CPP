#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <windows.h>

// these are for MaC OS
// #include <CoreFoundation/CFBundle.h>
// #include <ApplicationServices/ApplicationServices.h>

using namespace std;

class SearchEngine {
private:
    map<int, string> problems;
    map<int, string> solutions;
    vector<vector<int>> filter_search;

    void loadProblems(const string& filename) {
        ifstream file(filename);
        string line;
        int index = 1;
        while (getline(file, line)) {
            problems[index++] = line;
        }
    }

    void loadSolutions(const string& filename) {
        ifstream file(filename);
        string line;
        int index = 1;
        while (getline(file, line)) {
            solutions[index++] = line;
        }
    }

    void searchWord(const string& word, size_t key) {
        regex e("\\b" + word + "\\b", regex_constants::icase);
        for (const auto& problem : problems) {
            if (regex_search(problem.second, e)) {
                filter_search[key].push_back(problem.first);
            }
        }
    }

    void openURL(const string& url) {
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }

public:
    SearchEngine() : filter_search(10) {
        loadProblems("Generic_Problem.txt");
        loadSolutions("Generic_Solution.txt");
    }

    void search(const string& query) {
        vector<string> words;
        stringstream ss(query);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }

        for (size_t i = 0; i < words.size(); ++i) {
            searchWord(words[i], i);
        }

        vector<int> allMatches;
        for (const auto& matches : filter_search) {
            allMatches.insert(allMatches.end(), matches.begin(), matches.end());
        }

        if (allMatches.empty()) {
            cout << "No matches found." << endl;
            return;
        }

        sort(allMatches.begin(), allMatches.end());
        auto it = max_element(allMatches.begin(), allMatches.end(), 
            [&](int a, int b) { return count(allMatches.begin(), allMatches.end(), a) < count(allMatches.begin(), allMatches.end(), b); });

        int bestMatch = *it;
        cout << "Best match: " << problems[bestMatch] << endl;
        openURL(solutions[bestMatch]);
    }
};

int main() {
    SearchEngine engine;
    string query;
    cout << "Enter your search query: ";
    getline(cin, query);
    engine.search(query);
    return 0;
}