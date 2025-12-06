#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <queue>
#include "Stack.h"
#include "FileSearchTree.h"

class FileHandler {
    std::string currentFile;
    std::vector<std::string> fileData;
    std::queue<std::string> FilesQueues;
    Stack history;
    FileSearchTree treeIndex;

public:
    bool uploadFile(const std::string &filename);
    void AddToQueue(const std::string &filename);
    void displayfile() const;
    std::vector<std::string> getFileData() const;
    std::string getCurrentFile() const;
    void CreatFile(const std::string &filename);
    void DeleteFile(const std::string &filename);
    void encryption(const std::string &filename);
    void AddToStack(const std::string &filename);
    void ShowEcryptedFiles();
    void DecryptedFile(const std::string &filename);
    void searchInVault(std::string name);
    void showDirectory();
    std::string generateHash(const std::string &filename);
    void saveHashFile(std::string originalName, std::string hashString);
};

#endif