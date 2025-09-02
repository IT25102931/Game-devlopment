#include <stdio.h>
#include <stdlib.h>

// Functions
char** createBoard(int n);
void showBoard(char** board, int n);
int validMove(char** board, int n, int row, int col);
int checkWin(char** board, int n, char player);
int checkDraw(char** board, int n);
void freeBoard(char** board, int n);

int main() {
    int n;
    printf("Enter board size (3–10): ");
    scanf("%d", &n);

    if (n < 3 || n > 10) {
        printf("Invalid size!\n");
        return 0;
    }

    char** board = createBoard(n);
    FILE* logFile = fopen("tictactoe_log.txt", "w");

    char player = 'X';
    int row, col, finished = 0;

    while (!finished) {
        showBoard(board, n);
        printf("Player %c, enter row and column (1-%d): ", player, n);
        scanf("%d %d", &row, &col);

        row--; // adjust for 0 index
        col--;

        if (validMove(board, n, row, col)) {
            board[row][col] = player;

            // log board
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

            if (checkWin(board, n, player)) {
                showBoard(board, n);
                printf("Player %c wins!\n", player);
                fprintf(logFile, "Player %c wins!\n", player);
                finished = 1;
            } else if (checkDraw(board, n)) {
                showBoard(board, n);
                printf("Game is a draw!\n");
                fprintf(logFile, "Game is a draw!\n");
                finished = 1;
            } else {
                // switch player
                if (player == 'X')
                    player = 'O';
                else
                    player = 'X';
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }

    fclose(logFile);
    freeBoard(board, n);
    return 0;
}

// Create board dynamically
char** createBoard(int n) {
    char** board = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        board[i] = (char*)malloc(n * sizeof(char));
        for (int j = 0; j < n; j++) {
            board[i][j] = ' ';
        }
    }
    return board;
}

// Show board
void showBoard(char** board, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf(" %c ", board[i][j]);
            if (j < n - 1) printf("|");
        }
        printf("\n");
        if (i < n - 1) {
            for (int k = 0; k < n; k++) {
                printf("---");
                if (k < n - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Validate move
int validMove(char** board, int n, int row, int col) {
    if (row >= 0 && row < n && col >= 0 && col < n) {
        if (board[row][col] == ' ')
            return 1;
    }
    return 0;
}

// Check win
int checkWin(char** board, int n, char player) {
    int i, j;

    // rows
    for (i = 0; i < n; i++) {
        int win = 1;
        for (j = 0; j < n; j++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }

    // cols
    for (j = 0; j < n; j++) {
        int win = 1;
        for (i = 0; i < n; i++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }

    // main diagonal
    int win = 1;
    for (i = 0; i < n; i++) {
        if (board[i][i] != player) { win = 0; break; }
    }
    if (win) return 1;

    // other diagonal
    win = 1;
    for (i = 0; i < n; i++) {
        if (board[i][n - 1 - i] != player) { win = 0; break; }
    }
    if (win) return 1;

    return 0;
}

// Check draw
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
