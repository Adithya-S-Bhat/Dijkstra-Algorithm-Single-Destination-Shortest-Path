#include "PES1UG19CS027_H.h"
#include<stdio.h>
#include<stdlib.h>
void main(){
    adjList *graph;
    int num_vertices,destVertex;
    FILE *fp;
    if((fp=fopen("adjacencylist.txt","r"))==NULL){
        exit(1);
    }

    fscanf(fp,"%d",&num_vertices);
    fclose(fp);
    destVertex=num_vertices;
    int dist[num_vertices],predecessor[num_vertices];

    createList(&graph,num_vertices);
    readList(&graph,num_vertices);

    transposeGraph(&graph,num_vertices);
    dijkstra(&graph,destVertex,num_vertices,dist,predecessor);
    printSolution(dist,num_vertices,predecessor);
}