#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chocolate.h"

int visited_node_count = 0;// counts how many nodes AI visits in Minimax

void initialize_chocolate_game(struct ChocolateGame *game, int rows, int cols, int poison_row, int poison_col) {
    game->rows = rows;
    game->cols = cols;
    game->poison_row = poison_row;
    game->poison_col = poison_col;
    //we initilize the chocolate squares and make them available
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            game->board[i][j] = 1; // 1 means square exists
        }
    }
}

void print_chocolate_board(struct ChocolateGame *game) {
    printf("\nCurrent Chocolate Board:\n");
    printf("P = Poisonous Square, O = Available Square, . = Eaten Square\n\n");

    printf("   ");
    for (int j = 0; j < game->cols; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < game->rows; i++) {
        printf("%d  ", i);

        for (int j = 0; j < game->cols; j++) {
            if (game->board[i][j] == 0) {
                printf(". ");
            }
            else if (i == game->poison_row && j == game->poison_col) {
                printf("P ");
            }
            else {
                printf("O ");
            }
        }

        printf("\n");
    }

    printf("\n");
}

int eat_poison(struct ChocolateGame *game, int row, int col) {
    if (row <= game->poison_row && col <= game->poison_col) {
        return 1;
    }
    //we arem checking the if the choosedn rectangle contains posion square or not
    return 0;
}

int islegal(struct ChocolateGame *game, int row, int col) {
    if (row < 0 || row >= game->rows || col < 0 || col >= game->cols) { //we are checking the board boundries
        return 0;
    }

    if (game->board[row][col] == 0) { // we are checking if the square al ready taken
        return 0;
    }

    if (row == game->poison_row && col == game->poison_col) { // we can not select the poisonous square
        return 0;
    }

    if (eat_poison(game, row, col)) { // As we mention earlier this function is checking if the choosen rectangle contaiopns square or not
        return 0;
    }

    return 1; //If none of this rule are violated this means this is legal move
}

int haslegal(struct ChocolateGame *game) { // search all board cells for at least one legal move because if there is no more legal room we are finisihng the game
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (islegal(game, i, j)) {
                return 1;
            }
        }
    }

    return 0;
}

void apply_chocolate_move(struct ChocolateGame *game, int row, int col) { //this is the main fucntion that doing the removing selected square and below and also right of it
    for (int i = row; i < game->rows; i++) {
        for (int j = col; j < game->cols; j++) {
            if (!(i == game->poison_row && j == game->poison_col)) {
                game->board[i][j] = 0;
            }
        }
    }
}

void copy_chocolate_game(struct ChocolateGame *copy, struct ChocolateGame *original) { //this is essential because AI will use it to make a desicion with out break the original board.
    copy->rows = original->rows;
    copy->cols = original->cols;
    copy->poison_row = original->poison_row;
    copy->poison_col = original->poison_col;


    for (int i = 0; i < original->rows; i++) {
        for (int j = 0; j < original->cols; j++) {
            copy->board[i][j] = original->board[i][j];
        }
    }
}

void read_game_information(int rows, int cols, int poison_row, int poison_col) { // We are checking the if the values is valid for creating the game
    if (rows < 2 || rows > MAX_CHOCOLATE_SIZE || cols < 2 || cols > MAX_CHOCOLATE_SIZE) {
        printf("\nInvalid board size.Board size must be between 2 and %d",MAX_CHOCOLATE_SIZE);;
        exit(1);
    }

    if (poison_row < 0 || poison_row >= rows || poison_col < 0 || poison_col >= cols) {
        printf("\nInvalid poisonous square location.");
        exit(1);
    }
}

struct Move *get_random_legal_move(struct ChocolateGame *game) { //This function is reposnsible for random moves for LARA
    struct Move *LARA_random = (struct Move*)malloc(sizeof(struct Move));
    if (LARA_random == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    LARA_random->row = -1;
    LARA_random->col = -1;
    LARA_random->found = 0;
    LARA_random->node_count = 0;

    int count = 0;
    //First we count the how many legal moves we have
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (islegal(game, i, j)) {
                count++;
            }
        }
    }

    if (count == 0) {
        return LARA_random;
    }
    //Then between them we assign random index
    int random_index = rand() % count;
    int current_index = 0;

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (islegal(game, i, j)) {
                if (current_index == random_index) {
                    LARA_random->row = i;
                    LARA_random->col = j;
                    LARA_random->found = 1;
                    return LARA_random;
                }

                current_index++;
            }
        }
    }

    return LARA_random;
}

int minimax(struct ChocolateGame *game, int current_player) {
    visited_node_count++;
    //if current player has no legal move, current player loses
    if (!haslegal(game)) {
        if (current_player == 1) {
            return -1;
        }
        else {
            return 1;
        }
    }
    if (current_player == 1) {
        return max_value(game);
    }
    else {
        return min_value(game);
    }
}

int max_value(struct ChocolateGame *game) {
    int best_value = -999; //We assign great negative value just like the pseduo code
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (islegal(game, i, j)) {
                struct ChocolateGame *child = (struct ChocolateGame*)malloc(sizeof(struct ChocolateGame));
                if (child == NULL) {
                    printf("Memory allocation failed.\n");
                    exit(1);
                }
                // copy current state and apply move
                copy_chocolate_game(child, game);
                apply_chocolate_move(child, i, j);
                int value = minimax(child, -1); // after MAX move, MIN plays
                free(child);
                if (value > best_value) {
                    best_value = value;
                }
            }
        }
    }
    return best_value;
}

int min_value(struct ChocolateGame *game) {
    int best_value = 999;

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (islegal(game, i, j)) {
                struct ChocolateGame *child = (struct ChocolateGame*)malloc(sizeof(struct ChocolateGame));
                if (child == NULL) {
                    printf("Memory allocation failed.\n");
                    exit(1);
                }
                // copy current state and apply move
                copy_chocolate_game(child, game);
                apply_chocolate_move(child, i, j);

                int value = minimax(child, 1); // after MIN move, MAX plays
                free(child);

                if (value < best_value) {
                    best_value = value;
                }
            }
        }
    }

    return best_value;
}

struct Move *find_best_ai_move(struct ChocolateGame *game, int ai_player) {
    struct Move *best_move = (struct Move*)malloc(sizeof(struct Move));

    if (best_move == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    best_move->row = -1;
    best_move->col = -1;
    best_move->found = 0;
    best_move->node_count = 0;

    int best_value;

    visited_node_count = 0; // We are reseting node counter before AI decision
    //We dived the if the AI player is MAX or MIN then did the same operaiton that we did before in a different way at the end.
    if (ai_player == 1) {
        best_value = -999;

        for (int i = 0; i < game->rows; i++) { // trying all legal moves
            for (int j = 0; j < game->cols; j++) {
                if (islegal(game, i, j)) {
                    struct ChocolateGame *child = (struct ChocolateGame*)malloc(sizeof(struct ChocolateGame));

                    if (child == NULL) {
                        printf("Memory allocation failed.\n");
                        exit(1);
                    }
                    // copy current board and apply AI move
                    copy_chocolate_game(child, game);
                    apply_chocolate_move(child, i, j);

                    int value = minimax(child, -1); // after MAX AI move, MIN plays

                    free(child);

                    if (best_move->found == 0 || value > best_value) { // selection of  better move for MAX
                        best_value = value;
                        best_move->row = i;
                        best_move->col = j;
                        best_move->found = 1;
                    }
                }
            }
        }
    }
    else {
        best_value = 999;

        for (int i = 0; i < game->rows; i++) {
            for (int j = 0; j < game->cols; j++) {
                if (islegal(game, i, j)) {
                    struct ChocolateGame *child = (struct ChocolateGame*)malloc(sizeof(struct ChocolateGame));

                    if (child == NULL) {
                        printf("Memory allocation failed.\n");
                        exit(1);
                    }

                    copy_chocolate_game(child, game);
                    apply_chocolate_move(child, i, j);

                    int value = minimax(child, 1);

                    free(child);

                    if (best_move->found == 0 || value < best_value) {
                        best_value = value;
                        best_move->row = i;
                        best_move->col = j;
                        best_move->found = 1;
                    }
                }
            }
        }
    }

    best_move->node_count = visited_node_count;

    return best_move;
}

void play_chocolate_human_vs_human(void) {
    int rows;
    int cols;
    int poison_row;
    int poison_col;

    printf("Enter row count: ");
    scanf("%d", &rows);

    printf("\nEnter column count: ");
    scanf("%d", &cols);

    printf("\nEnter poisonous square row: ");
    scanf("%d", &poison_row);

    printf("\nEnter poisonous square column: ");
    scanf("%d", &poison_col);

    read_game_information(rows, cols, poison_row, poison_col);

    struct ChocolateGame *game = (struct ChocolateGame*)malloc(sizeof(struct ChocolateGame));
    if (game == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    initialize_chocolate_game(game, rows, cols, poison_row, poison_col);

    int current_player = 1; // Player 1 starts as MAX
    int row;
    int col;
    int game_over = 0;
    int Player=1;
    printf("\nHuman vs Human mode started.\n");
    print_chocolate_board(game);

    while (haslegal(game) && game_over == 0) {

        if (current_player==1) {  //For printing purposes we are assigning 1 for the Max is Player 1 and 0 for the MIN is Player 2
            Player = 1;
        }
        else {
            Player = 2;
        }
        printf("Player %d turn. Enter row and column: ", Player);
        scanf("%d %d", &row, &col);

        if (islegal(game, row, col)) {

            printf("Player %d selected (%d,%d)\n", Player, row, col);

            apply_chocolate_move(game, row, col);
            print_chocolate_board(game);

            if (!haslegal(game)) {
                printf("Player %d wins.\n", Player);

                if (current_player == 1) {
                    printf("Player 2 is forced to eat the poisonous square and loses.\n");
                }
                else {
                    printf("Player 1 is forced to eat the poisonous square and loses.\n");
                }

                game_over = 1;
            }
            else {
                current_player = -current_player;
            }
        }
        else {
            printf("Invalid move. Please try again.\n");
        }
    }

    free(game);
}

void play_chocolate_human_vs_ai(void) {
    int rows;
    int cols;
    int poison_row;
    int poison_col;

    printf("Enter row count: ");
    scanf("%d", &rows);

    printf("\nEnter column count: ");
    scanf("%d", &cols);

    printf("\nEnter poisonous square row: ");
    scanf("%d", &poison_row);

    printf("\nEnter poisonous square column: ");
    scanf("%d", &poison_col);

    read_game_information(rows, cols, poison_row, poison_col);

    struct ChocolateGame *game = (struct ChocolateGame*)malloc(sizeof(struct ChocolateGame));
    if (game == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    initialize_chocolate_game(game, rows, cols, poison_row, poison_col);

    char answer;
    int human_player;
    int ai_player;

    printf("\nDo you want to start first? y/n: ");
    scanf(" %c", &answer);

    if (answer == 'y' || answer == 'Y') {  //We are deciding here who will be max
        human_player = 1;  //Max Player
        ai_player = -1;    //Min Player
    }
    else {
        ai_player = 1;      //Max Player
        human_player = -1; //Min Player

    }

    int current_player = 1;
    int row;
    int col;
    int game_over = 0;

    printf("\nHuman vs AI mode started.\n");
    print_chocolate_board(game);

    while (haslegal(game)&& game_over==0) {
        int move_ready = 0;
        if (current_player == human_player) {
            printf("Player turn. Enter row and column: ");
            scanf("%d %d", &row, &col);

            if (islegal(game, row, col)) {
                printf("Player selected (%d,%d)\n", row, col);
                move_ready = 1;
            }
            else {
                printf("Invalid move. Please try again.\n");
            }
        }
        else {
            struct Move *ai_move = find_best_ai_move(game, ai_player);

            if (ai_move->found == 1) {
                row = ai_move->row;
                col = ai_move->col;

                printf("AI selected (%d,%d)\n", row, col);
                printf("AI visited node count: %d\n", ai_move->node_count);

                move_ready = 1;
            }
            else {
                game_over = 1;
            }

            free(ai_move);
        }
        if (move_ready == 1 && game_over == 0) { // apply move only if move is valid
            apply_chocolate_move(game, row, col);
            print_chocolate_board(game);

            if (!haslegal(game)) {
                if (current_player == human_player) {
                    printf("Player wins.\n");
                    printf("AI is forced to eat the poisonous square and loses.\n");
                }
                else {
                    printf("AI wins.\n");
                    printf("Player is forced to eat the poisonous square and loses.\n");
                }

                game_over = 1;
            }
            else {
                current_player = -current_player;  //Chaning the players
            }
        }
    }

    free(game);
}

void run_lara_vs_ai_simulation(int number_of_games) {
    int lara_wins = 0;
    int ai_wins = 0;
    int total_node_count = 0;

    srand(time(NULL));

    printf("\n----- Lara vs AI Poisonous Chocolate Simulation -----\n");

    for (int game_number = 1; game_number <= number_of_games; game_number++) { // runing for 100 games
        int rows = 2 + rand() % (MAX_CHOCOLATE_SIZE - 1); //Ramdomized boards rows and columns for Similation
        int cols = 2 + rand() % (MAX_CHOCOLATE_SIZE - 1);

        struct ChocolateGame *game = (struct ChocolateGame*)malloc(sizeof(struct ChocolateGame));
        if (game == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }

        initialize_chocolate_game(game, rows, cols, 0, 0);
        int lara_player;
        int temp = rand() % 2;  //Deciding the if LARA is starting or the AI is starting  by generation 0 and 1 if it is 1 lara will start if it is 0 AI will start fot that specific similation
        if (temp == 1) {
            lara_player = 1;
        }
        else {
            lara_player = -1;
        }
        int ai_player = -lara_player;   //Assingin the AI opposite player tag
        int current_player = 1;
        int move_count = 1;
        int game_over = 0;
        int row;
        int col;


        printf("\nGame %d started. Board size: %dx%d\n", game_number, rows, cols);
        if (lara_player == 1) {
            printf("Lara starts First\n");
        }
        else {
            printf("AI starts First\n");
        }
        print_chocolate_board(game);

        while (haslegal(game) && game_over == 0) {
            if (current_player == lara_player) {
                struct Move *lara_move = get_random_legal_move(game); // Lara chooses random legal move

                if (lara_move->found == 1) {
                    row = lara_move->row;
                    col = lara_move->col;

                    printf("Move %d: Lara selected (%d,%d)\n", move_count, row, col);

                    apply_chocolate_move(game, row, col);
                    print_chocolate_board(game);
                }
                else {
                    game_over = 1;
                }

                free(lara_move);
            }
            else {
                struct Move *ai_move = find_best_ai_move(game, ai_player); // AI chooses best minimax move

                if (ai_move->found == 1) {
                    row = ai_move->row;
                    col = ai_move->col;

                    total_node_count = total_node_count + ai_move->node_count;

                    printf("Move %d: AI selected (%d,%d)\n", move_count, row, col);
                    printf("AI visited node count: %d\n", ai_move->node_count);

                    apply_chocolate_move(game, row, col);
                    print_chocolate_board(game);
                }
                else {
                    game_over = 1;
                }

                free(ai_move);
            }

            if (game_over == 0) {  // checking if game ended after move
                if (!haslegal(game)) {
                    if (current_player == lara_player) {
                        lara_wins++;
                        printf("Game %d result: Lara wins.\n", game_number);
                    }
                    else {
                        ai_wins++;
                        printf("Game %d result: AI wins.\n", game_number);
                    }

                    game_over = 1;
                }
                else {
                    current_player = -current_player; // switching  player as we said earlier
                    move_count++;
                }
            }
        }

        free(game);
    }

    printf("\n----- Simulation Result -----\n");
    printf("Total game count: %d\n", number_of_games);
    printf("Lara win count: %d\n", lara_wins);
    printf("AI win count: %d\n", ai_wins);
    printf("Lara winning percentage: %.2f\n", ((float)lara_wins / number_of_games) * 100);
    printf("AI winning percentage: %.2f\n", ((float)ai_wins / number_of_games) * 100);
    printf("Total AI visited node count: %d\n", total_node_count);
}