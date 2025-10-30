merge_sort: Merge_sort.c
	@gcc -o merge_sort Merge_sort.c -lpthread

matrix_mult: Matrix_Multiplication.c
	@gcc -o matrix_mult Matrix_Multiplication.c -lpthread

clean:
	@rm -f merge_sort matrix_mult