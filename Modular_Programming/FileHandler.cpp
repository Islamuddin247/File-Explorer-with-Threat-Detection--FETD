#include "FileHandler.h"
#include <fstream>

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


void FileHandler::CreatFile(const string &filename) {
    ofstream NewFile(filename, ios::out);
    if (!NewFile) {
        cout << " Error creating file " << filename << "\n";
        return;
    }
    cout << " File " << filename << " created successfully.\n";

    int choice;
    cout << " Do you want to add content to the file now? (1 for Yes / 0 for No): ";
    cin >> choice;
    cin.ignore();
    if (choice == 1) {
        string line;
        cout << " Enter content line by line. Type 'END' on a new line to finish:\n";
        while (true) {
            getline(cin, line);
            if (line == "END") break;
            NewFile << line << "\n";
        }
        cout << " Content added to " << filename << "\n";
    }
    NewFile.close();

    // ✅ Add the newly created file to queue
    AddToQueue(filename);
}

void FileHandler::DeleteFile(const string &filename) {
    bool deleted = false;

    // 1️⃣ Delete file from disk
    if (remove(filename.c_str()) != 0) {
        cout << " Error deleting file: " << filename << "\n";
    } else {
        cout << " File deleted successfully: " << filename << "\n";
        deleted = true;
    }

    // 2️⃣ Remove file from queue
    queue<string> temp;
    bool found = false;

    while (!FilesQueues.empty()) {
        if (FilesQueues.front() == filename) {
            found = true; // skip this file
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

    // 3️⃣ Optional: message summary
    if (deleted && found)
        cout << " File '" << filename << "' deleted and dequeued successfully.\n";
}
