/*
Дана последовательность целых чисел a1...an и натуральное число k,
такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
Требуется отсортировать последовательность. Последовательность может быть очень длинной.
Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.
*/
#include <assert.h>
#include <iostream>
#include <vector>
#include <queue>

void print_vec(std::vector<int> &vec) {
    for (auto v: vec) {
        std::cout << v << " ";
    }
}

// Не нашел как применить слияние если достаточно эффектино решается кучей
int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    int bSize = k;
    std::vector<int> buffer;
    buffer.reserve(bSize);
    std::priority_queue<int, std::vector<int>, std::greater<int>> queue;

    int value = 0;
    int i = 0;
    // заполение кучи k элементами
    for (; i < k; ++i) {
        std::cin >> value;
        queue.push(value);
    }

    // идем по массиву доставая из кучи минимальный и добавляя элемент из потока
    for (; i < n; ++i) {
        buffer.push_back(queue.top());
        queue.pop();
        std::cin >> value;
        queue.push(value);
        if (buffer.size() == bSize) {
            print_vec(buffer);
            buffer.clear();
        }
    }
    // выводим что осталось в буфере
    print_vec(buffer);
    // выводим что осталось в куче
    while (!queue.empty()) {
        std::cout << queue.top() << " ";
        queue.pop();
    }

    return 0;
}
