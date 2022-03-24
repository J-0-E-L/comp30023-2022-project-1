#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"

int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag);
graph_t *read_file(char *filename, int cflag);

int main(int argc, char *argv[]) {
	int fflag, eflag, cflag;
	char *filename;

	if (set_flags(argc, argv, &fflag, &filename, &eflag, &cflag)) {
		return 1;
	}

	graph_t *graph = read_file(filename, cflag);	
	if (!graph) {
		return 1;
	}
	/* Task 1 */
	printf("Processes %d\n", graph->n_edges);
	printf("Files %d\n", graph->n_vertices);

	/* Task 2 */
	if (eflag) {
		int time = 0; // TODO: implement this
		printf("Execution time %d\n", time);
	}
	
	return 0;
}


int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag) {
	/* Get command line options - heavily based on the example given at: "https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html" */
	*fflag = 0, *eflag = 0, *cflag = 0;

	opterr = 0;
	int c;
	while ((c = getopt(argc, argv, "f:e")) != -1) { switch (c) { 
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

graph_t *read_file(char *filename, int cflag) {
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "Unable to find file '%s'.\n", filename);
		return NULL;
	}

	graph_t *graph = get_graph();

	int name, file1_name, file2_name;
	while (fscanf(fp, "%d %d %d\n", &name, &file1_name, &file2_name) == 3) {
		add_edge(graph, name, file1_name, file2_name);
	}
	fclose(fp);

	return graph;
}
