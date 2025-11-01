#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

vector <int> z_function (string search)
{
    int n = search.size();
    vector <int> z(n);

    z[0] = 0;

    int l = 0;
    int r = 0;

    for (int i = 1; i < n; i++) {
        if (i< r){
            z[i] = min(r-i, z[i-l]);
        }
        while (i + z[i] < n && search[z[i]] == search[i + z[i]]){
            z[i]++;
        }
        if (i + z[i] > r){
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

int main(){
    ifstream book("in.txt");

    vector<pair<string, int>> searchWords = {{"UTTERSON", 0}, {"lawyer", 0}, {"DOOR", 0}, {"mortify", 0}, {"demeanour", 0}, {"chambers", 0}, {"way", 0}, {"folks", 0}, {"you", 0}, {"family", 0}};


    string bff;

    vector<string> bookWords;

    vector<vector<int>> wordsFindPos (searchWords.size());

    for (int i = 0; i < searchWords.size(); i++)
    {
        transform(searchWords[i].first.begin(), searchWords[i].first.end(), searchWords[i].first.begin(), ::toupper);
    }

    while (book >> bff) {
        transform(bff.begin(), bff.end(), bff.begin(), ::toupper);
        bookWords.push_back(bff);
    }

    for (int i = 0; i < searchWords.size(); i++)
    {
        auto start = chrono::high_resolution_clock::now();
        for (int j = 0; j < bookWords.size(); j++)
        {
            string word = bookWords[j];
            int patternLength = searchWords[i].first.size();
            string combinedword = searchWords[i].first + "$" + word;
            vector<int> zCombined = z_function(combinedword);
            for (int k = patternLength + 1; k < zCombined.size(); k++)
            {
                if (zCombined[k] == patternLength) 
                {
                    searchWords[i].second++;
                    wordsFindPos[i].push_back(j); 
                }
            }
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> duration = end - start;
        cout << searchWords[i].first << ": " << duration.count() << " ms \n";
    }

    for (int i = 0; i < searchWords.size(); i++)
    {
        cout << searchWords[i].first << ": " << searchWords[i].second << "\n";
        for (int pos : wordsFindPos[i])
        {
            string result;
            for (int k = 0; k < 10 && pos + k < bookWords.size(); k ++) {
                result += bookWords[pos + k] + " ";
            }
            cout << result << "\n";
        }
    }

    return 0;
}