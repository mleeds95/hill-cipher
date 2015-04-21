// File: Matrix.cpp
// Author: Matthew Leeds
// This class defines a matrix that can be used for multiplication/addition/subtraction.

#include "Matrix.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <cmath>

// This constructor allocates space for the matrix.
// If identity is true, construct the identity matrix.
Matrix::Matrix(int size, bool identity) {
    this->_size = size;
    this->_matrix = new int*[this->_size];
    for (int i = 0; i < this->_size; ++i) {
        this->_matrix[i] = new int[this->_size];
        if (identity) {
            for (int j = 0; j < this->_size; ++j)
                this->_matrix[i][j] = (i == j ? 1 : 0);
        }
    }
}

// copy constructor
Matrix::Matrix(const Matrix& other) {
    this->_size = other._size;
    this->_matrix = new int*[this->_size];
    for (int i = 0; i < this->_size; ++i) {
        this->_matrix[i] = new int[this->_size];
        for (int j = 0; j < this->_size; ++j) {
            this->_matrix[i][j] = other._matrix[i][j];
        }
    }
}

// This constructor allocates space and fills the matrix with values from a file.
// It is assumed that the file is formatted correctly, and that the 
// ifstream object is seeked to the right position before being passed to this.
Matrix::Matrix(ifstream& inFile, int size) {
    this->_size = size;
    this->_matrix = new int*[this->_size];
    for (int i = 0; i < this->_size; i++) {
        this->_matrix[i] = new int[this->_size];
    }
    // For each line, interpret everything but commas as integers
    for (int i = 0; i < size; i++) {
        string line = "";
        inFile >> line;
        stringstream ss(line);
        int j = 0;
        int num;
        while (ss >> num) {
            this->_matrix[i][j] = num;
            j++;
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < this->_size; i++) {
        delete []this->_matrix[i];
    }
    delete []this->_matrix;
}

// Find the inverse of this using Gauss-Jordan elimination, and return a pointer to it.
Matrix* Matrix::findGaussJordanInverse() {
    Matrix* copy = this;
    int n = this->_size;
    Matrix* inv = new Matrix(n, true);
    int alpha;
    int diag;
    for (int i = 0; i < n; ++i) {
        diag = copy->_matrix[i][i];
        for (int j = 0; j < n; ++j) {
            copy->_matrix[i][j] /= diag;
            inv->_matrix[i][j] /= diag; 
        }
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            alpha = copy->_matrix[j][i] / copy->_matrix[i][i];
            for (int k = 0; k < n; ++k) {
                copy->_matrix[j][k] -= alpha * copy->_matrix[i][k];
                inv->_matrix[j][k] -= alpha * inv->_matrix[i][k];
            }
        }
    }
    return inv;
}

// Multiplies this matrix by the n by 1 matrix given, mod 29
int* Matrix::multiplyMod29(int* arr) {
    int n = this->_size;
    int* result = new int[n];
    for (int i = 0; i < n; ++i) {
        result[i] = 0;
        for (int j = 0; j < n; ++j) {
            result[i] += this->_matrix[i][j] * arr[j];
        }
        result[i] = result[i] % 29;
    }
    return result;
}

// Find the maximum magnitude value in the matrix. This can be useful for 
// determining how wide to make columns when printing it out.
int Matrix::findMaxMagnitude() const{
    int maxVal = 0;
    for (int i = 0; i < this->_size; i++) {
        for (int j = 0; j < this->_size; j++) {
            if (abs(this->_matrix[i][j]) > maxVal) {
                maxVal = abs(this->_matrix[i][j]);
            }
        }
    }
    return maxVal;
}

// This overloads the redirection operator so matrices can be printed using cout.
// Values are printed in fixed-width columns, right-aligned.
ostream& operator<<(ostream& os, const Matrix& m) {
    int maxVal = m.findMaxMagnitude();
    string maxValString;
    ostringstream convert;
    convert << maxVal;
    maxValString = convert.str();
    int extraSpace = 2;
    int fieldWidth = extraSpace + maxValString.size();
    for (int i = 0; i < m._size; i++) {
        for (int j = 0; j < m._size; j++) {
            os << right << setw(fieldWidth) << m._matrix[i][j];
        }
        os << endl;
    }
    return os;
}

// This overloads the addition operator to add two square matrices 
// by adding their respective elements.
Matrix* operator+(const Matrix& m1, const Matrix& m2) {
    int n = m1._size;
    Matrix* pSumMatrix = new Matrix(n, false);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pSumMatrix->_matrix[i][j] = (m1._matrix[i][j] + m2._matrix[i][j]);
        }
    }
    return pSumMatrix; 
}

// This overloads the subtraction operator for (square) matrices.
Matrix* operator-(const Matrix& m1, const Matrix& m2) {
    int n = m1._size;
    Matrix* pDifferenceMatrix = new Matrix(n, false);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pDifferenceMatrix->_matrix[i][j] = (m1._matrix[i][j] - m2._matrix[i][j]);
        }
    }
    return pDifferenceMatrix; 
}

