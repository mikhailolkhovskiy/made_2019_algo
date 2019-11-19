/*
Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло
на позиции с индексом k (0..n-1) в отсортированном массиве. Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные
элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного,
то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте
методом прохода двумя итераторами от начала массива к концу
*/
#include <iostream>
#include <vector>
#include <queue>
#include <cstdint>

int random(int s, int e) {
    return static_cast<int>((e - s)*static_cast<double>(rand())/(RAND_MAX)) + s;
}

int selectPivot(std::vector<int64_t> &arr, int s, int e) {

    int a = random(s, e);
    int b = random(s, e);
    int c = random(s, e);
    if (arr[a] > arr[b]) {
        std::swap(a, b);
    }
    if (arr[b] > arr[c]) {
        std::swap(b, c);
    }
    if (arr[a] > arr[b]) {
        std::swap(a, b);
    }
    return b;
}

int partition(std::vector<int64_t> &arr, int s, int e) {
    int p = selectPivot(arr, s, e);
    int64_t pivot = arr[p];
    std::swap(arr[p], arr[e]);
    int i = s;
    int j = s;

    while (j < e) {
        if (arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
        ++j;
    }
    std::swap(arr[i], arr[e]);
    return i;
}

int64_t findKStat(std::vector<int64_t> &arr, int k) {
    srand(int(time(0)));
    int s = 0;
    int e = arr.size() - 1;
    while (s < e) {
        int i = partition(arr, s, e);
        if (i == k) {
            return arr[i];
        } else if (k < i) {
            e = i - 1;
        } else {
            s = i + 1;
        }
    }
    return arr[s];
}

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    std::vector<int64_t> arr;
    arr.reserve(n);
    int value = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> value;
        arr.push_back(value);
    }
    std::cout << findKStat(arr, k);
}
