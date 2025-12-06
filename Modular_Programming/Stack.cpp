#include "Stack.h"
#include <iostream>
#include <fstream>
#include <cstdio> // for remove()

using namespace std;

Stack::Stack() {
    history = nullptr;
}

void Stack::push(string data) {
    LogNode *tem = new LogNode(data);
    if (!history) {
        history = tem;
    } else {
        tem->next = history;
        history = tem;
    }
}

void Stack::pop() {
    if (!history) {
        cout << "No Encrypted File : " << endl;
        return;
    }
    LogNode *tem = history;
    history = history->next;
    cout << "Popped From Stack " << tem->message << endl;
    delete tem;
}

string Stack::Top() {
    if (!history) {
        cout << "No Encrypted File : " << endl;
        return "-1";
    } else {
        return history->message;
    }
}

void Stack::Delete(string name) {
    // Only attempt to delete file on disk if it actually exists
    ifstream checkFile(name);
    if (checkFile.is_open()) {
        checkFile.close();
        if (remove(name.c_str()) == 0) {
            cout << " File deleted successfully: " << name << "\n";
        } else {
            cout << " Error deleting file: " << name << "\n";
        }
    }

    LogNode *tem = history;
    LogNode *prev = nullptr;
    bool find = false;
    while (tem) {
        if (tem->message == name) {
            find = true;
            break;
        }
        prev = tem;
        tem = tem->next;
    }
    if (find) {
        if (prev) {
            prev->next = tem->next;
            delete tem;
        } else {
            history = tem->next;
            delete tem;
        }
        cout << "Removed from stack " << endl;
    } else {
        cout << "Not Encrypted !! " << endl;
    }
}

bool Stack::FindFile(string name, int x) {
    LogNode *tem = history;
    while (tem) {
        if (tem->message == name) {
            return true;
        }
        tem = tem->next;
    }
    return false;
}

void Stack::ShowEncryptedFile() {
    if (!history) {
        cout << "NO Encrypted file !!" << endl;
        return;
    }
    LogNode *tem = history;
    cout << "\n========Encrypted FIles============" << endl;
    while (tem) {
        cout << tem->message << endl;
        tem = tem->next;
    }
}