
//Each node in adjacency list
typedef struct adjacencyRowList{
    int vertex;
    int weight;
    struct adjacencyRowList *next;
}adjRowList;

//adjacency list
typedef struct adjacencyColumnList{
    int vertex;
    adjRowList *head;
}adjList;

//Each node in Min Heap
typedef struct minHeapNode{
    int vertex;
    int weight;
    int predecessor;
}minHeapNode;

typedef struct MinHeap
{
    int size;//number of heap nodes    
    int capacity; 
    int *pos;//for storing position/index of a vertex in the heap
    minHeapNode **array;
}MinHeap;

//Function Prototype
void createList(adjList **graph,int num_vertices);//for creating Adjacency List
void insertList(adjList **graph,int sourceVertex,int vertex,int weight);//for inserting a node into adjacency list
void readList(adjList **graph,int num_vertices);//for reading input from the file
void printList(adjList **graph,int num_vertices);//for printing adjacency list
//for obtaining the transpose of the graph as the given problem is Single Destination Shortest Path which is opposite of Dijkstra problem
void transposeGraph(adjList **graph,int num_vertices);

MinHeap* createMinHeap(int capacity);//to create Min Heap Node
MinHeap* createMinHeap(int capacity);//to create a Min Heap
void swapMinHeapNode(minHeapNode** a,minHeapNode** b);//for swapping heap nodes
void decreaseKey(MinHeap* minHeap,int v, int dist,int u);//To decrease distance value of a given vertex v and to update its predecessor
void minHeapify(struct MinHeap* minHeap,int idx);//heapify at given index and also updates position of nodes when they are swapped.
minHeapNode* extractMin(MinHeap *minHeap);//To delete/extract minimum node from heap based on the weight/distance
//bool isInMinHeap(MinHeap *minHeap, int v);//to check if a given vertex 'v' is in min heap or not

void dijkstra(adjList** graph, int src,int num_vertices,int dist[num_vertices],int predecessor[num_vertices]);

void printPath(int num_vertices,int predecessor[num_vertices], int j);//print shortest path from source to j
void printSolution(int dist[], int num_vertices,int predecessor[num_vertices]);