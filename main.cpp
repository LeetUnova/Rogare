#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;

void run(string&);

int main() {
    ifstream file("game.rog");

    if (!file)
        return 1;

    string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    run(contents);

    return 0;
}

int getInt(string&, int&);
void skipBlock(string&, int&);
void getString(string&, int&, int, int[]);
void printString(string&, int, int[]);

void run(string &str) {
    int memory[100] = {0};
    int index = 0;
    bool condition = false;
    bool needIgnore = false;

    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if (isspace(c))
            continue;
        switch (c) {
            case '.':
                cout<< (char)memory[index];
                break;
            case ',':
                cout<< memory[index];
                break;
            case '_':
                index = getInt(str, i);
                break;
            case 's':
                memory[index] = getInt(str, i);
                break;
            case '@':
                memory[index] = str[++i];
                break;
            case '+':
                memory[index] += getInt(str, i);
                break;
            case '-':
                memory[index] -= getInt(str, i);
                break;
            case '*':
                memory[index] *= getInt(str, i);
                break;
            case '/':
                memory[index] /= getInt(str, i);
                break;
            case '=':
                condition = memory[index] == getInt(str, i);
                break;
            case '!':
                condition = memory[index] != getInt(str, i);
                break;
            case '>':
                condition = memory[index] > getInt(str, i);
                break;
            case '<':
                condition = memory[index] < getInt(str, i);
                break;
            case 'i':
                if (!condition) {
                    i += 2;
                    skipBlock(str, i);
                }
                else i++;
                break;
            case 'u':
                if (condition) {
                    i += 2;
                    skipBlock(str, i);
                }
                else i++;
                break;
            case '#':
                needIgnore = true;
                cin >> memory[index];
                break;
            case '$':
                getInt(str, i);
                break;
            case ':': {
                char b;
                needIgnore = true;
                cin >> b;
                memory[index] = (int)b;
                break;
            }
            case '%': {
                int num = getInt(str, i);
                for (;i < str.length(); i++)
                    if (str[i] == '$')
                        if (getInt(str, i) == num)
                            break;
                break;
            }
            case '^': {
                int num = getInt(str, i);
                for (;i > 0; i--) {
                    int u = i;
                    if (str[i] == '$') {
                        if (getInt(str, i) == num)
                            break;
                        i = u;
                    }
                }
                break;
            }
            case '"':
                getString(str, i, index, memory);
                break;
            case ';':
                printString(str, index, memory);
                break;
            case '?': {
                string line;
                if (needIgnore) cin.ignore();
                needIgnore = false;
                getline(cin, line);
                memory[index] = line.length();
                for (int i = 0; i < line.length(); i++)
                    memory[index + i + 1] = line[i];
                break;
            }
        }
    }
}

int getInt(string &str, int &i) {
    string num;
    for (i++; i < str.length(); i++) {
        char c = str[i];
        if (!isdigit(c)) break;
        num += c;
    }
    i--;
    return stoi(num);
}

void getString(string &str, int &i, int index, int memory[]) {
    i += 1;
    int l = 1;
    for (int u = 1;;u++) {
        int c = str[i++];
        if (c == '"')
            break;
        memory[index + u + 1] = c;
        l++;
    }
    i--;
    memory[index] = l;
}

void printString(string &str, int index, int memory[]) {
    int l = memory[index];
    for (int i = 1; i <= l; i++)
        cout<< (char)memory[index + i];
}

void skipBlock(string &str, int &i) {
    int opennings = 1;
    for (;i < str.length(); i++) {
        switch (str[i]) {
            case '{':
                opennings++;
                break;
            case '}':
                opennings--;
                break;
        }
        if (opennings <= 0)
            return;
    }
}