#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100
#define NAME_LEN 50
#define INF INT_MAX

// Structure for adjacency list node
typedef struct AdjListNode {
    int dest;
    int distance;
    struct AdjListNode* next;
} AdjListNode;

// Structure for graph
typedef struct Graph {
    int numVertices;
    char locationNames[MAX][NAME_LEN];
    AdjListNode* adjLists[MAX];
} Graph;

// Create adjacency list node
AdjListNode* createNode(int dest, int distance) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->distance = distance;
    newNode->next = NULL;
    return newNode;
}

// Create a graph
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = 0;
    for (int i = 0; i < MAX; i++)
        graph->adjLists[i] = NULL;
    return graph;
}

// Get index of location by name
int getLocationIndex(Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->locationNames[i], name) == 0)
            return i;
    }
    return -1;
}

// Add a location
void addLocation(Graph* graph, const char* name) {
    if (graph->numVertices >= MAX) {
        printf("Maximum number of locations reached.\n");
        return;
    }

    if (getLocationIndex(graph, name) != -1) {
        printf("Location already exists.\n");
        return;
    }

    strcpy(graph->locationNames[graph->numVertices], name);
    graph->adjLists[graph->numVertices] = NULL;
    graph->numVertices++;
    printf("Location '%s' added.\n", name);
}

// Add a road (bidirectional)
void addRoad(Graph* graph, const char* srcName, const char* destName, int distance) {
    int src = getLocationIndex(graph, srcName);
    int dest = getLocationIndex(graph, destName);

    if (src == -1 || dest == -1) {
        printf("One or both locations not found. Please add them first.\n");
        return;
    }

    // src -> dest
    AdjListNode* node = createNode(dest, distance);
    node->next = graph->adjLists[src];
    graph->adjLists[src] = node;

    // dest -> src
    node = createNode(src, distance);
    node->next = graph->adjLists[dest];
    graph->adjLists[dest] = node;

    printf("Road added between '%s' and '%s' with distance %d.\n", srcName, destName, distance);
}

// Minimum distance node
int minDistance(int dist[], bool visited[], int n) {
    int min = INF, min_index = -1;
    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

// Print path
void printPath(int parent[], int j, Graph* graph) {
    if (parent[j] == -1) {
        printf("%s", graph->locationNames[j]);
        return;
    }
    printPath(parent, parent[j], graph);
    printf(" -> %s", graph->locationNames[j]);
}

// Dijkstra's Algorithm
void findShortestPath(Graph* graph, const char* srcName, const char* destName) {
    int src = getLocationIndex(graph, srcName);
    int dest = getLocationIndex(graph, destName);

    if (src == -1 || dest == -1) {
        printf("Invalid source or destination.\n");
        return;
    }

    int V = graph->numVertices;
    int dist[MAX];
    bool visited[MAX];
    int parent[MAX];

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited, V);
        if (u == -1) break;

        visited[u] = true;

        AdjListNode* temp = graph->adjLists[u];
        while (temp) {
            int v = temp->dest;
            if (!visited[v] && dist[u] != INF &&
                dist[u] + temp->distance < dist[v]) {
                dist[v] = dist[u] + temp->distance;
                parent[v] = u;
            }
            temp = temp->next;
        }
    }

    if (dist[dest] == INF) {
        printf("No path exists from %s to %s.\n", srcName, destName);
    } else {
        printf("Shortest distance from %s to %s is %d\n", srcName, destName, dist[dest]);
        printf("Path: ");
        printPath(parent, dest, graph);
        printf("\n");
    }
}

// Display the map
void displayMap(Graph* graph) {
    printf("\n--- Map ---\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%s -> ", graph->locationNames[i]);
        AdjListNode* temp = graph->adjLists[i];
        while (temp) {
            printf("(%s, %d) ", graph->locationNames[temp->dest], temp->distance);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Main menu
int main() {
    Graph* map = createGraph();
    int choice;
    char name1[NAME_LEN], name2[NAME_LEN];
    int distance;

    while (1) {
        printf("\n=== Map Navigation System ===\n");
        printf("1. Add Location\n");
        printf("2. Add Road Between Locations\n");
        printf("3. Display Map\n");
        printf("4. Find Shortest Path\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter location name: ");
                fgets(name1, NAME_LEN, stdin);
                name1[strcspn(name1, "\n")] = 0;
                addLocation(map, name1);
                break;
            case 2:
                printf("Enter source location: ");
                fgets(name1, NAME_LEN, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("Enter destination location: ");
                fgets(name2, NAME_LEN, stdin);
                name2[strcspn(name2, "\n")] = 0;
                printf("Enter distance between them: ");
                scanf("%d", &distance);
                getchar();
                addRoad(map, name1, name2, distance);
                break;
            case 3:
                displayMap(map);
                break;
            case 4:
                printf("Enter source location: ");
                fgets(name1, NAME_LEN, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("Enter destination location: ");
                fgets(name2, NAME_LEN, stdin);
                name2[strcspn(name2, "\n")] = 0;
                findShortestPath(map, name1, name2);
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
