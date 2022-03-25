#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "graph.h"

int set_flags(int argc, char *argv[], int *fflag, char **filename, int *eflag, int *cflag);
graph_t *read_file(char *filename, int cflag);

int has_cycle(graph_t *graph);
int _has_cycle(graph_t *graph, vertex_t *vertex);
int count_cycles(graph_t *graph, edge_t *edge);
int n_trails(graph_t *graph, vertex_t *source, vertex_t *target);

int main(int argc, char *argv[]) {
	int fflag, eflag, cflag;
	char *filename;

	/* Get command line arguments */
	if (set_flags(argc, argv, &fflag, &filename, &eflag, &cflag)) {
		return 1;
	}

	/* Load the target file into a graph structure */
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
	} else {
		/* Task 3, 4, 5 */
		int first = 1;
		while (has_cycle(graph)) {
	
			/* Get the edge which is contained in the most cycles */
			edge_t *best = NULL;
			int max_cycles = -1;
			for (int i = 0; i < graph->n_edges; i++) {
				int n_cycles = count_cycles(graph, graph->edges[i]);
				if (n_cycles > max_cycles || (n_cycles == max_cycles && graph->edges[i]->name < best->name)) {
					best = graph->edges[i];
					max_cycles = n_cycles;
				}
			}
			
			/* Remove the found edge */
			if (first) {
				printf("Deadlock detected\n");
				printf("Terminate %d", best->name);
				first = 0;
			} else {
				printf(" %d", best->name);
			}
			remove_edge(graph, best->name);
		}
		/* Were there any deadlocks? */
		if (first) {
			printf("No deadlocks\n");
		} else {
			printf("\n");
		}
	}
	free_graph(graph);
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

int has_cycle(graph_t *graph) {
	for (int i = 0; i < graph->n_vertices; i++) {
		graph->vertices[i]->tag = 0;
	}

	for (int i = 0; i < graph->n_vertices; i++) {
		if (_has_cycle(graph, graph->vertices[i])) {
			return 1;
		}
	}

	return 0;
}

int _has_cycle(graph_t *graph, vertex_t *vertex) {
	vertex->tag = 1;
	/* Look through all outgoing edges */
	for (int i = 0; i < graph->n_edges; i++) {
		if (incidence(graph->edges[i], vertex) == -1) {

			if (graph->edges[i]->end->tag == 1) {
				return 1;
			}

			if (graph->edges[i]->end->tag == 0 && _has_cycle(graph, graph->edges[i]->end)) {
				return 1;
			}

		}
	}
	vertex->tag = 2;
	return 0;
}

int count_cycles(graph_t *graph, edge_t *edge) {
	for (int i = 0; i < graph->n_vertices; i++) {
		graph->vertices[i]->tag = 0;
	}
	return n_trails(graph, edge->end, edge->start);
}

int n_trails(graph_t *graph, vertex_t *source, vertex_t *target) {
	int out = 0;
	source->tag = 1;
	for (int i = 0; i < graph->n_edges; i++) {
		if (incidence(graph->edges[i], source) == -1) {
			if (incidence(graph->edges[i], target) == 1) {
				out++;
			}
			if (!graph->edges[i]->end->tag) {
				out += n_trails(graph, graph->edges[i]->end, target);
			}
		}
	}
	source->tag = 0;
	return out;
}

