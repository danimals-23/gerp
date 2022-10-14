/*
 *   gerp.cpp
 *   Daniel Peng + Thomas Lai
 *   04/20/22
 *
 *   CS15 Project 3 : Gerp
 *   
 *   Implementation of the gerp class, which is based off of the linux -grep
 *   command. This class allows users to search for all occurrences of
 *   a given word in every file within a directory.
 *   
 */

#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <ostream>
#include "gerp.h"
#include "stringProcessing.h"


using namespace std;

/*
 * name:      gerp.cpp default constructor
 * purpose:   initializes a gerp object
 * arguments: none
 * returns:   none
 * effects:   none
 */
Gerp::Gerp() {

}

/*
 * name:      gerp.cpp destructor
 * purpose:   recycles all heap-allocated memory
 * arguments: none
 * returns:   none
 * effects:   none
 */
Gerp::~Gerp() {

}

/*
 * name:      gerp.cpp default constructor
 * purpose:   initializes a gerp object
 * arguments: a string for input directory, a output file
 * returns:   none
 * effects:   opens the output file to be printed to, and creates the
 *            hash map with the input directory
 */
Gerp::Gerp(std::string inputDirectory, std::string outputFile) {
    open_or_die(output, outputFile);
    runTraversal(inputDirectory);
}

/*
 * name:      gerp.cpp run function
 * purpose:   runs the command loop using a helper function
 * arguments: none
 * returns:   none
 * effects:   writes to cerr once the helper function ends
 */
void Gerp::run() {
    runHelper(cin);
    cout << "Goodbye! Thank you and have a nice day." << endl;
}

/*
 * name:      gerp.cpp runHelper function
 * purpose:   runs the command loop which query's the user
 * arguments: an input stream
 * returns:   none
 * effects:   depending on user query, writes to an output file
 */
void Gerp::runHelper(std::istream &input) {
    string str = "";
    string userQuery = "";
    string newStr = "";
    string outputFile = "";

    while (not input.fail() and (str != "@q" and str != "@quit")) {
        cout << "Query? ";
        input >> str;

        if (str == "@i" or str == "@insensitive") {
            input >> userQuery;
            newStr = stripNonAlphaNum(userQuery);
            if(newStr != ""){
                hashMap.case_insensitive(newStr, output);
            }
            else{
                output << " Not Found." << endl;
            }

        }
        else if (str == "@f") {
            input >> userQuery;
            output.close();
            open_or_die(output, userQuery);

        }
        else if (str != "@q" and str != "@quit") {
            newStr = stripNonAlphaNum(str);
            if(newStr != ""){
                hashMap.case_sensitive(newStr, output);
            }
            else{
                output << " Not Found. Try with @insensitive or @i." << endl;
            }
        }
    }
}

/*
 * name:      gerp.cpp runTraversal function
 * purpose:   traverses through a directory and uses a helper function
 *            to traverse through the files
 * arguments: a directory string
 * returns:   none
 * effects:   traverses through a directory, populating the hash table
 */
void Gerp::runTraversal(string directory) {
    FSTree myTree(directory);
    DirNode *root = myTree.getRoot();
    runTraversalHelper(root);
}

/*
 * name:      gerp.cpp runTraversalHelper function
 * purpose:   recurses through a directory and hashes a file if found
 * arguments: a DirNode
 * returns:   none
 * effects:   helper function that recursively finds files in a directory
 *            and hashes it
 */
void Gerp::runTraversalHelper(DirNode *node) {
    // Base case 
    if (node == nullptr or node->isEmpty()) {
        return;
    }

    // If a subdirectory is found, then traverse through each
    if (node->hasSubDir()) {
        int n = node->numSubDirs();
        for (int i = 0; i < n; i++) {
            runTraversalHelper(node->getSubDir(i));
        }
    }

    // If there is a file in the current directory, then traverse through
    // each file and hash each file
    if (node->hasFiles()) {
        int n = node->numFiles();
        for (int i = 0; i < n; i++) {
            string path = print_path(node, "") + node->getFile(i);
            hashFile(path);
        }
    }
    
    return;
}

/*
 * name:      gerp.cpp print_path function
 * purpose:   prints the entire file path of a file, including itself
 * arguments: a DirNode and a path name as a string
 * returns:   the string of the entire path
 * effects:   recursively prints the entire filepath
 */
string Gerp::print_path(DirNode *node, string pathName) 
{
    // base case, if node is nullptr
    if (node == nullptr) {
        return "";
    }

    // Recursively creates pathName until the root is found
    pathName = print_path(node->getParent(), pathName);
    // append the name of the current node
    pathName += node->getName() + '/';
    return pathName;
}

/*
 * name:      gerp.cpp open_or_die function
 * purpose:   opens a file with any type of stream
 * arguments: a stream of streamType, and a file name
 * returns:   none
 * effects:   opens a file with a given type of stream
 */
template<typename streamtype>
void Gerp::open_or_die(streamtype &stream, string file_name)
{
    stream.open(file_name);
    if (not stream.is_open()) {
        cerr << "Unable to open: " << file_name << endl;
        exit(EXIT_FAILURE);
    }
}

/*
 * name:      gerp.cpp hashFile function
 * purpose:   hashes every word inside a file to the hashMap
 * arguments: a string representing the file path name
 * returns:   none
 * effects:   hashes the contents of a file into the hashMap
 */

void Gerp::hashFile(string pathName) {
    ifstream input; 
    open_or_die(input, pathName); // opening input file

    hashMap.add_path_name(pathName); // adds the path to filePath array
    int path_loc = hashMap.file_path_size() - 1; // gets the index of it

    string currentLine;
    int line = 1; // lines start at 1
    while(getline(input, currentLine)) { // while we are getting a line
        hashLine(path_loc, currentLine, line); // hash the current line
        line++; // increment line number
    }
    
    input.close(); // close file
}

/*
 * name:      gerp.cpp hashLine function
 * purpose:   hashes every single word of a given line
 * arguments: the index of the file path, the entire string, the lineNum
 * returns:   none
 * effects:   hashes every single word of a given line
 */

void Gerp::hashLine(int pathNum, string stringName, int lineNum) {
    stringstream ss(stringName); // create stringstream of entire line
    string word;
    while(ss >> word) { // read in the stream word-by-word
        string newWord = stripNonAlphaNum(word); // strip the word
        
        hashMap.insertChaining(newWord, pathNum, lineNum); // insert into map
    }
}


