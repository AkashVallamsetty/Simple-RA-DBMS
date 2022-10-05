#include "global.h"
/**
 * @brief 
 * SYNTAX: PRINT relation_name
 */
bool syntacticParsePRINT()
{
    logger.log("syntacticParsePRINT");
    if (tokenizedQuery.size() != 2 && tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    if(tokenizedQuery.size() == 3 && tokenizedQuery[1]!="MATRIX"){
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    if(tokenizedQuery.size() == 3){
        parsedQuery.queryType = PRINT_MATRIX;
        parsedQuery.printRelationName = tokenizedQuery[2];
    }
    else{
        parsedQuery.queryType = PRINT;
        parsedQuery.printRelationName = tokenizedQuery[1];
    }
    return true;
}

bool semanticParsePRINT()
{
    logger.log("semanticParsePRINT");
    if (!matrixCatalogue.isMatrix(parsedQuery.printRelationName) && !tableCatalogue.isTable(parsedQuery.printRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    return true;
}

void executePRINT()
{
    if(parsedQuery.queryType==PRINT){
        logger.log("executePRINT");
        Table* table = tableCatalogue.getTable(parsedQuery.printRelationName);
        table->print();
    }
    else{
        logger.log("executePRINT_MATRIX");
        Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.printRelationName);
        matrix->print();
    }
    return;
}
