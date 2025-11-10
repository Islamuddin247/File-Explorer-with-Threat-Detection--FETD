#ifndef MANAGER_H
#define MANAGER_H

#include "Logger.h"
#include "FileHandler.h"

class Manager {
public:
    void run();

private:
    Logger log;
    FileHandler fl;

    void uploadFile();
    void encryptFile();
    void hashFile();
    void scanFile();
    void showLogs();
    void FileMenu();
};

#endif
