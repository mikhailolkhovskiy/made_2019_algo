#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "Huffman.h"

struct Node {
    Node(byte c) : symbol{c} {}
    ~Node() {
        if (left != nullptr) {
            delete left;
        }
        if (right != nullptr) {
            delete right;
        }
    }
    bool isLeaf() {
        return left == nullptr && right == nullptr;
    }
    byte symbol = 0;
    int count = 0;
    Node *left = nullptr;
    Node *right = nullptr;
};

struct Code {
    std::vector<byte> code;
    int len = 0;

    void addBit(bool bit) {
        int i = len / 8;
        int k = len % 8;
        if (k == 0) {
            code.push_back(0);
        }
        code[i] = code[i] | bit << k;
        ++len;
    }
};

struct NodeCmp {
    bool operator()(const Node* n1, const Node* n2) const {
        return n1->count > n2->count;
    }
};

class BitsWriter {
public:
    BitsWriter(IOutputStream* out): out{out}{}
    void writeBit(bool bit);
    void writeByte(byte b);
    void writeCode(Code &code);
    int flush();
    int getBitsCount() {
        return bitsCount;
    }

private:
    IOutputStream* out;
    byte accumulator = 0;
    int bitsCount = 0;
};

int BitsWriter::flush() {
    if (bitsCount > 0) {
        out->Write(accumulator);
    }
    return bitsCount;
}

void BitsWriter::writeBit(bool bit) {
    accumulator |= bit << bitsCount;
    ++bitsCount;
    if (bitsCount == 8) {
        bitsCount = 0;
        out->Write(accumulator);
        accumulator = 0;
    }
}

void BitsWriter::writeByte(byte b) {
    if (bitsCount == 0) {
        out->Write(b);
    } else {
        accumulator |= b << bitsCount;
        out->Write(accumulator);
        accumulator = b >> (8 - bitsCount);
    }
}

void BitsWriter::writeCode(Code& c) {
    unsigned int n = c.len / 8;
    for (unsigned int i = 0; i < n; ++i) {
        writeByte(c.code[i]);
    }
    if (n < c.code.size()) {
        byte b = c.code[c.code.size() - 1];
        int k = c.len % 8;
        accumulator |= b << bitsCount;
        if (8 - bitsCount <= k) {
            out->Write(accumulator);
            accumulator = b >> (8 - bitsCount);
            bitsCount = k - (8 - bitsCount);
        } else {
            bitsCount += k;
        }
    }
}

class BitsReader {
public:
    BitsReader(IInputStream* in): in{in} {
        hasData = in->Read(curByte);
        lastByte = !in->Read(nextByte);
        bitsCount = 8;
    }
    bool readBit(bool& bit);
    bool readByte(byte& b);
    bool isLastByte() {
        return lastByte;
    }
    int getBitsCount() {
        return bitsCount;
    }

private:
    IInputStream* in;
    byte curByte = 0;
    byte nextByte = 0;
    int bitsCount = 0;
    bool hasData = false;
    bool lastByte = false;
};

bool BitsReader::readBit(bool& bit) {
    if (hasData) {
        bit = curByte & 1;
        curByte = curByte >> 1;
        --bitsCount;
        if (bitsCount == 0) {
            if (lastByte) {
                hasData = false;
            } else {
                curByte = nextByte;
                bitsCount = 8;
                lastByte = !in->Read(nextByte);
            }
        }
        return true;
    } else {
        return false;
    }
}

bool BitsReader::readByte(byte& b) {
    if (hasData && (bitsCount == 8 || !lastByte)) {
        b = (nextByte << (bitsCount)) | curByte;
        curByte = nextByte >> (8 - bitsCount);
        if (lastByte) {
            hasData = false;
        } else {
            lastByte = !in->Read(nextByte);
        }
        return true;
    } else {
        return false;
    }
}

long buildCodeMap(std::unordered_map<byte, Code>& codes, std::vector<bool>& path, Node *node) {
    if (node->left == nullptr) {
        Code c;
        for (auto x : path) {
            c.addBit(x);
        }
        codes[node->symbol] = c;
        return (c.len * node->count);
    }
    path.push_back(false);
    long extraBits1 = buildCodeMap(codes, path, node->left);
    path.pop_back();
    path.push_back(true);
    long extraBits2 = buildCodeMap(codes, path, node->right);
    path.pop_back();
    return extraBits1 + extraBits2;
}

void writeTree(Node* node, BitsWriter& writer) {
    if (node->left == nullptr) {
        writer.writeBit(1);
        writer.writeByte(node->symbol);
    } else {
        writer.writeBit(0);
        writeTree(node->left, writer);
        writeTree(node->right, writer);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::unordered_map<byte, Node*> m;
    byte value;
    std::vector<byte> data;
    // читаем данные и подсчитываем частоты
    while (original.Read(value)) {
        data.push_back(value);
        if (m.find(value) == m.end()) {
            Node* n = new Node{value};
            n->count = 1;
            m[value] = n;
        } else {
            m[value]->count += 1;
        }
    }
    std::priority_queue<Node*, std::vector<Node*>, NodeCmp> queue;
    for (auto x : m) {
        queue.push(x.second);
    }
    //строим дерево
    while (queue.size() > 1) {
        auto n1 = queue.top();
        queue.pop();
        auto n2 = queue.top();
        queue.pop();
        Node* n = new Node(0);
        n->left = n1;
        n->right = n2;
        n->count = n1->count + n2->count;
        queue.push(n);
    }
    Node* root = queue.top();
    queue.pop();

    std::unordered_map<byte, Code> codes;
    std::vector<bool> path;
    //строим таблицу символов и считаем размер результата
    long size = buildCodeMap(codes, path, root);
    int extraBits = size % 8;
    unsigned long estimatedCompressedSize = codes.size()*9 + 3 + size;
    estimatedCompressedSize = estimatedCompressedSize/8 + ((estimatedCompressedSize%8 > 0)?1:0);

    BitsWriter writer(&compressed);
    //проверяем имеет ли смысл сжимать
    if (estimatedCompressedSize < data.size()) {
        //сначала сохраняем дерево
        writeTree(root, writer);

        extraBits = (extraBits + 3 + writer.getBitsCount()) % 8;
        //сохраняем в 3 бита количество бит с информацией в последнем байте
        writer.writeBit((extraBits >> 2) & 1);
        writer.writeBit((extraBits >> 1) & 1);
        writer.writeBit(extraBits & 1);
        //записываем сжатые данные
        for (auto d : data) {
            writer.writeCode(codes[d]);
        }
    } else {
        //помечаем что не стали сжимать
        writer.writeBit(1);
        //и копируем не сжатые данные
        for (byte x : data) {
            writer.writeByte(x);
        }
    }
    //сбрасываем последний байт
    writer.flush();
    delete root;
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    BitsReader reader{&compressed};
    bool bit = false;
    byte symbol = 0;
    reader.readBit(bit);
    if (bit) {
        //если не сжимали то просто копируем данные
        while (reader.readByte(symbol)) {
            original.Write(symbol);
        }
    } else {
        Node* root = new Node(0);
        std::vector<Node*> stack;
        stack.push_back(root);
        //читаем дерево кодов
        while (!stack.empty()) {
            Node* cur = stack.back();
            reader.readBit(bit);
            Node* newNode = new Node(0);
            if (cur->left == nullptr) {
                cur->left = newNode;
            } else {
                cur->right = newNode;
                stack.pop_back();
            }
            if (!bit) {
                stack.push_back(newNode);
            } else {
                reader.readByte(symbol);
                newNode->symbol = symbol;
            }
        }
        //читаем 3 бита с инфой о последнем байте
        int extraBits = 0;
        reader.readBit(bit);
        extraBits |= bit << 2;
        reader.readBit(bit);
        extraBits |= bit << 1;
        reader.readBit(bit);
        extraBits |= bit;
        Node* cur = root;
        //расжимаем данные использую дерево кодов
        while (!reader.isLastByte() || reader.getBitsCount() > 8 - extraBits) {
            reader.readBit(bit);
            if (bit) {
                cur = cur->right;
            } else {
                cur = cur->left;
            }
            if (cur->isLeaf()) {
                original.Write(cur->symbol);
                cur = root;
            }
        }
        delete root;
    }
}
