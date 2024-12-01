/* AoC 2024 - syminical */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "map.h"
#include "map.c"



#define HELP_TEXT		"\nUsage: list_similarity [FILE]\nPrint the similarity score for the two lists in FILE.\n\ne.g. FILE:\n   3  4\n   9  5\n   8  7\n\n"
#define MAX_LINE_LEN	15	/* $ wc -L FILE  (don't forget the \n and the \0) */



/* file io */
void	open_files (FILE **in_file, char *path);
void	close_files (FILE **in_file);
size_t	f_get_line (FILE **in_file, char *line_buffer, size_t max_len);

/* input processing */
int qsort_compare_ul(const void *i, const void *j);



int main (size_t arg_count, char *args[arg_count])
{
	/* ensure the input file is provided */
	if (arg_count < 2) {
		printf(HELP_TEXT);
		exit(0);
	}

	FILE *in_file;
	open_files(&in_file, args[1]);

	/* find out how many lines the file has */
	size_t line_len;
	size_t n_lines = 0;
	char line[MAX_LINE_LEN];
	while ((line_len=f_get_line(&in_file, line, MAX_LINE_LEN)) != 0) {
		n_lines += 1;
	}

	/* populate the lists */
	rewind(in_file);
	char *tok_str;
	char list_a[n_lines][MAX_LINE_LEN / 2];
	char terrible_hashmap_library_bandaid[n_lines][MAX_LINE_LEN / 2];
	hashmap *list_b = hashmap_create(); /* str: size_t */
	uintptr_t hm_result;
	int hm_error;
	for (
		size_t i=0;
		(line_len=f_get_line(&in_file, line, MAX_LINE_LEN)) != 0;
		++i
	) {
		/* add str to list_a */
		tok_str = strtok(line, " ");
		memcpy(list_a[i], tok_str, strlen(tok_str)+1);

		/* list_b stuff */
		tok_str = strtok(NULL, " \n");
		memcpy(terrible_hashmap_library_bandaid[i], tok_str, strlen(tok_str)+1);
		/* check if key exists in list_b */
		if (hashmap_get(list_b, terrible_hashmap_library_bandaid[i],
				strlen(terrible_hashmap_library_bandaid[i]), &hm_result)) {
			/* exists, so update counter */
			hm_error = hashmap_set(list_b, terrible_hashmap_library_bandaid[i],
				strlen(terrible_hashmap_library_bandaid[i]), (size_t)hm_result + 1);
		} else {
			/* does not exist, so add it */
			hm_error = hashmap_set(list_b, terrible_hashmap_library_bandaid[i],
				strlen(terrible_hashmap_library_bandaid[i]), (size_t)1);
		}
		if (hm_error == -1)
			fprintf(stderr, "hashmap_set: %s\n", strerror(errno));
	}

	/* create a sum of similarity scores */
	unsigned long sum = 0;
	for (size_t i=0; i < n_lines; ++i) {
		if (hashmap_get(list_b, list_a[i], strlen(list_a[i]), &hm_result)) {
			sum += strtoul(list_a[i], NULL, 10) * (size_t)hm_result;
		}
	}

	printf("%zu\n", sum);

	/* clean up */
	close_files(&in_file);
	hashmap_free(list_b);
	exit(0);
}


/*
   - read a line from in_file into s limited by max_len
   - includes trailing '\n'
   = return functional length of s (not including the terminating '\0')
*/
size_t f_get_line (FILE **in_file, char *line_buffer, size_t max_len)
{
	int c;
	size_t i;
	for (
		i=0;
			i < max_len
			&& (c=fgetc(*in_file)) != EOF
			&& c != '\n'
		;++i
	) {
		line_buffer[i] = c;
	}
	if (c == '\n') {
		line_buffer[i] = c;
		++i;
	}
	line_buffer[i] = '\0';
	return i;
}


/*
	- check if the file exists
	- open the file stream for in_file
*/
void open_files (FILE **in_file, char *path)
{
	/* open in_file as "r" and make sure it exists */
	*in_file = fopen(path, "r");
	if (*in_file == NULL) {
		printf("%s does not exist!\n", path);
		exit(1);
	}
}


/* realistically this is not needed */
void close_files (FILE **in_file)
{
	// good-boy bloat :)
	fclose(*in_file);
}


/*
	https://stackoverflow.com/a/1788048
	https://www.w3schools.com/c/ref_stdlib_qsort.php
	[1]: i > j, [0]: equal, [-1]: j > i
*/
int qsort_compare_ul(const void *a, const void *b) {
	const unsigned long *m = a;
	const unsigned long *n = b;
	return (*m > *n) - (*m < *n);
}
