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
typedef struct
{
    int row;
    int col;
    Matrix *mat1;
    Matrix *mat2;
    Matrix *result;
} ElementArgs;

void *calculateElement(void *arg)
{
    ElementArgs *e = (ElementArgs *)arg;

    e->result->data[e->row][e->col] = 0;
    for (int i = 0; i < e->mat1->cols; i++)
    {
        e->result->data[e->row][e->col] += e->mat1->data[e->row][i] * e->mat2->data[i][e->col];
    }

    free(e);
    return NULL;
}

void multiplyMatricesPerElement(Matrix *mat1, Matrix *mat2, Matrix **result)
{
    // Check if multiplication is possible
    if (mat1->cols != mat2->rows)
    {
        printf("Invalid Multiplication!");
        return;
    }

    // Create result matrix
    *result = createMatrix(mat1->rows, mat2->cols);

    // Create threads for each element
    int n = mat1->rows * mat2->cols;
    pthread_t threads[n];

    // Start threads to calculate each element
    for (int i = 0; i < mat1->rows; i++)
    {
        for (int j = 0; j < mat2->cols; j++)
        {
            // Create argument struct for each element calculation
            ElementArgs *e = malloc(sizeof(ElementArgs));
            e->row = i;
            e->col = j;
            e->mat1 = mat1;
            e->mat2 = mat2;
            e->result = *result;
            pthread_create(&threads[i * mat2->cols + j], NULL, calculateElement, e);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return;
}

// Method 2: One thread per row
typedef struct
{
    int row;
    Matrix *mat1;
    Matrix *mat2;
    Matrix *result;
} RowArgs;

void *calculateRow(void *arg)
{
    RowArgs *r = (RowArgs *)arg;
    for (int k = 0; k < r->mat2->cols; k++)
    {
        r->result->data[r->row][k] = 0;
        for (int i = 0; i < r->mat1->cols; i++)
        {
            r->result->data[r->row][k] += r->mat1->data[r->row][i] * r->mat2->data[i][k];
        }
    }

    free(r);
    return NULL;
}

void multiplyMatricesPerRow(Matrix *mat1, Matrix *mat2, Matrix **result)
{
    // Check if multiplication is possible
    if (mat1->cols != mat2->rows)
    {
        printf("Invalid Multiplication!");
        return;
    }

    // Create result matrix
    *result = createMatrix(mat1->rows, mat2->cols);

    // Create threads for each row
    int n = mat1->rows;
    pthread_t threads[n];

    // Start threads to calculate each row
    for (int i = 0; i < n; i++)
    {
        RowArgs *r = malloc(sizeof(RowArgs));
        r->row = i;
        r->mat1 = mat1;
        r->mat2 = mat2;
        r->result = *result;
        pthread_create(&threads[i], NULL, calculateRow, r);
    }

    // Wait for all threads to finish
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return;
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
    struct timeval start, end;
    // Get start time
    gettimeofday(&start, NULL);
    // Multiply matrices using per element threading
    multiplyMatricesPerElement(mat1, mat2, &result1);
    // Get end time
    gettimeofday(&end, NULL);
    // Calculate elapsed time in microseconds
    double micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printMatrix(result1);
    printf("END1 %.2f us\n", micros);

    // Method 2: Per row
    // Get start time
    gettimeofday(&start, NULL);
    // Multiply matrices using per row threading
    multiplyMatricesPerRow(mat1, mat2, &result2);
    // Get end time
    gettimeofday(&end, NULL);
    // Calculate elapsed time in microseconds
    micros = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printMatrix(result2);
    printf("END2 %.2f us\n", micros);

    // Cleanup
    freeMatrix(mat1);
    freeMatrix(mat2);
    freeMatrix(result1);
    freeMatrix(result2);

    return 0;
}
