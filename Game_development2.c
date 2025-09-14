#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // for random computer moves

// -------------------- Function Prototypes --------------------
char** createBoard(int n);
void showBoard(char** board, int n);
int validMove(char** board, int n, int row, int col);
int checkWin(char** board, int n, char player);
int checkDraw(char** board, int n);
void freeBoard(char** board, int n);
void logBoard(FILE* logFile, char** board, int n);

// -------------------- Main Function --------------------
int main() {
    int n, humans;

    // Ask for board size
    printf("Enter board size (3–10): ");
    scanf("%d", &n);

    if (n < 3 || n > 10) {
        printf("Invalid size! Must be between 3 and 10.\n");
        return 0;
    }

    // Ask how many human players
    printf("Enter number of human players (1–3): ");
    scanf("%d", &humans);

    if (humans < 1 || humans > 3) {
        printf("Invalid choice! Must be 1, 2, or 3.\n");
        return 0;
    }

    // Create board and open log file
    char** board = createBoard(n);
    FILE* logFile = fopen("tictactoe_log.txt", "w");
    if (!logFile) {
        printf("Could not open log file.\n");
        return 1;
    }

    // Player symbols
    char players[3] = {'X', 'O', 'Z'};

    int row, col;
    int finished = 0;
    int turn = 0; // 0 = X, 1 = O, 2 = Z

    srand(time(NULL)); // seed for random moves

    // -------------------- Game Loop --------------------
    while (!finished) {
        showBoard(board, n);
        char current = players[turn];

        printf("Player %c's turn\n", current);

        // Check if this player is human or computer
        if (turn < humans) {
            // Human player input
            printf("Enter row and column (1-%d): ", n);
            scanf("%d %d", &row, &col);
            row--; col--; // adjust to 0-based
        } else {
            // Computer player makes a random move
            do {
                row = rand() % n;
                col = rand() % n;
            } while (!validMove(board, n, row, col));
            printf("Computer (%c) chooses: %d %d\n", current, row + 1, col + 1);
        }

        // Check if move is valid
        if (validMove(board, n, row, col)) {
            board[row][col] = current; // place symbol

            // Log current board
            logBoard(logFile, board, n);

            // Check win
            if (checkWin(board, n, current)) {
                showBoard(board, n);
                printf("🎉 Player %c wins!\n", current);
                fprintf(logFile, "Player %c wins!\n", current);
                finished = 1;
            }
            // Check draw
            else if (checkDraw(board, n)) {
                showBoard(board, n);
                printf("🤝 Game is a draw!\n");
                fprintf(logFile, "Game is a draw!\n");
                finished = 1;
            }
            // Next turn (cycle 0→1→2→0)
            else {
                turn = (turn + 1) % 3;
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }

    fclose(logFile);
    freeBoard(board, n);
    return 0;
}

// -------------------- Functions --------------------

// Create an empty board dynamically
char** createBoard(int n) {
    char** board = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        board[i] = (char*)malloc(n * sizeof(char));
        for (int j = 0; j < n; j++) {
            board[i][j] = ' '; // empty space
        }
    }
    return board;
}

// Display the board with row/col numbers
void showBoard(char** board, int n) {
    printf("\n    ");
    for (int j = 0; j < n; j++) printf(" %d  ", j + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("   ");
        for (int k = 0; k < n; k++) printf("----");
        printf("-\n");

        printf("%2d |", i + 1);
        for (int j = 0; j < n; j++) {
            printf(" %c |", board[i][j]);
        }
        printf("\n");
    }

    printf("   ");
    for (int k = 0; k < n; k++) printf("----");
    printf("-\n\n");
}

// Validate a move
int validMove(char** board, int n, int row, int col) {
    if (row >= 0 && row < n && col >= 0 && col < n) {
        if (board[row][col] == ' ') return 1;
    }
    return 0;
}

// Check win conditions
int checkWin(char** board, int n, char player) {
    int i, j;

    // Rows
    for (i = 0; i < n; i++) {
        int win = 1;
        for (j = 0; j < n; j++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }

    // Columns
    for (j = 0; j < n; j++) {
        int win = 1;
        for (i = 0; i < n; i++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }

    // Main diagonal
    int win = 1;
    for (i = 0; i < n; i++) {
        if (board[i][i] != player) { win = 0; break; }
    }
    if (win) return 1;

    // Anti diagonal
    win = 1;
    for (i = 0; i < n; i++) {
        if (board[i][n - 1 - i] != player) { win = 0; break; }
    }
    if (win) return 1;

    return 0;
}

// Check for draw
int checkDraw(char** board, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

// Free memory
void freeBoard(char** board, int n) {
    for (int i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
}

// Write board to log file
void logBoard(FILE* logFile, char** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(logFile, " %c ", board[i][j]);
            if (j < n - 1) fprintf(logFile, "|");
        }
        fprintf(logFile, "\n");
        if (i < n - 1) {
            for (int k = 0; k < n; k++) {
                fprintf(logFile, "---");
                if (k < n - 1) fprintf(logFile, "+");
            }
            fprintf(logFile, "\n");
        }
    }
    fprintf(logFile, "\n");
}
