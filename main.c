#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"

int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag);
graph_t *read_file(char *filename, int cflag);

int count_cycles(graph_t *graph, edge_t *edge);
int _count_cycles(graph_t *graph, vertex_t *target, vertex_t *source);

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
	
	/* Task 3, 4, 5 */
	int *n_cycles = (int *)malloc(graph->n_edges * sizeof(int));

	for (int i = 0; i < graph->n_edges; i++) {
		n_cycles[i] = count_cycles(graph, graph->edges[i]);
	}

	// TODO: use n_cycles info
	for (int i = 0; i < graph->n_edges; i++) {
		printf("%d\n", n_cycles[i]);
	}

	free(n_cycles);

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

int count_cycles(graph_t *graph, edge_t *edge) {
	int n_cycles = 0;
	vertex_t *source = edge->end, *target = edge->start;
	
	n_cycles = _count_cycles(graph, target, source);
	
	return n_cycles;
}

int _count_cycles(graph_t *graph, vertex_t *target, vertex_t *source) {
	int n_cycles = 0;
	for (int i = 0; i < graph->n_edges; i++) {
		if (graph->edges[i]->start == source) {
			if (graph->edges[i]->end == target) {
				n_cycles++;
			} else {
				n_cycles += _count_cycles(graph, target, graph->edges[i]->end);
			}
		}
	}
	return n_cycles;
}
