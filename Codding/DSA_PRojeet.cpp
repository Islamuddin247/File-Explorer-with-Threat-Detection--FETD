#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
//            Start
//  To write and read  txt file 
class FileHandler{
    string currentFile;
    vector<string> fileData;
    public:
    bool uploadFile(const string filename);
    void displayfile();
    vector<string> getFileData ();
    string getCurrentFile();
};

bool FileHandler::uploadFile(const string filename){
    ifstream fin(filename);
    if(!fin.is_open()){
        cout<<"File is not open \n Error: File is not found "<<endl;
        return false;
    }

    currentFile=filename;
    fileData.clear();

    string line;
    while (getline(fin,line))
    {
        fileData.push_back(line);
    }
    fin.close();
    cout<<"File "<<filename<<" is loaded successfully "<<endl;
    return true;
    
}
void FileHandler::displayfile(){
    if(fileData.empty()){
        cout<<"No File Loaded "<<endl;
        return;
    }
    cout<<"File content ( "<<currentFile<<" ): is"<<endl;
    for(const string& line: fileData ){
        cout<<line<<endl;
    }
    cout<<endl;
} 
vector<string> FileHandler::getFileData(){
    return fileData;
}
string FileHandler::getCurrentFile(){
    return currentFile;
}
//       End
//    
//  Stack Implemented to store the History and track the nodes
//        Start
class LogNode{
    public:
    string message;
    LogNode* next;
    LogNode(string msg){
        message=msg;
        next=nullptr;
    }
};

//  Loger class the for the loggind the users 
//      Start
class Logger{
    LogNode* head;

    public:
    Logger();
    ~Logger();
    void logEvent(const string& message);
    void showLogs();
};

Logger::Logger(){
    head=nullptr;
}
Logger::~Logger(){
    LogNode* tem;
    while (head!=nullptr)
    {
        tem=head;
        head=head->next;
        delete tem;
    }
    
}
void Logger::logEvent(const string& message){
    LogNode* newNode=new LogNode(message);
    newNode->next=head;
    head=newNode;
}
void Logger::showLogs(){
    if(!head){
        cout<<"No logs yet !!! "<<endl;
        return;
    }
    cout<<"\n********* ACTION LOGS ***********\n";
    LogNode* tem=head;
    while (tem!=nullptr)
    {
        cout<<"  "<<tem->message<<endl;
        tem=tem->next;
    }
    cout<<"*******************"<<endl;
    
}
//  End of logger

//  This class manage the whole project and it links the all functions
//    Start
class Manager {
public:
    void run();
private:
    void uploadFile();
    void encryptFile();
    void hashFile();
    void scanFile();
    void showLogs();
};

void Manager::run() {
    int choice;
    do {
        cout << "\n========== Secure File Vault ==========\n";
        cout << "1. Upload File\n";
        cout << "2. Encrypt File\n";
        cout << "3. Generate Hash\n";
        cout << "4. Scan File for Malware\n";
        cout << "5. View Logs\n";
        cout << "0. Exit\n";
        cout << "======================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: uploadFile(); break;
            case 2: encryptFile(); break;
            case 3: hashFile(); break;
            case 4: scanFile(); break;
            case 5: showLogs(); break;
            case 0: cout << "Exiting program...\n"; break;
            default: cout << "❌ Invalid option.\n";
        }
    } while (choice != 0);
}

void Manager::uploadFile() {
    string name;
    cout << "Enter filename to upload: ";
    cin >> name;
    
}

void Manager::encryptFile() {
    string name;
    cout << "Enter filename to encrypt: ";
    cin >> name;
    
}

void Manager::hashFile() {
    string name;
    cout << "Enter filename to hash: ";
    cin >> name;
    
}

void Manager::scanFile() {
    string name;
    cout << "Enter filename to scan: ";
    cin >> name;
    
}

void Manager::showLogs() {
    
}
//    End


//  Main for running the program

int main() {
    Manager manager;
    manager.run();
    return 0;
}
// g++ main.cpp Manager.cpp -Iinclude -o main
// ./main