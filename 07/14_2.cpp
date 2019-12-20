/*
Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 2. С помощью алгоритма Крускала.
 */
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

class DSU {
public:
    explicit DSU(size_t size);

    int find(int element);
    void merge(int left, int right);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

DSU::DSU(size_t size) :
            parent(size, -1),
            rank(size, 1) {}

int DSU::find(int element) {
    if (parent[element] == -1) {
        return element;
    }
    return parent[element] = find(parent[element]);
}

void DSU::merge(int left, int right) {
    const int deputat_left = find(left);
    const int deputat_right = find(right);
    if (rank[deputat_left] == rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
        ++rank[deputat_left];
    } else if(rank[deputat_left] > rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
    } else {
        parent[deputat_left] = deputat_right;
    }
}

struct Edge {
    int v1;
    int v2;
    int w;

    bool operator<(const Edge& o) const {
        return w < o.w;
    }
};

struct Graph {

    explicit Graph(int v, int e): vN{v}, eN{e} {}

    void addEdge(int v1, int v2, int w) {
        edges.push_back(Edge{v1, v2, w});
    }

    int vN;
    int eN;
    std::vector<Edge> edges;
};

int findMstWeight(Graph& graph) {
    std::sort(graph.edges.begin(), graph.edges.end(), std::less<Edge>());
    std::vector<Edge> mst;
    DSU dsu(graph.vN);
    for (Edge e : graph.edges) {
        if (dsu.find(e.v1) != dsu.find(e.v2)) {
            mst.push_back(e);
            dsu.merge(e.v1, e.v2);
            if (mst.size() == graph.vN - 1) {
                break;
            }
        }
    }
    int w = 0;
    for (Edge e : mst) {
        w += e.w;
    }
    return w;
}

int main() {
    int n = 0;
    int m = 0;
    std::ifstream fin("kruskal.in");

    fin >> n;
    fin >> m;
    Graph graph{n, m};
    int v1 = 0;
    int v2 = 0;
    int w = 0;
    for (int i = 0; i < m; ++i) {
        fin >> v1 >> v2 >> w;
        graph.addEdge(v1-1, v2-1, w);
    }
    std::ofstream fout("bridges.out");
    fout << findMstWeight(graph);
    return 0;
}
