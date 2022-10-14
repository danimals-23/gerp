/*
 *   gerp.h
 *   Daniel Peng + Thomas Lai
 *   04/20/22
 *
 *   CS15 Project 3 : Gerp
 *   
 *   Interface for the gerp class, where users can query words
 *   from a given directory, and returns all matching words along with the
 *   filePath + line number to a given output file.
 *
 */

#ifndef GERP_H
#define GERP_H

#include "stringTable.h"
#include <iostream>
#include <fstream>
#include <string>

#include "DirNode.h"
#include "FSTree.h"

// Gerp class declaration
class Gerp {
    public:
        // Default constructor and destructor
        Gerp();
        ~Gerp();
        
        // Constructor that takes in a directory and output file
        Gerp(std::string inputDirectory, std::string outputFile);

        // public run function
        void run();
    private:

        // private hashMap
        stringTable hashMap;

        // output File stream
        std::ofstream output;

        // private runHelper function that runs a command loop
        void runHelper(std::istream &input);

        // runTraversal goes through the files in a directory
        void runTraversal(std::string directory);
        void runTraversalHelper(DirNode *node);
        // printPath returns the entire filepath as a string
        std::string print_path(DirNode *node, std::string pathName);

        // open_or_die function to open streams
        template<typename streamtype>
        void open_or_die(streamtype &stream, std::string file_name);

        // hashing file-by-file
        void hashFile(std::string pathName);

        // hashing line-by-line within a file
        void hashLine(int pathNum, std::string stringName, int lineNum);

};


#endif
