// File: main.cpp
// Author: Matthew Leeds
// Purpose: Implement a Hill cipher algorithm for a Linear Algebra project.
// Usage: $ ./hillcipher -d|e <filename>
// where the file's first line is "n=<block size>" and the following n lines
// are the rows of the key matrix (comma delimited). n must be < 4
// The -d option decrypts from stdin to stdout; -e encrypts likewise.
// Plaintext and ciphertext must be strictly A-Z or "." or "," or " "

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <fstream>
#include <tuple>
#include "Matrix.h"

using namespace std;

// These functions convert "0-28" to "A-Z,. " and vice versa.
void mapIntegersToCharacters(const int* arr, int size);
tuple<int**, int> mapCharactersToIntegers(int n);
// finds the modular multiplicative inverse
int findModMultInv(int num, int mod);

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
        cerr << "Error: File '" << filename << "' not found or inaccessible." << endl;
        return 2;
    }
    // Assume the data file is formatted correctly.
    string line1;
    inFile >> line1;
    int n = atoi(line1.substr(2).c_str()); // it's an n by n matrix
    if (n > 3 || n < 2) {
        cerr << "Error: Currently n=2 and n=3 are the only supported block sizes." << endl;
        inFile.close();
        return 3;
    }
    Matrix* keyMatrix = new Matrix(inFile, n);
    inFile.close();
    int det = keyMatrix->findDeterminant();
    if (det == 0) {
        cerr << "Error: Given key matrix not invertible!";
        cout << *keyMatrix << endl;
        return 4;
    }
    // Attempt encryption or decryption
    if (ENCRYPT) { // encrypt stdin -> stdout
        // read a line from stdin
        tuple<int**, int> input = mapCharactersToIntegers(n);
        // m is the number of n-length substrings the input text was broken into
        int m = get<1>(input); 
        int** plaintext = get<0>(input);
        int** ciphertext = new int*[m];
        for (int i = 0; i < m; ++i) {
            // encrypt each by multiplying by the key matrix
            ciphertext[i] = keyMatrix->modMultiplyVector(plaintext[i], 29);
            // write the result to stdout as characters
            mapIntegersToCharacters(ciphertext[i], n);
        }
        cout << endl;
        for (int i = 0; i < m; ++i)
            delete[] ciphertext[i];
        delete ciphertext;
        for (int i = 0; i < m; ++i)
            delete[] plaintext[i];
        delete plaintext;
    } else { // decrypt stdin -> stdout
        // read a line from stdin
        tuple<int**, int> input = mapCharactersToIntegers(n);
        // m is the number of n-length substrings the input text was broken into
        int m = get<1>(input); 
        int** ciphertext = get<0>(input);
        int** plaintext = new int*[m];
        // find the adjoint of the key matrix
        Matrix* adj = keyMatrix->findAdjoint();
        // find the multiplicative inverse of the determinant mod 29
        int detInv = findModMultInv(det, 29);
        // multiply the adjoint by this value
        adj->modMultiplyScalar(detInv, 29);
        // We now have the inverse mod 29
        Matrix* inv = adj;
        for (int i = 0; i < m; ++i) {
            // Multiply each substring by the ciphertext to decrypt.
            plaintext[i] = inv->modMultiplyVector(ciphertext[i], 29);
            // write the result to stdout as characters
            mapIntegersToCharacters(plaintext[i], n);
        }
        cout << endl;
        for (int i = 0; i < m; ++i)
            delete[] ciphertext[i];
        delete ciphertext;
        for (int i = 0; i < m; ++i)
            delete[] plaintext[i];
        delete plaintext;
    }
    delete keyMatrix;
    return 0;
}

// map integers to characters to integers as defined and write to stdout
void mapIntegersToCharacters(const int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] < 26 && arr[i] >= 0) cout << (char) (arr[i] + 65);
        else if (arr[i] == 26) cout << ".";
        else if (arr[i] == 27) cout << ",";
        else if (arr[i] == 28) cout << " ";
        else cout << "?";
    }
}

// map characters from stdin to integer arrays based on predefined mappings
tuple<int**, int> mapCharactersToIntegers(int n) {
    string inText;
    cin >> inText;
    // pad until it's a multiple of n
    while (inText.length() % n != 0) {
        inText += " ";
    }
    int m = inText.length() / n;
    // allocate an array for the m substrings of length n
    int** arr = new int*[m];
    // iterate over the substrings, writing appropriate integers to arr
    for (int i = 0; i < m; ++i) {
        arr[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            char c = inText.at(i*n + j);
            if (c >= 'A' && c <= 'Z') arr[i][j] = ((int) c) - 65;
            else if (c == '.') arr[i][j] = 26;
            else if (c == ',') arr[i][j] = 27;
            else if (c == ' ') arr[i][j] = 28;
            else arr[i][j] = -1; // invalid character
        }
    }
    tuple<int**, int> returnVal(arr, m);
    return returnVal;
}

// finds x such that (num * x) % mod = 1
// Guaranteed te work with prime moduli such as 29
// This uses the Extended Euclidean algorithm.
int findModMultInv(int num, int mod) {
    int b0 = mod;
    int t, q;
    int x0 = 0;
    int x1 = 1;
    if (mod == 1) return 1;
    while (num > 1) {
        q = num / mod;
        t = mod;
        mod = num % mod;
        num = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += b0;
    return x1;
}
