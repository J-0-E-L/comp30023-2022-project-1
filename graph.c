#include <graph.h>
#include <assert.h>

#define INIT_VERTICES_ALLOCED 8
#define INIT_EDGES_ALLOCED 8

struct graph {
	vertex_t **vertices;
	edge_t **edges;
	int n_vertices, n_edges, vertices_alloced, edges_alloced;
};

struct vertex {
	int name;
};

struct edge {
	int name;
	vertex_t *start, *end;
};

graph_t *get_graph() {
	graph_t *new = (graph_t *graph)malloc(sizeof(graph_t));
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
	// TODO: allow binary search
	vertex_t *new = (vertex_t *)malloc(sizeof(vertex_t));
	assert(new);
	new->name = name;

	if (graph->n_vertices >= graph->vertices_alloced) {
		graph->vertices = (vertex_t **)realloc(2 * graph->n_vertices, sizeof(vertex_t *));
		assert(graph->vertices);
		graph->vertices_alloced *= 2;
	}
	graph->vertices[n_vertices] = new;
	graph->n_vertices++;
	return new;
}

edge_t *add_edge(graph_t *graph, int name, int start, int end) {
	// TODO: allow binary search
	edge_t *new = (edge_t *)malloc(sizeof(edge_t));
	assert(new);
	new->name = name;
	new->start = start;
	new->end = end;

	if (graph->n_edges >= graph->edges_alloced) {
		graph->edges = (edge_t **)realloc(2 * graph->n_edges, sizeof(edge_t *));
		assert(graph->edges);
		graph->edges_alloced *= 2;
	}
	graph->edges[n_edges] = new;
	graph->n_edges++;
	return new;
}

vertex_t *find_vertex(graph_t *graph, int name) {
	for (int i = 0; i < graph->num_vertices; i++) { // TODO: binary search
		if (graph->vertices[i]->name == name) {
			return graph->vertices[i];
		}
	}
	return NULL;
}

edge_t *find_edge(graph_t *graph, int name) {
	for (int i = 0; i < graph->num_edges; i++) { // TODO: binary search
		if (graph->edges[i]->name == name) {
			return graph->edges[i];
		}
	}
	return NULL;
}

int incidence(edge_t *edge, vertex_t *vertex) {
	switch (vertex) {
		case edge->start:
			return -1;
		case edge->end:
			return 1;
		default:
			return 0;
	}
}
