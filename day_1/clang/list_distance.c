/* AoC 2024 - syminical */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define HELP_TEXT		"\nUsage: list_distance [FILE]\nPrint the distance between the two lists in FILE.\n\ne.g. FILE:\n   3  4\n   9  5\n   8  7\n\n"
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
		return 0;
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
	unsigned long list_a[n_lines];
	unsigned long list_b[n_lines];
	for (
		size_t i=0;
		(line_len=f_get_line(&in_file, line, MAX_LINE_LEN));
		++i
	) {
		tok_str = strtok(line, " ");
		list_a[i] = strtoul(tok_str, NULL, 0);

		tok_str = strtok(NULL, " ");
		list_b[i] = strtoul(tok_str, NULL, 0);
	}

	/* sort the lists */
	qsort(list_a, n_lines, sizeof(list_a[0]), qsort_compare_ul);
	qsort(list_b, n_lines, sizeof(list_b[0]), qsort_compare_ul);

	/* create a sum of differences between the lists */
	unsigned long sum = 0;
	for (size_t i=0; i < n_lines; ++i) {
		if (list_a[i] > list_b[i])
			sum += list_a[i] - list_b[i];
		else
			sum += list_b[i] - list_a[i];
	}

	printf("%zu\n", sum);

	/* clean up */
	close_files(&in_file);
	return 0;
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
