#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<vector>
#include<iomanip>
#include<locale>

using namespace std;

// 判断字符是否为字母或数字
bool isAlphanumeric(char c) {
    locale loc;
    return isalnum(c, loc);
}

bool kind(char a, char b) {
    if (isAlphanumeric(a) && isAlphanumeric(b)) {
        if (isalpha(a) && isalpha(b)) {
            return true;
        }
        else if (isdigit(a) && isdigit(b)) {
            return true;
        }
    }
    else if (!(isAlphanumeric(a)) && !(isAlphanumeric(b))) {
        return true;
    }
    else {
        return false;
    }
}

// 密码细致结构类
class structure_password {
public:
    vector<string> password;
    vector<int> passnum;
    vector<pair<string, long long>> pass_num;
    char structure;
    int structure_num = 0;
    int time = 0;

    // 判断口令结构的函数
    void judge_struture() {
        int len = pass_num[0].first.length();
        char kind = '0';
        for (int i = 0; i < len; i++) {
            // 判断字符为 L，D，S
            if (isAlphanumeric(pass_num[0].first[i])) {
                if (isalpha(pass_num[0].first[i])) {
                    kind = 'L';
                }
                else if (isdigit(pass_num[0].first[i])) {
                    kind = 'D';
                }
            }
            else {
                kind = 'S';
            }
            // 存入结构中
            if (i == 0) {
                structure = kind;
                structure_num++;
            }
            else {
                structure_num++;
            }
        }
    }

    structure_password() {};

    structure_password(string s) {
        pass_num.push_back(make_pair(s, 1));
        time = 1;
        judge_struture();
    }

    ~structure_password() {}
};

void para_judge(vector<structure_password>& p, structure_password t) {
    bool found = false;
    for (auto& temp : p) {
        if (temp.structure == t.structure && temp.structure_num == t.structure_num) {
            found = true;
            temp.time++;
            bool passwordFound = false;
            for (auto& templ : temp.pass_num) {
                if (templ.first == t.pass_num[0].first) {
                    templ.second++;
                    passwordFound = true;
                    break;
                }
            }
            if (!passwordFound) {
                temp.pass_num.push_back(make_pair(t.pass_num[0].first, 1));
            }
            break;
        }
    }
    if (!found) {
        p.push_back(t);
    }
}

int main() {
    ifstream file("train.txt");
    if (!file.is_open()) {
        cerr << "无法打开文件 train.txt" << endl;
        return 1;
    }
    vector<string> password_para;
    string temple;
    while (getline(file, temple)) {
        for (int i = 0; i < temple.length();) {
            int j = i;
            while (j < temple.length() - 1 && kind(temple[j], temple[j + 1])) {
                j++;
            }
            string subtem = temple.substr(i, j - i + 1);
            cout << subtem << endl;
            i = j + 1;
            password_para.push_back(subtem);
        }
    }
    file.close();

    vector<structure_password> total_password_para;
    if (!password_para.empty()) {
        structure_password temple(password_para[0]);
        total_password_para.push_back(temple);
    }
    for (size_t i = 1; i < password_para.size(); i++) {
        structure_password temple(password_para[i]);
        para_judge(total_password_para, temple);
    }
    for (const auto& t : total_password_para) {
        for (const auto& q : t.pass_num) {
            double result = static_cast<double>(q.second) / t.time;
            cout << t.structure << t.structure_num << " " << q.first << " " << fixed << setprecision(6) << result << endl;
        }
    }
    ofstream outFile("output01.txt");
    if (outFile.is_open()) {
        for (const auto& t : total_password_para) {
            for (const auto& q : t.pass_num) {
                double result = static_cast<double>(q.second) / t.time;
                outFile << t.structure << t.structure_num << " " << t.time << " " << q.first << " " << fixed << setprecision(10) << result << endl;
            }
        }
    }
    outFile.close();
    return 0;
}
