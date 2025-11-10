#include "Logger.h"

LogNode::LogNode(const string &msg) : message(msg), next(nullptr) {}

Logger::Logger() : head(nullptr) {}

Logger::~Logger() {
    LogNode *temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        delete temp;
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
    LogNode *temp = head;
    while (temp != nullptr) {
        cout << " " << temp->message << "\n";
        temp = temp->next;
    }
    cout << " ********************************\n";
}
