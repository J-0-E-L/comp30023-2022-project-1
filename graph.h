typedef struct graph graph_t;
typedef struct vertex vertex_t;
typedef struct edge edge_t;

vertex_t *find_vertex(int name);
edge_t *find_edge(int name);

vertex_t *add_vertex(int name);
edge_t *add_edge(int name, int start, int end);

int is_incident(edge_t *edge, vertex_t *vertex, int directed);
