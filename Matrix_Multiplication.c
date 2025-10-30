#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Structure to hold matrix data
typedef struct
{
    int rows;
    int cols;
    int **data;
} Matrix;

// Function to allocate matrix
Matrix *createMatrix(int rows, int cols)
{
    // Allocate space for matrix structure
    Matrix *m = malloc(sizeof(Matrix));
    // Allocate space for array of row pointers
    m->data = malloc(rows * sizeof(int *));
    // Allocate space for each row
    for (int i = 0; i < rows; i++)
    {
        m->data[i] = malloc(cols * sizeof(int));
    }

    m->rows = rows;
    m->cols = cols;

    return m;
}

// Function to free matrix
void freeMatrix(Matrix *mat)
{
    // Free allocated space for each row
    for (int i = 0; i < mat->rows; i++)
    {
        free(mat->data[i]);
    }
    // Free allocated space for the array of row pointers
    free(mat->data);
    // Free allocated space for the matrix structure
    free(mat);

    return;
}

// Function to read matrices from file
int readMatrices(Matrix **mat1, Matrix **mat2)
{
    FILE *file = fopen("input_matrix", "r");
    if (!file)
    {
        printf("Error opening file.\n");
        return -1;
    }

    int row1, row2, col1, col2;

    fscanf(file, "%d", &row1);
    fscanf(file, "%d", &col1);

    *mat1 = createMatrix(row1, col1);
    if (!*mat1)
        return -1;
    
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col1; j++)
        {
            fscanf(file, "%d", &(*mat1)->data[i][j]);
        }
    }
    
    fscanf(file, "%d", &row2);
    fscanf(file, "%d", &col2);

    *mat2 = createMatrix(row2, col2);
    if (!*mat2)
        return -1;

    for (int i = 0; i < row2; i++)
    {
        for (int j = 0; j < col2; j++)
        {
            fscanf(file, "%d", &(*mat2)->data[i][j]);
        }
    }

    fclose(file);
    return 0;
}

// Function to print matrix
void printMatrix(Matrix *mat)
{
    for (int i = 0; i < mat->rows; i++)
    {
        for (int j = 0; j < mat->cols; j++)
        {
            printf("%d", mat->data[i][j]);
            if (j < mat->cols - 1)
                printf(" ");
        }
        printf("\n");
    }
}

// Method 1: One thread per element
void multiplyMatricesPerElement(Matrix *mat1, Matrix *mat2, Matrix **result)
{
}

// Method 2: One thread per row
void multiplyMatricesPerRow(Matrix *mat1, Matrix *mat2, Matrix **result)
{
}

int main()
{

    Matrix *mat1, *mat2, *result1, *result2;

    // Read matrices from file
    if (readMatrices(&mat1, &mat2) != 0)
    {
        fprintf(stderr, "Error reading matrices\n");
        return 1;
    }

    // Method 1: Per element
    multiplyMatricesPerElement(mat1, mat2, &result1);
    printMatrix(result1);

    // Method 2: Per row
    multiplyMatricesPerRow(mat1, mat2, &result2);
    printMatrix(result2);

    // Cleanup
    freeMatrix(mat1);
    freeMatrix(mat2);
    freeMatrix(result1);
    freeMatrix(result2);

    return 0;
}
