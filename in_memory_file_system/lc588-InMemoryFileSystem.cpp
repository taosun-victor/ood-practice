#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Entry {
private:
    string name;
public:
    Entry(const string& name){
        this->name = name;
    }
    string getName(){
        return name;
    }
    virtual bool isDirectory() = 0;    
    virtual vector<string> ls() = 0;
};

class File : public Entry {
private:
    string content;
public:
    File(string name) : Entry(name){
        this->content = "";
    }
    
    bool isDirectory() override {
        return false;
    }
    
    vector<string> ls()override {
        return {getName()};
    }
    
    string readContent(){
        return this->content;
    }
    
    void appendContent(string newContent){
        this->content.append(newContent);
    }
};

class Directory : public Entry {
private:
    map<string, Entry*> children;
public:
    Directory(string name) : Entry(name) {}
    
    bool isDirectory() override {
        return true;
    }
    
    vector<string> ls() override{
        vector<string> res;
        for (auto c : children)
            res.push_back(c.first);
        return res;        
    }
    
    Entry* findChild(string childName) {
        if (children.count(childName))
            return children[childName];
        return NULL;
    }
    
    void addChild(Entry* child){
        children[child->getName()] = child;
    }
    
    void search(vector<string>& res){
    	if (this->isDirectory() == false){
    		res.push_back(this->getName());
    		return;
		}
		
		// some problems here ...
		// when the child node of current node is a File node,
		// casting it to Directory* is not compatible
		// However, the dev C++ compiler looks OK with this
		// while the C++ compiler on Leetcode will abort due to error
		for (auto m : ((Directory*)this)->children)
			((Directory*)m.second)->search(res);
	}
};

class FileSystem {
private:
    Directory* root;
    
    vector<string> tokenize(const string& path){
        istringstream iss(path);
        string token;
        vector<string> dirs;
        
        while(getline(iss, token, '/'))
            dirs.push_back(token);
        return dirs;
    }
    
    Entry* findEntry(vector<string>& dirs){
        Entry* curr = root;
        for (string dir : dirs){
            if (((Directory*)curr)->findChild(dir) != NULL)
                curr = ((Directory*)curr)->findChild(dir);
        }
        return curr;
    }
    
public:
    FileSystem() {
        root = new Directory("Root");
    }
    
    vector<string> ls(string path) {
        vector<string> dirs = tokenize(path);
        return findEntry(dirs)->ls();
    }
    
    void mkdir(string path) {
        vector<string> dirs = tokenize(path);
        Entry* curr = root;
        for (string& dir : dirs){
            if (((Directory*)curr)->findChild(dir) == NULL){
                ((Directory*)curr)->addChild(new Directory(dir));
            }
            curr = ((Directory*)curr)->findChild(dir);
        }
    }
    
    void addContentToFile(string filePath, string content) {
        vector<string> dirs = tokenize(filePath);
        string fileName = dirs.back();
        dirs.pop_back();
        
        Entry* curr = findEntry(dirs);
        
        if (((Directory*)curr)->findChild(fileName) == NULL)
            ((Directory*)curr)->addChild(new File(fileName));
        curr = ((Directory*)curr)->findChild(fileName);
        ((File*)curr)->appendContent(content);
    }
    
    string readContentFromFile(string filePath) {
        vector<string> dirs = tokenize(filePath);
        Entry* curr = findEntry(dirs);
        
        if (curr == NULL)
            return "";
        return ((File*)curr)->readContent();
    }
    
    vector<string> searchFiles(){
    	vector<string> res;
    	root->search(res);
    	return res;
	}
};


int main() {
	FileSystem fs;
	
	vector<string> allEntries = fs.ls("/");
	for (const string& str : allEntries)
		cout << str << ", ";
	cout << endl;
	
	fs.mkdir("/a/b/c");
	fs.addContentToFile("/a/b/c/test.txt", "Hello! This is a test.");
	
	fs.mkdir("/a/node/tree");
	fs.addContentToFile("/a/hw.out", "Hello World");
	
	allEntries = fs.ls("/a");
	for (const string& str : allEntries)
		cout << str << ", ";
	cout << endl;
	
	cout << fs.readContentFromFile("/a/b/c/test.txt") << endl;
	cout << fs.readContentFromFile("/a/hw.out") << endl;
	
	cout << "----------------- List All Files ----------------" << endl;
	vector<string> allFiles = fs.searchFiles();
	for (string file : allFiles)
		cout << file << ", ";
	cout << endl;
	
	return 0;
}


