/*
 *   stringProcessing.cpp
 *   Daniel Peng + Thomas Lai
 *   04/20/22
 *
 *   CS15 Project 3 : Gerp
 *   
 *   Implementation of the stringProcessing class, which parses a string and
 *   removes all non-alphanumerical characters from the string.
 *   
 */

#include <string>
#include <iostream>
#include "stringProcessing.h"
#include <stdio.h>
#include <ctype.h>

using namespace std; 

/*
 * name:      strinProcessing.cpp stripNonAlphaNum function
 * purpose:   parses a string and removes all non-alphanumerical characters
 * arguments: a string to be parsed
 * returns:   the original string without non-alphanumerical characters
 * effects:   parses a string, removing non-alphanumerical characters
 */

string stripNonAlphaNum(string s) {
    int begin = 0;
    int end = s.length() - 1;

    if (begin > end) { // empty case
        return "";
    } 

    // parses starting through the beginning
    while (!isalnum(s[begin]) and begin <= end) {
        begin++;
    }

    // parses starting from the back
    while (end >= 0 and !isalnum(s[end])) {
        end--;
    }

    if (begin > end) {
        return "";
    }
    else {
        return s.substr(begin, end - begin + 1);
    }

}
