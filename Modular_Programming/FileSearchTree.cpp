#include "FileSearchTree.h"
#include <iostream>

using namespace std;

FileSearchTree::FileSearchTree() : root(nullptr) {}

BSTNode* FileSearchTree::insert(BSTNode* node, string name) {
    if (!node) return new BSTNode(name);
    if (name < node->fileName)
        node->left = insert(node->left, name);
    else if (name > node->fileName)
        node->right = insert(node->right, name);
    return node;
}

bool FileSearchTree::search(BSTNode* node, string name) {
    if (!node) return false;
    if (node->fileName == name) return true;
    
    if (name < node->fileName) 
        return search(node->left, name);
    else 
        return search(node->right, name);
}

void FileSearchTree::inOrder(BSTNode* node) {
    if (!node) return;
    inOrder(node->left);
    cout << "  - " << node->fileName << "\n";
    inOrder(node->right);
}

BSTNode* FileSearchTree::minValueNode(BSTNode* node) {
    BSTNode* current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

BSTNode* FileSearchTree::deleteNode(BSTNode* root, string name) {
    if (!root) return root;

    if (name < root->fileName)
        root->left = deleteNode(root->left, name);
    else if (name > root->fileName)
        root->right = deleteNode(root->right, name);
    else {
        // Node with only one child or no child
        if (!root->left) {
            BSTNode* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            BSTNode* temp = root->left;
            delete root;
            return temp;
        }
        // Node with two children
        BSTNode* temp = minValueNode(root->right);
        root->fileName = temp->fileName;
        root->right = deleteNode(root->right, temp->fileName);
    }
    return root;
}

void FileSearchTree::addFile(string name) {
    root = insert(root, name);
}

bool FileSearchTree::findFile(string name) {
    return search(root, name);
}

void FileSearchTree::removeFile(string name) {
    root = deleteNode(root, name);
}

void FileSearchTree::showSortedFiles() {
    if (!root) {
        cout << "  (Index is empty)\n";
        return;
    }
    cout << "\n --- Files (Alphabetical Order via Tree) ---\n";
    inOrder(root);
    cout << " -------------------------------------------\n";
}