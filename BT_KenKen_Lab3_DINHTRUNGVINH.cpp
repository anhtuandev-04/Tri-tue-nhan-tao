#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 4
#define MAX_CAGES 10
#define MAX_CELLS 16

typedef struct {
    int row, col;
} Cell;

typedef struct {
    Cell cells[SIZE * SIZE];
    int cell_count;
    char op; // '+', '-', '*', '/', 's' (single)
    int target;
} Cage;

typedef struct {
    int grid[SIZE][SIZE];
    Cage cages[MAX_CAGES];
    int cage_count;
} KenKen;

// Check if a value is valid in a row
bool isValidInRow(KenKen *puzzle, int row, int value) {
    for (int col = 0; col < SIZE; col++) {
        if (puzzle->grid[row][col] == value)
            return false;
    }
    return true;
}

// Check if a value is valid in a column
bool isValidInColumn(KenKen *puzzle, int col, int value) {
    for (int row = 0; row < SIZE; row++) {
        if (puzzle->grid[row][col] == value)
            return false;
    }
    return true;
}

// Find the cage containing a cell
int findCage(KenKen *puzzle, int row, int col) {
    for (int i = 0; i < puzzle->cage_count; i++) {
        for (int j = 0; j < puzzle->cages[i].cell_count; j++) {
            if (puzzle->cages[i].cells[j].row == row && puzzle->cages[i].cells[j].col == col)
                return i;
        }
    }
    return -1;
}

// Check if a cage’s constraint is satisfied
bool isCageValid(KenKen *puzzle, int cage_idx) {
    Cage *cage = &puzzle->cages[cage_idx];
    int values[SIZE * SIZE];
    int count = 0;

    // Collect assigned values in the cage
    for (int i = 0; i < cage->cell_count; i++) {
        int r = cage->cells[i].row;
        int c = cage->cells[i].col;
        if (puzzle->grid[r][c] == 0)
            return true; // Partially filled
        values[count++] = puzzle->grid[r][c];
    }

    if (count != cage->cell_count)
        return true; // Not fully filled

    if (cage->op == 's') {
        return values[0] == cage->target;
    } else if (cage->op == '+') {
        int sum = 0;
        for (int i = 0; i < count; i++)
            sum += values[i];
        return sum == cage->target;
    } else if (cage->op == '*') {
        int prod = 1;
        for (int i = 0; i < count; i++)
            prod *= values[i];
        return prod == cage->target;
    } else if (cage->op == '-') {
        int max = values[0] > values[1] ? values[0] : values[1];
        int min = values[0] < values[1] ? values[0] : values[1];
        return max - min == cage->target;
    } else if (cage->op == '/') {
        int max = values[0] > values[1] ? values[0] : values[1];
        int min = values[0] < values[1] ? values[0] : values[1];
        if (min == 0) return false; // Avoid division by zero
        return max / min == cage->target;
    }
    return false;
}

// Backtracking solver
bool solveKenKen(KenKen *puzzle, int cell_idx) {
    if (cell_idx >= SIZE * SIZE) {
        return true;
    }

    int row = cell_idx / SIZE;
    int col = cell_idx % SIZE;

    if (puzzle->grid[row][col] != 0)
        return solveKenKen(puzzle, cell_idx + 1);

    for (int value = 1; value <= SIZE; value++) {
        if (isValidInRow(puzzle, row, value) && isValidInColumn(puzzle, col, value)) {
            puzzle->grid[row][col] = value;
            int cage_idx = findCage(puzzle, row, col);
            if (isCageValid(puzzle, cage_idx)) {
                if (solveKenKen(puzzle, cell_idx + 1))
                    return true;
            }
            puzzle->grid[row][col] = 0;
        }
    }
    return false;
}

// Print the grid
void printKenKen(KenKen *puzzle) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", puzzle->grid[i][j]);
        }
        printf("\n");
    }
}

// Initialize the puzzle
void initKenKen(KenKen *puzzle) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            puzzle->grid[i][j] = 0;
    puzzle->cage_count = 0;
}

// Add a cage
void addCage(KenKen *puzzle, int cell_count, Cell *cells, char op, int target) {
    Cage *cage = &puzzle->cages[puzzle->cage_count];
    cage->cell_count = cell_count;
    cage->op = op;
    cage->target = target;
    for (int i = 0; i < cell_count; i++)
        cage->cells[i] = cells[i];
    puzzle->cage_count++;
}

int main() {
    KenKen puzzle;
    initKenKen(&puzzle);

    // Define corrected cages
    Cell cageA[] = {{0, 0}, {0, 1}, {1, 0}}; // 7+
    Cell cageB[] = {{0, 2}, {1, 2}};         // 2/
    Cell cageC[] = {{0, 3}, {1, 3}};         // 4+
    Cell cageD[] = {{1, 1}};                 // 3 (single)
    Cell cageE[] = {{2, 0}, {2, 1}};         // 7+
    Cell cageF[] = {{2, 2}, {3, 2}};         // 3*
    Cell cageG[] = {{2, 3}};                 // 2 (single)
    Cell cageH[] = {{3, 0}};                 // 4 (single)
    Cell cageI[] = {{3, 1}};                 // 1 (single)
    Cell cageJ[] = {{3, 3}};                 // 4 (single)

    addCage(&puzzle, 3, cageA, '+', 7);
    addCage(&puzzle, 2, cageB, '/', 2);
    addCage(&puzzle, 2, cageC, '+', 4);
    addCage(&puzzle, 1, cageD, 's', 3);
    addCage(&puzzle, 2, cageE, '+', 7);
    addCage(&puzzle, 2, cageF, '*', 3);
    addCage(&puzzle, 1, cageG, 's', 2);
    addCage(&puzzle, 1, cageH, 's', 4);
    addCage(&puzzle, 1, cageI, 's', 1);
    addCage(&puzzle, 1, cageJ, 's', 4);

    if (solveKenKen(&puzzle, 0)) {
        printf("KenKen Solution:\n");
        printKenKen(&puzzle);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
