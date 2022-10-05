#include "global.h"

bool syntacticParseCROSS_TRANSPOSE()
{
    logger.log("syntacticParseCROSS_TRANSPOSE");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = CROSS_TRANSPOSE;
    parsedQuery.crossFirstRelationName = tokenizedQuery[1];
    parsedQuery.crossSecondRelationName = tokenizedQuery[2];
    return true;
}

bool semanticParseCROSS_TRANSPOSE()
{
    logger.log("semanticParseCROSS_TRANSPOSE");

    if (!matrixCatalogue.isMatrix(parsedQuery.crossFirstRelationName) || !matrixCatalogue.isMatrix(parsedQuery.crossSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Cross relations don't exist" << endl;
        return false;
    }
    Matrix mat1 = *(matrixCatalogue.getMatrix(parsedQuery.crossFirstRelationName));
    Matrix mat2 = *(matrixCatalogue.getMatrix(parsedQuery.crossSecondRelationName));
    if(mat1.columnCount!=mat1.rowCount || mat2.columnCount!=mat2.rowCount){
        cout << "SEMANTIC ERROR: Both matrices should be nxn" << endl;
        return false;
    }
    if(mat1.columnCount!=mat2.columnCount){
        cout<<"SEMANTIC ERROR: Both matrices should have the same dimensions" << endl;
        return false;
    }
    return true;
}


void executeCROSS_TRANSPOSE()
{
    logger.log("executeCROSS_TRANSPOSE");

    Matrix mat1 = *(matrixCatalogue.getMatrix(parsedQuery.crossFirstRelationName));
    Matrix mat2 = *(matrixCatalogue.getMatrix(parsedQuery.crossSecondRelationName));

    for(int i=0; i<mat1.rowCount; i++){
        for(int j=0; j<mat1.columnCount; j++){
            int x1 = mat1.getIJ(i, j);
            int x2 = mat2.getIJ(j, i);

            mat1.updIJ(i, j, x2);
            mat2.updIJ(j, i, x1);
        }
        // cout<<endl;
    }
    return;
}