#include <stdio.h>
#include <stdlib.h>

#define NB_ROWS 9
#define NB_COLUMNS 9
#define MAX_VALUE 10
#define EMPTY 0
#define AREA_SQUARE_SIZE 3
#define MAX_LENGTH 81
#define INF 1000

// Coordinate structure
typedef struct {
    int x, y;
} Coord;

// List of coordinates
typedef struct {
    Coord data[MAX_LENGTH];
    int size;
} ListCoord;

// List of values
typedef struct {
    int data[MAX_VALUE];
    int size;
} List;

// Constraints matrix
typedef struct {
    int data[NB_ROWS * NB_COLUMNS][NB_ROWS * NB_COLUMNS];
    int n;
} Constrains;

// Sudoku structure
typedef struct {
    int cells[NB_ROWS][NB_COLUMNS];
    Constrains constrains;
} Sudoku;

// Initialize ListCoord
void initListCoord(ListCoord *list) {
    list->size = 0;
}

void appendListCoord(ListCoord *list, Coord coord) {
    list->data[list->size++] = coord;
}

// Initialize List
void initList(List *list) {
    list->size = 0;
}

void appendList(List *list, int value) {
    list->data[list->size++] = value;
}

// Initialize Constrains
void initConstrains(Constrains *constrains) {
    int i, j;
    for (i = 0; i < NB_ROWS * NB_COLUMNS; i++) {
        for (j = 0; j < NB_ROWS * NB_COLUMNS; j++) {
            constrains->data[i][j] = 0;
        }
    }
    constrains->n = NB_ROWS * NB_COLUMNS;
}

void clearConstrains(Constrains *constrains) {
    initConstrains(constrains);
}

// Convert Coord to vertex index
int coordToVertex(Coord pos) {
    return pos.x * NB_COLUMNS + pos.y;
}

// Convert vertex index to Coord
Coord vertexToCoord(int vertex) {
    Coord pos;
    pos.x = vertex / NB_COLUMNS;
    pos.y = vertex % NB_COLUMNS;
    return pos;
}

// Add a constraint between two positions
int addConstrain(Constrains *constrains, Coord pos1, Coord pos2) {
    int v1 = coordToVertex(pos1);
    int v2 = coordToVertex(pos2);
    if (constrains->data[v1][v2] == 0) {
        constrains->data[v1][v2] = 1;
        constrains->data[v2][v1] = 1;
        return 1;
    }
    return 0;
}

// Get all positions constrained with the given position
ListCoord getConstrains(Constrains constrains, Coord position) {
    ListCoord result;
    initListCoord(&result);
    int v = coordToVertex(position);
    int i;
    for (i = 0; i < constrains.n; i++) {
        if (constrains.data[v][i] == 1) {
            appendListCoord(&result, vertexToCoord(i));
        }
    }
    return result;
}

// Spread constraints from a position
void spreadConstrainsFrom(Coord position, Constrains *constrains, ListCoord *changeds) {
    int row = position.x, column = position.y;
    int i, j;
    // Row constraints
    for (i = 0; i < NB_COLUMNS; i++) {
        if (i != column) {
            Coord pos = {row, i};
            if (addConstrain(constrains, position, pos))
                appendListCoord(changeds, pos);
        }
    }
    // Column constraints
    for (i = 0; i < NB_ROWS; i++) {
        if (i != row) {
            Coord pos = {i, column};
            if (addConstrain(constrains, position, pos))
                appendListCoord(changeds, pos);
        }
    }
    // Subgrid constraints
    int areaX = (row / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
    int areaY = (column / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
    for (i = 0; i < AREA_SQUARE_SIZE; i++) {
        for (j = 0; j < AREA_SQUARE_SIZE; j++) {
            if (areaX + i != row || areaY + j != column) {
                Coord pos = {areaX + i, areaY + j};
                if (addConstrain(constrains, position, pos))
                    appendListCoord(changeds, pos);
            }
        }
    }
}

// Get available values for a cell
List getAvailableValues(Coord position, Sudoku sudoku) {
    ListCoord posList = getConstrains(sudoku.constrains, position);
    int availables[MAX_VALUE] = {0};
    int i;
    for (i = 1; i < MAX_VALUE; i++)
        availables[i] = 1;
    for (i = 0; i < posList.size; i++) {
        Coord pos = posList.data[i];
        if (sudoku.cells[pos.x][pos.y] != EMPTY) {
            availables[sudoku.cells[pos.x][pos.y]] = 0;
        }
    }
    List result;
    initList(&result);
    for (i = 1; i < MAX_VALUE; i++) {
        if (availables[i])
            appendList(&result, i);
    }
    return result;
}

// Get next empty cell with minimum domain
Coord getNextMinDomainCell(Sudoku sudoku) {
    int minLength = INF;
    Coord result = {-1, -1};
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            if (sudoku.cells[i][j] == EMPTY) {
                Coord pos = {i, j};
                int availablesLength = getAvailableValues(pos, sudoku).size;
                if (availablesLength < minLength) {
                    minLength = availablesLength;
                    result = pos;
                }
            }
        }
    }
    return result;
}

// Check if Sudoku is fully filled
int isFilledSudoku(Sudoku sudoku) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            if (sudoku.cells[i][j] == EMPTY)
                return 0;
        }
    }
    return 1;
}

// Backtracking solver
int exploredCounter = 0;
int sudokuBackTracking(Sudoku *sudoku) {
    if (isFilledSudoku(*sudoku))
        return 1;
    Coord position = getNextMinDomainCell(*sudoku);
    if (position.x == -1 && position.y == -1)
        return 0; // No empty cells found
    List availables = getAvailableValues(position, *sudoku);
    if (availables.size == 0)
        return 0;
    int j;
    for (j = 0; j < availables.size; j++) {
        int value = availables.data[j];
        sudoku->cells[position.x][position.y] = value;
        exploredCounter++;
        if (sudokuBackTracking(sudoku))
            return 1;
        sudoku->cells[position.x][position.y] = EMPTY;
    }
    return 0;
}

// Solve Sudoku
Sudoku solveSudoku(Sudoku sudoku) {
    int i, j;
    clearConstrains(&sudoku.constrains);
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            if (sudoku.cells[i][j] != EMPTY) {
                ListCoord history;
                initListCoord(&history);
                Coord pos = {i, j};
                spreadConstrainsFrom(pos, &sudoku.constrains, &history);
            }
        }
    }
    exploredCounter = 0;
    if (sudokuBackTracking(&sudoku))
        printf("Solved\n");
    else
        printf("Can not solve\n");
    printf("Explored %d states\n", exploredCounter);
    return sudoku;
}

// Initialize Sudoku with values
void initSudokuWithValues(Sudoku *sudoku, int inputs[NB_ROWS][NB_COLUMNS]) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            sudoku->cells[i][j] = inputs[i][j];
        }
    }
    initConstrains(&sudoku->constrains);
}

// Print Sudoku
void printSudoku(Sudoku sudoku) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        if (i % 3 == 0 && i != 0)
            printf("- - - - - - - - - - -\n");
        for (j = 0; j < NB_COLUMNS; j++) {
            if (j % 3 == 0 && j != 0)
                printf("| ");
            if (sudoku.cells[i][j] == EMPTY)
                printf(". ");
            else
                printf("%d ", sudoku.cells[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Main function
int main() {
    int inputs1[NB_ROWS][NB_COLUMNS] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 0},
        {4, 0, 0, 8, 0, 3, 0, 0, 0},
        {7, 0, 0, 0, 2, 0, 0, 0, 0},
        {0, 6, 0, 0, 0, 0, 2, 0, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 3},
        {0, 0, 0, 0, 8, 0, 0, 0, 0}
    };
    Sudoku sudoku;
    initSudokuWithValues(&sudoku, inputs1);
    printf("Initial Sudoku:\n");
    printSudoku(sudoku);
    Sudoku result = solveSudoku(sudoku);
    printf("Solved Sudoku:\n");
    printSudoku(result);
    return 0;
}
