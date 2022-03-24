typedef struct vertex {
	int name;
} vertex_t;

typedef struct edge {
	int name;
	vertex_t *start, *end;
} edge_t;

typedef struct graph {
	vertex_t **vertices;
	edge_t **edges;
	int n_vertices, n_edges, vertices_alloced, edges_alloced;
} graph_t;

graph_t *get_graph();
void free_graph(graph_t *graph);

vertex_t *add_vertex(graph_t *graph, int name);
edge_t *add_edge(graph_t *graph, int name, int start, int end);

vertex_t *find_vertex(graph_t *graph, int name);
edge_t *find_edge(graph_t *graph, int name);

int incidence(edge_t *edge, vertex_t *vertex);
