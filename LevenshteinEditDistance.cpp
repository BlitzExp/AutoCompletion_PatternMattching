#include <bits/stdc++.h>
#include <fstream>
#include <chrono>

using namespace std;

int levenshtein( string& S1, const string& S2) {
    int changes = 0;
    int size1 = S1.size();
    int size2 = S2.size();

    vector<vector<int>> dp(size1 + 1, vector<int>(size2 + 1, 0));

    for (int i = 0; i < size1 +1; i++)
    {
        dp[i][0] = i;
        for (int j = 0; j < size2 +1; j++)
        {
            if (i == 0)
            {
                dp[0][j] = j;
                continue;
            }
            
            if (S1[i -1] == S2[j -1])
            {
                dp[i][j] = dp[i -1][j -1];
            }
            else
            {
                dp[i][j] = 1 + min({ dp[i -1][j], dp[i][j -1], dp[i -1][j -1] });
            }

        }
    }
    changes = dp[size1][size2];
    return changes;
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

    if (book1.size() % BLOCK_SIZE != 0) {
        blocks1.pop_back();
        blocks1.push_back(book1.substr(book1.size() - BLOCK_SIZE, BLOCK_SIZE));
    }

    if (book2.size() % BLOCK_SIZE != 0) {
        blocks2.pop_back();
        blocks2.push_back(book2.substr(book2.size() - BLOCK_SIZE, BLOCK_SIZE));
    }

    cout << "Comparing " << blocks1.size() << " x " << blocks2.size() << " blocks..." << endl;

    string bestSubseq;
    int minChanges = 99999999;
    int block1select = 0;
    int block2select = 0;

    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < blocks1.size(); ++i) {
        for (size_t j = 0; j < blocks2.size(); ++j) {
            int changes = levenshtein(blocks1[i], blocks2[j]);
            if (changes < minChanges) {
                minChanges = changes;
                block1select = i;
                block2select = j;
                cout << "New best (" << changes << " changes) found at block "
                     << i << "," << j << endl;
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "\n==== Final Result ====\n";
    cout << "Minimum Changes: " << minChanges << endl;
    cout << "Similarity: " << (100.0 * (blocks1[block1select].size() - minChanges) / blocks1[block1select].size()) << "%" << endl;
    cout << "Runtime : " << elapsed.count() << " seconds" << endl;

    return 0;
}