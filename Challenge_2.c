#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Challenge_2.h"
#include "chocolate.h"

void initialize_labyrinth(struct Labyrinth *Labyrinth) {
    Labyrinth->total_room =0;
    for (int i=0;i<MAX_ROOMS; i++) {
        Labyrinth->rooms[i]=NULL;
    }

    strcpy(Labyrinth->mad_scientist_room,"");
    strcpy(Labyrinth->exit_room,"");
}

void initialize_room(struct Room *room) { // initialize everything, set to null and zero.
    room->visited =0;
    room->neighbor_count =0;
    room->parent =NULL;
    room->h = 0;
    for (int i = 0;i<MAX_NEIGHBORS;i++) {
        room->neighbors[i] = NULL;
    }
}

struct Room* Room_Finder(struct Labyrinth *Labyrinth ,char* search_name) {
    for (int i = 0; i < Labyrinth->total_room; i++) {
        if (strcmp(Labyrinth->rooms[i]->name,search_name) == 0)
        {
            return Labyrinth->rooms[i];
        }
    }
}

void build_labyrinth(struct Labyrinth *Labyrinth, const char* filename) {
    FILE* file = fopen(filename, "r"); // open file as "file".
    char temp_name[10];
    int temp_x, temp_y; // varibales for x and y coordinates of a room.
    char n1[10], n2[10], n3[10], n4[10]; // variables for neighbors.
    char current_name[10];
    int ignore_x, ignore_y; // just dummy variables, will be ignored.

    if (file == NULL) {
        printf("Error: Could not open %s\n", filename);
        exit(1);
    }

    char line[256];


    while (fgets(line, sizeof(line), file)) { // get a whole line.
        line[strcspn(line, "\n")] = 0; // strcspn removes new line character at the end of string.
        if (strncmp(line, "mad scientest", 13) == 0) {
            sscanf(line, "mad scientest %s", Labyrinth->mad_scientist_room);
            continue; // if line contains "mad scientist" assign mad scientist room and continue with loop. We are done in this line.
        }
        if (strncmp(line, "Exit", 4) == 0) {
            sscanf(line, "Exit %s", Labyrinth->exit_room);
            continue; // if line contains "exit" assign exit room and continue with loop. We are done in this line.
        }



        if (sscanf(line, "%s %d %d", temp_name, &temp_x, &temp_y) >= 3) {
            // Ex: A1 0 1 (A1 room in coordinates (0,1))
            // if we successfully match it, read the room and assign it as a new room.
            // the reason why we first assign rooms A1(1,2), B1(2,3) etc and not room neighbors is because neighbor are not
            // known, first we initialize rooms then we will proceed with neighbors.

            struct Room* new_room = (struct Room*)malloc(sizeof(struct Room)); // malloc new room.

            strcpy(new_room->name, temp_name); // assign values of the new room.
            new_room->x = temp_x;
            new_room->y = temp_y;
            initialize_room(new_room);
            Labyrinth->rooms[Labyrinth->total_room] = new_room;
            Labyrinth->total_room++; // always increment total room counter after assigning a new room.
        }
    }

    fseek(file, 0, SEEK_SET); // this function basically asks program to read from the beginning of the file.

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // strcspn removes new line character at the end of string.

        // Skip the mad scientist and exit lines, right now we are only interested in neighbors.
        if (strncmp(line, "mad", 3) == 0 || strncmp(line, "Exit", 4) == 0) {
            continue;
        }


        // If there are exactly 4 neighbors, assign neighbors for that current room one by one...
        if (sscanf(line, "%s %d %d %s %s %s %s", current_name, &ignore_x, &ignore_y, n1, n2, n3, n4) == 7) {
            struct Room* current_room = Room_Finder(Labyrinth,current_name);
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n1);
            current_room->neighbor_count++;
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n2);
            current_room->neighbor_count++;
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n3);
            current_room->neighbor_count++;
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n4);
            current_room->neighbor_count++;
        }

        // If there are exactly 3 neighbors, assign neighbors for that current room.
        else if (sscanf(line, "%s %d %d %s %s %s", current_name, &ignore_x, &ignore_y, n1, n2, n3) == 6) {
            struct Room* current_room = Room_Finder(Labyrinth,current_name);
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n1);
            current_room->neighbor_count++;
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n2);
            current_room->neighbor_count++;
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n3);
            current_room->neighbor_count++;
        }

        // If there are exactly 2 neighbors, assign neighbors for that current room.
        else if (sscanf(line, "%s %d %d %s %s", current_name, &ignore_x, &ignore_y, n1, n2) == 5) {
            struct Room* current_room = Room_Finder(Labyrinth,current_name);
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n1);
            current_room->neighbor_count++;
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n2);
            current_room->neighbor_count++;
        }

        // If there is exactly 1 neighbor, assign neighbor for current room.
        else if (sscanf(line, "%s %d %d %s", current_name, &ignore_x, &ignore_y, n1) == 4) {
            struct Room* current_room = Room_Finder(Labyrinth,current_name);
            current_room->neighbors[current_room->neighbor_count] = Room_Finder(Labyrinth,n1);
            current_room->neighbor_count++;
        }
    }
    fclose(file);
}

int get_manhattan(struct Room* current, struct Room* target) {
    int x = 0;
    int y = 0;
    if (current->x>target->x) { // to avoid negative values.
        x = current->x - target->x;
    }
    else {
        x =  target->x - current->x;
    }
    if (current->y > target->y) {
        y = current->y - target->y;
    }
    else {
        y= target->y - current->y;
    }
    return  x+y;
}

void print_labyrinth_path(struct Room* node) {
    if (node == NULL) { // if no parent, return
        return;
    }
    print_labyrinth_path(node->parent); // goes to the root node recursively.
    printf("%s -> ", node->name); // prints starting from root node.
}

void Greedy_Best_First(struct Labyrinth *Labyrinth,char* start_name, char* goal_name) {
    struct Room* start = Room_Finder(Labyrinth,start_name);
    struct Room* goal = Room_Finder(Labyrinth,goal_name);

    if (start == NULL || goal == NULL) {
        printf("Error: Could not find start or goal room!\n");
        return;
    }

    struct Room* Expansion_list[100];
    int Expansion_count = 0;

    start->h = get_manhattan(start, goal); // initial.
    Expansion_list[Expansion_count] = start;
    Expansion_count++;
    start->visited = 1; // mark as visited.

    printf("Expansion order: ");

    while (Expansion_count > 0) {

        int best = 0;
        for (int i = 1; i < Expansion_count; i++) {
            if (Expansion_list[i]->h < Expansion_list[best]->h) { // find lowest h(n). Greedy will only follow heuristic.
                best= i;
            }
        }

        struct Room* current = Expansion_list[best];


        for (int i = best; i < Expansion_count - 1; i++) { // shift whole array by 1. similar to A*.
            Expansion_list[i] = Expansion_list[i + 1];
        }
        Expansion_count--;

        printf("%s ", current->name); // Print expansion


        if (current == goal) { // if goal is found, print path.

            printf("\nSolution path: ");
            print_labyrinth_path(current);

            printf("Arrived!\n\n");
            if (strcmp(current->name,Labyrinth->mad_scientist_room)==0) {
                current->parent =NULL; // set parent as null because we will print 1- start -> mad scientist, 2- mad scientist -> exit.
            }
            return;
        }

        // Expand neighbors
        for (int i = 0; i < current->neighbor_count; i++) {
            struct Room* neighbor = current->neighbors[i];

            if (neighbor->visited == 0) { // only visit unvisited nodes.
                neighbor->visited = 1; // mark unvisited node as visited node.
                neighbor->parent = current;
                neighbor->h = get_manhattan(neighbor, goal);

                Expansion_list[Expansion_count] = neighbor;
                Expansion_count++;
            }
        }
    }
    printf("\nFailed to find a path.\n\n");
}


void Challenge_Room2(){
    struct Labyrinth *labyrinth = (struct Labyrinth*) malloc(sizeof(struct Labyrinth));
    initialize_labyrinth(labyrinth);
    printf("Loading Labyrinth Map...\n");
    build_labyrinth(labyrinth,"challenge2.txt");

    printf("\n----- Step 1 - Lara (A2) finding Mad Scientist (%s) -----\n",labyrinth->mad_scientist_room);
    Greedy_Best_First(labyrinth,"A2", labyrinth->mad_scientist_room);
    printf("ASSIGNMENT 2 SIMILATION STARING\n");
    run_lara_vs_ai_simulation(100);
    printf("ASSIGNMENT 2  SIMILATION FINISHED\n");

    printf("----- Step 2 - Mad Scientist (%s) finding Exit (%s) -----\n", labyrinth->mad_scientist_room, labyrinth->exit_room);
    Greedy_Best_First(labyrinth,labyrinth->mad_scientist_room, labyrinth->exit_room);

    for (int i = 0; i < labyrinth->total_room; i++) // free memory allocation (for every single room)
        free(labyrinth->rooms[i]);

    free(labyrinth);
}