#include "HuffmanTree.hpp"
#include <fstream>
#include <sstream>
#include <iostream> // Include for std::cerr

HuffmanTree::Node::Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}

HuffmanTree::Node::Node(Node *left, Node *right) : ch('\0'), freq(left->freq + right->freq), left(left), right(right) {}

HuffmanTree::Node::~Node() {
    delete left;
    delete right;
}

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::~HuffmanTree() {
    deleteTree(root);
}

void HuffmanTree::build(const std::string &data) {
    buildFrequencyTable(data);
    buildHuffmanTree();
    buildCodes(root, "");
}

void HuffmanTree::buildFrequencyTable(const std::string &data) {
    for (char ch : data) {
        frequencyTable[ch]++;
    }
}

void HuffmanTree::buildHuffmanTree() {
    auto comp = [](Node *left, Node *right) { return left->freq > right->freq; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> pq(comp);

    for (auto pair : frequencyTable) {
        nodes.push_back(new Node(pair.first, pair.second));
    }

    for (Node *node : nodes) {
        pq.push(node);
    }

    while (pq.size() != 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }

    root = pq.top();
}

void HuffmanTree::buildCodes(Node *node, const std::string &str) {
    if (!node) return;

    if (!node->left && !node->right) {
        huffmanCode[node->ch] = str;
    }

    buildCodes(node->left, str + "0");
    buildCodes(node->right, str + "1");
}

std::string HuffmanTree::encode(const std::string &data) {
    std::string encodedString;
    for (char ch : data) {
        encodedString += huffmanCode[ch];
    }
    return encodedString;
}

std::string HuffmanTree::decode(const std::string &data) {
    std::string decodedString;
    Node *current = root;
    for (char bit : data) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (!current->left && !current->right) {
            decodedString += current->ch;
            current = root;
        }
    }
    return decodedString;
}

void HuffmanTree::deleteTree(Node *node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

void HuffmanTree::saveTreeToFile(const std::string &filePath) {
    std::ofstream outFile(filePath, std::ios::binary);
    saveTreeHelper(root, outFile);
    outFile.close();
}

void HuffmanTree::saveTreeHelper(Node *node, std::ofstream &outFile) {
    if (node) {
        if (!node->left && !node->right) {
            outFile.put('L'); // Leaf node marker
            outFile.put(node->ch);
        } else {
            outFile.put('I'); // Internal node marker
            saveTreeHelper(node->left, outFile);
            saveTreeHelper(node->right, outFile);
        }
    }
}

void HuffmanTree::loadTreeFromFile(const std::string &filePath) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open tree file for loading." << std::endl;
        return;
    }

    root = loadTreeHelper(inFile);
    inFile.close();
}

HuffmanTree::Node* HuffmanTree::loadTreeHelper(std::ifstream &inFile) {
    if (!inFile.good()) return nullptr;

    char marker = inFile.get();
    if (marker == 'L') {
        char ch = inFile.get();
        return new Node(ch, 0);
    } else if (marker == 'I') {
        Node *left = loadTreeHelper(inFile);
        Node *right = loadTreeHelper(inFile);
        return new Node(left, right);
    }

    return nullptr;
}
