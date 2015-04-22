// File: Matrix.h
// Author: Matthew Leeds

#include <string>
#include <fstream>

using namespace std;

class Matrix {
    public:
        Matrix(int size, bool identity);
        Matrix(ifstream& inFile, int size);
        Matrix(const Matrix& other);
        ~Matrix();
        int findDeterminant();
        Matrix* getSubmatrix(int i, int j);
        Matrix* findAdjoint();
        int* multiplyMod29(int* arr);
        int findMaxMagnitude() const;
        friend ostream& operator<<(ostream& os, const Matrix& m);
        friend Matrix* operator+(const Matrix& m1, const Matrix& m2);
        friend Matrix* operator-(const Matrix& m1, const Matrix& m2);
        int** _matrix;
        int _size;
};
