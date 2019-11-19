/*
Во всех задачах из следующего списка следует написать структуру данных,
обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение.Если команда pop вызвана
для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали.
Реализовать дек с динамическим зацикленным буфером.
*/
#include <assert.h>
#include <iostream>

const int MIN_BUFFER_SIZE = 10;

template <typename T>
class Deque {
 public:
    explicit Deque(int = MIN_BUFFER_SIZE);
    ~Deque();

    void PushFront(const T&);
    T PopFront();
    void PushBack(const T&);
    T PopBack();
    bool IsEmpty();

 private:
    void ResizeIfNeeded();
    T * buffer;
    int bufferSize;
    int size;
    int head;
    int tail;
};

template <typename T>
Deque<T>::Deque(int n)
:   buffer {new T[n]},
    bufferSize {n},
    size {0},
    head {0},
    tail {0}
{
}

template <typename T>
Deque<T>::~Deque() {
    delete [] buffer;
}

template <typename T>
bool Deque<T>::IsEmpty() {
    return size == 0;
}

template <typename T>
void Deque<T>::PushFront(const T& value) {
    if (IsEmpty()) {
        buffer[head] = value;
    } else {
        ResizeIfNeeded();
        head = (head - 1 + bufferSize) % bufferSize;
        buffer[head] = value;
    }
    ++size;
}

template <typename T>
T Deque<T>::PopFront() {
    assert(!IsEmpty());
    T result = buffer[head];
    --size;
    if (!IsEmpty()) {
        head = (head + 1) % bufferSize;
        ResizeIfNeeded();
    }
    return result;
}

template <typename T>
void Deque<T>::PushBack(const T& value) {
    if (IsEmpty()) {
        buffer[tail] = value;
    } else {
        ResizeIfNeeded();
        tail = (tail + 1) % bufferSize;
        buffer[tail] = value;
    }
    ++size;
}

template <typename T>
T Deque<T>::PopBack() {
    assert(!IsEmpty());
    T result = buffer[tail];
    --size;
    if (!IsEmpty()) {
        tail = (tail - 1 + bufferSize) % bufferSize;
        ResizeIfNeeded();
    }
    return result;
}

template <typename T>
void Deque<T>::ResizeIfNeeded() {
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
        if (head <= tail) {
            std::copy(buffer + head, buffer + tail + 1, newBuffer);
        } else {
            std::copy(buffer + head, buffer + bufferSize, newBuffer);
            std::copy(buffer, buffer + tail + 1, newBuffer + (bufferSize - head));
        }
        head = 0;
        tail = size - 1;
        delete [] buffer;
        buffer = newBuffer;
        bufferSize = newBufferSize;
    }
}

int main() {
    int commands_count = 0;
    std::cin >> commands_count;

    Deque<int> deque;
    for (int i = 0; i < commands_count; ++i) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;
        if (command == 1) {
            deque.PushFront(value);
        } else if (command == 3) {
            deque.PushBack(value);
        } else if (command == 2 || command == 4) {
            if (deque.IsEmpty()) {
                if (value != -1) {
                    std::cout << "NO";
                    return 0;
                }
            } else {
                int result = -1;
                if (command == 2) {
                    result = deque.PopFront();
                } else {
                    result = deque.PopBack();
                }
                if (result != value) {
                    std::cout << "NO";
                    return 0;
                }
            }
        }
    }
    std::cout << "YES";
    return 0;
}
