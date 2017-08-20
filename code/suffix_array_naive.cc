#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>


using std::cout;
using std::cin;
using std::endl;
using std::map;
using std::vector;
using std::string;
using std::sort;


int main() {
    map<string, int> suffixArrayIndices;
    vector<string> suffixArray;
    string inputString;

    // input string
    cin >> inputString;

    // append all suffixes
    inputString += "$";
    for (int i = 0; i < inputString.size(); i++) {
        string suffix = inputString.substr(
                i, inputString.size() - i);

        suffixArrayIndices[suffix] = i;
        suffixArray.push_back(suffix);
    }

    // sort
    sort(suffixArray.begin(), suffixArray.end());

    // out
    for (auto suffix : suffixArray) {
        cout << suffixArrayIndices[suffix] << ": " << suffix << endl;
    }

    return 0;
}
