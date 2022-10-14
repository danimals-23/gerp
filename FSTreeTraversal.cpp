/*
*/

#include "DirNode.h"
#include "FSTree.h"

#include <string>
#include <iostream>
using namespace std;

void runTraversal(string directory, ostream &stream);
void runHelper(DirNode *node, ostream &stream);
void print_path(DirNode *node, ostream &stream);

// int main(int argc, char *argv[]) {

//     if (argc != 2) {
//         return 1;
//     } 
    
//     run(argv[1]);
//     return 0;
// }

/**/
void runTraversal(string directory, ostream &stream) {
    try {
        FSTree myTree(directory);
        
        DirNode *root = myTree.getRoot();
        runHelper(root, stream);
    } catch(const runtime_error &e) {
        cerr << e.what() << endl;
    }
}

void runHelper(DirNode *node, ostream &stream) {
    // Base case 
    if (node == nullptr or node->isEmpty()) {
        return;
    }

    if (node->hasSubDir()) {
        int n = node->numSubDirs();
        for (int i = 0; i < n; i++) {
            runHelper(node->getSubDir(i), stream);
        }
    }

    if (node->hasFiles()) {
        int n = node->numFiles();
        for (int i = 0; i < n; i++) {
            print_path(node, stream);
            stream << node->getFile(i) << endl;

        }
    }
    
    return;
}

void print_path(DirNode *node, ostream &stream) {
    
    if (node == nullptr) {
        return;
    }

    print_path(node->getParent(), stream);
    stream << node->getName() << '/';
}

    
    










