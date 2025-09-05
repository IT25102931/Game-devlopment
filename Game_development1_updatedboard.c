#include <stdio.h>
#include <stdlib.h>

// Updated my diplay-board for more easier to readable
// Updates - give a no. for each cloumn and row , now its easier to idetify the the box player needs
//  Function types 
char** createBoard(int n);              // Create a new board dynamically
void showBoard(char** board, int n);    // Display the board
int validMove(char** board, int n, int row, int col);  // Check if a move is valid
int checkWin(char** board, int n, char player);        // Check if a player won
int checkDraw(char** board, int n);    // Check if game ended in a draw
void freeBoard(char** board, int n);   // Free memory at the end

//  Main Function
int main() {
    int n;

    // Ask the user for board size
    printf("Enter board size (3–10): ");
    scanf("%d", &n);

    // Validate the board size ( Board size can be 3 by 3 to 10 by 10 )
    if (n < 3 || n > 10) {
        printf("Invalid size! Please restart and choose between 3 and 10.\n");
        return 0;
    }

    // Create the game board using dynamic memory
    char** board = createBoard(n);

    // Open a file to log the game
    FILE* logFile = fopen("tictactoe_log.txt", "w"); // In my game created a log file to save the actions while playing the game
    char player = 'X';   // Start with Player X
    int row, col;        // User's chosen row and column
    int finished = 0;    // Game continues until this becomes 1

    // Game loop
    while (!finished) {
        // Show the current board
        showBoard(board, n);

        // Ask the current player for their move
        printf("Player %c, enter row and column (1-%d): ", player, n);
        scanf("%d %d", &row, &col);

        // Adjust input to match array index (0-based in C)
        row--;
        col--;

        // Check if the move is valid
        if (validMove(board, n, row, col)) {
            // Place the player's mark (X or O) on the board
            board[row][col] = player;

            // ---- Log the board into the file ----
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

            // ---- Check if the current player has won ----
            if (checkWin(board, n, player)) {
                showBoard(board, n);
                printf("Player %c wins!\n", player);
                fprintf(logFile, "Player %c wins!\n", player);
                finished = 1; // End the game
            }
            // ---- Check if it's a draw ----
            else if (checkDraw(board, n)) {
                showBoard(board, n);
                printf("Game is a draw!\n");
                fprintf(logFile, "Game is a draw!\n");
                finished = 1; // End the game
            }
            // ---- Otherwise, switch player ----
            else {
                player = (player == 'X') ? 'O' : 'X';
            }
        } 
        else {
            // If move is invalid, show error and continue
            printf("Invalid move. Try again.\n");
        }
    }

    // Close the log file
    fclose(logFile);

    // Free memory used by the board
    freeBoard(board, n);

    return 0;
}

// Function Definitions 

// Function: Create a new board filled with spaces
char** createBoard(int n) {
    char** board = (char**)malloc(n * sizeof(char*)); // Allocate memory for rows
    for (int i = 0; i < n; i++) {
        board[i] = (char*)malloc(n * sizeof(char));   // Allocate memory for columns
        for (int j = 0; j < n; j++) {
            board[i][j] = ' '; // Fill with blank spaces initially
        }
    }
    return board;
}

// Function: Show the board with row/col numbers
void showBoard(char** board, int n) {
    printf("\n   "); // spacing for column numbers

    // Print column numbers at the top
    for (int j = 0; j < n; j++) {
        printf(" %d ", j + 1);
        if (j < n - 1) printf(" ");
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf(" %d ", i + 1); // Print row number at the start
        for (int j = 0; j < n; j++) {
            printf(" %c ", board[i][j]);
            if (j < n - 1) printf("|");
        }
        printf("\n");

        // Print row separators
        if (i < n - 1) {
            printf("   "); // Align under column numbers
            for (int k = 0; k < n; k++) {
                printf("---");
                if (k < n - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Function: Check if a move is valid (inside board and empty cell)
int validMove(char** board, int n, int row, int col) {
    if (row >= 0 && row < n && col >= 0 && col < n) {
        if (board[row][col] == ' ')
            return 1; // Valid move
    }
    return 0; // Invalid move
}

// Function: Check win conditions for the player
int checkWin(char** board, int n, char player) {
    int i, j;

    // Check rows
    for (i = 0; i < n; i++) {
        int win = 1;
        for (j = 0; j < n; j++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }

    // Check columns
    for (j = 0; j < n; j++) {
        int win = 1;
        for (i = 0; i < n; i++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }

    // Check main diagonal
    int win = 1;
    for (i = 0; i < n; i++) {
        if (board[i][i] != player) { win = 0; break; }
    }
    if (win) return 1;

    // Check other-diagonal ( except main diagonal )
    win = 1;
    for (i = 0; i < n; i++) {
        if (board[i][n - 1 - i] != player) { win = 0; break; }
    }
    if (win) return 1;

    return 0; // No win
}

// Function: Check if the board is full (draw)
int checkDraw(char** board, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board[i][j] == ' ')
                return 0; // Still moves available
        }
    }
    return 1; // No spaces left -> draw
}

// Function: Free the memory for the board
void freeBoard(char** board, int n) {
    for (int i = 0; i < n; i++) {
        free(board[i]); // Free each row
    }
    free(board); // Free the row pointer array
}
