#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"

#define INIT_VERTICES_ALLOCED 4
#define INIT_EDGES_ALLOCED 4

graph_t *get_graph() {
	graph_t *new = (graph_t *)malloc(sizeof(graph_t));
	assert(new);
	new->n_vertices = 0;
	new->n_edges = 0;

	new->vertices = (vertex_t **)malloc(INIT_VERTICES_ALLOCED * sizeof(vertex_t *));
	assert(new->vertices);
	new->vertices_alloced = INIT_VERTICES_ALLOCED;

	new->edges = (edge_t **)malloc(INIT_EDGES_ALLOCED * sizeof(edge_t *));
	assert(new->edges);
	new->edges_alloced = INIT_EDGES_ALLOCED;
	
	return new;
}

void free_graph(graph_t *graph) {
	for (int i = 0; i < graph->n_vertices; i++) {
		free(graph->vertices[i]);
	}

	for (int i = 0; i < graph->n_edges; i++) {
		free(graph->edges[i]);
	}
	free(graph);
}

int add_vertex(graph_t *graph, int name) {
	/* Disallow duplicates */
	int i = find_vertex(graph, name);
	if (i != -1) {
		return i;
	}

	vertex_t *new = (vertex_t *)malloc(sizeof(vertex_t));
	assert(new);
	new->name = name;

	if (graph->n_vertices >= graph->vertices_alloced) {
		//printf("vertices %d -> %d\n", graph->vertices_alloced, graph->vertices_alloced * 2);
		graph->vertices = (vertex_t **)realloc(graph->vertices, 2 * graph->vertices_alloced * sizeof(vertex_t *));
		assert(graph->vertices);
		graph->vertices_alloced *= 2;
	}
	graph->vertices[graph->n_vertices] = new;
	graph->n_vertices++;
	return graph->n_vertices - 1;
}

int add_edge(graph_t *graph, int name, int start, int end) {
	/* Disallow duplicates */
	int i = find_edge(graph, name);
	if (i != -1) {
		return i;
	}
	
	edge_t *new = (edge_t *)malloc(sizeof(edge_t));
	assert(new);
	new->name = name;
	new->start = add_vertex(graph, start);
	new->end = add_vertex(graph, end);

	if (graph->n_edges >= graph->edges_alloced) {
		//printf("edges %d -> %d\n", graph->edges_alloced, graph->edges_alloced * 2);
		graph->edges = (edge_t **)realloc(graph->edges, 2 * graph->edges_alloced * sizeof(vertex_t *));
		assert(graph->edges);
		graph->edges_alloced *= 2;
	}
	graph->edges[graph->n_edges] = new;
	graph->n_edges++;
	return graph->n_edges - 1;
}

int find_vertex(graph_t *graph, int name) {
	for (int i = 0; i < graph->n_vertices; i++) { // TODO: binary search
		if (graph->vertices[i]->name == name) {
			return i;
		}
	}
	return -1;
}

int find_edge(graph_t *graph, int name) {
	for (int i = 0; i < graph->n_edges; i++) { // TODO: binary search
		if (graph->edges[i]->name == name) {
			return i;
		}
	}
	return -1;
}

int incidence(graph_t *graph, int edge, int vertex) {
	assert(0 <= edge && edge < graph->n_edges);
	assert(0 <= vertex && vertex < graph->n_vertices);
	if (graph->edges[edge]->start == vertex) return -1;
	if (graph->edges[edge]->end == vertex) return 1;
	return 0;
}
