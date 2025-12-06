#include "Manager.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm> // for min/max

using namespace std;

void Manager::run() {
    int choice;
    do {
        cout << "\n========== Secure File Vault ==========" << "\n";
        cout << "1.  Upload File\n";
        cout << "2.  Encrypt File (placeholder)\n";
        cout << "3.  Generate Hash (placeholder)\n";
        cout << "4.  Scan File for Malware (placeholder)\n";
        cout << "5.  View Logs\n";
        cout << "6.  File Menu\n";
        cout << "7.  Search File (Binary Tree)\n";
        cout << "8.  List All Files (Sorted A-Z)\n";
        cout << "0.  Exit\n";
        cout << "======================================\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        case 7: {
            string name;
            cout << " Enter name to search: "; cin >> name;
            fl.searchInVault(name);
            break;
        }
        case 8: fl.showDirectory(); break;
        case 0: cout << " Exiting program...\n"; break;
        default: cout << " Invalid option.\n";
        }
    } while (choice != 0);
}

void Manager::LoadSignatures() {
    ifstream fin("virus_signatures.txt", ios::in);
    if (!fin.is_open()) {
        cout << "\n Warning: virus_signatures.txt is not loaded" << endl;
        return;
    }
    threatSignatures.clear();
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) 
            threatSignatures.push_back(line);
    }
    fin.close();
    cout << "Threat database updated. " << threatSignatures.size() << " signatures loaded.\n";
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
        cout << "1. Do you want to retry\n2. Create a New file with this name  \n0 else exit\nEnter your choice: ";
        cin >> retry;
        if (retry == 1) {
            uploadFile();
        } else if (retry == 2) {
            fl.CreatFile(name);
            if (fl.uploadFile(name)) {
                log.logEvent(" Uploaded: " + name);
                fl.AddToQueue(name);
            }
        }
    }
}

void Manager::encryptFile() {
    int option;
    cout << "Enter Option \n1 Encyption  \n2 Decryption : ";
    cin >> option;
    if (option == 1) {
        string name;
        cout << "Enter filename to encrypt: ";
        cin >> name;
        fl.encryption(name);
        cout << " (placeholder) Encrypting " << name << " ... Done.\n";
        log.logEvent(" Encrypted: " + name);
    } else if (option == 2) {
        string name;
        cout << "Enter filename to Dencrypt: ";
        cin >> name;
        fl.DecryptedFile(name);
        cout << " (placeholder) Dencrypting " << name << " ... Done.\n";
        log.logEvent(" Dencrypted: " + name);
    }
}

void Manager::hashFile() {
    string name;
    cout << "Enter filename to hash: ";
    cin >> name;
    
    ifstream check(name);
    if (!check.is_open()) {
        cout << " Error: File not found: " << name << "\n";
        log.logEvent(" Hash failed (not found): " + name);
        return;
    }
    check.close();

    string hashString = fl.generateHash(name);
    if (hashString.empty()) {
        cout << " File is empty or hash generation failed.\n";
        log.logEvent(" Hashed (empty file): " + name);
        return;
    }

    fl.saveHashFile(name, hashString);
    cout << " Successfully created and saved line-by-line hash for " << name << "\n";
    log.logEvent(" Hashed (Line-by-Line) and Saved: " + name);
}

void Manager::scanFile() {
    LoadSignatures();
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
        for (char &c : line) {
            c = tolower(c);
        }
        for (const string &sig : threatSignatures) {
            if (line.find(sig) != string::npos) {
                cout << " [ALERT] Threat detected: " << sig << " on line " << endl;
                suspicious = true;
            }
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
        cout << "\n========== File Menu ==========" << "\n";
        cout << "1.  Display Current File\n";
        cout << "2.  Show Current Filename\n";
        cout << "3.  Show File Data Summary\n";
        cout << "4.  Create New File\n";
        cout << "5.  Delete a File\n";
        cout << "6.  Show Encrypted  Files\n";
        cout << "0.  Back to Main Menu\n";
        cout << "================================\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: fl.displayfile(); break;
        case 2: {
            string cur = fl.getCurrentFile();
            if (cur.empty()) cout << "  No current file loaded.\n";
            else cout << " Current file: " << cur << "\n";
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
                for (size_t i = 0; i < limit; ++i)
                    cout << i + 1 << ": " << data[i] << "\n";
            }
            break;
        }
        case 4: {
            string filename;
            cout << " Enter the name of the file to be created: ";
            cin >> filename;
            fl.CreatFile(filename);
            fl.AddToQueue(filename);
            log.logEvent(" Created: " + filename);
            break;
        }
        case 5: {
            string filename;
            cout << " Enter the name of the file to be deleted: ";
            cin >> filename;
            fl.DeleteFile(filename);
            log.logEvent(" Deleted: " + filename);
            break;
        }
        case 6:
            fl.ShowEcryptedFiles();
            log.logEvent("Encryted Files Showed");
            break;
        case 0:
            cout << " Returning to main menu...\n";
            break;
        default:
            cout << " Invalid option.\n";
        }
    } while (choice != 0);
}