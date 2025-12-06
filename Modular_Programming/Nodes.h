#ifndef NODES_H
#define NODES_H

#include <string>

// Node for Stack and Logger
class LogNode {
public:
    std::string message;
    LogNode *next;
    LogNode(const std::string &msg) : message(msg), next(nullptr) {}
};

// Node for Binary Search Tree
struct BSTNode {
    std::string fileName;
    BSTNode *left;
    BSTNode *right;
    BSTNode(std::string name) : fileName(name), left(nullptr), right(nullptr) {}
};

#endif