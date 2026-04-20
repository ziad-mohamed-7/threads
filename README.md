# Operating Systems Lab: Pthreads

This project explores multi-threading concepts using the POSIX threads (Pthreads) library. It implements two popular algorithms, Merge Sort and Matrix Multiplication. In Matrix Multiplication, it's using different threading strategies: Per-Element & Per-Row threading, and comparing their  performance.

## Project Structure

- **Merge_sort.c**: Implementation of a multi-threaded merge sort.
- **Matrix_Multiplication.c**: Implementation of multi-threaded matrix multiplication using two different methods.
- **Makefile**: Used to compile both programs.
- **README.md**: This file.
- **Test Files**: (should  be created in same project directory)
  - **input**: Example input file for the Merge Sort program.
  - **input_matrix**: Example input file for the Matrix Multiplication program. 

## How to Compile

A Makefile is provided to compile both C programs.

- To compile the Merge Sort program, run:
  ```sh
  make merge_sort
  ```
- To compile the Matrix Multiplication program, run:
  ```sh
  make matrix_mult
  ```
- To remove the compiled executables:
  ```sh
  make clean
  ```

---

## Part 1: Multi-Threaded Merge Sort

### Description

This program implements the Merge Sort algorithm using Pthreads. Each time the list is divided for the "divide and conquer" step, two new threads are created to sort each half recursively. This continues until the base case (a sub-list of 0 or 1 element) is reached.

### Input Format

The program reads from a file named `input` in the working directory.

- **Line 1:** `[n]` (the number of elements in the array)
- **Line 2:** `[n elements]` (the array elements, space-separated)

**Example input file:**
```
10 
100 20 15 3 4 8 7 -1 0 33
```

### How to Run

- Compile the program:
  ```sh
  make merge_sort
  ```
- Run the executable:
  ```sh
  ./merge_sort
  ```

### Example Output

```
Sorted array:
-1 0 3 4 7 8 15 20 33 100 
```

---

## Part 2: Multi-Threaded Matrix Multiplication

### Description

This program implements matrix multiplication using two different Pthread strategies and compares their performance:

- **Method 1 (Per-Element):** Creates one thread for each element in the result matrix.
- **Method 2 (Per-Row):** Creates one thread for each row in the result matrix.

### Input Format

The program reads from a file named `input_matrix` in the working directory.

- **Line 1:** `[rows_mat1] [cols_mat1]`
- **Next rows_mat1 lines:** The elements of the first matrix.
- **Next line:** `[rows_mat2] [cols_mat2]`
- **Next rows_mat2 lines:** The elements of the second matrix.

**Example `input_matrix` file:**
```
3 5
1 -2 3 4 5
1 2 -3 4 5
-1 2 3 4 5
5 4
-1 2 3 4 1
-2 3 4 1 2
-3 4 1 2 3
-4 -1 -2 -3 -4
```

### How to Run

- Compile the program:
  ```sh
  make matrix_mult
  ```
- Run the executable:
  ```sh
  ./matrix_mult
  ```

### Example Output

The program prints the resulting matrix from Method 1, its elapsed time, the matrix from Method 2, and its elapsed time.

```
-1 10 -15 -28
-3 -10 15 -36
5 -2 -9 -20
END1 648.000000 us
-1 10 -15 -28
-3 -10 15 -36
5 -2 -9 -20
END2 104.000000 us
```

---

## Performance Analysis

As shown in the example output, Method 2 (one thread per row) is significantly faster than Method 1 (one thread per element).

- **Method 1 (Per-Element):** is a slow approach because the cost of creating and managing the threads (where the OS performs context switching) is significantly greater than the tiny amount of computational work each thread actually performs. Most of the 648 microseconds were spent just on overhead.
- **Method 2 (Per-Row):** is a faster approach because the cost of creating the threads is much smaller and there is lesser management overhead. Each thread does a much larger amount of work (calculating an entire row), making that initial creation cost "worth it."

---
