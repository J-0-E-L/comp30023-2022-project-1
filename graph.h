typedef struct graph graph_t;
typedef struct vertex vertex_t;
typedef struct edge edge_t;

graph_t *get_graph();
void free_graph(graph_t *graph);

vertex_t *add_vertex(graph_t *graph, int name);
edge_t *add_edge(graph_t *graph, int name, int start, int end);

vertex_t *find_vertex(graph_t *graph, int name);
edge_t *find_edge(graph_t *graph, int name);

int incidence(edge_t *edge, vertex_t *vertex);
