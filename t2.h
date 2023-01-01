
#ifndef T2_H_
#define T2_H_
#define g_max 9000

typedef struct vertex{
    int stop_id;
    char name[100];
    double latitude;
    double longitude;
} vertex;


typedef struct edge{
    int v1,v2;
    int weight;
} edge;



typedef struct Graph
{
    int A[g_max][g_max];
} Graph;

int load_edges ( char *fname ); //loads the edges from the CSV file of name fname
int load_vertices ( char *fname );  //loads the vertices from the CSV file of name fname
void shortest_path(int startNode, int endNode); // prints the shortest path between startNode and endNode, if there is any
void free_memory ( void ) ; // frees any memory that was used

#endif
