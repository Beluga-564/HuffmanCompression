#include <iostream>
#include <fstream>
#include <sstream>
#include "HuffmanTree.hpp"

void compressFile(const std::string &inputFilePath, const std::string &outputFilePath, const std::string &treeFilePath) {
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file for compression." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string data = buffer.str();

    HuffmanTree huffmanTree;
    huffmanTree.build(data);
    std::string encodedData = huffmanTree.encode(data);

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file for compression." << std::endl;
        return;
    }
    outputFile << encodedData;

    huffmanTree.saveTreeToFile(treeFilePath);
}

void decompressFile(const std::string &inputFilePath, const std::string &outputFilePath, const std::string &treeFilePath) {
    HuffmanTree huffmanTree;
    huffmanTree.loadTreeFromFile(treeFilePath);

    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file for decompression." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string encodedData = buffer.str();

    std::string decodedData = huffmanTree.decode(encodedData);

    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file for decompression." << std::endl;
        return;
    }
    outputFile << decodedData;
}

int main() {
    std::string inputFilePath = "input.txt";
    std::string compressedFilePath = "output.huff";
    std::string decompressedFilePath = "decompressed.txt";
    std::string treeFilePath = "huffman_tree.dat";

    // Compress input.txt and save the Huffman tree to huffman_tree.dat
    compressFile(inputFilePath, compressedFilePath, treeFilePath);

    // Decompress output.huff using huffman_tree.dat and save to decompressed.txt
    decompressFile(compressedFilePath, decompressedFilePath, treeFilePath);

    std::cout << "Compression and decompression completed." << std::endl;

    return 0;
}
