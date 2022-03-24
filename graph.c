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

vertex_t *add_vertex(graph_t *graph, int name) {
	assert(graph);
	/* Disallow duplicates */
	vertex_t *old = find_vertex(graph, name);
	if (old) {
		return old;
	}

	vertex_t *new = (vertex_t *)malloc(sizeof(vertex_t));
	assert(new);
	new->name = name;
	new->tag = 0;

	if (graph->n_vertices >= graph->vertices_alloced) {
		//printf("vertices %d -> %d\n", graph->vertices_alloced, graph->vertices_alloced * 2);
		graph->vertices = (vertex_t **)realloc(graph->vertices, 2 * graph->vertices_alloced * sizeof(vertex_t *));
		assert(graph->vertices);
		graph->vertices_alloced *= 2;
	}
	graph->vertices[graph->n_vertices] = new;
	graph->n_vertices++;
	return new;
}

edge_t *add_edge(graph_t *graph, int name, int start, int end) {
	assert(graph);
	/* Disallow duplicates */
	edge_t *old = find_edge(graph, name);
	if (old) {
		return old;
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
	return new;
}

//int remove_vertex(graph_t *graph, int name);

int remove_edge(graph_t *graph, int name) {
	for (int i = 0; i < graph->n_edges; i++) {
		if (graph->edges[i]->name == name) {
			free(graph->edges[i]);
			graph->edges[i] = graph->edges[graph->n_edges - 1];
			graph->edges[graph->n_edges - 1] = NULL; // TODO: am I really morally obligated to do this?
			graph->n_edges--;
			return 1;
		}
	}
	return 0;
}

vertex_t *find_vertex(graph_t *graph, int name) {
	assert(graph);
	for (int i = 0; i < graph->n_vertices; i++) {
		if (graph->vertices[i]->name == name) {
			return graph->vertices[i];
		}
	}
	return NULL;
}

edge_t *find_edge(graph_t *graph, int name) {
	assert(graph);
	for (int i = 0; i < graph->n_edges; i++) {
		if (graph->edges[i]->name == name) {
			return graph->edges[i];
		}
	}
	return NULL;
}

int incidence(edge_t *edge, vertex_t *vertex) {
	assert(edge), assert(vertex);
	
	if (edge->start == vertex) return -1;
	if (edge->end == vertex) return 1;
	return 0;
}
