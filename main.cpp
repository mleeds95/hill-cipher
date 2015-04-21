// File: main.cpp
// Author: Matthew Leeds
// Purpose: Implement a Hill cipher algorithm for a Linear Algebra project.
// Usage: $ ./hillcipher -d|e <filename>
// where the file's first line is "n=<block size>" and the following n lines
// are the rows of the key matrix (comma delimited).
// The -d option decrypts from stdin to stdout; -e encrypts likewise.
// The key matrix is assumed to be invertible (as this is a requirement of the algorithm).

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "Matrix.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./hillcipher -d|e <filename>" << endl;
        return 1;
    }
    bool ENCRYPT; // decrypt if false
    string option = argv[1];
    if (option == "-e") ENCRYPT = true;
    else if (option == "-d") ENCRYPT = false;
    else {
        cerr << "Usage: ./hillcipher -d|e <filename>" << endl;
        return 1;
    }
    const char* filename = argv[2];
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Error: File '" << filename << "' not found or inaccessible.\n";
        return 2;
    }
    int n; // matrix is n by n
    Matrix* keyMatrix;
    // Assume the data file is formatted correctly.
    string line1;
    inFile >> line1;
    n = atoi(line1.substr(2).c_str());
    keyMatrix = new Matrix(inFile, n);
    inFile.close();
    // At this point, keyMatrix should be initialized.
    cout << *keyMatrix << endl;
    //TODO encrypt/decrypt
    delete keyMatrix;
    return 0;
}
