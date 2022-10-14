/*
 *   stringTable.cpp
 *   Daniel Peng + Thomas Lai
 *   04/20/22
 *
 *   CS15 Project 3 : Gerp
 *   
 *   Implementation of the stringTable class, which represents a
 *   hashmap with containers, keys, and values. This implementation of the
 *   hashmap utilizes chaining in order to deal with duplicates.
 *
 */

#include "stringTable.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <istream>

using namespace std;

/*
 * name:      stringTable.cpp default constructor
 * purpose:   initializes a new stringTable/hashmap
 * arguments: none
 * returns:   none
 * effects:   initializes a new hashMap, with the table size set to 100.
 *            Also, initializes empty vectors within the array.
 */
stringTable::stringTable()
{
    currentTableSize = INITIAL_TABLE_SIZE;
    numItemsInTable = 0;
    chainedTable = new Container[currentTableSize];
    for (int i = 0; i < currentTableSize; i++) {
        vector<ContainerNode> empty;
        chainedTable[i].nodeList = empty;
    }
}

/*
 * name:      stringTable.cpp destructor
 * purpose:   recycles any heap-allocated memory
 * arguments: none
 * returns:   none
 * effects:   clears every vector inside the hashmap, and then deletes the
 *            array itself.
 */
stringTable::~stringTable()
{
    for (int i = 0; i < currentTableSize; i++) {
        chainedTable[i].nodeList.clear();
    }
    delete [] chainedTable;
}

/*
 * name:      stringTable.cpp insertChaining function
 * purpose:   inserts a key along with the value into the hashmap
 * arguments: a string, an integer representing the index of the filepath,
 *            and an integer representing the line number
 * returns:   none
 * effects:   hashes the lower-case value of the key, then inserts an entry
 *            into the hashtable.
 */
void stringTable::insertChaining(KeyType key, int path, int line)
{
    string copy = key; // copies the key for case-sensitive
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    int index = good_hash_function(key) % currentTableSize; // finds index

    Entry e(Location(path, line), copy); // creates an entry to be inserted

    if (chainedTable[index].nodeList.empty()) { // empty bucket case
        numItemsInTable++; // only in this case will we have to increment this
        ContainerNode n(key, e); // creates new ContainerNode
        chainedTable[index].nodeList.push_back(n);
    }
    else{ // iterate through nodeList to see if the key exists
        bool found = false;
        unsigned long i = 0;
        while(not found and i < chainedTable[index].nodeList.size()){ // exists
            if(key == chainedTable[index].nodeList.at(i).key){
                found = true;
                chainedTable[index].nodeList.at(i).entryList.push_back(e);
            }
            else{
                i++;
            }
        }
        if(not found){ // if it does not exist, create new ContainerNode
            ContainerNode n(key, e);
            chainedTable[index].nodeList.push_back(n);
        }
    }
    if (getLoadFactor() > .7) { // checks for load factor
        expand(); // expand if necessary
    }
}

/*
 * name:      stringTable.cpp good_hash_function
 * purpose:   hashes a key using std::hash
 * arguments: a key
 * returns:   a size_t representing the key after hashing
 * effects:   uses std::hash to hash a given string 'key'
 */
size_t stringTable::good_hash_function(KeyType key)
{
    return std::hash<KeyType>{}(key);
}

/*
 * name:      stringTable.cpp getLoadFactor
 * purpose:   gets the load factor of the current hash table
 * arguments: none
 * returns:   a float representing the load factor
 * effects:   gets the load factor by dividing the number of buckets with
 *            elements by the current table size.
 */
float stringTable::getLoadFactor()
{
    return (float) numItemsInTable / (float) currentTableSize;
}

/*
 * name:      stringTable.cpp expand function
 * purpose:   expands the current hash map, re-hashes every current entry
 * arguments: none
 * returns:   none
 * effects:   creates a new hashMap with double the current size, and
 *            re-hashes the current elements to the new map.
 */
void stringTable::expand()
{
    // Note: we can multiply by 2, because currentTableSize is never 0
    //       (see constructor)
    currentTableSize *= 2;

    // initializes a new table on the heap
    Container *newTable = new Container[currentTableSize];

    // loops through the original table
    for (int i = 0; i < currentTableSize/2; i++) {
        
        vector<ContainerNode> curr_list = chainedTable[i].nodeList;
        // if the current list is not empty, go through and re-hash
        if (not curr_list.empty()) {
            for(unsigned long j = 0; j < curr_list.size(); j++){
                int index = good_hash_function(curr_list[j].key) 
                                                % currentTableSize;
        // only push bash the ContainerNodes within the list, because each
        // ContainerNode has the same key
                newTable[index].nodeList.push_back(curr_list[j]);
            }
        } 
    }
    // delete old table
    delete [] chainedTable;
    chainedTable = newTable;
}

/*
 * name:      stringTable.cpp case_insensitive output function
 * purpose:   prints all the case insensitive outputs to an output file
 * arguments: a user's query, an output stream
 * returns:   none
 * effects:   prints the case insensitive version of the user's query from
 *            the hash map
 */
void stringTable::case_insensitive(std::string query, ostream &output)
{
    string copy = query; // copy, in case none is found
    transform(query.begin(), query.end(), query.begin(), ::tolower);

    int index = good_hash_function(query) % currentTableSize; // hashing lower
    Container *l = &chainedTable[index];

    bool exist = false;
    unsigned long i = 0;
    // Loop until the ContainerNode is found
    while(not exist and i < l->nodeList.size()){
        if (query == l->nodeList[i].key) { // if the key matches, then print

            exist = true;
            int path = (l->nodeList[i].entryList.at(0).value.pathIndex);
            ifstream file;
            string fpath = filePaths.at(path); // gets filePath
            open_or_die(file, fpath); // opens first file
            
            int curr_line = 0;
            string currentLine = "";
            print_case_insensitive(i, file, path, curr_line, currentLine, l,
                          output); // prints out everything from the vector 
        }
        i++;
    }
    if (not exist) { // Case insensitive search doesn't find anything
        output << copy << " Not Found." << endl;
    }
}

/*
 * name:      stringTable.cpp case_sensitive output function
 * purpose:   prints all the case sensitive outputs to an output file
 * arguments: a user's query, an output stream
 * returns:   none
 * effects:   prints the case sensitive version of the user's query from
 *            the hash map
 */
void stringTable::case_sensitive(std::string query, ostream &output)
{
    string copy = query; // copy the query, hash by lowercase
    transform(query.begin(), query.end(), query.begin(), ::tolower);
    int index = good_hash_function(query) % currentTableSize;
    Container *l = &chainedTable[index];

    bool exist = false;
    unsigned long i = 0;

    while(not exist and i < l->nodeList.size()){ // loop until node is found
        if (query == l->nodeList[i].key) {

            exist = true;
            int path = (l->nodeList[i].entryList.at(0).value.pathIndex);
            ifstream file;
            string fpath = filePaths.at(path);
            open_or_die(file, fpath);
            
            int curr_line = 0;
            string currentLine = "";
            print_case_sensitive(copy, i, file, path, curr_line, currentLine,
                                l, output); // case sensitive print helper
        }
        i++;
    }
    if(not exist) { // if not found, then output
        output << copy << " Not Found. Try with @insensitive or @i." << endl;
    }
}

/*
 * name:      stringTable.cpp print_case_insensitive helper function
 * purpose:   helps prints all the case insensitive outputs to an output file
 * arguments: integer index (index of current ContainerNode), 
 *            filestream (opened file), integer path (index for filePath), 
 *            integer representing line number, string representing the file's
 *            line, a pointer to a cointainer, and the output file
 *
 * returns:   none
 * effects:   prints the case insensitive version of the user's query from
 *            the hash map
 */
void stringTable::print_case_insensitive(int index, ifstream &file, int path,
                                int line_num, string currentLine, Container *c,
                                ostream &output)
{
    ContainerNode *curr_container = &(c->nodeList[index]);
    // looping through container's entryList
    for(unsigned long j = 0; j < curr_container->entryList.size(); j++){
        // if the entry contains a new file index
        if(curr_container->entryList.at(j).value.pathIndex != path){
            path = (curr_container->entryList.at(j).value.pathIndex);
            file.close(); // close current ifstream
            file.clear();
            string fpath = filePaths.at(path);
            open_or_die(file, fpath); // open new ifstream to read from
            line_num = 0;
        }
        int line = (curr_container->entryList.at(j).value.lineNum);
        int num = line;
        if(line_num != num){ // while current line number is not the same
            while (num != line_num) {
                getline(file, currentLine); // get the new line from file
                num = num - 1;
            }
            line_num = line; // in order to deal with duplicates
                             // prevents same line from running twice
            output << filePaths.at(path) << ":" << line << ": ";
            output << currentLine << endl;
        }
    }
}

/*
 * name:      stringTable.cpp print_case_sensitive helper function
 * purpose:   helps prints all the case sensitive outputs to an output file
 * arguments: user query, integer index (index of current ContainerNode), 
 *            filestream (opened file), integer path (index for filePath), 
 *            integer representing line number, string representing the file's
 *            line, a pointer to a cointainer, and the output file
 *
 * returns:   none
 * effects:   prints the case sensitive version of the user's query from
 *            the hash map
 */
void stringTable::print_case_sensitive(string query, int index, ifstream &file,
                                int path, int line_num, string currentLine,
                                Container *c, ostream &output)
{
    ContainerNode *curr_container = &(c->nodeList[index]);
    int copy_num = 0;
    bool exist = false;
    // loops through the entryLists containing the key
    for(unsigned long j = 0; j < curr_container->entryList.size(); j++) {
        // if the path index is different, a new file must be opened
        if(curr_container->entryList.at(j).value.pathIndex != path){
            path = (curr_container->entryList.at(j).value.pathIndex);
            file.close(); // close ifstream
            file.clear();
            string fpath = filePaths.at(path);
            open_or_die(file, fpath); // open new file
            line_num = 0; // reset current line number to 0
        }
        int line = (curr_container->entryList.at(j).value.lineNum);
        int num = line;
        if(curr_container->entryList.at(j).key_case == query
            and copy_num != num) { // if the key_case is the same as the query
            exist = true;          // and the line is not the same (duplicate)
            if(line_num != num){
                while (num != line_num) {
                    getline(file, currentLine);
                    num = num - 1;
                }
                line_num = line; // prevents the same line from outputting
                copy_num = line;
            }
            output << filePaths.at(path) << ":" << line << ": ";
            output << currentLine << endl;
        }
    }
    if(not exist){
        output << query << " Not Found. Try with @insensitive or @i." << endl;

    }
}

/*
 * name:      stringTable.cpp open_or_die function
 * purpose:   opens a file with any type of stream
 * arguments: a stream of streamType, and a file name
 * returns:   none
 * effects:   opens a file with a given type of stream
 */
template<typename streamtype>
void stringTable::open_or_die(streamtype &stream, string file_name)
{
    stream.open(file_name);
    if (not stream.is_open()) {
        cerr << "Unable to open: " << file_name << endl;
        exit(EXIT_FAILURE);
    }
}

/*
 * name:      stringTable.cpp file_at function
 * purpose:   gets a file path given an index to the filePath array
 * arguments: an integer index representing the path index
 * returns:   a string representing the file path
 * effects:   gets the file path string from the filePath array
 */
string stringTable::file_at(int index)
{
    return filePaths.at(index);
}

/*
 * name:      stringTable.cpp file_path_size function
 * purpose:   gets the size of the filePath array
 * arguments: none
 * returns:   an integer representing the filePath array's size
 * effects:   none
 */
int stringTable::file_path_size()
{
    return filePaths.size();
}

/*
 * name:      stringTable.cpp add_path_name function
 * purpose:   adds a file path string to the filePath array
 * arguments: a reference to a file path string
 * returns:   none
 * effects:   adds a new file path string to the filePath array
 */
void stringTable::add_path_name(string &pathName)
{
    filePaths.push_back(pathName);
}