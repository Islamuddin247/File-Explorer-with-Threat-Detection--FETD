#include "Logger.h"
#include <iostream>

using namespace std;

Logger::Logger() : head(nullptr) {}

Logger::~Logger() {
    LogNode *tem;
    while (head != nullptr) {
        tem = head;
        head = head->next;
        delete tem;
    }
}

void Logger::logEvent(const string &message) {
    LogNode *newNode = new LogNode(message);
    newNode->next = head;
    head = newNode;
}

void Logger::showLogs() const {
    if (!head) {
        cout << " No logs yet.\n";
        return;
    }
    cout << "\n ********* ACTION LOGS ***********\n";
    LogNode *tem = head;
    while (tem != nullptr) {
        cout << " " << tem->message << "\n";
        tem = tem->next;
    }
    cout << " ********************************\n";
}