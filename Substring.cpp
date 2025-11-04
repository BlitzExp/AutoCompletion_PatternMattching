#include <bits/stdc++.h>
#include <fstream>
#include <chrono>

using namespace std;

string lcSub(const string& S1, const string& S2) {
    int n = S1.size();
    int m = S2.size();

    vector<vector<int>> lc(n, vector<int>(m, 0));
    int maxLen = 0, endIndex = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (S1[i] == S2[j]) {
                if (i == 0 || j == 0)
                    lc[i][j] = 1;
                else
                    lc[i][j] = lc[i - 1][j - 1] + 1;

                if (lc[i][j] > maxLen) {
                    maxLen = lc[i][j];
                    endIndex = i;
                }
            } else {
                lc[i][j] = 0;
            }
        }
    }

    return (maxLen > 0) ? S1.substr(endIndex - maxLen + 1, maxLen) : "";
}


vector<string> loadWords(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error abriendo " << filename << endl;
        exit(1);
    }

    string w;
    vector<string> words;
    while (file >> w) {
        string clean = "";
        for (char c : w)
            if (isalnum((unsigned char)c))
                clean += (char)tolower((unsigned char)c);
        if (!clean.empty())
            words.push_back(clean);
    }
    return words;
}


string joinWords(const vector<string>& words, int startWord, int maxChars) {
    string res;
    for (int i = startWord; i < (int)words.size(); ++i) {
        string add = (res.empty() ? words[i] : "" + words[i]);
        if ((int)res.size() + (int)add.size() > maxChars) break;
        res += add;
    }
    return res;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> w1 = loadWords("book1.txt");
    vector<string> w2 = loadWords("book2.txt");

    const int WINDOW = 5000;
    

    int block1len = 0;
    int block2len = 0;
    string bestSub = "";

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < (int)w1.size(); i += WINDOW) {
        string part1 = joinWords(w1, i, i + WINDOW);
        for (int j = 0; j < (int)w2.size(); j += WINDOW) {
            string part2 = joinWords(w2, j, j + WINDOW);

            string sub = lcSub(part1, part2);
            if (sub.size() > bestSub.size()) {
            
                block1len = part1.size();
                block2len = part2.size();
                bestSub = sub;
                cout << "Nueva coincidencia más larga (" << sub.size() << "): " << sub << '\n';
            }
        }
        if (i > 6000) break;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;


    cout << "\n============================\n";
    cout << "Longest common substring overall:\n";
    cout << bestSub << "\n";
    cout << "Length: " << bestSub.size() << endl;
    cout << "Similarity (book1): " << (100.0 * bestSub.size() / block1len) << "%" << endl;
    cout << "Similarity (book2): " << (100.0 * bestSub.size() / block2len) << "%" << endl;
    cout << "Similarity (both): " << (200.0 * bestSub.size() / (block1len + block2len)) << "%" << endl;
    cout << "Runtime : " << elapsed.count() << " seconds" << endl;
}