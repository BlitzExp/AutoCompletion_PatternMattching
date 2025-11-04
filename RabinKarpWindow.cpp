#include <bits/stdc++.h>
#include <fstream>
#include <chrono>
using namespace std;

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

#include <bits/stdc++.h>
using namespace std;

string rabinKarpLCS(const string& S1, const string& S2) {
    if (S1.empty() || S2.empty()) return "";

    const string &A = (S1.size() <= S2.size() ? S1 : S2);
    const string &B = (S1.size() <= S2.size() ? S2 : S1);
    const int n = (int)A.size(), m = (int)B.size();

    const uint64_t MOD1 = 1000000007ULL;
    const uint64_t MOD2 = 1000000009ULL;
    const uint64_t BASE = 256ULL;

    auto hasCommonLen = [&](int L, int& outA, int& outB) -> bool {
        if (L == 0) { outA = outB = 0; return true; }
        if (L > n || L > m) return false;

        uint64_t p1 = 1, p2 = 1;
        for (int i = 1; i < L; ++i) { p1 = (p1 * BASE) % MOD1; p2 = (p2 * BASE) % MOD2; }

        auto pack = [](uint64_t h1, uint64_t h2) -> uint64_t { return (h1 << 32) | h2; };

        unordered_map<uint64_t, vector<int>> idx;
        idx.reserve((size_t)max(0, m - L + 1));
        idx.max_load_factor(0.7f);

        uint64_t hb1 = 0, hb2 = 0;
        for (int i = 0; i < L; ++i) {
            unsigned char c = (unsigned char)B[i];
            hb1 = (hb1 * BASE + c) % MOD1;
            hb2 = (hb2 * BASE + c) % MOD2;
        }
        idx[pack(hb1, hb2)].push_back(0);
        for (int j = 1; j + L <= m; ++j) {
            unsigned char left = (unsigned char)B[j - 1], right = (unsigned char)B[j + L - 1];
            hb1 = ((hb1 + MOD1 - (left * p1) % MOD1) * BASE + right) % MOD1;
            hb2 = ((hb2 + MOD2 - (left * p2) % MOD2) * BASE + right) % MOD2;
            idx[pack(hb1, hb2)].push_back(j);
        }

        uint64_t ha1 = 0, ha2 = 0;
        for (int i = 0; i < L; ++i) {
            unsigned char c = (unsigned char)A[i];
            ha1 = (ha1 * BASE + c) % MOD1;
            ha2 = (ha2 * BASE + c) % MOD2;
        }
        auto hit0 = idx.find(pack(ha1, ha2));
        if (hit0 != idx.end()) {
            for (int posB : hit0->second)
                if (A.compare(0, L, B, posB, L) == 0) { outA = 0; outB = posB; return true; }
        }
        for (int i = 1; i + L <= n; ++i) {
            unsigned char left = (unsigned char)A[i - 1], right = (unsigned char)A[i + L - 1];
            ha1 = ((ha1 + MOD1 - (left * p1) % MOD1) * BASE + right) % MOD1;
            ha2 = ((ha2 + MOD2 - (left * p2) % MOD2) * BASE + right) % MOD2;
            auto it = idx.find(pack(ha1, ha2));
            if (it != idx.end()) {
                for (int posB : it->second)
                    if (A.compare(i, L, B, posB, L) == 0) { outA = i; outB = posB; return true; }
            }
        }
        return false;
    };

    int lo = 0, hi = min(n, m), bestLen = 0, bestA = 0, bestB = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2, iA = 0, iB = 0;
        if (hasCommonLen(mid, iA, iB)) { bestLen = mid; bestA = iA; bestB = iB; lo = mid + 1; }
        else hi = mid - 1;
    }

    if (bestLen == 0) return "";
    int startInS1 = (S1.size() <= S2.size() ? bestA : bestB);
    return S1.substr(startInS1, bestLen);
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Reading books...\n";
    string book1 = cleanText("book1.txt");
    string book2 = cleanText("book2.txt");

    if (book1.empty() || book2.empty()) {
        cerr << "One of the cleaned texts is empty. Abort.\n";
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();
    string best = rabinKarpLCS(book1, book2);
    auto end = chrono::high_resolution_clock::now();
    double secs = chrono::duration<double>(end - start).count();

    cout << "\n==== Final Result ====\n";
    cout << "Longest common substring (contiguous):\n";
    cout << best << "\n\n";
    cout << "Length: " << best.size() << "\n";
    size_t denom = min(book1.size(), book2.size());
    double pct = denom ? (100.0 * best.size() / 5000) : 0.0;
    cout << "Similarity (vs shorter text): " << fixed << setprecision(4) << pct << "%\n";
    cout << "Runtime: " << secs << " s\n";
    return 0;
}