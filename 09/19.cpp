/*
Задача 19. Поиск точек в прямоугольнике (5 баллов)
На вход подаются точки и прямоугольники. Точка задается двумя координатами (x, y).
Прямоугольник - четверкой чисел [left, bottom, right, top]. Точка (left, bottom) -
принадлежит прямоугольнику, точка (right, top) - нет. (left < right, bottom < top)
Для каждого прямоугольника нужно вывести, сколько добавленных точек он содержит.
X  в диапазоне [-180, 180)
Y -  [-90, 90)
Количество точек <= 100000, Количество прямоугольников <= 1000
Для решения задачи необходимо реализовать алгоритм “Geohashing”.
 */
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

const char* BASE32 = "0123456789bcdefghjkmnpqrstuvwxyz";
const double LAT_MIN = -90;
const double LAT_MAX = 90;
const double LNG_MIN = -180;
const double LNG_MAX = 180;
const double LNG_RANGE = LNG_MAX - LNG_MIN;
const double LAT_RANGE = LAT_MAX - LAT_MIN;

struct Point {
    double lng;
    double lat;
};

struct Rect {
    Point lb;
    Point rt;
    bool contains(Point& p) {
        return lb.lng <= p.lng && lb.lat <= p.lat && p.lng < rt.lng && p.lat < rt.lat;
    }
};

struct Node {
    std::unordered_map<char, std::shared_ptr<Node>> go;
    std::vector<Point> points;
};

class GeohashTrie {
public:
    explicit GeohashTrie(std::vector<Point>& points, double lngRes, double latRes);
    int countPoints(Rect& rect);
private:
    void initLevelRes(double lngRes, double latRes);
    void add(std::string& geohash, Point& point);
    std::string getGeoHash(Point& point, int len);
    int countPoints(std::string& geohash, Rect& rect);
    int countPoints(std::shared_ptr<Node>& node, Rect& rect);

    std::shared_ptr<Node> root;
    std::vector<double> lngHashLevelRes;
    std::vector<double> latHashLevelRes;
};

void GeohashTrie::initLevelRes(double lngRes, double latRes) {
    double lngRange = LNG_RANGE;
    double latRange = LAT_RANGE;
    int bits = 0;
    while (lngRange > lngRes || latRange > latRes || bits % 5 != 0) {
        if (bits % 2 == 0) {
            lngRange /= 2;
        } else {
            latRange /= 2;
        }
        ++bits;
        if (bits % 5 == 0) {
            lngHashLevelRes.push_back(lngRange);
            latHashLevelRes.push_back(latRange);
//            std::cout << lngRange << " " << latRange << std::endl;
        }
    }
}

void GeohashTrie::add(std::string& geohash, Point& point) {
    std::shared_ptr<Node> current = root;
    for (char c : geohash) {
        auto next = current->go.find(c);
        if (next == current->go.end()) {
            current = current->go[c] = std::make_shared<Node>();
        } else {
            current = next->second;
        }
    }
    current->points.push_back(point);
}

std::string GeohashTrie::getGeoHash(Point& point, int len) {
    std::string geohash(len, ' ');
    int bits = 0;
    double lngMin = LNG_MIN;
    double lngMax = LNG_MAX;
    double latMin = LAT_MIN;
    double latMax = LAT_MAX;
    double mid = 0;
    int code = 0;
    while (bits < len * 5) {
        if (bits % 2 == 0) {
            mid = (lngMax + lngMin)/2.;
            if (point.lng >= mid) {
                code = code*2 + 1;
                lngMin = mid;
            } else {
                code = code*2;
                lngMax = mid;
            }
        } else {
            mid = (latMax + latMin)/2.;
            if (point.lat >= mid) {
                code = code*2 + 1;
                latMin = mid;
            } else {
                code = code*2;
                latMax = mid;
            }
        }
        ++bits;
        if (bits % 5 == 0) {
            geohash[bits/5-1] = BASE32[code];
            code = 0;
        }
    }
    return geohash;
}

GeohashTrie::GeohashTrie(std::vector<Point>& points, double lngRes, double latRes) {
    initLevelRes(lngRes, latRes);
    int len = lngHashLevelRes.size();
    root = std::make_shared<Node>();
    for (Point p: points) {
        std::string geohash = getGeoHash(p, len);
        add(geohash, p);
    }
}

int GeohashTrie::countPoints(std::shared_ptr<Node>& node, Rect& rect) {
    int count = 0;

    for (auto elem : node->go) {
        count += countPoints(elem.second, rect);
    }
    for (auto p : node->points) {
        if (rect.contains(p)) {
            ++count;
        }
    }
    return count;
}

int GeohashTrie::countPoints(std::string& geohash, Rect& rect) {
    std::shared_ptr<Node> current = root;
    for (char c : geohash) {
        auto next = current->go.find(c);
        if (next == current->go.end()) {
            return 0;
        }
        current = next->second;
    }
    return countPoints(current, rect);
}

int GeohashTrie::countPoints(Rect& rect) {
    int count = 0;
    std::unordered_set<std::string> hashes;
    double lngRes = rect.rt.lng - rect.lb.lng;
    double latRes = rect.rt.lat - rect.lb.lat;
    int level = 0;
    for (; level < lngHashLevelRes.size(); ++level) {
        if (lngHashLevelRes[level] < lngRes || latHashLevelRes[level] < latRes) {
            break;
        }
    }
    Point lt{rect.lb.lng, rect.rt.lat};
    Point rb{rect.rt.lng, rect.lb.lat};
    hashes.insert(getGeoHash(rect.lb, level));
    hashes.insert(getGeoHash(rect.rt, level));
    hashes.insert(getGeoHash(lt, level));
    hashes.insert(getGeoHash(rb, level));
    for (auto h : hashes) {
        count += countPoints(h, rect);
    }
    return count;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> points;
    points.reserve(n);
    double x = 0;
    double y = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> x >> y;
        points.push_back({x, y});
    }
    std::vector<Rect> rectangles;
    std::cin >> n;
    rectangles.reserve(n);
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double minLngRes = LNG_RANGE;
    double minLatRes = LAT_RANGE;
    for (int i = 0; i < n; ++i) {
        std::cin >> x1 >> y1 >> x2 >> y2;
        if (x2 - x1 < minLngRes) {
            minLngRes = x2 - x1;
        }
        if (y2 - y1 < minLatRes) {
            minLatRes = y2 - y1;
        }
        rectangles.push_back({{x1, y1}, {x2, y2}});
    }

    GeohashTrie ght(points, minLngRes, minLatRes);

    for (Rect r : rectangles) {
        std::cout << ght.countPoints(r) << std::endl;
    }
}
