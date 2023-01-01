#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include"t2.h"
#define max_string 80

vertex * vertices[g_max];

Graph * graph;
int vcount=0;

void init() //intialise all edges of the graph to 0 (no edge)
{
    graph=malloc(sizeof(Graph));
    for(int i=0;i<g_max;i++)
    {
        for(int j=0;j<g_max;j++)
        {
            graph->A[i][j]=0;
        }
    }
}

int next_token (FILE *f, char * buf, int string_max ) { 
   

    int i =0 , fin = 0, inQuotes = 0;

    for(;;)
    {
        buf[i] = fgetc(f);
        if(buf[i] == '"')
        {
            inQuotes = !inQuotes;
            buf[i] = fgetc(f);
        }
        if(buf[i] == ',' && !inQuotes)
            break;
        if (feof(f) || buf[i]=='\n')
        {
            fin = 1;
            break;
        }
        if(i<string_max-1)
        {
            i++;
        }

    }
buf[i]=0;
return fin;
}

edge* get_edge(FILE * file)  //stores and returns edge details from the file
{
    char buff[max_string];
    edge * e = malloc(sizeof(edge));

    next_token(file, buff, max_string);
    e->v1=atoi(buff);
    next_token(file,buff,max_string);
    e->v2=atoi(buff);
    next_token(file,buff,max_string);
    e->weight=atoi(buff);

    return e;
}




vertex * get_vertex(FILE * file)    //stores and returns vertex details from the file
{
    char buff[max_string];

    vertex * v = malloc(sizeof(vertex));

    next_token(file,buff,max_string);
    v->stop_id=atoi(buff);
    next_token(file,v->name,max_string);
    next_token(file,buff,max_string);
    v->latitude=atof(buff);
    next_token(file,buff,max_string);
    v->longitude=atof(buff);

    return v;
}


void add_edge(int from, int to, int weight)  //adds passed edge to adjacency matrix
{
    graph->A[from][to]=graph->A[to][from]=weight;
}


int load_edges(char * fname)  //loads edges sequentially into the matrix
{
    FILE *f;
    int p=0;
    int q=0;
    int weight;
    int ecount=0;

    f=fopen(fname,"r");
    if(!f)
    {
        printf("File %s could not be opened\n", fname);
        return 0;
    }
    get_edge(f);
    init();

    edge * t = malloc(sizeof(edge));
    while(!feof(f))
    {
        t = get_edge(f);
        p = t->v1;
        q = t->v2;
        weight = t->weight;
        add_edge(p,q,weight);
        ecount+=1;
    }

    free(t);
    fclose(f);
    printf("Loaded %d edges\n", ecount);

    return 1;
}

int load_vertices(char * fname)  //loads vertices sequentially into vertex array
{
    FILE * f = fopen(fname, "r");
    int vcount=0;

    f=fopen(fname,"r");
    if(!f)
    {
        printf("Could not open file %s\n", fname);
        return 0;
    }
    get_vertex(f);
    vertex * t = malloc(sizeof(vertex));

    while(!feof(f))
    {
        t = get_vertex(f);
        vertices[t->stop_id] = t;
        vcount+=1;
    }
    t=NULL;
    free(t);
    fclose(f);
    printf("Loaded %d vertices\n", vcount);

    return 1;
}

int get_min(Graph * g, int dist[g_max], int t_f[g_max]) //returns index of min value of current set of unsearched vertices
{
    int min = INT_MAX;
    int min_ind;
    for(int a=0;a<g_max;a++)
    {
        if(t_f[a]==0 && dist[a]<min)
        {    
            min = dist[a];
            min_ind=a;
        }
    }
    return min_ind;
}


void dijkstras_alg(Graph *g, int origin, int dest)
{
    int dist[g_max], check[g_max], prev[g_max];
    for (int i = 0; i < g_max; i++)
    {
        dist[i] = INT_MAX; //set distance to infinity
        check[i] = 0;   
        prev[i] = 0; 
    }
    dist[origin] = 0; // distance to the origin will always be 0

    for (int v = 0; v < g_max; v++)
    {
        int curr = get_min(g, dist, check);  
        check[curr] = 1;                        // set current node index to true

        for (int b = 0; b < g_max; b++)
        {
            if (!check[b] && g->A[curr][b] && dist[curr] != INT_MAX && dist[curr] + g->A[curr][b] < dist[b])
            {
                prev[b] = curr;
                dist[b] = dist[curr] + g->A[curr][b];
            }
        }
    }
    while (prev[dest] != origin)
    {
        printf("%d %s\n", vertices[prev[dest]]->stop_id, vertices[prev[dest]]->name);
        dest = prev[dest];
    }
}



void shortest_path(int startNode, int endNode)      //actual shortest path function which calls dijkstra's function
{
    //swap(&startNode,&endNode);
    printf("%d %s\n", vertices[startNode]->stop_id, vertices[startNode]->name);
    dijkstras_alg(graph,endNode, startNode);
    printf("%d %s\n", vertices[endNode]->stop_id, vertices[endNode]->name);
}


void free_memory()
{
    free(graph);
}


 