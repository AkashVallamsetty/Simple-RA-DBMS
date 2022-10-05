## Q2
## Page Layout
I store the rowCount and columnCount for each matrix in it's class object. This information enables us to store the matrix in a dense form in a row major fashion.

Along with this, I also store the block number and position in the block each row starts at.

There is no space between rows in blocks, hence minimal space wastage.

## CROSS_TRANSPOSE
I implemented 2 new functions in the Matrix class, namely "getIJ(i, j)" and "updIJ(i, j, val)". As their names suggest, they fetch/update the (i, j)th value of a matrix after rerading it from a block.
To cross transpose, i simply ran 2 nested loops, i goes from 0->n and j goes from 0->n

I read the A[i][j] and B[j][i]
and update A[i][j] with the value of B[j][i] and similarly for B[j][i].

## SPARSE MATRICES
## Storing
Sparse matrices are the matrices that contains many zero elements. Thus they can be stored in more memory efficient way than dense matrices. Let us only store the non-zero elements along with their locations and there are many ways to implement this, the disadvantage of storing like this is the increased complexity of accessing an element.

Let us store each non zero entry as a 3 elements i.e row, column, value in a single row of the table structure that is given in DBMS. After entering one entry the next non zero entry is entered in the following row of table.

While we storing entries make sure that the sparse matrix is traversed in Row Major Order. Thus the regeneration of matrix for print/ export will be efficient.

## LOAD
since sparse matrix is stored as a table, it is loaded like anyother table.

## PRINT and EXPORT
Initially place the cursor at the first row of table loaded.
Traverse a N * N matrix indices in Row Major order and compare the (row, column) value of the traversing matrix with that of cursor. If they match then the value in third column is the corresponding row x column element otherwise take that element as zero. move the cursor to next row and repeat the process

## TRANSPOSE OF SPARSE MATRIX
Transpose of sparse and dense matrices are performed differently since they are stored differently.
Sparse matrix is stored in table, let us transform the matrix page wise and merge them in Row Mazor order.
That is traverse through each page by swapping row and column values of each tuple in the row of page.
Now sort this swapped page in such a way that all tuples are arranged in Row Mazor order. Repeat same for all pages.
Now we have pages with each one has triplets in Row Mazor order but that order is not maintained between pages so we cannot simple merge all pages in this stage.
Now we will travel through each pair of pages and sort them in ascending order of rows (Row Mazor), so after each iteration the rows of smaller row value will come into page 1, page 2, page 3 .... in order i.e we are applying selection sort where minimum value will be pushed to first place after each iteration.
Finally we have pages in the Row Mazor order that can be merged and thus producing TRANSPOSE OF SPARSE MATRIX.
PRINT and EXPORT commands are followed from above.
