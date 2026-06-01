#include "helper_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct graphHead *createGraph(void) {  //Basic graph creation that we learned in CNG 315
    struct graphHead *head = (struct graphHead*)malloc(sizeof(struct graphHead)); //Memory allocation and check
    if (head==NULL) {
        printf("Memory allocation failed for graph.\n");
        exit(1);
    }
    head->count =0;
    head->first = NULL;
    return  head;
}
struct graphVertex *createVertex(const char *name) { //Basic vertex creation that we learned in CNG 315 take the name and create the vertex with that name
    struct graphVertex *vertex = (struct graphVertex*)malloc(sizeof(struct graphVertex));
    if (vertex==NULL) {
        printf("Memory allocation failed for vertex.\n");
        exit(1);
    }
    strcpy(vertex->vertexName, name);
    vertex->next = NULL;
    vertex->firstArc = NULL;
    vertex->visited = 0;
    return vertex;
}
struct graphArc *createArc(struct graphVertex *destination,int cost) { //Basic arc creation that we learned in CNG 315
    struct graphArc *Arc = (struct graphArc *)malloc(sizeof(struct graphArc));
    if (Arc==NULL) {
        printf("Memory allocation failed for Arc.\n");
        exit(1);
    }
    Arc->cost = cost;  //we are connected the vertexes each other with the specific cost of the path
    Arc->destination = destination; //whcih vertex are wo going to connect
    Arc->next = NULL;
    return  Arc;
}
struct searchNode *createSearchNode(struct graphVertex *vertex, int cost, struct searchNode *parent) { // This function
    //creates a search node to be used in search algorithms like BFS and UCS. The search node holds the actual graph
    //vertex, parent information, total cost,and the next pointer to connect to within the queue.
    struct searchNode *temp = (struct searchNode *)malloc(sizeof(struct searchNode));
    if (temp==NULL) {
        printf("Memory allocation failed for search node.\n");
        exit(1);
    }
    temp->vertex = vertex;  //which graph node this search node represents in this way we can  access the name of the search node
    temp->totalCost = cost;
    temp->parent = parent; //this holds the what node that we come thorugh the that node
    temp->next = NULL;

    return temp;
}
struct Queue *createQueue(void) { //Basic linked-list queue creation that we learned in CNG 213
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    if (queue==NULL) {
        printf("Memory allocation failed for priority queue.\n");
        exit(1);
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}
struct PriorityQueue *createPriorityQueue(void) { //Basic linked-list priorityqueue creation that we learned in CNG 213
    struct PriorityQueue *PriorityQueue = (struct PriorityQueue *)malloc(sizeof(struct PriorityQueue));

    if (PriorityQueue == NULL) {
        printf("Memory allocation failed for priority queue.\n");
        exit(1);
    }
    PriorityQueue->front = NULL;
    return PriorityQueue;
}

void insertVertex(struct graphHead *head, const char *name){ //Basic vertex insertion that we learned in CNG 315
    //This function add a new vertex to the graph
    struct graphVertex *vertex = createVertex(name);
    if (head->first == NULL) {
        head->first = vertex;
    }
    else {
        struct graphVertex *temp = head->first;  //Basic iteration method to reach at the end of the list
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = vertex;
    }
    head->count++;
}
int insertArc(struct graphHead *head, const char *from, const char *to, int cost) //Basic arc insertion that we learned in CNG 315
{   // This function adds a directional connection between two vertices in the graph.
    // "from" is the source vertex, "to" is the destination vertex, and "cost" is the path cost.
    struct graphVertex *fromVertex = VertexFinder(head,from);
    struct graphVertex *toVertex = VertexFinder(head,to);
    if (fromVertex == NULL) {
        printf("From Vertex not found");
        return -2;
    }
    if (toVertex == NULL) {
        printf("To Vertex not found");
        return -3;
    }
    struct graphArc *arc  = createArc(toVertex,cost);
    if (fromVertex->firstArc == NULL) {
        fromVertex->firstArc = arc;
    }
    else {
        struct graphArc *temp = fromVertex->firstArc;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = arc;
    }
    return 1;
}
void enqueue(struct Queue *queue, struct searchNode *node) { //Basic enquue operation that we learned in CNG 213
    //This function add a searchnode inside a queue
    if (isQueueEmpty(queue)) {
        queue->front = node;
        queue->rear = node;
    }
    else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
}
void enqueuePriority(struct PriorityQueue *PriorityQueue, struct searchNode *Node){  // node which will be added to priority queue.

    if (PriorityQueue->front == NULL || Node->totalCost < PriorityQueue->front->totalCost) { //If the queue is empty,
        //place it directly as the first element, or place it first if Nodes cost is smaller
        //than the previous node's cost. So basicly, this function adds the search node to the priority queue in order of cost.
        Node->next = PriorityQueue->front; //
        PriorityQueue->front = Node;
        return;
    }

    struct searchNode *temp = PriorityQueue->front;

    while (temp->next != NULL && temp->next->totalCost <= Node->totalCost) {
        temp = temp->next;
    }

    Node->next = temp->next;
    temp->next = Node;

}

struct searchNode *dequeue(struct Queue *queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }
    struct searchNode *temp = queue->front; //in order to avoid dangling pointer or garbage variable
    queue->front = queue->front->next;
    if (queue->front==NULL) { //This condition is required because if front is null this means queue is empty so we have to equalize rear to null as well
        queue->rear=NULL;
    }
    temp->next = NULL;
    queue->size--;
    return  temp;
}
struct searchNode *dequeuePriority(struct PriorityQueue *PriorityQueue) {
    if (isPriorityQueueEmpty(PriorityQueue)) {
        return NULL;
    }
    struct searchNode *temp = PriorityQueue->front;
    PriorityQueue->front = PriorityQueue->front->next;
    temp->next = NULL;
    return temp;
}

struct graphVertex *VertexFinder(struct graphHead *head, const char *name) {
    // This function searches for vertices by name within the graph.
    // If a vertex is found, it returns its pointer otherwise, it returns NULL.
    struct graphVertex *temp = head->first;
    while (temp != NULL && strcmp(name,temp->vertexName)!=0) {
        temp=temp->next;
    }
    return temp;
}
int isQueueEmpty(struct Queue *queue) {
    if (queue->size ==0) {
        return 1;
    }
    return 0;
}
int isPriorityQueueEmpty(struct PriorityQueue *PriorityQueue) {
    if (PriorityQueue->front== NULL) {
        return 1;
    }
    return 0;
}
int is_challange_room(char*name) {
    if (strcmp(name,"C1")==0 || strcmp(name,"C2")==0) {
        return 1;
    }
    return 0;
}
int which_chllange_room(char* name)
{
    if (is_challange_room(name)) {
        if (strcmp(name,"C1")==0){
            return 1;
        }
    return 2;
    }
}
void resetVisited(struct graphHead *head) {
    // This function resets the visited value of all vertices in the graph.
    struct graphVertex *temp = head->first;
    while (temp!=NULL) {
        temp->visited = 0;
        temp = temp->next;
    }
}

void UCS(struct graphHead *head, const char *start, const char *goal) {
    struct graphVertex *start_vertex = VertexFinder(head,start);  //Find the vertex exist or not
    struct graphVertex *goal_vertex = VertexFinder(head,goal);
    if (start_vertex==NULL) {
        printf("Start_vertex not found");
        return;
    }
    if (goal_vertex==NULL) {
        printf("Goal_vertex not found");
        return;
    }
    resetVisited(head); //Reset the visited statements for vertexes if there was a search before that operation i want to clear the visited flag
    char *solutionPath[10];   //Real Path that we want to  see
    char *tempPath[10];  //Revers path list
    int temp_counter = 0; //how many nodes in that path
    struct PriorityQueue *Queue = createPriorityQueue();
    struct searchNode *start_Node = createSearchNode(start_vertex,0,NULL);
    enqueuePriority(Queue,start_Node);
    while(!isPriorityQueueEmpty(Queue)) {
        struct searchNode *current = dequeuePriority(Queue);
        if (current->vertex->visited == 1) {
            continue;
        }  //IF the node already  visited we skip that vertex
        current->vertex->visited = 1;
        printf("Expanded node : %s\n",current->vertex->vertexName);
        switch (which_chllange_room(current->vertex->vertexName)) {
            case 1:
                printf("You enter the chalange room 1...");
                A_Star(3,3,0);
                printf("You exit the chalange room 1...\n");
            break;
            case 2:
                printf("You enter the chalange room 2...");
                Challenge_Room2();
                printf("You exit the chalange room 2...\n");
            break;
            default:
                break;
        }
        if (current->vertex == goal_vertex) {   //IF We are at the final state
            struct searchNode *temp = current;   // we are checkşing the parents of that node  and save it in the temppath array then reverse it to the solution path
            while (temp!=NULL) {
                tempPath[temp_counter] = temp->vertex->vertexName;
                temp_counter++;
                temp = temp->parent;
            }
            int j = 0;
            for (int i = temp_counter-1; i>=0 ;i--) {
                solutionPath[j] = tempPath[i];
                j++;
            }
            printf("Path is ");
            for (int i = 0;i<j;i++) {
                printf("%s ",solutionPath[i]);
            }
            return;
        }
        struct graphArc *arc = current->vertex->firstArc;

        while (arc != NULL) {  //Checking the all neighbour vertecis
            if (arc->destination->visited == 0) {
                int newCost = current->totalCost + arc->cost;
                struct searchNode *child = createSearchNode(arc->destination, newCost, current);
                // Create a new search node for the neighbor.
                // The parent becomes current, so the path can be established later.
                enqueuePriority(Queue, child);
            }
            arc = arc->next;
        }
    }
}
void BFS(struct graphHead *head,const char* start,const char* goal)
{
    struct graphVertex *start_vertex = VertexFinder(head,start);  //Find the vertex exist or not
    struct graphVertex *goal_vertex = VertexFinder(head,goal);
    if (start_vertex==NULL) {
        printf("Start_vertex not found");
        return;
    }
    if (goal_vertex==NULL) {
        printf("Goal_vertex not found");
        return;
    }
    resetVisited(head);
    char *solutionPath[10];   //Real Path that we want to  see
    char *tempPath[10];  //Revers path list
    int temp_counter = 0; //how many nodes in that path
    struct searchNode *start_Node = createSearchNode(start_vertex,0,NULL);
    struct Queue *queue = createQueue();
    enqueue(queue,start_Node);
    start_vertex->visited = 1;
    while (!isQueueEmpty(queue))
    {
        struct searchNode *temp = dequeue(queue);
        printf("Expanded node : %s\n",temp->vertex->vertexName);
        switch (which_chllange_room(temp->vertex->vertexName)) {
            case 1:
                printf("You enter the chalange room 1...\n");
                A_Star(3,3,0);
                printf("You exit the chalange room 1...\n");
            break;
            case 2:
                printf("You enter the chalange room 2...\n");
                Challenge_Room2();
                printf("You exit the chalange room 2...\n");
            break;
            default:
                break;
        }
        if (temp->vertex == goal_vertex) {   //IF We are at the final state
            struct searchNode *temp_node = temp;   // we are checking the parents of that node  and save it in the temppath array then reverse it to the solution path
            while (temp_node!=NULL) {
                tempPath[temp_counter] = temp_node->vertex->vertexName;
                temp_counter++;
                temp_node = temp_node->parent;
            }
            int j = 0;
            for (int i = temp_counter-1; i>=0 ;i--) {
                solutionPath[j] = tempPath[i];
                j++;
            }
            printf("Path is ");
            for (int i = 0;i<j;i++) {
                printf("%s ",solutionPath[i]);
            }
            return;
        }
        struct graphArc *temp_arc = temp->vertex->firstArc;
        while (temp_arc!=NULL) {
            if (temp_arc->destination->visited==0) {
                temp_arc->destination->visited = 1;
                struct searchNode *newNode = createSearchNode(temp_arc->destination,0,temp);
                enqueue(queue,newNode);
            }
            temp_arc = temp_arc->next;
        }
    }
}