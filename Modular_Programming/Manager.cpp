#include "Manager.h"
#include <fstream>
#include <algorithm>

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
        int retry;
        cout << "1. Retry\n2. Create New file\n0. Exit\nChoice: ";
        cin >> retry;
        if (retry == 1) uploadFile();
        else if (retry == 2) {
            fl.CreatFile(name);
            if (fl.uploadFile(name)) {
                log.logEvent(" Uploaded: " + name);
                fl.AddToQueue(name);
            }
        }
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
        cout << "  Suspicious content found!\n";
        log.logEvent(" Scanned (suspicious): " + name);
    } else {
        cout << "  No threats detected.\n";
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
        cout << "3.  File Data Summary\n";
        cout << "4.  Create New File\n";
        cout << "5.  Delete File\n";
        cout << "0.  Back\n";
        cout << "================================\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore();
            cout << " Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: fl.displayfile(); break;
            case 2: {
                string cur = fl.getCurrentFile();
                cout << (cur.empty() ? " No file loaded.\n" : " Current file: " + cur + "\n");
                break;
            }
            case 3: {
                auto data = fl.getFileData();
                if (data.empty()) cout << " No file data loaded.\n";
                else {
                    cout << " File has " << data.size() << " line(s).\n";
                    size_t limit = min<size_t>(data.size(), 10);
                    for (size_t i = 0; i < limit; ++i)
                        cout << i + 1 << ": " << data[i] << "\n";
                }
                break;
            }
             case 4:
        {
            string filename;
            cout << " Enter the name of the file to be created: ";
            cin >> filename;
            fl.CreatFile(filename);
            fl.AddToQueue(filename);
            log.logEvent(" Created: " + filename);
            break;
        }
            case 5:
        {
            string filename;
            cout << " Enter the name of the file to be deleted: ";
            cin >> filename;
            fl.DeleteFile(filename);
            log.logEvent(" Deleted: " + filename);
            break;
        }
            case 0: cout << " Returning to main menu...\n"; break;
            default: cout << " Invalid option.\n";
        }
    } while (choice != 0);
}
