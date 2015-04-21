// File: Matrix.h
// Author: Matthew Leeds

#include <string>
#include <fstream>

using namespace std;

class Matrix {
    public:
        Matrix(int size);
        Matrix(ifstream& inFile, int size);
        ~Matrix();
        int findMaxMagnitude() const;
        Matrix* addSquareSubmatrices(bool subtract, int n, int rowStart,  int colStart, 
                                                           int rowStart2, int colStart2) const;
        void addExternalMatrices(Matrix* pM1, bool subtract, Matrix* pM2, bool append, int n, int row, int col);
        friend ostream& operator<<(ostream& os, const Matrix& m);
        friend Matrix* operator+(const Matrix& m1, const Matrix& m2);
        friend Matrix* operator-(const Matrix& m1, const Matrix& m2);

    private:
        int** _matrix;
        int _size;
};