#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag);
int read_file(char *filename, int cflag);

int main(int argc, char *argv[]) {
	int fflag, eflag, cflag;
	char *filename;

	if (set_flags(argc, argv, &fflag, &filename, &eflag, &cflag)) {
		return 1;
	}

	
	if (read_file(filename, cflag)) {
		return 1;
	}

	return 0;
}


int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag) {
	/* Get command line options - heavily based on the example given at: "https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html" */
	*fflag = 0, *eflag = 0, *cflag = 0;

	opterr = 0;
	int c;
	while ((c = getopt(argc, argv, "f:e")) != -1) {
		switch (c) { 
			case 'f':
				*fflag = 1;
				*filename = optarg;
				break;
			
			case 'e':
				*eflag = 1;
				break;

			case 'c':
				*cflag = 1;
				break;

			case '?':
				if (optopt == 'f') {
					fprintf(stderr, "Option -%c requires an argument.\n", optopt); 
				} else {
					fprintf(stderr, "Invalid option '-%c'.\n", optopt); // TODO: deal with non-printable characters
				}
				return 1;

			default:
				return 1;
		}	
	}

	if (!*fflag) {
		fprintf(stderr, "Option -f is required.\n");
		return 1;
	}	

	return 0;
}

int read_file(char *filename, int cflag) {
	FILE *fp = fopen(filename, "r");	
	if (!fp) {
		fprintf(stderr, "Unable to find file '%s'.\n", filename);
		return 1;
	}
	
	int i, j, k;
	while (fscanf(fp, "%d %d %d\n", &i, &j, &k) == 3) {
		printf("%d %d %d\n", i, j, k); // TODO: load info into a nice data structure
	}

	fclose(fp);
	return 0;
}
