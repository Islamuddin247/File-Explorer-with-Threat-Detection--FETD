#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

class FileHandler {
    string currentFile;
    vector<string> fileData;
    queue<string> FilesQueues;

public:
    bool uploadFile(const string &filename);
    void AddToQueue(const string &filename);
    void displayfile() const;
    vector<string> getFileData() const;
    string getCurrentFile() const;
    void CreatFile(const string &filename);
    void DeleteFile(const string &filename);
};

#endif