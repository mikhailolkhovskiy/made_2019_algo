/*
16. Поиск подстроки
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
p <= 30000, n <= 300000.
Использовать один из методов:
- С помощью префикс-функции;
 */
#include <iostream>
#include <vector>

std::vector<int> find(std::string& pattern, std::string& text) {
    std::vector<int> result;
    std::vector<int> pi(pattern.size() + 1, 0);
    int pLen = pattern.size();
    int tLen = text.size();
    int prevPi = 0;
    int curPi = 0;

    if (pLen == 0 || tLen ==0) {
        return result;
    }
    char curC = 0;
    for (int i = 1; i < tLen + pLen; ++i) {
        curC = (i < pLen) ? pattern[i] : text[i - pLen];
        int j = prevPi;
        while (j > 0 && curC != pattern[j]) {
            j = pi[j - 1];
        }
        if (curC == pattern[j]) {
            ++j;
        }
        curPi = j;

        if (i < pLen) {
            pi[i] = curPi;
            if (i == pLen - 1) {
                curPi = 0;
            }
        } else if (curPi == pLen) {
            result.push_back(i - pLen - curPi + 1);
        }
        prevPi = curPi;
    }
    return result;
}

int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern;
    std::cin >> text;
    std::vector<int> result = find(pattern, text);
    for (auto p : result) {
        std::cout << p << " ";
    }
}
