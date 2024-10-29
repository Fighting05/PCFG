#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <queue>

using namespace std;

class pass_str_01 {
public:
    string structure;
    double probability;
    vector<string> components;
    pass_str_01(string s, double p) : structure(s), probability(p) {
        string temp;
        for (char c : s) {
            if (isalpha(c)) {
                if (!temp.empty()) {
                    components.push_back(temp);
                    temp.clear();
                }
                temp += c;
            }
            else {
                temp += c;
            }
        }
        if (!temp.empty()) {
            components.push_back(temp);
        }
    }
};

class templepass {
public:
    string structure;
    int time;
    string password;
    double probability;

    templepass() : time(0), probability(0.0) {}
    templepass(string s, int t, string p, double d) : structure(s), time(t), password(p), probability(d) {}
};

class password_prob {
public:
    string password;
    double prob;

    password_prob(string pwd, double prob) : password(pwd), prob(prob) {}
};

// 自定义比较器
struct ComparePasswordProb {
    bool operator()(const password_prob& a, const password_prob& b) {
        return a.prob < b.prob; // 从小到大排列（优先队列，最小在前）
    }
};

// 更新密码生成函数
bool result1(int i, int level, string& s, double& d, pass_str_01 record, const unordered_map<string, vector<templepass>>& records2, unordered_map<string, pair<string, double>>& memo, priority_queue<password_prob, vector<password_prob>, ComparePasswordProb>& result, double threshold, ofstream& output, int& total_passwords, const int limit) {
    string temple = record.components[level];

    if (level == i - 1) {
        if (records2.find(temple) != records2.end()) {
            for (const auto& record2 : records2.at(temple)) {
                s += record2.password;
                d *= record2.probability;

                // 使用动态阈值判断，存储满足条件的结果
                if (d > threshold) {
                    // 如果优先队列中的密码数量未达到上限，则加入
                    if (total_passwords < limit) {
                        result.push(password_prob(s, d));
                        output << s << endl; // 直接写入文件
                        total_passwords++; // 每找到一个有效密码，更新数量

                        // 每达到一万个密码，输出当前生成数量
                        if (total_passwords % 10000 == 0) {
                            cout << "当前已生成 " << total_passwords << " 个密码" << endl; // 每一万生成一次提示
                        }
                    }
                }

                // 回溯，修正状态
                s = s.substr(0, s.size() - record2.password.size());
                d /= record2.probability; // 恢复概率状态
            }
        }
        return total_passwords >= limit; // 判断是否已达到上限
    }

    bool found = false; // 用于检查是否找到记录
    if (records2.find(temple) != records2.end()) {
        for (const auto& record2 : records2.at(temple)) {
            s += record2.password; // 添加当前构建的密码
            d *= record2.probability; // 更新当前的概率

            // 回调到下一层递归
            if (result1(i, level + 1, s, d, record, records2, memo, result, threshold, output, total_passwords, limit)) {
                return true; // 达到限制
            }

            // 回溯处理
            s = s.substr(0, s.size() - record2.password.size());
            d /= record2.probability;

            found = true; // 找到记录标记
        }
    }

    // 确保缓存中记录的是最新的组合
    if (found) {
        memo[temple] = make_pair(s, d);
    }
    return false; // 未达到限制
}

int main() {
    ifstream input1("output.txt");
    ifstream input2("output01.txt");

    if (!input1.is_open()) {
        cerr << "无法打开文件 output.txt" << endl;
        return 1;
    }
    if (!input2.is_open()) {
        cerr << "无法打开文件 output01.txt" << endl;
        return 1;
    }

    string line;
    vector<pass_str_01> records;
    unordered_map<string, vector<templepass>> records2;
    unordered_map<string, pair<string, double>> memo;
    priority_queue<password_prob, vector<password_prob>, ComparePasswordProb> result;

    while (getline(input1, line)) {
        istringstream iss(line);
        string str;
        double dbl;
        if (iss >> str >> dbl) {
            records.push_back(pass_str_01(str, dbl));
        }
    }
    input1.close();

    // 按概率从大到小排序
    sort(records.begin(), records.end(), [](const pass_str_01& a, const pass_str_01& b) {
        return a.probability > b.probability;
        });

    while (getline(input2, line)) {
        istringstream iss(line);
        string str, pass;
        int tim;
        double pro;
        if (iss >> str >> tim >> pass >> pro) {
            records2[str].push_back(templepass(str, tim, pass, pro));
        }
    }
    input2.close();

    double threshold = 1e-15; // 设置阈值，根据需要修改
    int total_passwords = 0; // 初始化总密码数量
    const int limit = 20000000; // 限制密码数量

    // 创建输出文件
    ofstream output("result00.txt");
    if (!output.is_open()) {
        cerr << "无法打开文件 result.txt" << endl;
        return 1;
    }

    // 生成密码并写入文件
    for (const auto& record : records) {
        string s = "";
        double d = record.probability;
        if (result1(record.components.size(), 0, s, d, record, records2, memo, result, threshold, output, total_passwords, limit)) {
            break; // 如果达到限制则停止生成
        }
    }

    output.close(); // 关闭文件
    cout << "已生成并写入 " << total_passwords << " 个密码到 result00.txt。" << endl; // 总提示信息

    // 从优先队列中提取最高概率的3000000个密码并保存到result01.txt
    ofstream output01("result.txt");
    if (!output01.is_open()) {
        cerr << "无法打开文件 result.txt" << endl;
        return 1;
    }

    int count = 0;
    while (!result.empty() && count < 3000000) {
        const auto& item = result.top();
        output01 << item.password << endl; // 写入password
        result.pop();
        count++;
    }

    output01.close(); // 关闭result.txt
    cout << "已提取并写入 " << count << " 个最高概率密码到 result.txt。" << endl; // 总提示信息

    return 0;
}


































