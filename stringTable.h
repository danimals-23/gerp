/*
 *   stringTable.h
 *   Daniel Peng + Thomas Lai
 *   04/20/22
 *
 *   CS15 Project 3 : Gerp
 *   
 *   Interface for the stringTable class, which represents a hash
 *   map. This data structure utilizes chaining for duplicates, and stores
 *   and hashes keys (words) along with their values (line number and path
 *   of file).
 *
 */

#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include <string>
#include <vector>


//
// Location struct, which is our 'value'. This struct contains the index of
// the filepath, along with the line number.
//
struct Location {

        //value
        int pathIndex;
        int lineNum;

        // Default constructor for Location struct
        Location() 
        {
            pathIndex = -1;
            lineNum = -1; 
        }

        // Constructor for location which takes in two integers
        Location(int path, int line)
        {
                pathIndex = path;
                lineNum = line;
        }
};

// Definitions for the key and value types:
// Our KeyType is a string, which represents a single word.
// Our ValueType is of type Location, which is defined above.
typedef std::string   KeyType;
typedef Location      ValueType;

// Class definition
class stringTable
{
public:
    // constructors and destructors
	stringTable();
	~stringTable();

    // good hash function for hashing keys
    enum HashFunction {GOOD_HASH_FUNCTION};

    // case insensitive print
    void case_insensitive(std::string query, std::ostream &output);
    
    // case sensitive print
    void case_sensitive(std::string query, std::ostream &output);

    // insertion with chaining
	void insertChaining(KeyType key, int path, int line);

    // duplicate checker (obsolete function, not being used anymore)
    // can delete
    bool hasDuplicate(std::string key, int num, int fileNum);

    // returns filepath from the privately-defined array of file paths
    std::string file_at(int index);

    // returns size of filepath array
    int file_path_size();

    // adds a new filepath to the array, passing by reference
    void add_path_name(std::string &pathName);



private:

    // Entry struct, which contains a value, which is a Location.
    // key_case is the case-sensitive version of the original key.
    struct Entry {
		ValueType  value;
        std::string key_case;
        Entry(ValueType v, std::string key_cased) {
            value = v;
            key_case = key_cased;
        }
	};

    // ContainerNode struct, which contains the lower-case version of a
    // key. It also contains a vector of Entrys, which are defined above.
    struct ContainerNode {
		KeyType    key;
        std::vector<Entry> entryList;
        ContainerNode(KeyType k, Entry e) {
            key = k;
            entryList.push_back(e);
        }
	};

    // Container struct, which contains a vector of ContainerNodes.
    // This is our "bucket" in the hash map. If different keys are hashed
    // to the same Container, they are then added to the next ContainerNode
    // in our nodeList.
    struct Container {
        std::vector<ContainerNode> nodeList;
	};

    // Our main hash map, an arrayList of Containers
    Container *chainedTable;

    // Vector that stores the filePaths, in order to reduce memory usage
    // and redundancy.
    std::vector <std::string> filePaths;

    // initial table size defined to be 100
	static const int INITIAL_TABLE_SIZE = 1000;

    // table size and items counter
    int currentTableSize;
    int numItemsInTable;

    
    // Helper functions for expanding our table and checking the load factor
	void  expand();
    float getLoadFactor();

    // hash function
    static size_t good_hash_function(KeyType key);

    // opening file with stream template
    template<typename streamtype>
    void open_or_die(streamtype &stream, std::string file_name);

    // case insensitive print helper function
    void print_case_insensitive(int index, std::ifstream &file, int path,
                       int line_num, std::string currentLine, Container *c,
                       std::ostream &output);
    
    // case sensitive print helper function
    void print_case_sensitive(std::string query, int index, std::ifstream &file, 
                        int path, int line_num, std::string currentLine,
                        Container *c, std::ostream &output);

};
#endif	

