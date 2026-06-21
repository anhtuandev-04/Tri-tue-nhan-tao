#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 8

typedef struct {
    int board[SIZE]; // board[row] = column
} Chessboard;

// Check if a queen can be placed at (row, col)
bool isSafe(Chessboard *board, int row, int col) {
    for (int prev_row = 0; prev_row < row; prev_row++) {
        int prev_col = board->board[prev_row];
        // Check same column
        if (prev_col == col)
            return false;
        // Check diagonals
        if (abs(prev_row - row) == abs(prev_col - col))
            return false;
    }
    return true;
}

// Backtracking solver
bool solveQueens(Chessboard *board, int row) {
    if (row >= SIZE)
        return true;

    for (int col = 0; col < SIZE; col++) {
        if (isSafe(board, row, col)) {
            board->board[row] = col;
            if (solveQueens(board, row + 1))
                return true;
            board->board[row] = -1;
        }
    }
    return false;
}

// Print the board
void printQueens(Chessboard *board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board->board[i] == j)
                printf("Q ");
            else
                printf(". ");
        }
        printf("\n");
    }
}

// Initialize board
void initQueens(Chessboard *board) {
    for (int i = 0; i < SIZE; i++)
        board->board[i] = -1;
}

int main() {
    Chessboard board;
    initQueens(&board);

    if (solveQueens(&board, 0)) {
        printf("8-Queens Solution:\n");
        printQueens(&board);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
