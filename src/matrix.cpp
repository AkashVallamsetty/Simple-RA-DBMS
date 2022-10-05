#include "global.h"

/**
 * @brief Construct a new Matrix::Matrix object
 *
 */
Matrix::Matrix()
{
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Table:: Table object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param matrixName 
 */
Matrix::Matrix(string matrixName)
{
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + matrixName + ".csv";
    this->matrixName = matrixName;
}


/**
 * @brief The load function is used when the LOAD MATRIX command is encountered. It
 * reads data from the source file, splits it into blocks.
 *
 * @return true if the matrix has been successfully loaded 
 * @return false if an error occurred 
 */
bool Matrix::load()
{
    logger.log("Matrix::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line)){
        fin.close();
        if(this->getNumColumns(line))
            if (this->blockify())
                return true;
        cout<<this->columnCount<<endl;
    }
    fin.close();
    return false;
}


bool Matrix::getNumColumns(string firstLine){
    logger.log("Matrix::getNumColumns");
    string word;
    stringstream s(firstLine);
    int num_columns=0;

    while (getline(s, word, ','))
    {
        num_columns += 1;
    }

    this->columnCount = num_columns;
    this->maxIntsPerBlock = (BLOCK_SIZE * 1000)/sizeof(int);
    
    return true;
}


/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Matrix::blockify()
{
    logger.log("Matrix::blockify");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(this->maxIntsPerBlock, 0);
    vector<vector<int>> rowsInPage(1, row);
    int perPageCounter = 0;
    
    while (getline(fin, line))
    {
        stringstream s(line);
        this->rowCount += 1;
        this->rowLocation.push_back({this->blockCount, perPageCounter});

        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (!getline(s, word, ','))
                return false;
            row[perPageCounter] = stoi(word);
            perPageCounter += 1;

            if (perPageCounter == this->maxIntsPerBlock){
                rowsInPage[0] = row;
                bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, 1);
                row = vector<int>(this->maxIntsPerBlock, 0);
                this->blockCount++;
                perPageCounter = 0;
            }
        }
    }

    if(row.size()){
        rowsInPage[0] = row;
        bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, 1);
        row = vector<int>(this->maxIntsPerBlock, 0);
        this->blockCount++;
        perPageCounter = 0;
    }

    return true;
}


int Matrix::getIJ(int i, int j){
    logger.log("Matrix::getIJ");

    int block = rowLocation[i].first;
    int location_in_block = rowLocation[i].second;

    for(int ele = 1; ele <= j; ele++){
        location_in_block++;
        if(location_in_block==maxIntsPerBlock){
            location_in_block=0;
            block++;
        }
    }

    string pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(block);
    int readsf = 0;
    int ret;
    ifstream fin(pageName, ios::in);
    while(readsf <= location_in_block){
        fin>>ret;
        readsf++;
    }
    return ret;
}

void Matrix::updIJ(int i, int j, int val){
    logger.log("Matrix::updIJ");

    int block = rowLocation[i].first;
    int location_in_block = rowLocation[i].second;

    for(int ele = 1; ele <= j; ele++){
        location_in_block++;
        if(location_in_block==maxIntsPerBlock){
            location_in_block=0;
            block++;
        }
    }

    string pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(block);
    int readsf = 0;
    int ret;
    ifstream fin(pageName, ios::in);
    vector<int> block_row;
    while(readsf<maxIntsPerBlock){
        fin>>ret;
        if(!ret)
            break;
        if(readsf == location_in_block)
            block_row.push_back(val);
        else    
            block_row.push_back(ret);
        readsf++;
    }
    fin.close();

    ofstream fout(pageName, ios::out);

    for(auto c: block_row){
        fout<<c<<" ";
    }
}

/**
 * @brief Function prints the first few rows of the table. If the table contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Matrix::print()
{
    logger.log("Matrix::print");
    // uint count = min((long long)PRINT_COUNT, this->rowCount);

    //print headings
    // this->writeRow(this->columns, cout);
    int pageCounter=0;
    string pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(pageCounter);
    ifstream fin(pageName, ios::in);
    int read_so_far = 0;
    int num_elements = columnCount*rowCount;

    while(read_so_far<num_elements){
        int element;
        fin>>element;
        read_so_far+=1;
        if(read_so_far%maxIntsPerBlock == 0){
            pageCounter++;
            pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(pageCounter);
            fin = ifstream(pageName, ios::in);
        }

        int i=(read_so_far-1)/columnCount;
        int j=(read_so_far-1)%columnCount;
        if(i>=20){
            break;
        }
        if(j==0)
            cout<<endl;
        if(j>=20)
            continue;
        cout<<element<<" ";
    }
    fin.close();
    // vector<int> row;
    // for (int rowCounter = 0; rowCounter < min(rowCount, 20); rowCounter++)
    // {
    //     int page_num = rowLocation[rowCounter].first;
    //     int curr_location = rowLocation[rowCounter].second;
    //     this->page = bufferManager.getPage(matrixName, rowLocation[rowCounter].first);

    //     for(int colCounter=0; colCounter < min(20, columnCount); colCounter++){

    //     }

    //     row = cursor.getNext();
    //     this->writeRow(row, cout);
    // }
    // printRowCount(this->rowCount);
}

void Matrix::makePermanent()
{
    logger.log("Matrix::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    int pageCounter=0;
    string pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(pageCounter);
    ifstream fin(pageName, ios::in);
    int read_so_far = 0;
    int num_elements = columnCount*rowCount;

    while(read_so_far<num_elements){
        int element;
        fin>>element;
        read_so_far+=1;
        if(read_so_far%maxIntsPerBlock == 0){
            pageCounter++;
            pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(pageCounter);
            fin = ifstream(pageName, ios::in);
        }

        int i=(read_so_far-1)/columnCount;
        int j=(read_so_far-1)%columnCount;
        if(j==0 && i!=0)
            fout<<endl;
        fout<<element;
        if(j!=columnCount-1)
            fout<<", ";
    }
    fin.close();
    fout.close();
}

/**
 * @brief This function returns one row of the table using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
// void Matrix::getNextPage(Cursor *cursor)
// {
//     logger.log("Table::getNext");

//         if (cursor->pageIndex < this->blockCount - 1)
//         {
//             cursor->nextPage(cursor->pageIndex+1);
//         }
// }



/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
// void Table::makePermanent()
// {
//     logger.log("Table::makePermanent");
//     if(!this->isPermanent())
//         bufferManager.deleteFile(this->sourceFileName);
//     string newSourceFile = "../data/" + this->tableName + ".csv";
//     ofstream fout(newSourceFile, ios::out);

//     //print headings
//     this->writeRow(this->columns, fout);

//     Cursor cursor(this->tableName, 0);
//     vector<int> row;
//     for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
//     {
//         row = cursor.getNext();
//         this->writeRow(row, fout);
//     }
//     fout.close();
// }

/**
 * @brief Function to check if table is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->matrixName + ".csv")
    return true;
    return false;
}

/**
 * @brief The unload function removes the table from the database by deleting
 * all temporary files created as part of this table
 *
 */
// void Table::unload(){
//     logger.log("Table::~unload");
//     for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
//         bufferManager.deleteFile(this->tableName, pageCounter);
//     if (!isPermanent())
//         bufferManager.deleteFile(this->sourceFileName);
// }

/**
 * @brief Function that returns a cursor that reads rows from this table
 * 
 * @return Cursor 
 */
// Cursor Table::getCursor()
// {
//     logger.log("Table::getCursor");
//     Cursor cursor(this->tableName, 0);
//     return cursor;
// }
/**
 * @brief Function that returns the index of column indicated by columnName
 * 
 * @param columnName 
 * @return int 
 */
// int Table::getColumnIndex(string columnName)
// {
//     logger.log("Table::getColumnIndex");
//     for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
//     {
//         if (this->columns[columnCounter] == columnName)
//             return columnCounter;
//     }
// }

void Matrix::unload(){
    logger.log("Matrix::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->matrixName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}
