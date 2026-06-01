#ifndef STRUCTER_H
#define STRUCTER_H
#define MAX_ROOMS 100
#define MAX_NEIGHBORS 10
#define MAX_NAME 10

struct graphArc {
    struct graphVertex *destination;
    struct graphArc *next;
    int cost;
};

struct graphVertex {
    struct graphVertex *next;
    char vertexName[10];
    struct graphArc *firstArc;
    int visited;
};

struct graphHead {
    int count;
    struct graphVertex *first;
};

struct searchNode {
    struct graphVertex *vertex;
    struct searchNode *parent;
    struct searchNode *next;
    int totalCost;
};

struct PriorityQueue {
    struct searchNode *front;
};

struct Queue {
    struct searchNode *front;
    struct searchNode *rear;
    int size;
};

struct Node { // for challenge room 1.
    int G;                // guards on left side.
    int P;                // prisoners in left side.
    int boat_position;    // boat position. 0=left, 1=right
    int cost;             // cost = number of moves.
    int h;                // heuristic value.
    int fn;               // f(n) = g(n) + h(n) in other words fn = cost + heuristic.
    struct Node * parent;  // to go back where we came from. TO find path.
};


struct Room { // for challenge room 2.
    char name[MAX_NAME];        // e.g., "A2", "B1"
    int x;                      // X coordinate
    int y;                      // Y coordinate
    struct Room* neighbors[MAX_NEIGHBORS]; // pointers for connected rooms (neighbors)
    int neighbor_count;         // How many connections this room actually has
    // for Greedy Best First Search
    int h;                      // manhattan distance to target
    struct Room* parent;        // to trace back.
    int visited;                // 0 = unvisited, 1 = visited (for graph search)
};

struct Labyrinth {
    struct Room *rooms[MAX_ROOMS];
    int total_room;

    char mad_scientist_room[MAX_NAME];
    char exit_room[MAX_NAME];
};

#endif //STRUCTER_H