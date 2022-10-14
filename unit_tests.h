/*
*  unit_tests.h
*  Thomas Lai & Daniel Peng
*  4/12/2022
*
*  CS 15 Proj 3 
*
*  Purpose: Uses unit_testing frame work to test string manipulation 
*           FSTree Classes, each function testing edge cases and other 
*           cases to thoroughly check each functions to see if they 
*           work as expected.
*/
#include "stringProcessing.h"
#include "gerp.h"
#include <string>
#include <sstream>
#include <fstream>

#include <iostream>
#include <cassert>

using namespace std;

//------ String Manipulation Tests -------

// Tests the string in the project specs
void test1() {
    string a = "@##!!#!@!#COMP-15!!!!!!!";
    assert(stripNonAlphaNum(a) == "COMP-15");
}

// tests to make sure an empty string works
void test2_empty_string() {
    string a = "";
    assert(stripNonAlphaNum(a) == "");
}

// tests to make sure that an empty string is returned when
// called on a string of only non-alphanumerical characters
void test3_non_alphanum() {
    string a = "@##!!#!@!#";
    assert(stripNonAlphaNum(a) == "");
}

// tests to make sure that a string with only alphanumerical characters
// will not be parsed
void test4_alphanum() {
    string a = "COMP-15";
    assert(stripNonAlphaNum(a) == "COMP-15");
}

// tests to make sure the example {..... string in the autograder test works.
void test5_alphanum() {
    string a = "{.....";
    cerr << stripNonAlphaNum(a) << endl;
}

