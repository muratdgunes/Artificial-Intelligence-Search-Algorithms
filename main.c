#include <stdio.h>
#include "helper_functions.h"
#include "Challenge_2.h"
#include "chocolate.h"

int main(void) {

    struct graphHead *myGraph = createGraph();

    insertVertex(myGraph,"S");
    insertVertex(myGraph,"C1");
    insertVertex(myGraph,"F");
    insertVertex(myGraph,"C2");
    insertVertex(myGraph,"E");
    insertArc(myGraph, "S", "C1", 99);
    insertArc(myGraph, "S", "F", 80);
    insertArc(myGraph, "F", "C2", 97);
    insertArc(myGraph, "C1", "E", 211);
    insertArc(myGraph, "C2", "E", 101);
    int choice;

    printf("Welcome to Agent Lara's Rescue Mission ----> choose the Algorithm to Apply:\n1) UCS\n2) BFS\n3) Poisonous Chocolate - Human vs Human\n4) Poisonous Chocolate - Human vs AI\nChoice: ");
    scanf("%d",&choice);
    printf("\n");
    while (1) {
        switch (choice) {
            case 1:
                UCS(myGraph, "S", "E");
            return 0;
            case 2:
                BFS(myGraph,"S","E");
            return 0;

            case 3:
                play_chocolate_human_vs_human();
            return 0;

            case 4:
                play_chocolate_human_vs_ai();
            return 0;

            default:
                printf("Please choose the Algorithm to Apply:\n1) UCS\n2) BFS\nChoice:  ");
            scanf("%d",&choice);
            printf("\n");
            break;
        }
    }
}
