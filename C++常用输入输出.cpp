#include "bits/stdc++.h"
#include <iostream>

using namespace std;



//去掉string两端的字符
static string& strip(string& s, const string& chars = " ")
{
    s.erase(0, s.find_first_not_of(chars.c_str()));
    s.erase(s.find_last_not_of(chars.c_str()) + 1);
    return s;
}

//分割字符串
static void split(const string& s, vector<string>& tokens, const string& delimiters = " ")
{
    string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}

//字符串转动态数组
vector<int> stringToVector(string &s){
    int l=s.find('[');
    int r=s.find(']');
    s=s.substr(l+1,r-1);
    stringstream  ss(s);
    string line;
    vector<int> res;
    while (getline(ss,line,',')){
        res.push_back(stoi(line));
    }
    return res;
}



int main(){
	
	return 0;
}

////整型数组输入：
//不直接告知有多少组输入，而是由EOF来判断。
int main() {
    int a, b;
    vector<int> res;
    while (cin >> a >> b) {
        res.push_back(a+b);
    }
}

//在终端的一行中输入非固定数目的整型数字，并存到数组中，中间以空格（或者其他单字符,./）分隔。
vector<int> nums;
int num;
while (cin >> num) {
	nums.push_back(num);
	if(getchar() == '\n')
		break;
}

////字符串输入
//给定一行字符串，每个字符串用空格间隔，一个样例为一行
string str;
vector<string> strs;
while (cin >> str) {
	strs.push_back(str);
	if(getchar() == '\n'){
		break;
	}
}

//给定一行字符串，每个字符串用 ， 分割，一个样例为一行
    string input;
    getline(cin, input);
    vector<string> strs;
    string str;
    stringstream ss(input);
    while (getline(ss, str, ',')) {
        strs.push_back(str);
    }