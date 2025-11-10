#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
using namespace std;

class LogNode {
public:
    string message;
    LogNode *next;
    LogNode(const string &msg);
};

class Logger {
    LogNode *head;

public:
    Logger();
    ~Logger();
    void logEvent(const string &message);
    void showLogs() const;
};

#endif
