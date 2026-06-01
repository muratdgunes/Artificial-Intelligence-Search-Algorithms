#ifndef CHALLENGE_2_H
#define CHALLENGE_2_H
#include "structer.h"



void initialize_labyrinth(struct Labyrinth *);
void initialize_room(struct Room *);
struct Room* Room_Finder(struct Labyrinth *, char *s);
void build_labyrinth(struct Labyrinth *, const char *);
int get_manhattan(struct Room *, struct Room *);
void print_labyrinth_path(struct Room *);
void Greedy_Best_First(struct Labyrinth *, char *, char *);
void Challenge_Room2(void);



#endif //CHALLENGE_2_H
