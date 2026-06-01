#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#include "Challenge_1.h"
#include "Challenge_2.h"
#include "structer.h"

struct graphHead *createGraph(void);
struct graphVertex *createVertex(const char*);
struct graphArc *createArc(struct graphVertex*,int);


struct graphVertex *VertexFinder(struct graphHead*,const char *);
void insertVertex(struct graphHead *, const char *);
int insertArc(struct graphHead *, const char *, const char *, int);
void resetVisited(struct graphHead *);
void printGraph(struct graphHead *);
struct searchNode *createSearchNode(struct graphVertex *, int, struct searchNode *);

struct PriorityQueue *createPriorityQueue(void);
void enqueuePriority(struct PriorityQueue *, struct searchNode *);
struct searchNode *dequeuePriority(struct PriorityQueue *);
int isPriorityQueueEmpty(struct PriorityQueue *);

struct Queue *createQueue(void);
void enqueue(struct Queue *, struct searchNode *);
struct searchNode *dequeue(struct Queue *);
int isQueueEmpty(struct Queue *);

int is_challange_room(char*);
int which_chllange_room(char*);

void UCS(struct graphHead *, const char *, const char *);
void BFS(struct graphHead *,const char*,const char*);

#endif //HELPER_FUNCTIONS_H