/*
Дана последовательность целых чисел a1...an и натуральное число k,
такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
Требуется отсортировать последовательность. Последовательность может быть очень длинной.
Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.
*/
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

void print_vec(std::vector<int> &vec, int limit=-1) {
    for (int i = 0; i < vec.size() && (limit == -1 || i < limit); ++i) {
        std::cout << vec[i] << " ";
    }
}

void merge(std::vector<int> &vec1, std::vector<int> &vec2, std::vector<int> &result) {
    int i1 = 0;
    int i2 = 0;
    while (i1 < vec1.size() && i2 < vec2.size()) {
        if (vec1[i1] <= vec2[i2]) {
            result.push_back(vec1[i1]);
            ++i1;
        } else {
            result.push_back(vec2[i2]);
            ++i2;
        }
    }
    while (i1 < vec1.size()) {
        result.push_back(vec1[i1]);
        ++i1;
    }
    while (i2 < vec2.size()) {
        result.push_back(vec2[i2]);
        ++i2;
    }
}

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    std::vector<int> buffer1;
    std::vector<int> buffer2;
    std::vector<int> mergeBuffer;
    buffer1.reserve(k);
    buffer2.reserve(k);
    mergeBuffer.reserve(k);

    int value = 0;
    int i = 0;
    // читаем первые k элементов
    for (; i < k && i < n; ++i) {
        std::cin >> value;
        buffer1.push_back(value);
    }
    std::sort(buffer1.begin(), buffer1.end());

    while (i < n) {
        // читаем следующие k элементов
        for (int j= 0; j < k && i < n; ++i, ++j) {
            std::cin >> value;
            buffer2.push_back(value);
        }
        std::sort(buffer2.begin(), buffer2.end());

        // сливаем отсортированные группы по k
        merge(buffer1, buffer2, mergeBuffer);
        buffer1.clear();
        buffer2.clear();
        // выводим первые k
        print_vec(mergeBuffer, k);
        // сохраняем оставшиеся k до следующего merge
        for (int j = k; j < mergeBuffer.size(); ++j) {
            buffer1.push_back(mergeBuffer[j]);
        }
        mergeBuffer.clear();
    }
    // выводим что осталось в буфере
    print_vec(buffer1);

    return 0;
}
