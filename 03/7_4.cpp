/*
7_4. Порядковые статистики Сплей
Множество натуральных чисел постоянно меняется.
Элементы в нем добавляются и удаляются по одному.
Вычислите указанные порядковые статистики после каждого добавления или удаления.
Требуемая скорость выполнения запроса - O(log n) амортизированно.
В реализации используйте сплей-деревья.
*/
#include <iostream>
#include <queue>
#include <functional>
#include <exception>

template <class T>
struct Node {
    explicit Node(T value) : value{value} {}
    T value = 0;
    int size = 1;
    Node<T>* parent = nullptr;
    Node<T>* left = nullptr;
    Node<T>* right = nullptr;

    void updateSize() {
        size = 1;
        if (left != nullptr) {
            size += left->size;
        }
        if (right != nullptr) {
            size += right->size;
        }
    }

    int getLeftSize() {
        if (left == nullptr) {
            return 0;
        } else {
            return left->size;
        }
    }

    int getRightSize() {
        if (right == nullptr) {
            return 0;
        } else {
            return right->size;
        }
    }

    bool hasParent() {
        return parent != nullptr;
    }
    bool isLeftChild() {
        return hasParent() && parent->left == this;
    }
    bool isRightChild() {
        return hasParent() && parent->right == this;
    }
    void setLeft(Node<T>* node) {
        left = node;
        if (node != nullptr) {
          node->parent = this;
        }
        updateSize();
    }
    void setRight(Node<T>* node) {
        right = node;
        if (node != nullptr) {
            node->parent = this;
        }
        updateSize();
    }
    void swapChild(Node<T>* from, Node<T>* to) {
        if (left == from) {
            setLeft(to);
        } else if (right == from) {
            setRight(to);
        }
    }
};

template <class T>
using TraverseNodeCallback = std::function<void(Node<T>*)>;

template <class T>
class Tree {
 public:
    ~Tree();
    void add(T value);
    void remove(T value);
    T getKstat(int k);
 private:
    Node<T>* root = nullptr;
    void traverseNodesLevelOrder(TraverseNodeCallback<T>);
    void leftRotate(Node<T>*);
    void rightRotate(Node<T>*);
    void splay(Node<T>*);
    Node<T>* max(Node<T>*);
    Node<T>* min(Node<T>*);
    Node<T>* find(T);
};

template <class T>
Tree<T>::~Tree() {
    TraverseNodeCallback<T> callback = [](Node<T>* node){delete node;};
    traverseNodesLevelOrder(callback);
}

template <class T>
void Tree<T>::add(T value) {
    auto node = new Node<T>(value);
    if (root == nullptr) {
        root = node;
    } else {
        auto cur = root;
        while (true) {
            if (cur->value <= value) {
                if (cur->right == nullptr) {
                    cur->setRight(node);
                    break;
                } else {
                    cur = cur->right;
                }
            } else {
                if (cur->left == nullptr) {
                    cur->setLeft(node);
                    break;
                } else {
                    cur = cur->left;
                }
            }
        }
        while (cur->hasParent()) {
            cur->updateSize();
            cur = cur->parent;
        }
        splay(node);
        root = node;
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
void Tree<T>::leftRotate(Node<T> * cur) {
    if (cur->hasParent()) {
        auto p = cur->parent;
        auto gp = p->parent;
        p->setRight(cur->left);
        cur->setLeft(p);
        cur->parent = gp;
        if (gp != nullptr) {
            gp->swapChild(p, cur);
        }
    }
}


template <class T>
void Tree<T>::rightRotate(Node<T> *cur) {
    if (cur->hasParent()) {
        auto p = cur->parent;
        auto gp = p->parent;
        p->setLeft(cur->right);
        cur->setRight(p);
        cur->parent = gp;
        if (gp != nullptr) {
            gp->swapChild(p, cur);
        }
    }
}

template <class T>
void Tree<T>::splay(Node<T>* cur) {
    while (cur->hasParent()) {
        auto p = cur->parent;
        if (cur->isLeftChild()) {
            if (!p->hasParent()) {
                rightRotate(cur);
            } else if (p->isLeftChild()) {
                rightRotate(p);
                rightRotate(cur);
            } else {
                rightRotate(cur);
                leftRotate(cur);
            }
        } else {
            if (!p->hasParent()) {
                leftRotate(cur);
            } else if (p->isRightChild()) {
                leftRotate(p);
                leftRotate(cur);
            } else {
                leftRotate(cur);
                rightRotate(cur);
            }
        }
    }
}

template <class T>
Node<T>* Tree<T>::find(T value) {
    auto cur = root;
    while (cur != nullptr) {
        if (cur->value < value) {
            cur = cur->right;
        } else if (cur->value > value) {
            cur = cur->left;
        } else {
            splay(cur);
            root = cur;
            return root;
        }
    }
    return nullptr;
}

template <class T>
Node<T>* Tree<T>::max(Node<T>* node) {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

template <class T>
Node<T>* Tree<T>::min(Node<T>* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template <class T>
void Tree<T>::remove(T value) {
    auto node = find(value);
    if (node != nullptr) {
        if (node->left == nullptr) {
            root = node->right;
            root->parent = nullptr;
        } else {
            auto leftTreeRoot = node->left;
            leftTreeRoot->parent = nullptr;
            auto maxLeft = max(leftTreeRoot);
            splay(maxLeft);
            maxLeft->setRight(node->right);
            root = maxLeft;
        }
        delete node;
    }
}

template <class T>
T Tree<T>::getKstat(int k) {
    if (root == nullptr || root->size <= k) {
        throw std::out_of_range("Invalid k stat");
    }
    auto cur = root;
    while (true) {
        if (cur->getLeftSize() > k) {
            cur = cur->left;
        } else if (k == cur->getLeftSize()) {
            return cur->value;
        } else {
            k -= 1 + cur->getLeftSize();
            cur = cur->right;
        }
    }
}


int main() {
    int n = 0;
    std::cin >> n;

    Tree<int> tree;
    int cmd = 0;
    int k = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> cmd >> k;
        if (cmd > 0) {
            tree.add(cmd);
        } else {
            tree.remove(-cmd);
        }
        std::cout << tree.getKstat(k) << " ";
    }

    return 0;
}
