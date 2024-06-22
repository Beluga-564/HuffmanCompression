#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <string>
#include <unordered_map>
#include <queue>

class HuffmanTree {
public:
    HuffmanTree();
    ~HuffmanTree();

    void build(const std::string &data);
    std::string encode(const std::string &data);
    std::string decode(const std::string &data);

    void saveTreeToFile(const std::string &filePath);
    void loadTreeFromFile(const std::string &filePath);

private:
    struct Node {
        char ch;
        int freq;
        Node *left, *right;

        Node(char ch, int freq);
        Node(Node *left, Node *right);
        ~Node();
    };

    Node *root;
    std::unordered_map<char, std::string> huffmanCode;

    void buildFrequencyTable(const std::string &data);
    void buildHuffmanTree();
    void buildCodes(Node *node, const std::string &str);
    void deleteTree(Node *node);

    void saveTreeHelper(Node *node, std::ofstream &outFile); // Declaration added
    Node* loadTreeHelper(std::ifstream &inFile); // Declaration added

    std::unordered_map<char, int> frequencyTable;
    std::vector<Node*> nodes;
};

#endif // HUFFMAN_TREE_HPP
