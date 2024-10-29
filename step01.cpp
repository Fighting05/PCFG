
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

// 密码结构类
class structure_password {
public:
    string password;
    int para_num = 0;
    vector<char> structure;
    vector<int> structure_num;

    // 判断口令结构的函数
    void judge_structure() {
        int len = password.length();
        char kind = '0';
        for (int i = 0; i < len; i++) {
            // 判断字符为 L，D，S
            if (isAlphanumeric(password[i])) {
                if (isalpha(password[i])) {
                    kind = 'L';
                }
                else if (isdigit(password[i])) {
                    kind = 'D';
                }
            }
            else {
                kind = 'S';
            }

            // 存入结构中
            if (i == 0) {
                para_num++;
                structure.push_back(kind);
                structure_num.push_back(1);
            }
            else {
                if (kind == structure.back()) {
                    structure_num.back()++;
                }
                else {
                    para_num++;
                    structure.push_back(kind);
                    structure_num.push_back(1);
                }
            }
        }
    }

    void structure_initialize(string s) {
        password = s;
        judge_structure();
    }

    structure_password() {}

    ~structure_password() {}
};

// 密码模板类
class structure_template {
public:
    string structure;
    int para_num;
    int time;

    structure_template() {
        para_num = 0;
        time = 0;
    }

    void template_initialize(structure_password p) {
        structure.clear();
        for (int i = 0; i < p.para_num; i++) {
            structure.push_back(p.structure[i]);
            if (p.structure_num[i] < 10) {
                structure.push_back(static_cast<char>('0' + p.structure_num[i]));
            }
            else {
                structure.push_back(static_cast<char>('0' + p.structure_num[i] / 10));
                structure.push_back(static_cast<char>('0' + p.structure_num[i] % 10));
            }
        }
        time = 1;
    }
};

void template_judge(vector<structure_template>& p, structure_template t) {
    for (auto& temp : p) {
        if (temp.structure == t.structure) {
            temp.time++;
            return;
        }
    }
    p.push_back(t);
}

int main() {
    int size = 0;
    vector<structure_password> total_password;
    ifstream file("train.txt");
    if (!file.is_open()) {
        cerr << "无法打开文件 train.txt" << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        size++;
        structure_password sp;
        sp.structure_initialize(line);
        total_password.push_back(sp);
    }
    file.close();

    vector<structure_template> total_pass_template;
    if (!total_password.empty()) {
        structure_template temp;
        temp.template_initialize(total_password[0]);
        total_pass_template.push_back(temp);
    }

    for (size_t i = 1; i < total_password.size(); i++) {
        structure_template temp;
        temp.template_initialize(total_password[i]);
        template_judge(total_pass_template, temp);
    }

    // 输出模板及概率
    ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        cerr << "无法打开文件进行写入。" << endl;
        return 1;
    }
    for (const auto& t : total_pass_template) {
        if (t.structure.find("00") == string::npos) { 
            double result = static_cast<double>(t.time) / size;
            outFile << t.structure << " " << fixed << setprecision(10) << result << endl;
        }
    }
    outFile.close();

    return 0;
}

