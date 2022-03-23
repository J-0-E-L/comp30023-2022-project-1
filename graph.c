#include <stdlib.h>
#include <assert.h>
#include "graph.h"

#define INIT_VERTICES_ALLOCED 8
#define INIT_EDGES_ALLOCED 8

graph_t *get_graph() {
	graph_t *new = (graph_t *)malloc(sizeof(graph_t));
	assert(new);
	new->n_vertices = 0;
	new->n_edges = 0;

	new->vertices = (vertex_t **)calloc(INIT_VERTICES_ALLOCED, sizeof(vertex_t));
	assert(new->vertices);
	new->vertices_alloced = INIT_VERTICES_ALLOCED;

	new->edges = (edge_t **)calloc(INIT_EDGES_ALLOCED, sizeof(edge_t));
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

vertex_t *add_vertex(graph_t *graph, int name) {
	/* Disallow duplicates */
	for (int i = 0; i < graph->n_vertices; i++) { // TODO: allow binary search
		if (graph->vertices[i]->name == name) { // TODO: use a vertex cmp function
			return graph->vertices[i]; // TODO: is this really the best return value?
		}
	}

	vertex_t *new = (vertex_t *)malloc(sizeof(vertex_t));
	assert(new);
	new->name = name;

	if (graph->n_vertices >= graph->vertices_alloced) {
		graph->vertices = (vertex_t **)realloc(graph->vertices, 2 * graph->vertices_alloced * sizeof(vertex_t *));
		assert(graph->vertices);
		graph->vertices_alloced *= 2;
	}
	graph->vertices[graph->n_vertices] = new;
	graph->n_vertices++;
	return new;
}

edge_t *add_edge(graph_t *graph, int name, int start, int end) {
	/* Disallow duplicates */
	for (int i = 0; i < graph->n_edges; i++) { // TODO: allow binary search
		if (graph->edges[i]->name == name && graph->edges[i]->start->name == start && graph->edges[i]->end->name == end) { // TODO: use an edge cmp function
			return graph->edges[i]; // TODO: is this really the best return value?
		}
	}
	edge_t *new = (edge_t *)malloc(sizeof(edge_t));
	assert(new);
	new->name = name;
	new->start = add_vertex(graph, start);
	new->end = add_vertex(graph, end);

	if (graph->n_edges >= graph->edges_alloced) {
		graph->edges = (edge_t **)realloc(graph->edges, 2 * graph->edges_alloced * sizeof(edge_t *));
		assert(graph->edges);
		graph->edges_alloced *= 2;
	}
	graph->edges[graph->n_edges] = new;
	graph->n_edges++;
	return new;
}

vertex_t *find_vertex(graph_t *graph, int name) {
	for (int i = 0; i < graph->n_vertices; i++) { // TODO: binary search
		if (graph->vertices[i]->name == name) {
			return graph->vertices[i];
		}
	}
	return NULL;
}

edge_t *find_edge(graph_t *graph, int name) {
	for (int i = 0; i < graph->n_edges; i++) { // TODO: binary search
		if (graph->edges[i]->name == name) {
			return graph->edges[i];
		}
	}
	return NULL;
}

int incidence(edge_t *edge, vertex_t *vertex) {
	if (vertex == edge->start) return -1;
	if (vertex == edge->end) return 1;
	return 0;
}
