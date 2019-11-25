/*
6_4. Обход дерева в порядке level-order
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево поиска, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке level-order (по слоям, “в ширину”).
*/
#include <iostream>
#include <queue>
#include <functional>

template <class T>
struct Node {
  explicit Node(T value) : value{value} {}

  T value = 0;
  Node* left = nullptr;
  Node* right = nullptr;
};

template <class T>
using TraverseCallback = std::function<void(const T&)>;

template <class T>
using TraverseNodeCallback = std::function<void(Node<T>*)>;

template <class T>
class Tree {
 public:
  ~Tree();

  void traverseLevelOrder(TraverseCallback<T>);
  void add(T value);

 private:
  Node<T>* root = nullptr;
  void traverseNodesLevelOrder(TraverseNodeCallback<T>);
};

template <class T>
Tree<T>::~Tree() {
    TraverseNodeCallback<T> callback = [](Node<T>* node){delete node;};
    traverseNodesLevelOrder(callback);
}

template <class T>
void Tree<T>::add(T value) {
    Node<T>* node = new Node<T>(value);
    if (root == nullptr) {
        root = node;
    } else {
        Node<T>* cur = root;
        while (true) {
            if (cur->value <= value) {
                if (cur->right == nullptr) {
                    cur->right = node;
                    break;
                } else {
                    cur = cur->right;
                }
            } else {
                if (cur->left == nullptr) {
                    cur->left = node;
                    break;
                } else {
                    cur = cur->left;
                }
            }
        }
    }
}

template <class T>
void Tree<T>::traverseNodesLevelOrder(TraverseNodeCallback<T> callback) {
    std::queue<Node<T>*> queue;
    queue.push(root);
    while (!queue.empty()) {
        auto cur = queue.front();
        queue.pop();
        if (cur->left != nullptr) {
            queue.push(cur->left);
        }
        if (cur->right != nullptr) {
            queue.push(cur->right);
        }
        callback(cur);
    }
}

template <class T>
void Tree<T>::traverseLevelOrder(TraverseCallback<T> callback) {
    TraverseNodeCallback<T> nodeCallback = [&](Node<T>* node){callback(node->value);};
    traverseNodesLevelOrder(nodeCallback);
}



int main() {
    int n = 0;
    std::cin >> n;

    Tree<int> tree;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        tree.add(value);
    }
    auto callback = [](const int& n){std::cout << n << " ";};
    tree.traverseLevelOrder(callback);

    return 0;
}
