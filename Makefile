merge_sort: Merge_sort.c
	@gcc -o merge_sort Merge_sort.c -lpthread

clean:
	@rm -f merge_sort