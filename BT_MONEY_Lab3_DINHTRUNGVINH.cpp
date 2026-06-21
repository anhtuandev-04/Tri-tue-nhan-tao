#include <stdio.h>
#include <stdbool.h>

#define NUM_LETTERS 8

typedef struct {
    int values[NUM_LETTERS]; // S, E, N, D, M, O, R, Y
    bool assigned[10];       // Track used digits
} Puzzle;

// Check if the equation holds
bool isValidEquation(Puzzle *puzzle) {
    int S = puzzle->values[0];
    int E = puzzle->values[1];
    int N = puzzle->values[2];
    int D = puzzle->values[3];
    int M = puzzle->values[4];
    int O = puzzle->values[5];
    int R = puzzle->values[6];
    int Y = puzzle->values[7];

    // No leading zeros
    if (S == 0 || M == 0)
        return false;

    int send = S * 1000 + E * 100 + N * 10 + D;
    int more = M * 1000 + O * 100 + R * 10 + E;
    int money = M * 10000 + O * 1000 + N * 100 + E * 10 + Y;

    return send + more == money;
}

// Backtracking solver
bool solvePuzzle(Puzzle *puzzle, int letter_idx) {
    if (letter_idx >= NUM_LETTERS) {
        return isValidEquation(puzzle);
    }

    for (int digit = 0; digit <= 9; digit++) {
        if (!puzzle->assigned[digit]) {
            puzzle->values[letter_idx] = digit;
            puzzle->assigned[digit] = true;

            if (solvePuzzle(puzzle, letter_idx + 1))
                return true;

            puzzle->assigned[digit] = false;
            puzzle->values[letter_idx] = -1;
        }
    }
    return false;
}

// Initialize puzzle
void initPuzzle(Puzzle *puzzle) {
    for (int i = 0; i < NUM_LETTERS; i++)
        puzzle->values[i] = -1;
    for (int i = 0; i < 10; i++)
        puzzle->assigned[i] = false;
}

// Print solution
void printSolution(Puzzle *puzzle) {
    printf("S=%d, E=%d, N=%d, D=%d, M=%d, O=%d, R=%d, Y=%d\n",
           puzzle->values[0], puzzle->values[1], puzzle->values[2], puzzle->values[3],
           puzzle->values[4], puzzle->values[5], puzzle->values[6], puzzle->values[7]);
    int send = puzzle->values[0] * 1000 + puzzle->values[1] * 100 + puzzle->values[2] * 10 + puzzle->values[3];
    int more = puzzle->values[4] * 1000 + puzzle->values[5] * 100 + puzzle->values[6] * 10 + puzzle->values[1];
    int money = puzzle->values[4] * 10000 + puzzle->values[5] * 1000 + puzzle->values[2] * 100 +
                puzzle->values[1] * 10 + puzzle->values[7];
    printf("%d + %d = %d\n", send, more, money);
}

int main() {
    Puzzle puzzle;
    initPuzzle(&puzzle);

    if (solvePuzzle(&puzzle, 0)) {
        printf("Solution found:\n");
        printSolution(&puzzle);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
