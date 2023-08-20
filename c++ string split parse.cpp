#include "main.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


//设计C++函数 将其中的一种字符替换成另一个字符或者字符串
void replaceChar(std::string& str, char oldChar, const std::string& newStr) {
    std::string::size_type pos = 0;
    while ((pos = str.find(oldChar, pos)) != std::string::npos) {
        str.replace(pos, 1, newStr);
        pos += newStr.length();
    }
}

//设计C++函数 将其中的一种字符替换成另一个字符或者字符串
void replaceChar(std::string& str, char oldChar, char newChar) {
    for (char& c : str) {
        if (c == oldChar) {
            c = newChar;
        }
    }
}

//去掉两端的字符
static string& strip(string& s, const string& chars = " ")
{
    s.erase(0, s.find_first_not_of(chars.c_str()));
    s.erase(s.find_last_not_of(chars.c_str()) + 1);
    return s;
}

//将string分割为vector
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

static void parse(string& s, map<string,string>& items)
{
    vector<string> elements;
    s.erase(0, s.find_first_not_of(" {"));
    s.erase(s.find_last_not_of("} ") + 1);
    split(s, elements, ",");
    for (vector<string>::iterator iter=elements.begin(); iter != elements.end(); iter++) {
        vector<string> kv;
        split(*iter, kv, ":");
        if (kv.size() != 2) continue;
        items[strip(kv[0], " \"")] = strip(kv[1], " \"");
    }
}

int main(){
    string a("a,nb,sdfaf,asdfsa,");
    vector<string> token;
    string d(",");
    split(a,token,d);
    for (const auto &item: token){
        cout<<item<<endl;
    }


    string data = "  {  \"key1\"  :  \"data1\"  ,  \"key2\"  :  \"data2\"    }  ";
    map<string,string> items;
    parse(data, items);

    for (map<string,string>::iterator iter=items.begin(); iter != items.end(); iter++) {
        cout << "key=" << (*iter).first << ",value=" << (*iter).second << endl;
    }

}