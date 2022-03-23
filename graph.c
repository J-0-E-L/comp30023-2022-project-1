#include <graph.h>

struct graph {
	vertex_t **vertices;
	edge_t **edges;
	int n_vertices, n_edges, n_vertices_alloced, n_edges_alloced;
};

struct vertex {
	int name;
};

struct edge {
	int name;
	vertex_t *start, *end;
};

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

vertex_t *add_vertex(graph_t *graph, int name) {
	// TODO: allow binary search
	vertex_t *new = (vertex_t *)malloc(sizeof(vertex_t));
	new->name = name;

	if (graph->n_vertices >= graph->n_vertices_alloced) {
		graph->vertices = (vertex_t **)realloc(2 * graph->n_vertices, sizeof(vertex_t *));
		graph->n_vertices_alloced *= 2;
	}
	graph->vertices[n_vertices] = new;
	graph->n_vertices++;
	return new;
}

edge_t *add_edge(graph_t *graph, int name, int start, int end);
	// TODO: allow binary search
	edge_t *new = (edge_t *)malloc(sizeof(edge_t));
	new->name = name;
	new->start = start;
	new->end = end;

	if (graph->n_edges >= graph->n_edges_alloced) {
		graph->edges = (edge_t **)realloc(2 * graph->n_edges, sizeof(edge_t *));
		graph->n_edges_alloced *= 2;
	}
	graph->edges[n_edges] = new;
	graph->n_edges++;
	return new;
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
