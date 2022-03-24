typedef struct vertex {
	int name;
} vertex_t;

typedef struct edge {
	int name, start, end;
} edge_t;

typedef struct graph {
	vertex_t **vertices;
	edge_t **edges;
	int n_vertices, n_edges, vertices_alloced, edges_alloced;
} graph_t;

graph_t *get_graph();
void free_graph(graph_t *graph);

int add_vertex(graph_t *graph, int name);
int add_edge(graph_t *graph, int name, int start, int end);

int find_vertex(graph_t *graph, int name);
int find_edge(graph_t *graph, int name);

int incidence(graph_t *graph, int edge_index, int vertex_index);
