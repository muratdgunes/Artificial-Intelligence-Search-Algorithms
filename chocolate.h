#ifndef CHOCOLATE_H
#define CHOCOLATE_H

#define MAX_CHOCOLATE_SIZE 4 //WE HIGHLY RECCOMMEND TO USE IT WITH SIZE 4 Because if you make 5 AI will takes a lot of time while expanding nodes (approximately 3 minutes for each 5x4 and approximately few hours for each 5x5 board but at the end it will calculate it)

struct ChocolateGame {
    int rows;
    int cols;
    int poison_row;
    int poison_col;
    int board[MAX_CHOCOLATE_SIZE][MAX_CHOCOLATE_SIZE];
};

struct Move {
    int row;
    int col;
    int found;
    int node_count;
};

void initialize_chocolate_game(struct ChocolateGame *, int , int , int , int );
void print_chocolate_board(struct ChocolateGame *);
int eat_poison(struct ChocolateGame *, int , int);
int islegal(struct ChocolateGame *, int , int );
int haslegal(struct ChocolateGame *);
void apply_chocolate_move(struct ChocolateGame *, int , int );
void copy_chocolate_game(struct ChocolateGame *, struct ChocolateGame *);
int minimax(struct ChocolateGame *, int );
struct Move *find_best_ai_move(struct ChocolateGame *, int );
void read_game_information(int, int, int, int);
void play_chocolate_human_vs_human(void);
void play_chocolate_human_vs_ai(void);
struct Move *get_random_legal_move(struct ChocolateGame *);
void run_lara_vs_ai_simulation(int );
int max_value(struct ChocolateGame *);
int min_value(struct ChocolateGame *);
#endif
