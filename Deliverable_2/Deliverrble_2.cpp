#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
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
};

bool FileHandler::uploadFile(const string &filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << " Error: File not found: " << filename << "\n";
        return false;
    }

    currentFile = filename;

    fileData.clear();

    string line;
    while (getline(fin, line)) {
        fileData.push_back(line);
    }
    fin.close();

    cout << " File loaded: " << filename << "\n";
    
    return true;
}

void FileHandler::AddToQueue(const string &filename) {
    FilesQueues.push(filename);
    cout << "  Queued: " << filename << "\n";
}

void FileHandler::displayfile() const {
    if (fileData.empty()) {
        cout << "  No file loaded to display.\n";
        return;
    }
    cout << "\n File: " << currentFile << "\n--------------------------------\n";
    for (const string &line : fileData) {
        cout << line << "\n";
    }
    cout << "--------------------------------\n";
}

vector<string> FileHandler::getFileData() const {
    return fileData;
}

string FileHandler::getCurrentFile() const {
    return currentFile;
}

class LogNode {
public:
    string message;
    LogNode *next;
    LogNode(const string &msg) : message(msg), next(nullptr) {}
};

class Logger {
    LogNode *head;

public:
    Logger();
    ~Logger();
    void logEvent(const string &message);
    void showLogs() const;
};

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

void Manager::run() {
    int choice;
    do {
        cout << "\n========== Secure File Vault ==========\n";
        cout << "1.  Upload File\n";
        cout << "2.  Encrypt File (placeholder)\n";
        cout << "3.  Generate Hash (placeholder)\n";
        cout << "4.  Scan File for Malware (placeholder)\n";
        cout << "5.  View Logs\n";
        cout << "6.  File Menu\n";
        cout << "0.  Exit\n";
        cout << "======================================\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore('\n');
            cout << " Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: uploadFile(); break;
            case 2: encryptFile(); break;
            case 3: hashFile(); break;
            case 4: scanFile(); break;
            case 5: showLogs(); break;
            case 6: FileMenu(); break;
            case 0: cout << " Exiting program...\n"; break;
            default: cout << " Invalid option.\n";
        }
    } while (choice != 0);
}

void Manager::uploadFile() {
    string name;
    cout << "Enter filename to upload: ";
    cin >> name;

    if (fl.uploadFile(name)) {
        log.logEvent(" Uploaded: " + name);
        fl.AddToQueue(name); 
    } else {
        log.logEvent(" Failed upload: " + name);
    }
}

void Manager::encryptFile() {
    string name;
    cout << "Enter filename to encrypt: ";
    cin >> name;

    ifstream fin(name);
    if (!fin.is_open()) {
        cout << " Error: File not found: " << name << "\n";
        log.logEvent(" Encrypt failed (not found): " + name);
        return;
    }
    fin.close();

    cout << " (placeholder) Encrypting " << name << " ... Done.\n";
    log.logEvent(" Encrypted: " + name);
}

void Manager::hashFile() {
    string name;
    cout << "Enter filename to hash: ";
    cin >> name;

    ifstream fin(name);
    if (!fin.is_open()) {
        cout << " Error: File not found: " << name << "\n";
        log.logEvent(" Hash failed (not found): " + name);
        return;
    }
    fin.close();

    cout << " (placeholder) Hash generated for " << name << ": [SAMPLE_HASH]\n";
    log.logEvent(" Hashed: " + name);
}

void Manager::scanFile() {
    string name;
    cout << "Enter filename to scan: ";
    cin >> name;

    ifstream fin(name);
    if (!fin.is_open()) {
        cout << " Error: File not found: " << name << "\n";
        log.logEvent(" Scan failed (not found): " + name);
        return;
    }

    string line;
    bool suspicious = false;
    while (getline(fin, line)) {
        if (line.find("malware") != string::npos || line.find("virus") != string::npos) {
            suspicious = true;
            break;
        }
    }
    fin.close();

    if (suspicious) {
        cout << "  Scan result for " << name << ": Suspicious content found!\n";
        log.logEvent(" Scanned (suspicious): " + name);
    } else {
        cout << " Scan result for " << name << ": No threats detected.\n";
        log.logEvent(" Scanned: " + name + " (no threats)");
    }
}

void Manager::showLogs() {
    log.showLogs();
}

void Manager::FileMenu() {
    int choice;
    do {
        cout << "\n========== File Menu ==========\n";
        cout << "1.  Display Current File\n";
        cout << "2.  Show Current Filename\n";
        cout << "3.  Show File Data Summary\n";
        cout << "0.  Back to Main Menu\n";
        cout << "================================\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore();
            cout << " Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                fl.displayfile();
                break;

            case 2: {
                string cur = fl.getCurrentFile();
                if (cur.empty()) {
                    cout << "  No current file loaded.\n";
                } else {
                    cout << " Current file: " << cur << "\n";
                }
                break;
            }

            case 3: {
                vector<string> data = fl.getFileData();
                if (data.empty()) {
                    cout << "  No file data loaded.\n";
                } else {
                    cout << " File has " << data.size() << " line(s).\n";
                    
                    size_t limit = min<size_t>(data.size(), 10);
                    cout << "Showing up to first " << limit << " line(s):\n";
                    for (size_t i = 0; i < limit; ++i) {
                        cout << i + 1 << ": " << data[i] << "\n";
                    }
                }
                break;
            }

            case 0:
                cout << " Returning to main menu...\n";
                break;

            default:
                cout << " Invalid option.\n";
        }

    } while (choice != 0);
}

int main() {
    Manager manager;
    manager.run();
    return 0;
}
