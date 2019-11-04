/*
2_4. Скользящий максимум.
Дан массив целых чисел A[0..n), n не превосходит 10^8.
Также задан размер некоторого окна (последовательно расположенных элементов массива)
в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k) вывести
значение максимума в окне. Скорость работы O(n log n), память O(n).
Формат входных данных. Вначале вводится n - количество элементов массива.
Затем вводится n строк со значением каждого элемента. Затем вводится k  - размер окна.
Формат выходных данных. Разделенные пробелом значения максимумов для каждого положения окна.
*/
#include <assert.h>
#include <algorithm>
#include <iostream>

const int MIN_BUFFER_SIZE = 10;

template <class T, class Cmp = std::less<T>>
class Heap {
 public:
    explicit Heap();
    ~Heap();

    void Push(T);
    T Pop();
    T Top();
    bool IsEmpty();

 private:
    void ResizeIfNeeded();
    void ShiftUp(int i);
    void ShiftDown(int i);
    T * buffer;
    int bufferSize;
    int size;
    Cmp comparator;
};

template <class T, class Cmp>
Heap<T, Cmp>::Heap()
:   buffer {new T[MIN_BUFFER_SIZE]},
    bufferSize {MIN_BUFFER_SIZE},
    size {0},
    comparator {Cmp()}
{
}

template <class T, class Cmp>
Heap<T, Cmp>::~Heap() {
    delete [] buffer;
}

template <class T, class Cmp>
bool Heap<T, Cmp>::IsEmpty() {
    return size == 0;
}

template <class T, class Cmp>
T Heap<T, Cmp>::Top() {
    assert(!IsEmpty());
    return buffer[0];
}

template <class T, class Cmp>
T Heap<T, Cmp>::Pop() {
    assert(!IsEmpty());
    auto result = buffer[0];
    buffer[0] = buffer[size-1];
    --size;
    if (!IsEmpty()) {
        ShiftDown(0);
        ResizeIfNeeded();
    }
    return result;
}

template <class T, class Cmp>
void Heap<T, Cmp>::ShiftDown(int i) {
    while (i < size) {
        int left = 2*i + 1;
        int right = 2*i + 2;
        int largest = i;
        if (left < size && comparator(buffer[largest], buffer[left])) {
            largest = left;
        }
        if (right < size && comparator(buffer[largest], buffer[right])) {
            largest = right;
        }
        if (largest != i) {
            std::swap(buffer[i], buffer[largest]);
            i = largest;
        } else {
            return;
        }
    }
}

template <class T, class Cmp>
void Heap<T, Cmp>::Push(T value) {
    if (IsEmpty()) {
        buffer[0] = value;
    } else {
        ResizeIfNeeded();
        buffer[size] = value;
        ShiftUp(size);
    }
    ++size;
}

template <class T, class Cmp>
void Heap<T, Cmp>::ShiftUp(int i) {
    int parent = (i - 1) / 2;
    while (parent >= 0 && i > 0) {
        if (comparator(buffer[parent], buffer[i])) {
            std::swap(buffer[i], buffer[parent]);
            i = parent;
            parent = (i - 1) / 2;
        } else {
            return;
        }
    }
}

template <class T, class Cmp>
void Heap<T, Cmp>::ResizeIfNeeded() {
    float resizeK = 1;
    if (size < bufferSize/3) {
        resizeK = 0.5;
    } else if (size == bufferSize) {
        resizeK = 2;
    }
    if (resizeK != 1) {
        int newBufferSize = bufferSize * resizeK;
        if (newBufferSize < MIN_BUFFER_SIZE) {
            return;
        }
        T * newBuffer = new T[newBufferSize];
        std::copy(buffer, buffer + bufferSize, newBuffer);
        delete [] buffer;
        buffer = newBuffer;
        bufferSize = newBufferSize;
    }
}

template <typename T>
struct Entry {
    T value;
    int idx;
    bool operator<(Entry other) const {
        return value < other.value;
    }
};

int* find_rolling_max(int arr[], int n, int k) {
    Heap<Entry<int>> heap;
    int *result = new int[n - k + 1];
    int head = 0;
    while (head < k) {
        Entry<int> e = {arr[head], head};
        heap.Push(e);
        ++head;
    }
    result[0] = heap.Top().value;
    int tail = 1;
    while (head < n) {
        Entry<int> e = {arr[head], head};
        heap.Push(e);
        ++head;
        auto maxEntry = heap.Top();
        while (maxEntry.idx < tail) {
            maxEntry = heap.Pop();
            maxEntry = heap.Top();
        }
        result[tail] = maxEntry.value;
        ++tail;
    }
    return result;
}

int main() {
    int n = 0;
    std::cin >> n;

    int *arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    int k = 0;
    std::cin >> k;
    int *rolling_max = find_rolling_max(arr, n, k);
    for (int i = 0; i < n - k + 1; ++i) {
        std::cout << rolling_max[i] << " ";
    }
    delete [] arr;
    delete [] rolling_max;
    return 0;
}
