#include <exception>
#include <iostream>
#include<bits/stdc++.h>
#include <unordered_map>

using namespace std;


void help(const string &input, int index, vector<int> &path, vector<vector<int>> &res) {
    if (path.size() == 4 && index == input.size()) {
        res.push_back(path);
        return;
    }
    if (path.size() > 4) {
        return;
    }
    if (index >= input.size()) {
        return;
    }
    for (int len = 1; len <= 3; ++len) {
        auto temp = input.substr(index, len);
        if (temp[0] == '0' && len > 1) continue;
        int val = std::atoi(temp.c_str());
        if (val >= 0 && val <= 255) {
            path.push_back(val);
            help(input, index + len, path, res);
            path.pop_back();
        }
    }
}

class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<int> path;
        vector<vector<int>> res;
        help(s, 0, path, res);
        vector<string> ans;
        for (const auto &item: res){
            string temp;
            for (int i = 0; i < 4; ++i) {
                temp += to_string(item[i]);
                if (i != 3) {
                    temp += ".";
                }
            }
            ans.push_back(temp);
        }
        return ans;
    }
};


int main() {
    string input = "25525511135";
    vector<int> path;
    vector<vector<int>> res;
    help(input, 0, path, res);
    for (const auto &item: res) {
        for (int i = 0; i < 4; ++i) {
            cout << item[i];
            if (i != 3) {
                cout << ".";
            }
        }
        cout << endl;
    }

    return 0;
}


#include <iostream>

using namespace std;

/*

// case 1
void func1() {
  char a[] = "abcde";
  a[0] = random();
  func1();
}

// case 2
const char *p = "abcde";
p[0] = 'f';
strlen(p);

// case 3
char* ptr = (char*)malloc(5);
memcpy(ptr, p, 5);
ptr[0] = 'f';
strlen(str);

// case 4
std::string s {"abcde"};
s[0] = 'f';
strlen(s.c_str());

// case 5
std::vector<char> v {'a', 'b', 'c', 'd', 'e'};
v[0] = 'f';
strlen(&v[0]);

*/

//// To execute C++, please define "int main()"
//int main() {
//    auto words = { "Hello, ", "World!", "\n" };
//    for (const char* const& word : words) {
//        cout << word;
//    }
//    return 0;
//}
