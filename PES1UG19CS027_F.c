#include "PES1UG19CS027_H.h"
#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include<stdbool.h>
#define MAX_SIZE 255

void createList(adjList **graph,int num_vertices){
    int i;
    *graph=(adjList*)malloc(sizeof(adjList)*(num_vertices+1));
    adjList *gpointer=*graph;
    for(int i=1;i<(num_vertices+1);i++){
        gpointer[i].vertex=i;
        gpointer[i].head=NULL;
    }
}

void insertList(adjList **graph,int sourceVertex,int vertex,int weight){
    adjRowList *p=(*graph)[sourceVertex].head;
    adjRowList *temp=(adjRowList*)malloc(sizeof(adjRowList));
    temp->vertex=vertex;
    temp->weight=weight;
    temp->next=NULL;
    if(p==NULL){
        (*graph)[sourceVertex].head=temp;
    }
    else{
        while(p->next!=NULL){
            p=p->next;
        }
        p->next=temp;
    }
}

void printList(adjList **graph,int num_vertices){
    adjRowList *p;
    for(int i=1;i<num_vertices+1;i++){
        printf("%d ",i);
        p=(*graph)[i].head;
        while(p!=NULL){
            printf("%d ",p->vertex);
            printf("%d ",p->weight);
            p=p->next;
        }
        printf("\n");
    }
}

void readList(adjList **graph,int num_vertices){
    FILE *fp=fopen("adjacencylist.txt","r");
    char str[MAX_SIZE],*c;
    fgets(str,MAX_SIZE,fp);//for just moving to next line
    int sourceVertex,vertex,weight,v;
    while(fgets(str,MAX_SIZE,fp)!=NULL){
        c=str;
        sourceVertex=0;
        while((*c)!=' '){
            sourceVertex=(sourceVertex*10)+((*c)-'0');
            c++;
        }
        c++;
        v=1;
        vertex=0;weight=0;
        while((*c)!='\0'){
            if((*c)=='\n'){
            }
            else if((*c)==' '){
                if(v==1){
                    v=0;
                    weight=0;
                }
                else{
                    insertList(graph,sourceVertex,vertex,weight);
                    vertex=0;
                    v=1;
                }
            }
            else if(v==1){
                vertex=(vertex*10)+((*c)-'0');
            }
            else if((*c)>='0' && (*c)<='9'){
                weight=(weight*10)+((*c)-'0');
            }
            c++;
        }
        insertList(graph,sourceVertex,vertex,weight);//for the last entered vertex
        vertex=0;
        weight=0;
    }
    fclose(fp);
}

void transposeGraph(adjList **graph,int num_vertices){
    adjList *tGraph;
    createList(&tGraph,num_vertices);
    adjRowList *p;
    for(int i=1;i<num_vertices+1;i++){
        p=(*graph)[i].head;
        while(p!=NULL){
            insertList(&tGraph,p->vertex,i,p->weight);
            p=p->next;
        }
    }
    (*graph)=tGraph;
}

minHeapNode* newMinHeapNode(int v,int weight){
    minHeapNode* node =(minHeapNode*)malloc(sizeof(minHeapNode));
    node->vertex = v;
    node->predecessor=-1;
    node->weight = weight;
    return node;
}

MinHeap* createMinHeap(int capacity){
    MinHeap* minHeap =(MinHeap*)malloc(sizeof(MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (minHeapNode**)malloc(capacity*sizeof(minHeapNode*));
    return minHeap;
}

void swapMinHeapNode(minHeapNode** a,minHeapNode** b){
    minHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void decreaseKey(MinHeap* minHeap,int v, int dist,int u){
    int i = minHeap->pos[v];// Get the index of v in  heap array
 
    minHeap->array[i]->weight = dist;
    minHeap->array[i]->predecessor=u;
 
    // Travel up while the complete tree is not hepified.
    //Bottom up Approach
    while (i && minHeap->array[i]->weight <minHeap->array[(i - 1) / 2]->weight)
    {
        minHeap->pos[minHeap->array[i]->vertex] =(i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->vertex] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
 
        i = (i - 1) / 2;
    }
}
 

void minHeapify(struct MinHeap* minHeap,int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size && minHeap->array[left]->weight < minHeap->array[smallest]->weight )
      smallest = left;
 
    if (right < minHeap->size && minHeap->array[right]->weight < minHeap->array[smallest]->weight )
      smallest = right;
 
    if (smallest != idx)
    {
        minHeapNode *smallestNode = minHeap->array[smallest];
        minHeapNode *idxNode = minHeap->array[idx];
 
        //Swap positions
        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
        minHeapify(minHeap, smallest);
    }
}

minHeapNode* extractMin(MinHeap *minHeap)
{
    if (minHeap->size == 0)
        return NULL;
 
    // Store the root node
    minHeapNode* root = minHeap->array[0];
 
    // Replace root node with last node
    minHeapNode* lastNode = minHeap->array[minHeap->size-1];
    minHeap->array[0] = lastNode;
 
    // Update positions
    minHeap->pos[root->vertex] = minHeap->size-1;
    minHeap->pos[lastNode->vertex] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);//heapify at root
    return root;
}

bool isInMinHeap(MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

void dijkstra(adjList** graph, int src,int num_vertices,int dist[num_vertices],int predecessor[num_vertices]){
    --src;
   
    int u=-1;
 
    MinHeap* minHeap = createMinHeap(num_vertices);
 
    // Initialize min heap with dist value of all vertices equal to infinity(i.e, INT_MAX)
    for (int v = 0; v < num_vertices; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v,dist[v]);
        minHeap->pos[v] = v;
        predecessor[v]=-1;
    }

    // Make dist value of src vertex is made 0 so that extractMin extracts src as the first node
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src],-1);
   
    // Initially size of min heap is equal to number of vertices
    minHeap->size = num_vertices;
 
    //Until size is !=0 we keep on updating the distances to their minimum and extract the node with minimum distance from the heap
    while (!(minHeap->size==0))
    {
        minHeapNode* minHeapNode = extractMin(minHeap);
        predecessor[minHeapNode->vertex]=minHeapNode->predecessor;

        u = minHeapNode->vertex;
        u++;

        //Traverse and update the dstance value to their minimum
        adjRowList* p =(*graph)[u].head;
        while (p != NULL)
        {
            int v = p->vertex;
            if (isInMinHeap(minHeap, v-1) && dist[u-1] != INT_MAX && p->weight + dist[u-1] < dist[v-1])
            {
                dist[v-1] = dist[u-1] + p->weight;
 
                decreaseKey(minHeap, v-1, dist[v-1],u);
            }
            p = p->next;
        }
    }
}

void printPath(int num_vertices,int predecessor[num_vertices], int j){
    if (predecessor[j-1] == -1){
        printf("%d ",j);
        return;
    }
  
    printf("%d ", j);
    printPath(num_vertices, predecessor,predecessor[j-1]);
  
}

void printSolution(int dist[], int n, int predecessor[n])
{
    for (int i = 1; i < n; i++)
    {
        printf("%d ",i);
        if(dist[i-1]==INT_MAX){
            printf("NO PATH\n");
        }
        else{
            printPath(n,predecessor,i);
            printf("%d\n",dist[i-1]);
        }
    }
}