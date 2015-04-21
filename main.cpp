// File: main.cpp
// Author: Matthew Leeds
// Purpose: Implement a Hill cipher algorithm for a Linear Algebra project.
// Usage: $ ./hillcipher -d|e <filename>
// where the file's first line is "n=<block size>" and the following n lines
// are the rows of the key matrix (comma delimited).
// The -d option decrypts from stdin to stdout; -e encrypts likewise.
// The key matrix is assumed to be invertible (as this is a requirement of the algorithm).
// Plaintext and ciphertext must be strictly A-Z or "." or "," or " "

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <tuple>
#include "Matrix.h"

using namespace std;

// These functions convert "0-28" to "A-Z,. " and vice versa.
void mapIntegersToCharacters(const int* arr, int size);
tuple<int*, int> mapCharactersToIntegers();

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
    const int arr[] = {7,4,11,11,14,28};
    mapIntegersToCharacters(arr, 6);
    tuple<int*,int> myT = mapCharactersToIntegers();
    int size = get<1>(myT);
    int* arr2 = get<0>(myT);
    for (int i = 0; i < size; ++i) {
        cout << arr2[i] << endl;
    }
    //TODO encrypt/decrypt
    delete keyMatrix;
    return 0;
}

void mapIntegersToCharacters(const int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] < 26 && arr[i] >= 0) cout << (char) (arr[i] + 65);
        else if (arr[i] == 26) cout << ".";
        else if (arr[i] == 27) cout << ",";
        else if (arr[i] == 28) cout << " ";
        else cout << "?";
    }
}

tuple<int*, int> mapCharactersToIntegers() {
    string inText;
    cin >> inText;
    int size = inText.length();
    int* arr = new int[size];
    for (unsigned int i = 0; i < size; ++i) {
        char c = inText.at(i);
        if (c >= 'A' && c <= 'Z') arr[i] = ((int) c) - 65;
        else if (c == '.') arr[i] = 26;
        else if (c == ',') arr[i] = 27;
        else if (c == ' ') arr[i] = 28;
        else arr[i] = -1;
    }
    tuple<int*, int> returnVal(arr, size);
    return returnVal;
}
