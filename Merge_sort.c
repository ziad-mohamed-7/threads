#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10000

// Struct for thread arguments
typedef struct
{
    int left;
    int right;
    int *arr;
} ThreadArgs;

// Merge function (same as your original)
void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void *threaded_merge_sort(void *arg)
{
    ThreadArgs *t_args = (ThreadArgs *)arg;

    // Base case: one element or empty list
    if (t_args->left >= t_args->right)
    {
        free(t_args); // Freeing allocated space
        return NULL;
    }

    // Recursive case:
    // Calculating mid for splitting
    int mid = (t_args->left + t_args->right) / 2;

    // Allcoating space for arguments of the two halves of the array
    ThreadArgs *t_left_args = malloc(sizeof(ThreadArgs));
    ThreadArgs *t_right_args = malloc(sizeof(ThreadArgs));

    // Initailizing the arguments of the left half
    t_left_args->arr = t_args->arr;
    t_left_args->left = t_args->left;
    t_left_args->right = mid;

    // Initializing the arguments of the right half
    t_right_args->arr = t_args->arr;
    t_right_args->left = mid + 1;
    t_right_args->right = t_args->right;

    // Creating two threads for recurrsively dividing each half
    pthread_t thread_left, thread_right;
    pthread_create(&thread_left, NULL, threaded_merge_sort, t_left_args);
    pthread_create(&thread_right, NULL, threaded_merge_sort, t_right_args);

    // Parent thread waiting for both threads to finish
    pthread_join(thread_left, NULL);
    pthread_join(thread_right, NULL);

    // Merging the two sorted halves
    merge(t_args->arr, t_args->left, mid, t_args->right);

    free(t_args); // Freeing allocated space
    return NULL;
}

int main()
{
    FILE *file = fopen("input", "r");
    if (!file)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);
    int arr[MAX];
    for (int i = 0; i < n; i++)
    {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    // Creating main thread to start sorting process
    ThreadArgs *main_args = malloc(sizeof(ThreadArgs));
    main_args->arr = arr;
    main_args->left = 0;
    main_args->right = n - 1;
    
    // Initializing and starting main thread
    pthread_t main_thread;
    pthread_create(&main_thread, NULL, threaded_merge_sort, main_args);
    
    // Waiting for main thread to finish
    pthread_join(main_thread, NULL);

    printf("Sorted array:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
