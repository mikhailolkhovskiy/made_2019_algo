/*
Задача 18. Построение выпуклой оболочки
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 1. С помощью алгоритма Грэхема.
 */
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

const double EPS1 = 0.00001;

struct Point {
    double x;
    double y;

    bool operator<(Point& o) {
        if (y != o.y) {
            return y < o.y;
        }
        return x < o.x;
    }
};


double vecDot(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x)*(c.y - b.y) - (b.y - a.y)*(c.x - b.x);
}

double len2(const Point& a, const Point& b) {
    return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

double findPerimeter(std::vector<Point>& points) {
    size_t minIdx = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        if (points[i] < points[minIdx]) {
            minIdx = i;
        }
    }
    Point minP = points[minIdx];
    std::swap(points[0], points[minIdx]);
    std::function<bool(Point&, Point&)> cmp = [minP](Point& p1, Point& p2) {
        double v = vecDot(minP, p1, p2);
        if (v <= EPS1 && v >= -EPS1) {
            return len2(minP, p1) < len2(minP, p2);
        } else {
            return v > 0.;
        }
    };
    std::sort(++(points.begin()), points.end(), cmp);
    std::vector<Point> result;
    result.push_back(points[0]);
    result.push_back(points[1]);
    for (size_t i = 2; i < points.size(); ++i) {
        Point cur = points[i];
        while (result.size() > 1 && vecDot(result[result.size() - 2], result.back(), cur) < EPS1) {
            result.pop_back();
        }
        result.push_back(cur);
    }
    std::vector<double> lens;
    double p = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        lens.push_back(std::sqrt(len2(result[i], result[(i + 1) % result.size()])));
    }
    std::sort(lens.begin(), lens.end());
    for (double v : lens) {
        p += v;
    }
    return p;
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
    std::cout.precision(15);
    std::cout << findPerimeter(points) << std::endl;
}
