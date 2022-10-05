#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <relation_name> 
 */

bool syntacticParseEXPORT()
{
    logger.log("syntacticParseEXPORT");
    if (tokenizedQuery.size() != 2 && tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    if(tokenizedQuery.size()==3){
        if(tokenizedQuery[1]!="MATRIX"){
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = EXPORT_MATRIX;
        parsedQuery.exportRelationName = tokenizedQuery[2];
        return true;

    }

    parsedQuery.queryType = EXPORT;
    parsedQuery.exportRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParseEXPORT()
{
    logger.log("semanticParseEXPORT");
    //Table should exist
    if(parsedQuery.queryType==EXPORT){
        if (tableCatalogue.isTable(parsedQuery.exportRelationName))
            return true;
        cout << "SEMANTIC ERROR: No such relation exists" << endl;
        
        return false;
    }
    if(parsedQuery.queryType==EXPORT_MATRIX){
        if (matrixCatalogue.isMatrix(parsedQuery.exportRelationName))
            return true;
        cout << "SEMANTIC ERROR: No such relation exists" << endl;
        
        return false;
    }
}

void executeEXPORT()
{
    logger.log("executeEXPORT");
    if(parsedQuery.queryType==EXPORT){
        Table* table = tableCatalogue.getTable(parsedQuery.exportRelationName);
        table->makePermanent();
        return;
    }

    Matrix* mat = matrixCatalogue.getMatrix(parsedQuery.exportRelationName);
    mat->makePermanent();
    return;
}