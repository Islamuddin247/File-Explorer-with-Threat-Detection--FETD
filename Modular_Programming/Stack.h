#ifndef STACK_H
#define STACK_H

#include "Nodes.h"
#include <string>

class Stack {
public:
    LogNode *history; // Renamed from 'histroy' to 'history'

    Stack();
    void push(std::string data);
    void pop();
    std::string Top();
    void Delete(std::string name);
    bool FindFile(std::string name, int x);
    void ShowEncryptedFile();
};

#endif