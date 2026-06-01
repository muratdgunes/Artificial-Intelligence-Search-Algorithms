#ifndef CHALLENGE_1_H
#define CHALLENGE_1_H
#include "structer.h"

int get_heuristic(int,int);
int is_valid(int,int);
struct Node * create_node(int,int,int,int,struct Node*);
void print_path(struct Node*);
void A_Star(int,int,int);

#endif //CHALLENGE_1_H