#ifndef MANAGER_H
#define MANAGER_H

#include "Logger.h"
#include "FileHandler.h"
#include <vector>
#include <string>

class Manager {
public:
    void run();

private:
    Logger log;
    FileHandler fl;
    std::vector<std::string> threatSignatures;

    void uploadFile();
    void encryptFile();
    void hashFile();
    void scanFile();
    void showLogs();
    void FileMenu();
    void LoadSignatures();
};

#endif