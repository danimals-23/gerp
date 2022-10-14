#########################################################
#
#	Daniel Peng + Thomas Lai
#	CS15 Proj 3: Gerp
# 	04/25/2022
#	MakeFile
#
#########################################################

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

# this rule builds the gerp executable
gerp: main.o stringProcessing.o stringTable.o gerp.o FSTree.o DirNode.o
	$(CXX) $(CXXFLAGS) -o gerp main.o stringProcessing.o stringTable.o gerp.o FSTree.o DirNode.o 


# This rule builds main
main.o: main.cpp gerp.h 
	$(CXX) $(CXXFLAGS) -c main.cpp

stringProcessing.o: stringProcessing.cpp stringProcessing.h
	$(CXX) $(CXXFLAGS) -c  stringProcessing.cpp

stringTable.o: stringTable.cpp stringTable.h
	$(CXX) $(CXXFLAGS) -c  stringTable.cpp

gerp.o: gerp.cpp gerp.h stringProcessing.h FSTree.h DirNode.h 
	$(CXX) $(CXXFLAGS) -c  gerp.cpp

unit_test: unit_test_driver.o stringProcessing.o FSTree.o DirNode.o
	$(CXX) $(CXXFLAGS) unit_test_driver.o stringProcessing.o \
	FSTree.o DirNode.o gerp.o stringTable.o


# This rule provides the final submission 
provide: 
	provide comp15 proj3_gerp Makefile README gerp.h gerp.cpp stringTable.h \
		stringTable.cpp main.cpp stringProcessing.h stringProcessing.cpp \
		testCommands.txt

# remove executables, object code, and temporary files from the current folder 
# -- the executable created by unit_test is called a.out
clean: 
	rm *.o *~ a.out





	








