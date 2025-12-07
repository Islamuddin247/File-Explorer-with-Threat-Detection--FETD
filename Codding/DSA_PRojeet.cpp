#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <limits> // added for safe cin.ignore usage
using namespace std;

class LogNode
{ 
public:
    string message;
    LogNode *next;
    LogNode(const string &msg) : message(msg), next(nullptr) {}
};

struct BSTNode {
    string fileName;
    BSTNode *left;
    BSTNode *right;
    BSTNode(string name) : fileName(name), left(nullptr), right(nullptr) {}
};

class FileSearchTree {
    BSTNode* root;

    // Recursive Insert: Places file in correct sorted position
    BSTNode* insert(BSTNode* node, string name) {
        if (!node) return new BSTNode(name);
        if (name < node->fileName)
            node->left = insert(node->left, name);
        else if (name > node->fileName)
            node->right = insert(node->right, name);
        return node; // Return unchanged node pointer
    }

    // Recursive Search: Finds file efficiently
    bool search(BSTNode* node, string name) {
        if (!node) return false;
        if (node->fileName == name) return true;
        
        if (name < node->fileName) 
            return search(node->left, name);
        else 
            return search(node->right, name);
    }

    // Recursive In-Order Traversal: Prints files A-Z
    void inOrder(BSTNode* node) {
        if (!node) return;
        inOrder(node->left);
        cout << "  - " << node->fileName << "\n";
        inOrder(node->right);
    }

    // Helper: Find minimum value node (for deletion logic)
    BSTNode* minValueNode(BSTNode* node) {
        BSTNode* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    // Recursive Delete: Removes file and re-links the tree
    BSTNode* deleteNode(BSTNode* root, string name) {
        if (!root) return root;

        if (name < root->fileName)
            root->left = deleteNode(root->left, name);
        else if (name > root->fileName)
            root->right = deleteNode(root->right, name);
        else {
            // Node with only one child or no child
            if (!root->left) {
                BSTNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                BSTNode* temp = root->left;
                delete root;
                return temp;
            }
            // Node with two children: Get the inorder successor
            BSTNode* temp = minValueNode(root->right);
            root->fileName = temp->fileName; // Copy content
            root->right = deleteNode(root->right, temp->fileName); // Delete duplicate
        }
        return root;
    }

public:
    FileSearchTree() : root(nullptr) {}

    void addFile(string name) {
        root = insert(root, name);
    }

    bool findFile(string name) {
        return search(root, name);
    }

    void removeFile(string name) {
        root = deleteNode(root, name);
    }

    void showSortedFiles() {
        if (!root) {
            cout << "  (Index is empty)\n";
            return;
        }
        cout << "\n --- Files (Alphabetical Order via Tree) ---\n";
        inOrder(root);
        cout << " -------------------------------------------\n";
    }
};

class Stack
{
public:
    LogNode *histroy; // as a Head
    Stack()
    {
        histroy = nullptr;
    }
    void push(string data)
    {
        LogNode *tem = new LogNode(data);
        if (!histroy)
        {
            histroy = tem;
        }
        else
        {
            tem->next = histroy;
            histroy = tem;
        }
    }
    void pop()
    {
        if (!histroy)
        {
            cout << "No Encrypted File : " << endl;
            return;
        }
        LogNode *tem = histroy;
        histroy = histroy->next;

        cout << "Popped From Stack " << tem->message << endl;
        delete tem;
    }
    string Top()
    {
        if (!histroy)
        {
            cout << "No Encrypted File : " << endl;
            return "-1";
        }
        else
        {
            return histroy->message;
        }
    }
    void Delete(string name)
    {
        //  only attempt to delete file on disk if it actually exists
        ifstream checkFile(name);
        if (checkFile.is_open())
        {
            checkFile.close();
            if (remove(name.c_str()) == 0)
            {
                cout << " File deleted successfully: " << name << "\n";
            }
            else
            {
                cout << " Error deleting file: " << name << "\n";
            }
        }

        LogNode *tem = histroy;
        LogNode *prev = nullptr;
        bool find = false;
        while (tem)
        {
            if (tem->message == name)
            {
                find = true;
                break;
            }
            prev = tem;
            tem = tem->next;
        }
        if (find)
        {
            if (prev)
            {

                prev->next = tem->next;
                delete tem;
            }
            else
            {
                histroy = tem->next;
                delete tem;
            }
            cout << "Removed from stack " << endl;
        }
        else
        {
            cout << "Not Encrypted !! " << endl;
        }
    }
    bool FindFile(string name, int x)
    {

        LogNode *tem = histroy;
        while (tem)
        {
            if (tem->message == name)
            {
                return true;
            }
            tem = tem->next;
        }
        return false;
    }

    void ShowEncryptedFile(){
        if (!histroy)
        {
            cout<<"NO Encrypted file !!"<<endl;
            return;
        }
        LogNode* tem=histroy;
        cout<<"|n========Encrypted FIles============"<<endl;
        while (tem !=nullptr)
        {
            cout<<tem->message<<endl;
            tem=tem->next;
        }
        
        
    }
};

// ===================== FileHandler =====================
class FileHandler
{
    string currentFile;
    vector<string> fileData;
    queue<string> FilesQueues;
    Stack history;
    FileSearchTree treeIndex;
public:
    // kept signatures similar; changed to const ref for efficiency
    bool uploadFile(const string &filename);
    void AddToQueue(const string &filename);

    void displayfile() const;
    vector<string> getFileData() const;
    string getCurrentFile() const;
    void CreatFile(const string &filename);//  creates the new txt file
    void DeleteFile(const string &filename); //     the file from queue and tree
    void encryption(const string &filename);
    void AddToStack(const string &filename);// Stores the encrypted files
    void ShowEcryptedFiles();
    void DecryptedFile(const string &filename);
    void searchInVault(string name);
    void showDirectory();// showes the Sorted files from the tree

    // ... inside FileHandler public section ...

    string generateHash(const string &filename) {
        ifstream file(filename);
        if (!file.is_open()) return ""; 

        string allHashes = ""; // Variable to hold the final multiline string
        string line;

        // 1. Read file Line-by-Line
        while (getline(file, line)) {
            
            // Reset hash calculation for this specific line
            unsigned long long hash = 5381; 
            for (char c : line) {
                hash = ((hash << 5) + hash) + c; 
            }

            // 2. Manual Hex Conversion (Moved INSIDE loop)
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

            // 3. Add this line's hash and a Newline to the result
            allHashes += hexResult + "\n";
        }

        file.close();
        return allHashes; // Returns a long string with all hashes separated by Enters
    }

    void saveHashFile(string originalName, string hashString) {
        string newFileName = originalName + ".hash";
        ofstream file(newFileName);
        
        if (file.is_open()) {
            // hashString already contains "\n" from the function above
            // so this will save neatly on separate lines.
            file << hashString; 
            file.close();
            cout << " Hash saved to file: " << newFileName << "\n";
            
            // Add to your system
            treeIndex.addFile(newFileName);
            AddToQueue(newFileName); 
        } else {
            cout << " Error creating hash file.\n";
        }
    }
};
void FileHandler::ShowEcryptedFiles(){
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
void FileHandler::CreatFile(const string &filename)
{
    ofstream NewFile(filename, ios::out);
    if (!NewFile)
    {
        cout << " Error creating file " << filename << "\n";
        return;
    }
    cout << " File " << filename << " created successfully.\n";

    // !!! ADD TO TREE !!!
    treeIndex.addFile(filename);

    int choice;
    cout << " Do you want to add content to the file now? (1 for Yes / 0 for No): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // make ignore robust
    if (choice == 1)
    {
        string line;
        cout << " Enter content line by line. Type 'END' on a new line to finish:\n";
        while (true)
        {
            getline(cin, line);
            if (line == "END" || line == "end")
                break;
            NewFile << line << "\n";
        }
        cout << " Content added to " << filename << "\n";
    }
    NewFile.close();

    //  Add the newly created file to queue
    // AddToQueue(filename);
}

void FileHandler::encryption(const string &filename)
{
    queue<string> tem;
    bool check = false;

    while (!FilesQueues.empty())
    {
        if (FilesQueues.front() == filename)
        {
            check = true;
        }
        tem.push(FilesQueues.front());
        FilesQueues.pop();
    }

    FilesQueues = tem;
    if (!check)
    {
        cout << "File not in queue." << endl;
        return;
    }
    string key;
    string encrypted = "Encrypted" + filename;
    if (check)
    {
        ifstream InFile(filename);
        if (!InFile.is_open())
        {
            cout << "Error opening " << filename << endl;
            return;
        }

        ofstream Efile(encrypted, ios::out);
        if (!Efile.is_open())
        {
            cout << "Error creating encrypted file: " << encrypted << endl;
            InFile.close();
            return;
        }

        cout << "Enter the Key : ";
        cin >> key;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //  consume newline

        string line;
        while (getline(InFile, line))
        {
            string cipher;
            int i = 0;
            for (char c : line)
            {
                if (isalpha(static_cast<unsigned char>(c)))
                {
                    char p = toupper(static_cast<unsigned char>(c));
                    char k = toupper(static_cast<unsigned char>(key[i % key.size()]));
                    char enc = ((p - 'A') + (k - 'A')) % 26 + 'A';
                    cipher.push_back(enc);
                    i++;
                }
                else
                    cipher.push_back(c);
            }
            Efile << cipher << endl;
        }

        // ensure files are closed before deleting original
        InFile.close();
        Efile.close();

        AddToStack(encrypted);
        AddToStack(key);

        // Delete original file from disk, queue AND TREE
        DeleteFile(filename);
    }
}
void FileHandler::DecryptedFile(const string &filename)
{
    string Fname = "Encrypted" + filename;
    if (history.FindFile(Fname, 1))
    {
        string key;
        cout << "Enter the Key : ";
        cin >> key;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (history.FindFile(key, 2))
        {
            ifstream Infile(Fname);
            if (!Infile.is_open())
            {
                cout << " Error opening: " << Fname << "\n";
                return;
            }
            ofstream WriteFile(filename, ios::out);
            if (!WriteFile.is_open())
            {
                cout << " Error creating output file: " << filename << "\n";
                Infile.close();
                return;
            }
            string cipher;

            while (getline(Infile, cipher))
            {
                string plain;
                int i = 0;
                for (char c : cipher)
                {
                    if (isalpha(static_cast<unsigned char>(c)))
                    {
                        char e = toupper(static_cast<unsigned char>(c));
                        char k = toupper(static_cast<unsigned char>(key[i % key.size()]));
                        char dec = ((e - 'A') - (k - 'A') + 26) % 26 + 'A';
                        plain.push_back(dec);
                        i++;
                    }
                    else
                        plain.push_back(c);
                }
                WriteFile << plain << endl;
            }
            Infile.close();
            WriteFile.close();

            AddToQueue(filename);
            // !!! Re-add decrypted file to Tree !!!
            treeIndex.addFile(filename);

            // Delete only the encrypted file and the key from stack
            history.Delete(Fname);
            history.Delete(key);
        }
        else
        {
            cout << "Key in not Correct " << endl;
        }
    }
    else
    {
        cout << "File is not Encrypted " << endl;
    }
}
void FileHandler::DeleteFile(const string &filename)
{
    bool deleted = false;

    //  Delete file from disk only if it exists
    ifstream check(filename);
    if (check.is_open())
    {
        check.close();
        if (remove(filename.c_str()) != 0)
        {
            cout << " Error deleting file: " << filename << "\n";
        }
        else
        {
            cout << " File deleted successfully: " << filename << "\n";
            deleted = true;
            treeIndex.removeFile(filename);
            cout << " [Tree] Removed from Search Index.\n";
        }
    }

    //  Remove file from queue
    queue<string> temp;
    bool found = false;

    while (!FilesQueues.empty())
    {
        if (FilesQueues.front() == filename)
        {
            found = true; // skip this file
        }
        else
        {
            temp.push(FilesQueues.front());
        }
        FilesQueues.pop();
    }
    FilesQueues = temp;

    if (found)
        cout << "  Removed from queue: " << filename << "\n";
    else
        cout << "  File not found in queue: " << filename << "\n";

    //  Optional: message summary
    if (deleted && found)
        cout << " File '" << filename << "' deleted and dequeued successfully.\n";
}
bool FileHandler::uploadFile(const string &filename)
{
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cout << " Error: File not found: " << filename << "\n";
        return false;
    }

    currentFile = filename;

    fileData.clear();

    string line;
    while (getline(fin, line))
    {
        fileData.push_back(line);
    }
    fin.close();

    cout << " File loaded: " << filename << "\n";
// !!! ADD TO TREE !!!
    treeIndex.addFile(filename);

    return true;
}

void FileHandler::AddToQueue(const string &filename)
{
    FilesQueues.push(filename);
    cout << "  Queued: " << filename << "\n";
}
void FileHandler::AddToStack(const string &filename)
{
    history.push(filename);
    cout << " Stacked : " << filename << endl;
}

void FileHandler::displayfile() const
{
    if (fileData.empty())
    {
        cout << "  No file loaded to display.\n";
        return;
    }
    cout << "\n File: " << currentFile << "\n--------------------------------\n";
    for (const string &line : fileData)
    {
        cout << line << "\n";
    }
    cout << "--------------------------------\n";
}

vector<string> FileHandler::getFileData() const
{
    return fileData;
}

string FileHandler::getCurrentFile() const
{
    return currentFile;
}

// ===================== LogNode & Logger =====================

class Logger
{
    LogNode *head;

public:
    Logger();
    ~Logger();
    void logEvent(const string &message);
    void showLogs() const;
};

Logger::Logger() : head(nullptr) {}
Logger::~Logger()
{
    LogNode *tem;
    while (head != nullptr)
    {
        tem = head;
        head = head->next;
        delete tem;
    }
}

void Logger::logEvent(const string &message)
{
    LogNode *newNode = new LogNode(message);
    newNode->next = head;
    head = newNode;
}

void Logger::showLogs() const
{
    if (!head)
    {
        cout << " No logs yet.\n";
        return;
    }
    cout << "\n ********* ACTION LOGS ***********\n";
    LogNode *tem = head;
    while (tem != nullptr)
    {
        cout << " " << tem->message << "\n";
        tem = tem->next;
    }
    cout << " ********************************\n";
}

// ===================== Manager =====================
class Manager
{
public:
    void run();

private:
    Logger log;
    FileHandler fl;
    vector<string> threatSignatures;

    void uploadFile();
    void encryptFile();
    void hashFile();
    void scanFile();
    void showLogs();
    void FileMenu();
    void LoadSignatures();
};

void Manager::run()
{
    int choice;
    do
    {
        cout << "\n========== Secure File Vault ==========" << "\n";
        cout << "1.  Upload File\n";
        cout << "2.  Encrypt/Decrypt File (placeholder)\n";
        cout << "3.  Generate Hash (placeholder)\n";
        cout << "4.  Scan File for Malware (placeholder)\n";
        cout << "5.  View Logs\n";
        cout << "6.  File Menu\n";
        cout << "7.  Search File (Binary Tree)\n";
        cout << "8.  List All Files (Sorted A-Z)\n";
        cout << "0.  Exit\n";
        cout << "======================================\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            uploadFile();
            break;
        case 2:
            encryptFile();
            break;
        case 3:
            hashFile();
            break;
        case 4:
            scanFile();
            break;
        case 5:
            showLogs();
            break;
        case 6:
            FileMenu();
            break;
        case 7: {
            string name;
            cout << " Enter name to search: "; cin >> name;
            fl.searchInVault(name);
            break;
        }
        case 8:
            fl.showDirectory();
            break;
        case 0:
            cout << " Exiting program...\n";
            break;
        default:
            cout << " Invalid option.\n";
        }
    } while (choice != 0);
}

void Manager::LoadSignatures()
{
    ifstream fin("virus_signatures.txt", ios::in);
    if (!fin.is_open())
    {
        cout << "\n Warning: virus_signatures.txt is not loaded" << endl;
        return;
    }

    // Clear previous signatures if any, to avoid duplicates if called twice
    threatSignatures.clear();

    string line;
    while (getline(fin, line))
    {
        if (!line.empty()) // Avoid adding empty lines
            threatSignatures.push_back(line);
    }
    fin.close();
    cout << "Threat database updated. " << threatSignatures.size() << " signatures loaded.\n";
}

void Manager::uploadFile()
{
    string name;
    cout << "Enter filename to upload: ";
    cin >> name;

    if (fl.uploadFile(name))
    {
        log.logEvent(" Uploaded: " + name);
        fl.AddToQueue(name); // keep queue usage visible (optional)
    }
    else
    {
        log.logEvent(" Failed upload: " + name);
        int retry;
        cout << "1. Do you want to retry\n2. Create a New file with this name  \n0 else exit\nEnter your choice: ";
        cin >> retry;
        if (retry == 1)
        {
            uploadFile();
        }
        else if (retry == 2)
        {
            fl.CreatFile(name);
            if (fl.uploadFile(name))
            {
                log.logEvent(" Uploaded: " + name);
                fl.AddToQueue(name); // keep queue usage visible (optional)
            }
        }
    }
}

void Manager::encryptFile()
{
    int option;
    cout << "Enter Option \n1 Encyption  \n2 Decryption : ";
    cin >> option;
    if (option == 1)
    {
        string name;
        cout << "Enter filename to encrypt: ";
        cin >> name;

        fl.encryption(name);
        cout << " (placeholder) Encrypting " << name << " ... Done.\n";
        log.logEvent(" Encrypted: " + name);
    }
    else if (option == 2)
    {
        string name;
        cout << "Enter filename to Dencrypt: ";
        cin >> name;

        fl.DecryptedFile(name);
        cout << " (placeholder) Dencrypting " << name << " ... Done.\n";
        log.logEvent(" Dencrypted: " + name);
    }
}

void Manager::hashFile()
{
    string name;
    cout << "Enter filename to hash: ";
    cin >> name;
    
    // Check if file exists *before* starting the hash process
    ifstream check(name);
    if (!check.is_open())
    {
        cout << " Error: File not found: " << name << "\n";
        log.logEvent(" Hash failed (not found): " + name);
        return;
    }
    check.close();

    // 1. Calculate the line-by-line hashes
    // The generateHash function now returns a single string containing all hashes separated by newlines.
    string hashString = fl.generateHash(name);

    // If the file was empty, generateHash might return an empty string.
    if (hashString.empty()) {
        cout << " File is empty or hash generation failed.\n";
        log.logEvent(" Hashed (empty file): " + name);
        return;
    }

    // 2. Save the multi-line hash string to disk and register it in the system
    fl.saveHashFile(name, hashString);

    cout << " Successfully created and saved line-by-line hash for " << name << "\n";
    log.logEvent(" Hashed (Line-by-Line) and Saved: " + name);
}

void Manager::scanFile()
{
    LoadSignatures();
    string name;
    cout << "Enter filename to scan: ";
    cin >> name;

    ifstream fin(name);
    if (!fin.is_open())
    {
        cout << " Error: File not found: " << name << "\n";
        log.logEvent(" Scan failed (not found): " + name);
        return;
    }

    // lightweight placeholder scan: check for suspicious keywords (very simple)
    string line;
    bool suspicious = false;
    while (getline(fin, line))
    {
        //String Lowercase Conversion Block
        // ==========================================
        for (char &c : line) {
            c = tolower(c);
        }

        for (const string &sig : threatSignatures)
        {
            if (line.find(sig) != string::npos)
            {
                cout << " [ALERT] Threat detected: " << sig << " on line " << endl;
                suspicious = true;
            }
        }
    }
    fin.close();

    if (suspicious)
    {
        cout << "  Scan result for " << name << ": Suspicious content found!\n";
        log.logEvent(" Scanned (suspicious): " + name);
    }
    else
    {
        cout << " Scan result for " << name << ": No threats detected.\n";
        log.logEvent(" Scanned: " + name + " (no threats)");
    }
}

void Manager::showLogs()
{
    log.showLogs();
}

void Manager::FileMenu()
{
    int choice;
    do
    {
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
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            fl.displayfile();
            break;

        case 2:
        {
            string cur = fl.getCurrentFile();
            if (cur.empty())
            {
                cout << "  No current file loaded.\n";
            }
            else
            {
                cout << " Current file: " << cur << "\n";
            }
            break;
        }

        case 3:
        {
            vector<string> data = fl.getFileData();
            if (data.empty())
            {
                cout << "  No file data loaded.\n";
            }
            else
            {
                cout << " File has " << data.size() << " line(s).\n";
                // show up to first 10 lines for convenience
                size_t limit = min<size_t>(data.size(), 10);
                cout << "Showing up to first " << limit << " line(s):\n";
                for (size_t i = 0; i < limit; ++i)
                {
                    cout << i + 1 << ": " << data[i] << "\n";
                }
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
        case 6:
        {
            fl.ShowEcryptedFiles();
            log.logEvent("Encryted Files Showed");
        }

        case 0:
            cout << " Returning to main menu...\n";
            break;

        default:
            cout << " Invalid option.\n";
        }

    } while (choice != 0);
}

// ===================== main =====================
int main()
{
    Manager manager;
    manager.run();
    return 0;
}
