vector<int> data;
int t;
while(cin>>t){
    data.push_back(t);
    if(cin.get()=='\n') break;
}





// 将一行由空格分隔的数字解析为一个数字列表
vector<int> parse(string line) {
    // 初始化一个空的数字列表
    vector<int> data;
    // 初始化一个空的字符串，用于存放当前读取的数字
    string num = "";
    // 遍历输入行中的每个字符
    for (char c: line) {
        // 如果字符是空格，说明已经读完一个数字
        if (c == ' ') {
            // 如果字符串不为空，将其转换为整数，并添加到数字列表中
            if (num != "") {
                data.push_back(stoi(num));
            }
            // 清空字符串，准备读取下一个数字
            num = "";
        }
            // 否则，将字符追加到字符串中
        else {
            num += c;
        }
    }
    // 如果字符串不为空，说明还有一个数字没有处理，将其转换为整数，并添加到数字列表中
    if (num != "") {
        data.push_back(stoi(num));
    }
    // 返回数字列表
    return data;
}