#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

using namespace std;

// 函数用于比较两个文件中的密码
int comparePasswords(const string& testFile, const string& resultFile) {
    ifstream test(testFile);
    ifstream result(resultFile);

    if (!test.is_open()) {
        cerr << "无法打开 test 文件." << endl;
        return -1;
    }

    if (!result.is_open()) {
        cerr << "无法打开 result 文件." << endl;
        return -1;
    }

    unordered_set<string> resultPasswords;
    string resultLine;

    // 读取 result 文件中的每一行（每个密码），并存储到集合中
    while (getline(result, resultLine)) {
        stringstream resultStream(resultLine);
        string resultPassword;
        resultStream >> resultPassword;
        resultPasswords.insert(resultPassword);
    }

    int totalInTest = 0;
    int matched = 0;

    // 读取 test 文件中的每一行（每个密码）
    string testLine;
    while (getline(test, testLine)) {
        totalInTest++;
        stringstream testStream(testLine);
        string testPassword;
        testStream >> testPassword;

        // 检查密码是否在集合中
        if (resultPasswords.find(testPassword) != resultPasswords.end()) {
            matched++;
        }
    }

    test.close();
    result.close();

    if (totalInTest == 0) {
        cerr << "test 文件中没有密码." << endl;
        return -1;
    }

    double ratio = (double)matched / totalInTest;
    cout << "破解的密码数量: " << matched << " 总密码数量: " << totalInTest << " 破解率: " << ratio * 100 << "%" << endl;
    return 0;
}

int main() {
    string testFileName = "test.txt";
    string resultFileName = "result.txt";

    int result = comparePasswords(testFileName, resultFileName);
    return result;
}


