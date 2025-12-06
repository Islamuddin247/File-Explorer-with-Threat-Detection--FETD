#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm> // for min
#include <cstdio>    // for remove

using namespace std;

void FileHandler::ShowEcryptedFiles() {
    history.ShowEncryptedFile();
}

void FileHandler::searchInVault(string name) {
    if(treeIndex.findFile(name))
        cout << " [TREE SEARCH] FOUND! '" << name << "' is in the vault.\n";
    else
        cout << " [TREE SEARCH] '" << name << "' NOT found.\n";
}

void FileHandler::showDirectory() {
    treeIndex.showSortedFiles();
}

void FileHandler::CreatFile(const string &filename) {
    ofstream NewFile(filename, ios::out);
    if (!NewFile) {
        cout << " Error creating file " << filename << "\n";
        return;
    }
    cout << " File " << filename << " created successfully.\n";

    treeIndex.addFile(filename);

    int choice;
    cout << " Do you want to add content to the file now? (1 for Yes / 0 for No): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (choice == 1) {
        string line;
        cout << " Enter content line by line. Type 'END' on a new line to finish:\n";
        while (true) {
            getline(cin, line);
            if (line == "END")
                break;
            NewFile << line << "\n";
        }
        cout << " Content added to " << filename << "\n";
    }
    NewFile.close();
}

void FileHandler::encryption(const string &filename) {
    queue<string> tem;
    bool check = false;

    while (!FilesQueues.empty()) {
        if (FilesQueues.front() == filename) {
            check = true;
        }
        tem.push(FilesQueues.front());
        FilesQueues.pop();
    }

    FilesQueues = tem;
    if (!check) {
        cout << "File not in queue." << endl;
        return;
    }
    string key;
    string encrypted = "Encrypted" + filename;
    if (check) {
        ifstream InFile(filename);
        if (!InFile.is_open()) {
            cout << "Error opening " << filename << endl;
            return;
        }

        ofstream Efile(encrypted, ios::out);
        if (!Efile.is_open()) {
            cout << "Error creating encrypted file: " << encrypted << endl;
            InFile.close();
            return;
        }

        cout << "Enter the Key : ";
        cin >> key;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string line;
        while (getline(InFile, line)) {
            string cipher;
            int i = 0;
            for (char c : line) {
                if (isalpha(static_cast<unsigned char>(c))) {
                    char p = toupper(static_cast<unsigned char>(c));
                    char k = toupper(static_cast<unsigned char>(key[i % key.size()]));
                    char enc = ((p - 'A') + (k - 'A')) % 26 + 'A';
                    cipher.push_back(enc);
                    i++;
                } else
                    cipher.push_back(c);
            }
            Efile << cipher << endl;
        }

        InFile.close();
        Efile.close();

        AddToStack(encrypted);
        AddToStack(key);
        DeleteFile(filename);
    }
}

void FileHandler::DecryptedFile(const string &filename) {
    string Fname = "Encrypted" + filename;
    if (history.FindFile(Fname, 1)) {
        string key;
        cout << "Enter the Key : ";
        cin >> key;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (history.FindFile(key, 2)) {
            ifstream Infile(Fname);
            if (!Infile.is_open()) {
                cout << " Error opening: " << Fname << "\n";
                return;
            }
            ofstream WriteFile(filename, ios::out);
            if (!WriteFile.is_open()) {
                cout << " Error creating output file: " << filename << "\n";
                Infile.close();
                return;
            }
            string cipher;

            while (getline(Infile, cipher)) {
                string plain;
                int i = 0;
                for (char c : cipher) {
                    if (isalpha(static_cast<unsigned char>(c))) {
                        char e = toupper(static_cast<unsigned char>(c));
                        char k = toupper(static_cast<unsigned char>(key[i % key.size()]));
                        char dec = ((e - 'A') - (k - 'A') + 26) % 26 + 'A';
                        plain.push_back(dec);
                        i++;
                    } else
                        plain.push_back(c);
                }
                WriteFile << plain << endl;
            }
            Infile.close();
            WriteFile.close();

            AddToQueue(filename);
            treeIndex.addFile(filename);
            history.Delete(Fname);
            history.Delete(key);
        } else {
            cout << "Key in not Correct " << endl;
        }
    } else {
        cout << "File is not Encrypted " << endl;
    }
}

void FileHandler::DeleteFile(const string &filename) {
    bool deleted = false;
    ifstream check(filename);
    if (check.is_open()) {
        check.close();
        if (remove(filename.c_str()) != 0) {
            cout << " Error deleting file: " << filename << "\n";
        } else {
            cout << " File deleted successfully: " << filename << "\n";
            deleted = true;
            treeIndex.removeFile(filename);
            cout << " [Tree] Removed from Search Index.\n";
        }
    }

    queue<string> temp;
    bool found = false;
    while (!FilesQueues.empty()) {
        if (FilesQueues.front() == filename) {
            found = true; 
        } else {
            temp.push(FilesQueues.front());
        }
        FilesQueues.pop();
    }
    FilesQueues = temp;

    if (found)
        cout << "  Removed from queue: " << filename << "\n";
    else
        cout << "  File not found in queue: " << filename << "\n";

    if (deleted && found)
        cout << " File '" << filename << "' deleted and dequeued successfully.\n";
}

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
    treeIndex.addFile(filename);
    return true;
}

void FileHandler::AddToQueue(const string &filename) {
    FilesQueues.push(filename);
    cout << "  Queued: " << filename << "\n";
}

void FileHandler::AddToStack(const string &filename) {
    history.push(filename);
    cout << " Stacked : " << filename << endl;
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

string FileHandler::generateHash(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) return ""; 

    string allHashes = "";
    string line;

    while (getline(file, line)) {
        unsigned long long hash = 5381; 
        for (char c : line) {
            hash = ((hash << 5) + hash) + c; 
        }

        string hexResult = "";
        string hexDigits = "0123456789abcdef"; 

        if (hash == 0) hexResult = "0";
        else {
            while (hash > 0) {
                int remainder = hash % 16;
                hexResult = hexDigits[remainder] + hexResult;
                hash /= 16;
            }
        }
        allHashes += hexResult + "\n";
    }
    file.close();
    return allHashes;
}

void FileHandler::saveHashFile(string originalName, string hashString) {
    string newFileName = originalName + ".hash";
    ofstream file(newFileName);

    if (file.is_open()) {
        file << hashString;
        file.close();
        cout << " Hash saved to file: " << newFileName << "\n";
        treeIndex.addFile(newFileName);
        AddToQueue(newFileName);
    } else {
        cout << " Error creating hash file.\n";
    }
}

void FileHandler::DeleteFile(const string &filename) {
    bool deleted = false;

    // 1️ Delete file from disk
    if (remove(filename.c_str()) != 0) {
        cout << " Error deleting file: " << filename << "\n";
    } else {
        cout << " File deleted: " << filename << "\n";
        deleted = true;
    }
}
