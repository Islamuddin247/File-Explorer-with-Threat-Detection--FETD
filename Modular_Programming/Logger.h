#ifndef LOGGER_H
#define LOGGER_H

#include "Nodes.h"
#include <string>

class Logger {
    LogNode *head;

public:
    Logger();
    ~Logger();
    void logEvent(const std::string &message);
    void showLogs() const;
};

#endif