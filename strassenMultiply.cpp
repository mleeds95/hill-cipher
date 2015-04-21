// File: strassenMultiply.cpp
// Author: Matthew Leeds
// Last Edit: 2015-01-25

#include "matrix.h"

using namespace std;

// This uses Strassen's method to take the product of two square matrices.
Matrix* strassenMultiply(Matrix* pA, Matrix* pB, int n, int row,  int col, 
                                                          int row2, int col2) {
    // Assume both input matrices' dimensions are powers of two (and equal dimensions).
    Matrix* pProductMatrix = new Matrix(n);
    if (n == 1) {
        pProductMatrix->_matrix[0][0] = pA->_matrix[row][col] * pB->_matrix[row2][col2];
        pProductMatrix->_numMultiplications = 1;
    } else if (n == 2) {
        // Direct multiplication is much faster than Strassen's for small n values.
        pProductMatrix->_matrix[0][0] = pA->_matrix[row][col]     * pB->_matrix[row2][col2]
                                      + pA->_matrix[row][col+1]   * pB->_matrix[row2+1][col2];
        pProductMatrix->_matrix[0][1] = pA->_matrix[row][col]     * pB->_matrix[row2][col2+1]
                                      + pA->_matrix[row][col+1]   * pB->_matrix[row2+1][col2+1];
        pProductMatrix->_matrix[1][0] = pA->_matrix[row+1][col]   * pB->_matrix[row2][col2]
                                      + pA->_matrix[row+1][col+1] * pB->_matrix[row2+1][col2];
        pProductMatrix->_matrix[1][1] = pA->_matrix[row+1][col]   * pB->_matrix[row2][col2+1]
                                      + pA->_matrix[row+1][col+1] * pB->_matrix[row2+1][col2+1];
        pProductMatrix->_numMultiplications = 8;
        pProductMatrix->_numAdditions = 4;
    } else {
        // Use Strassen's method for large n values.
        int halfN = n / 2;
        bool add = false;
        bool sub = true;
        // First, calculate 10 intermediate matrices S1..S10
        Matrix* pS1  = pB->addSquareSubmatrices(sub, halfN,         row2, col2 + halfN, row2 + halfN, col2 + halfN);
        Matrix* pS2  = pA->addSquareSubmatrices(add, halfN,          row,          col,          row,  col + halfN);
        Matrix* pS3  = pA->addSquareSubmatrices(add, halfN,  row + halfN,          col,  row + halfN,  col + halfN);
        Matrix* pS4  = pB->addSquareSubmatrices(sub, halfN, row2 + halfN,         col2,         row2,         col2);
        Matrix* pS5  = pA->addSquareSubmatrices(add, halfN,          row,          col,  row + halfN,  col + halfN);
        Matrix* pS6  = pB->addSquareSubmatrices(add, halfN,         row2,         col2, row2 + halfN, col2 + halfN);
        Matrix* pS7  = pA->addSquareSubmatrices(sub, halfN,          row,  col + halfN,  row + halfN,  col + halfN);
        Matrix* pS8  = pB->addSquareSubmatrices(add, halfN, row2 + halfN,         col2, row2 + halfN, col2 + halfN);
        Matrix* pS9  = pA->addSquareSubmatrices(sub, halfN,          row,          col,  row + halfN,          col);
        Matrix* pS10 = pB->addSquareSubmatrices(add, halfN,         row2,         col2,         row2, col2 + halfN);
        pProductMatrix->_numAdditions += 10 * halfN * halfN;
        // Now calculate 7 product matrices P1..P7,
        // and account for each one's operations.
        Matrix* pP1 = strassenMultiply(pA,  pS1, halfN,         row,         col,            0,            0);
        pProductMatrix->addOperationCounts(pP1);
        Matrix* pP2 = strassenMultiply(pS2,  pB, halfN,           0,           0, row2 + halfN, col2 + halfN);
        pProductMatrix->addOperationCounts(pP2);
        Matrix* pP3 = strassenMultiply(pS3,  pB, halfN,           0,           0,         row2,         col2);
        pProductMatrix->addOperationCounts(pP3);
        Matrix* pP4 = strassenMultiply(pA,  pS4, halfN, row + halfN, col + halfN,            0,            0);
        pProductMatrix->addOperationCounts(pP4);
        Matrix* pP5 = strassenMultiply(pS5,  pS6, halfN,          0,           0,            0,            0);
        pProductMatrix->addOperationCounts(pP5);
        Matrix* pP6 = strassenMultiply(pS7,  pS8, halfN,          0,           0,            0,            0);
        pProductMatrix->addOperationCounts(pP6);
        Matrix* pP7 = strassenMultiply(pS9, pS10, halfN,          0,           0,            0,            0);
        pProductMatrix->addOperationCounts(pP7);
        // Calculate the solution product matrix.
        pProductMatrix->addExternalMatrices(pP5, add, pP4, false, halfN,     0,     0);
        pProductMatrix->addExternalMatrices(pP6, sub, pP2,  true, halfN,     0,     0);
        pProductMatrix->addExternalMatrices(pP1, add, pP2, false, halfN,     0, halfN);
        pProductMatrix->addExternalMatrices(pP3, add, pP4, false, halfN, halfN,     0);
        pProductMatrix->addExternalMatrices(pP5, sub, pP3, false, halfN, halfN, halfN);
        pProductMatrix->addExternalMatrices(pP1, sub, pP7,  true, halfN, halfN, halfN);
        // Account for the 12 additions of P matrices and the 2 addition assignments.
        pProductMatrix->_numAdditions += 14 * halfN * halfN; 
        delete pS1; delete pS2; delete pS3; delete pS4; delete pS5;
        delete pS6; delete pS7; delete pS8; delete pS9; delete pS10;
        delete pP1; delete pP2; delete pP3; delete pP4; delete pP5; 
        delete pP6; delete pP7;
    }
    return pProductMatrix;
}
