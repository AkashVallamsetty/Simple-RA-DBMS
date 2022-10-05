#ifndef MATRIX_CATALOGUE_H
#define MATRIX_CATALOGUE_H

#include "matrix.h"

/**
 * @brief The MatrixCatalogue acts like an index of tables existing in the
 * system. Everytime a table is added(removed) to(from) the system, it needs to
 * be added(removed) to(from) the MatrixCatalogue. 
 *
 */
class MatrixCatalogue
{

    unordered_map<string, Matrix*> matrices;

public:
    MatrixCatalogue() {}
    void insertMatrix(Matrix* matrix);
    void deleteMatrix(string matrixName);
    Matrix* getMatrix(string matrixName);
    bool isMatrix(string matrixName);
    // bool isColumnFromTable(string columnName, string tableName);
    void print();
    ~MatrixCatalogue();
};

#endif