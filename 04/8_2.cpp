/*
 Реализуйте структуру данных типа “множество строк” на основе
 динамической хеш-таблицы с открытой адресацией.
 Хранимые строки непустые и состоят из строчных латинских букв.
 Хеш-функция строки должна быть реализована с помощью вычисления
 значения многочлена методом Горнера. Начальный размер таблицы
 должен быть равным 8-ми. Перехеширование выполняйте при добавлении
 элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
 Структура данных должна поддерживать операции добавления
 строки в множество, удаления строки из множества и проверки
 принадлежности данной строки множеству.
 1_2. Для разрешения коллизий используйте двойное хеширование.
 */
#include <assert.h>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using Hash = std::function<size_t(const std::string &)>;

class HashTable {
public:
    explicit HashTable(size_t initial_size, Hash &h1, Hash &h2);
    ~HashTable();

    bool has(const std::string &key) const;
    bool add(const std::string &key);
    bool remove(const std::string &key);

private:
    size_t hash(size_t h1, size_t h2, size_t i, size_t m) const;
    void resize(size_t newSize);
    struct HashTableNode {
        std::string key;
        size_t hash1;
        size_t hash2;

        HashTableNode(std::string key, size_t h1, size_t h2)
        :   key(std::move(key)),
            hash1{h1},
            hash2{h2}
        {}
    };
    HashTableNode* empty = new HashTableNode{"", 0, 0};
    std::vector<HashTableNode*> table;
    size_t size;
    Hash hash1;
    Hash hash2;

};

HashTable::HashTable(size_t initial_size, Hash &h1, Hash &h2)
:   table{initial_size, nullptr},
    size{0},
    hash1{h1},
    hash2{h2}
{}

HashTable::~HashTable() {
    for (HashTableNode *node : table) {
        if (node != nullptr && node != empty) {
            delete node;
        }
    }
    delete empty;
}

size_t HashTable::hash(size_t h1, size_t h2, size_t i, size_t m) const {
    return (h1 + i*(2*h2 +1)) % m;
}

bool HashTable::has(const std::string &key) const {
    assert(!key.empty());

    auto h1 = hash1(key);
    auto h = h1 % table.size();
    if (table[h] == nullptr) {
        return false;
    }
    auto h2 = hash2(key);
    for (size_t i = 0; i < table.size(); ++i) {
        h = hash(h1, h2, i, table.size());
        if (table[h] == nullptr) {
            break;
        } else if (table[h] != empty && table[h]->key == key) {
            return true;
        }
    }
    return false;
}

bool HashTable::add(const std::string &key) {
    assert(!key.empty());

    auto h1 = hash1(key);
    auto h2 = hash2(key);

    bool spotFound = false;
    size_t idx = 0;

    for (size_t i = 0; i < table.size(); ++i) {
        auto h = hash(h1, h2, i, table.size());
        if (table[h] == nullptr) {
            if (!spotFound) {
                spotFound = true;
                idx = h;
            }
            break;
        } else if (table[h] == empty) {
            spotFound = true;
            idx = h;
        } else if (table[h]->key == key) {
            return false;
        }
    }
    if (spotFound) {
        table[idx] = new HashTableNode(key, h1, h2);
        ++size;
        if (1.*size/table.size() > 3./4) {
            resize(2*table.size());
        }
    } else {
        resize(2*table.size());
        add(key);
    }
    return true;
}

bool HashTable::remove(const std::string &key) {
    assert(!key.empty());

    auto h1 = hash1(key);
    auto h2 = hash2(key);

    for (size_t i = 0; i < table.size(); ++i) {
        auto h = hash(h1, h2, i, table.size());
        if (table[h] == nullptr) {
            return false;
        } else if (table[h] != empty && table[h]->key == key) {
            --size;
            table[h] = empty;
            return true;
        }
    }

    if (table.size() > 8 && 1.*size/table.size() < 1./4) {
        resize(table.size()/2);
    }

    return false;
}

void HashTable::resize(size_t newSize) {
    std::vector<HashTableNode*> newTable{newSize, nullptr};
    for (auto node : table) {
        if (node != nullptr && node != empty) {
            for (size_t i = 0; i < table.size(); ++i) {
                auto h = hash(node->hash1, node->hash2, i, newTable.size());
                if (newTable[h] == nullptr) {
                    newTable[h] = node;
                    break;
                }
            }
        }
    }
    table = newTable;
}

int main() {

    Hash hash1 = [](const std::string &key) {
        auto m = std::numeric_limits<size_t>::max()/2;
        size_t hash = 0;
        for (std::string::size_type i = 0; i < key.size(); i++) {
            hash = (hash*31 + key[i]) % m;
        }
        return hash;
    };

    Hash hash2 = [](const std::string &key) {
        auto m = std::numeric_limits<size_t>::max()/2;
        size_t hash = 0;
        for (std::string::size_type i = 0; i < key.size(); i++) {
            hash = (hash + key[i]) % m;
        }
        return hash;
    };

    HashTable table(8, hash1, hash2);

    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        switch (command) {
        case '?':
            std::cout << (table.has(value) ? "OK" : "FAIL") << std::endl;
            break;
        case '+':
            std::cout << (table.add(value) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << (table.remove(value) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }
    return 0;
}
