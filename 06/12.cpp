/*
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа увеличивает число компонент связности.
Дан неориентированный граф, требуется найти в нем все мосты.
*/
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>

struct Node {
    explicit Node(int v, int edgeIdx) : v{v}, edgeIdx{edgeIdx} {}
    ~Node() {
        if (next != nullptr) {
            delete next;
        }
    }
    int edgeIdx;
    int v;
    Node* next = nullptr;
};

struct PathElem {
    int v;
    int e;
};

void addEdge(std::vector<Node*>& graph, int v1, int v2, int edgeIdx) {
    Node * node = new Node(v2, edgeIdx);
    node->next = graph[v1];
    graph[v1] = node;

}

std::vector<int> findBridges(std::vector<Node*>& graph) {
    int time = 0;
    std::unordered_map<int, Node*> inPath;
    std::vector<PathElem> path;
    std::vector<bool> used(graph.size(), false);
    std::vector<int> entry(graph.size(), 0);
    std::vector<int> lowest(graph.size(), 0);
    std::vector<int> bridges;
    for (int sv = 0; sv < graph.size(); ++sv) {
        if (!used[sv]) {
            inPath.clear();
            path.push_back({sv, -1});
            inPath[sv] = graph[sv];
            while (!path.empty()) {
                PathElem curElem = path.back();
                int curVertex = curElem.v;
                if (!used[curVertex])  {
                    used[curVertex] = true;
                    entry[curVertex] = lowest[curVertex] = time++;
                }
                Node* nextNode = inPath[curVertex];
                if (nextNode != nullptr) {
                    int nextVertex = nextNode->v;
                    int nextEdge = nextNode->edgeIdx;
                    if (nextEdge != curElem.e) {
                        if (used[nextVertex]) {
                            lowest[curVertex] = std::min(lowest[curVertex], entry[nextVertex]);
                        } else {
                            inPath[nextVertex] = graph[nextVertex];
                            path.push_back({nextVertex, nextNode->edgeIdx});
                        }
                    }
                    inPath[curVertex] = nextNode->next;
                } else {
                    int edge = path.back().e;
                    path.pop_back();
                    inPath.erase(curVertex);
                    if (!path.empty()) {
                        int prevVertex = path.back().v;
                        lowest[prevVertex] = std::min(lowest[prevVertex], lowest[curVertex]);
                        if (lowest[curVertex] > entry[prevVertex]) {
                            bridges.push_back(edge);
                        }
                    }
                }
            }
        }
    }
    return bridges;
}

int main() {
    int n = 0;
    int m = 0;
    std::ifstream fin("bridges.in");

    fin >> n;
    fin >> m;
    std::vector<Node*> graph(n, nullptr);
    int v1 = 0;
    int v2 = 0;
    int edgeIdx = 1;
    for (int i = 0; i < m; ++i) {
        fin >> v1 >> v2;
        v1 = v1 - 1;
        v2 = v2 - 1;
        addEdge(graph, v1, v2, edgeIdx);
        addEdge(graph, v2, v1, edgeIdx);
        ++edgeIdx;
    }
    std::ofstream fout("bridges.out");
    auto bridges = findBridges(graph);
    std::sort(bridges.begin(), bridges.end(), std::less<int>());
    fout << bridges.size() << std::endl;
    for (auto b : bridges) {
        fout << b << std::endl;
    }
    return 0;
}
