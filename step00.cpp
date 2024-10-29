
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cctype> 

using namespace std;

// 交换函数模板
template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// 随机打乱函数
void shuffle(vector<string>& lines) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (size_t i = lines.size() - 1; i > 0; --i) {
        size_t j = rand() % (i + 1);
        swap(lines[i], lines[j]);
    }
}

// 检查字符串是否符合要求
bool isValidPassword(const string& password) {
    if (password.length() >= 30) return false;  // 长度筛选
    for (char c : password) {
        if (c < 32 || c > 126) {  // 只允许可打印字符
            return false;
        }
    }
    return true;
}

int main() {
    ifstream inputFile("myspace.txt");
    if (!inputFile) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    vector<string> lines;
    string line;
    while (getline(inputFile, line)) {
        if (!line.empty() && isValidPassword(line)) {  // 过滤空行和不符合条件的口令
            lines.push_back(line);
        }
    }
    inputFile.close();

    shuffle(lines);

    size_t totalLines = lines.size();
    size_t part1Lines = static_cast<size_t>(totalLines * 0.5);  // 训练集比例50%

    ofstream outputFile1("train.txt");
    if (!outputFile1) {
        cerr << "Error opening output1 file." << endl;
        return 1;
    }
    for (size_t i = 0; i < part1Lines; ++i) {
        outputFile1 << lines[i] << endl;
    }
    outputFile1.close();

    ofstream outputFile2("test.txt");
    if (!outputFile2) {
        cerr << "Error opening output2 file." << endl;
        return 1;
    }
    for (size_t i = part1Lines; i < totalLines; ++i) {
        outputFile2 << lines[i] << endl;
    }
    outputFile2.close();

    return 0;
}

