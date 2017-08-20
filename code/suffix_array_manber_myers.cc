#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


using std::cout;
using std::cin;
using std::endl;
using std::sort;
using std::string;
using std::vector;


namespace {
    int gap = 1;
    int suffixArrayIndices[1000000];
    int rank[1000000];
    int tmp[1000000];
    string str;

    bool compareFunction(int i, int j) {
        if (rank[i] != rank[j]) {
            return rank[i] < rank[j];
        }

        i += gap;
        j += gap;

        return (i < str.size() && j < str.size()) ? rank[i] < rank[j] : i > j;
    }

    vector<string> suffixArray(string &inputString) {
        str = inputString + "$";

        // init
        for (int i = 0; i < str.size(); i++) {
            suffixArrayIndices[i] = i;
            rank[i] = str[i];
        }

        // fancy code :)
        for (gap = 1;; gap *= 2) {
            // sort
            sort(suffixArrayIndices, suffixArrayIndices + str.size(), compareFunction);

            // update ranks
            for (int i = 0; i < str.size() - 1; i++) {
                tmp[i + 1] = tmp[i] + compareFunction(suffixArrayIndices[i], suffixArrayIndices[i + 1]);
            }

            for (int i = 0; i < str.size(); i++) {
                rank[suffixArrayIndices[i]] = tmp[i];
            }

            if (tmp[str.size() - 1] == str.size() - 1) {
                break;
            }
        }
        
        //
        // @rank - ranks of a suffix
        // @suffixArrayIndices - start index of a suffix
        //
        vector<string> retSuffixArray;
        for (int i = 0; i < str.size(); i++) {
            int startIndex = suffixArrayIndices[i];
            retSuffixArray.push_back(str.substr(startIndex, str.size() - startIndex));
        }

        return retSuffixArray;
    }
};


int main() {
    string input;

    cin >> input;
    auto s = suffixArray(input);

    for (auto suffix : s) {
        cout << suffix << endl;
    }

    return 0;
}
