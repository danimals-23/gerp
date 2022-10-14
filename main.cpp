/*
 *   main.cpp
 *   Daniel Peng + Thomas Lai
 *   04/20/22
 *
 *   CS15 Project 3 : Gerp
 *   
 *   File which contains the main function that runs the executable and
 *   handles file input/output.
 *   
 */

#include "gerp.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 3) {
        cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
        exit(EXIT_FAILURE);
    } 

    // gets the inputDirectory and outputFile from the terminal
    string inputDirectory = argv[1];
    string outputFile = argv[2];

    try {
        Gerp g(inputDirectory, outputFile);
        g.run();
    } catch (std::runtime_error &e) {
        cout << "Could not build index, exiting." << endl;
    }
    return 0;
}