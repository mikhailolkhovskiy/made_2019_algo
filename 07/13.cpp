/**
 Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
 [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку.
 Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
 */

#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <cmath>
const size_t HashParameter = 37;

struct position {
    std::vector<char> chips;
    char zero_place;
    int walked = 0;
    int distance = 0;

    bool IsFinish() const;
    std::vector<position> Siblings() const;

    bool operator==(const position& other) const;
    bool operator!=(const position& other) const { return !operator==(other); }
    bool operator>(position other) const {return (walked + distance) > (other.distance + other.walked);}
};

namespace std {
template <>
struct hash<position> {
    size_t operator()(const position& key) const {
        size_t value = 0;
        for (auto chip : key.chips) {
            value = HashParameter * value + static_cast<size_t>(chip);
        }
        return value;
    }
};
}

const position FinishPosition{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, 15};

char GetMoveSymbol(const position& from, const position& to);

bool position::IsFinish() const {
    return operator==(FinishPosition);
}

std::vector<position> position::Siblings() const {
    std::vector<position> result;
    if (zero_place < 12) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 4]);
        sibling.zero_place += 4;
        result.emplace_back(sibling);
    }
    if (zero_place >= 4) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 4]);
        sibling.zero_place -= 4;
        result.emplace_back(sibling);
    }
    if (zero_place % 4 != 0) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 1]);
        sibling.zero_place -= 1;
        result.emplace_back(sibling);
    }
    if (zero_place % 4 != 3) {
        position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 1]);
        sibling.zero_place += 1;
        result.emplace_back(sibling);
    }
    return result;
}

bool position::operator==(const position& other) const {
    for (size_t i = 0; i < chips.size(); ++i ) {
        if (chips[i] != other.chips[i]) return false;
    }
    return true;
}

char GetMoveSymbol(const position& from, const position& to) {
    char zero_diff = to.zero_place - from.zero_place;
    switch (zero_diff) {
    case 1:
        return 'L'; // Ноль вправо -> фишка влево
    case -1:
        return 'R';
    case 4:
        return 'U';
    case -4:
        return 'D';
    default:
        assert(false);
    }
    return 0;
}

int manhattanDistance(char chip, int position) {
    return abs(chip/4 - position/4) + abs(chip%4 - position%4);
}

int heuristicDistance(position& p) {
    int result = 0;
    for (size_t i = 0; i < p.chips.size(); ++i) {
        if (p.chips[i] != 0 && i+1 != p.chips[i]) {
            result += manhattanDistance(p.chips[i], i);
        }
    }
    return result;
}


bool aStar(const position& start, std::unordered_map<position, position>& parents) {
    std::priority_queue<position, std::vector<position>, std::greater<position>> positions_queue;
    positions_queue.push(start);
    while (!positions_queue.empty()) {
        position current = positions_queue.top();
        positions_queue.pop();
        for (auto sibling : current.Siblings()) {
            if (parents.count(sibling)) {
                continue;
            }
            parents[sibling] = current;
            if (sibling.IsFinish()) {
                return true;
            }
            sibling.walked = 0;
            sibling.distance = heuristicDistance(current);
            positions_queue.push(sibling);
        }
    }
    return false;
}

std::vector<char> GetPath(const position& start,
        const std::unordered_map<position, position>& parents) {
    std::vector<char> result;

    position current = FinishPosition;
    while (current != start) {
        const position parent = parents.at(current);
        result.push_back(GetMoveSymbol(parent, current));
        current = parent;
    }
    // Развернем result, так как собирали его с конца.
    std::reverse(result.begin(), result.end());
    return result;
}

std::pair<bool, std::vector<char>> SolvePuzzle15(const position& start) {
    // Запустим BFS, запоминая предков всех пройденных позиций.
    std::unordered_map<position, position> parents;
    if (!aStar(start, parents)) {
        return std::make_pair(false, std::vector<char>());
    }
    return std::make_pair(true, GetPath(start, parents));
}

bool isSolvable(position& p) {
    int s = 0;
    for (int i = 0; i < p.chips.size(); ++i) {
        char chip = p.chips[i];
        if (chip > 0) {
            for (int j = i + 1; j < p.chips.size(); ++j) {
                if (p.chips[j] > 0 && p.chips[j] < chip) {
                    s++;
                }
            }
        }
    }
    s += p.zero_place/4 + 1;
    return s % 2 == 0;
}

int main() {
    position start;
    int v = 0;
    for (int i = 0; i < 16; ++i) {
        std::cin >> v;
        start.chips.push_back(v);
        if (v == 0) {
            start.zero_place = i;
        }
    }

    if (!isSolvable(start)) {
        std::cout << -1;
        return 0;
    }

    const auto result = SolvePuzzle15(start);

    if (result.first) {
        std::cout << result.second.size() << std::endl;
        for (const auto move : result.second) {
            std::cout << move;
        }
    } else {
        std::cout << -1;
    }
    return 0;
}
