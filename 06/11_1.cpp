/*
Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
*/
#include <iostream>
#include <vector>
#include <unordered_map>

struct Node {
    explicit Node(int v) : v{v} {}
    ~Node() {
        if (next != nullptr) {
            delete next;
        }
    }
    int v;
    Node *next;
};

void addEdge(std::vector<Node*>& graph, int v1, int v2) {
    Node * node = new Node(v2);
    node->next = graph[v1];
    graph[v1] = node;

}

int findMinCycle(std::vector<Node*>& graph) {
    int minCycle = graph.size() + 1;
    std::unordered_map<int, Node*> inPath;
    std::vector<int> path;
    for (int sv = 0; sv < graph.size(); ++sv) {
        path.clear();
        inPath.clear();
        path.push_back(sv);
        inPath[sv] = graph[sv];
        while (!path.empty()) {
            int curVertex = path.back();
            Node* nextNode = inPath[curVertex];
            if (nextNode != nullptr) {
                int nextVertex = nextNode->v;
                if (inPath.find(nextVertex) == inPath.end()) {
                    if (path.size() < minCycle + 1) {
                        inPath[nextVertex] = graph[nextVertex];
                        path.push_back(nextVertex);
                    }
                } else {
                    if (nextVertex != curVertex && (path.size() == 1 || nextVertex != path[path.size() - 2])) {
                        int i = 2;
                        while (i < path.size()) {
                            if (nextVertex == path[path.size() - i - 1]) {
                                break;
                            }
                            ++i;
                        }
                        if (i< path.size() && i < minCycle) {
                            minCycle = i + 1;
                        }
                        if (minCycle == 3) {
                            return 3;
                        }
                    }
                }
                inPath[curVertex] = nextNode->next;
            } else {
                path.pop_back();
                inPath.erase(curVertex);
            }
        }
    }
    return (minCycle == graph.size() + 1) ? -1 : minCycle;
}

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n;
    std::cin >> m;
    std::vector<Node*> graph(n, nullptr);
    int v1 = 0;
    int v2 = 0;
    for (int i = 0; i < m; ++i) {
        std::cin >> v1 >> v2;
        addEdge(graph, v1, v2);
        addEdge(graph, v2, v1);
    }
    std::cout << findMinCycle(graph) << std::endl;
    return 0;
}
