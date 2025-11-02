#include <bits/stdc++.h>
#include <fstream>
#include <chrono>

using namespace std;

string lcs(const string& S1, const string& S2) {
    int n = S1.size(), m = S2.size();
    if (n == 0 || m == 0) return "";

    vector<vector<int>> L(n, vector<int>(m, 0));

    L[0][0] = (S1[0] == S2[0]) ? 1 : 0;

    for (int i = 1; i < n; ++i)
        L[i][0] = (S1[i] == S2[0]) ? 1 : L[i - 1][0];

    for (int j = 1; j < m; ++j)
        L[0][j] = (S1[0] == S2[j]) ? 1 : L[0][j - 1];

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            if (S1[i] == S2[j])
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = max(L[i - 1][j], L[i][j - 1]);
        }
    }

    string result;
    int i = n - 1, j = m - 1;
    while (i >= 0 && j >= 0) {
        if (S1[i] == S2[j]) {
            result.push_back(S1[i]);
            i--; j--;
        } else if (i > 0 && L[i - 1][j] >= L[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    reverse(result.begin(), result.end());
    return result;
}

string cleanText(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << endl;
        return "";
    }

    string word, text;
    while (file >> word) {
        for (char c : word)
            if (isalnum((unsigned char)c))
                text += (char)tolower((unsigned char)c);
    }
    return text;
}

int main() {
    const int BLOCK_SIZE = 5000;
    cout << "Reading books..." << endl;

    string book1 = cleanText("book1.txt");
    string book2 = cleanText("book2.txt");

    cout << "Splitting texts into blocks..." << endl;

    vector<string> blocks1, blocks2;
    for (size_t i = 0; i < book1.size(); i += BLOCK_SIZE)
        blocks1.push_back(book1.substr(i, BLOCK_SIZE));
    for (size_t j = 0; j < book2.size(); j += BLOCK_SIZE)
        blocks2.push_back(book2.substr(j, BLOCK_SIZE));

    cout << "Comparing " << blocks1.size() << " x " << blocks2.size() << " blocks..." << endl;

    string bestSubseq;
    int bestLen = 0;
    int block1len = 0;
    int block2len = 0;

    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < blocks1.size(); ++i) {
        for (size_t j = 0; j < blocks2.size(); ++j) {
            string subseq = lcs(blocks1[i], blocks2[j]);
            if ((int)subseq.size() > bestLen) {
                bestLen = subseq.size();
                bestSubseq = subseq;
                block1len = blocks1[i].size();
                block2len = blocks2[j].size();
                cout << "New best (" << bestLen << " chars) found at block "
                     << i << "," << j << endl;
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "\n==== Final Result ====\n";
    cout << "Longest Common Subsequence length: " << bestLen << endl;
    cout << "Similarity (book1): " << (100.0 * bestLen / block1len) << "%" << endl;
    cout << "Similarity (book2): " << (100.0 * bestLen / block2len) << "%" << endl;
    cout << "Similarity (both): " << (200.0 * bestLen / (block1len + block2len)) << "%" << endl;
    cout << "Subsequence: " << bestSubseq << endl;
    cout << "Runtime : " << elapsed.count() << " seconds" << endl;

    return 0;
}