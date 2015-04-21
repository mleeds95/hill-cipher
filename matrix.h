// File: matrix.h
// Author: Matthew Leeds
// Last Edit: 2015-01-23

#include <string>
#include <fstream>

using namespace std;

class Matrix {
    public:
        Matrix(int size);
        Matrix(int size, unsigned int seed);
        Matrix(ifstream& inFile, int size);
        ~Matrix();
        int findMaxMagnitude() const;
        Matrix* addSquareSubmatrices(bool subtract, int n, int rowStart,  int colStart, 
                                                           int rowStart2, int colStart2) const;
        void addExternalMatrices(Matrix* pM1, bool subtract, Matrix* pM2, bool append, int n, int row, int col);
        void addOperationCounts(Matrix* pM);
        friend ostream& operator<<(ostream& os, const Matrix& m);
        friend Matrix* operator+(const Matrix& m1, const Matrix& m2);
        friend Matrix* operator-(const Matrix& m1, const Matrix& m2);
        friend Matrix* strassenMultiply(Matrix* pM1, Matrix* pM2, int n, int rowStart,  int colStart, 
                                                                         int rowStart2, int colStart2);
        inline int getNumMultiplications() {return _numMultiplications;}
        inline int getNumAdditions() {return _numAdditions;}

    private:
        int** _matrix;
        int _size;
        int _numMultiplications;
        int _numAdditions;
};
