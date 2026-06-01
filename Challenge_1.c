#include <stdio.h>
#include <stdlib.h>
#include "Challenge_1.h"

int get_heuristic(int G, int P) {
    return (G + P) / 2; // since someone needs to ride the boat, integer division is okay here. This will be admissible.
}
int is_valid(int G, int P) {
    if (G < 0 || G > 3 || P < 0 || P > 3) // if guards or prisoners are bigger than 3 or less than 0, return false.
        return 0;

    if (G > 0 && G < P) // guards must not be outnumbered at any times, return false if so. (this is for left bank)
        return 0;

    // Right bank constraint: Guards must not be outnumbered if present
    int rightBank_guards = 3 - G;
    int rightBank_prisoners = 3 - P;
    if (rightBank_guards > 0 && rightBank_guards < rightBank_prisoners) // guards must not be outnumbered at any times, return false if so. (right side of the bank)
        return 0;

    return 1; // when program reaches here, then no problem, return true.
}

struct Node * create_node(int guards, int prisoners, int boat, int c, struct Node* parent) {
    struct Node * new = (struct Node*)malloc(sizeof(struct Node));
    new->G = guards;
    new->P = prisoners;
    new->boat_position = boat;
    new->cost = c;
    new->h = get_heuristic(guards, prisoners);
    new->fn = new->cost + new->h; // f(n) = g(n) + h(n)
    new->parent = parent; // is to know where we came from.

    return new;
}
void print_path(struct Node* node) { // recursively print the solution path (Initial State -> Goal)
    if (node == NULL)
        return;

    print_path(node->parent);

    const char* boat_pos;
    if (node->boat_position == 0) // 0 is for left, 1 is for right.
        boat_pos = "Left";
    else
        boat_pos = "Right";


    printf("State: (%dG, %dP, %s) | | f(n)=%d (c=%d, h=%d)\n", node->G, node->P, boat_pos, node->fn, node->cost, node->h);
}
void A_Star(int start_G, int start_P, int start_B) {
    // There are 5 possible boat actions, 1 guard, 2 guards, 1 prisoner, 2 prisoners, 1 prisoner & 1 guard.
    int actions[5][2] = {{1, 0},{2, 0},{0, 1},{0, 2},{1, 1}};

    // Setup our Open List (waiting line) and Closed List (visited history)
    struct Node* expansion[1000];
    int expansion_count = 0;
    int visited[4][4][2] = {0};

    // 1. Put the starting state into the open list
    expansion[expansion_count] = create_node(start_G,start_P,start_B,0,NULL);
    expansion_count++;

    while (expansion_count > 0) {

        // find the lowest cost by comparing fn (cost + heuristic).
        int best = 0;
        for (int i = 1; i < expansion_count; i++) {
            if (expansion[i]->fn < expansion[best]->fn) {
                best = i;
            }
        }
        struct Node* current = expansion[best];


        for (int i = best; i < expansion_count - 1; i++) {
            expansion[i] = expansion[i + 1]; // shift expansion by one.
        }
        expansion_count--;


        if (current->G == 0 && current->P == 0 && current->boat_position == 1) { // if 0 guards & 0 prisoners in the left and boat is in the right, then we won.
            printf("\n--- Goal Reached! Solution Path ---\n");
            print_path(current);
            return; // go back.
        }

        visited[current->G][current->P][current->boat_position] = 1; // we flag this state as visited. We dont want to repeat our moves.


        for (int i = 0; i < 5; i++) { // try all boat actions one by one
            int move_G = actions[i][0];
            int move_P = actions[i][1];
            int next_G, next_P, next_B;

            if (current->boat_position == 0) { // if 0, then boat is on the left side, going to right.
                next_G = current->G - move_G;
                next_P = current->P - move_P;
                next_B = 1; // set as right
            }
            else { // else, then boat is on the right side, going to left.
                next_G = current->G + move_G;
                next_P = current->P + move_P;
                next_B = 0; // set as left
            }


            if (is_valid(next_G, next_P) && visited[next_G][next_P][next_B] == 0) {
                // if prisoners are not exceeding guards && state is not visited before, then it is valid move.
                int new_cost = current->cost + 1;

                // create this new, unvisited node.
                expansion[expansion_count] = create_node(next_G,next_P,next_B,new_cost,current);
                expansion_count++;
            }
        }
    }

    // If the loop finishes and we are here, the open list emptied without finding a goal
    printf("No solution found.\n");
}