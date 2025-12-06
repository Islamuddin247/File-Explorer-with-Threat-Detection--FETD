#ifndef FILESEARCHTREE_H
#define FILESEARCHTREE_H

#include "Nodes.h"
#include <string>

class FileSearchTree {
    BSTNode* root;

    BSTNode* insert(BSTNode* node, std::string name);
    bool search(BSTNode* node, std::string name);
    void inOrder(BSTNode* node);
    BSTNode* minValueNode(BSTNode* node);
    BSTNode* deleteNode(BSTNode* root, std::string name);

public:
    FileSearchTree();
    void addFile(std::string name);
    bool findFile(std::string name);
    void removeFile(std::string name);
    void showSortedFiles();
};

#endif