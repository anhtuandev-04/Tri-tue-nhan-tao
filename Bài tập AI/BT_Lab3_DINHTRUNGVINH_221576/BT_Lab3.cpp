#include <stdio.h>
#include <stdlib.h>

#define NB_ROWS 9
#define NB_COLUMNS 9
#define AREA_SQUARE_SIZE 3
#define MAX_VALUE 10
#define EMPTY 0
#define INF 1000000

// Cau truc toa do
typedef struct {
    int x, y;
} Coord;

// Cau truc danh danh sach toa do
typedef struct {
    Coord* data;
    int size;
} ListCoord;

// Cau truc danh sach gia tri
typedef struct {
    int* data;
    int size;
} List;

// Cau truc rang buoc 
typedef struct {
    int data[NB_ROWS * NB_COLUMNS][NB_ROWS * NB_COLUMNS];
    int n;
} Constrains;

// Cau truc Sudoku
typedef struct {
    int cells[NB_ROWS][NB_COLUMNS];
    Constrains constrains;
} Sudoku;

// Khoi tao danh sach toa do
void initListCoord(ListCoord* list) {
    list->data = (Coord*)malloc(sizeof(Coord) * NB_ROWS * NB_COLUMNS);
    list->size = 0;
}

void appendListCoord(ListCoord* list, Coord pos) {
    list->data[list->size++] = pos;
}

// Khoi tao danh sach gia tri
void initList(List* list) {
    list->data = (int*)malloc(sizeof(int) * MAX_VALUE);
    list->size = 0;
}

void appendList(List* list, int value) {
    list->data[list->size++] = value;
}

// Khoi tao rang buoc rong
void initConstrains(Constrains* constrains) {
    int i, j;
    for (i = 0; i < NB_ROWS * NB_COLUMNS; i++) {
        for (j = 0; j < NB_ROWS * NB_COLUMNS; j++) {
            constrains->data[i][j] = 0;
        }
    }
    constrains->n = NB_ROWS * NB_COLUMNS;
}

// Them rang buoc giua 2 vi tri
int addConstrain(Constrains* constrains, Coord pos1, Coord pos2) {
    int index1 = pos1.x * NB_COLUMNS + pos1.y;
    int index2 = pos2.x * NB_COLUMNS + pos2.y;
    if (constrains->data[index1][index2] == 0) {
        constrains->data[index1][index2] = 1;
        constrains->data[index2][index1] = 1;
        return 1;
    }
    return 0;
}

// Lan truyen rang buoc tu 1 vi tri
void spreadConstrainsFrom(Coord position, Constrains* constrains, ListCoord* changeds) {
    int row = position.x, column = position.y;
    int i, j;
    // Theo cot
    for (i = 0; i < NB_ROWS; i++) {
        if (i != row) {
            Coord pos = {i, column};
            if (addConstrain(constrains, position, pos))
                appendListCoord(changeds, pos);
        }
    }
    // Theo hang
    for (i = 0; i < NB_COLUMNS; i++) {
        if (i != column) {
            Coord pos = {row, i};
            if (addConstrain(constrains, position, pos))
                appendListCoord(changeds, pos);
        }
    }
    // Theo khoi
    for (i = 0; i < AREA_SQUARE_SIZE; i++) {
        for (j = 0; j < AREA_SQUARE_SIZE; j++) {
            int areaX = (row / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            int areaY = (column / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            if (areaX + i != row || areaY + j != column) {
                Coord pos = {areaX + i, areaY + j};
                if (addConstrain(constrains, position, pos))
                    appendListCoord(changeds, pos);
            }
        }
    }
}

// Lay danh sach gia tri su dung cho 1 ô
List getAvailableValues(Coord position, Sudoku sudoku) {
    int availables[MAX_VALUE] = {0};
    int i, j;
    for (i = 1; i < MAX_VALUE; i++) availables[i] = 1;

    // Kiem tra hang
    for (j = 0; j < NB_COLUMNS; j++) {
        if (sudoku.cells[position.x][j] != EMPTY) {
            availables[sudoku.cells[position.x][j]] = 0;
        }
    }
    // Ki?m tra c?t
    for (i = 0; i < NB_ROWS; i++) {
        if (sudoku.cells[i][position.y] != EMPTY) {
            availables[sudoku.cells[i][position.y]] = 0;
        }
    }
    // Kiem tra cot
    int startX = (position.x / 3) * 3;
    int startY = (position.y / 3) * 3;
    for (i = startX; i < startX + 3; i++) {
        for (j = startY; j < startY + 3; j++) {
            if (sudoku.cells[i][j] != EMPTY) {
                availables[sudoku.cells[i][j]] = 0;
            }
        }
    }

    List result;
    initList(&result);
    for (i = 1; i < MAX_VALUE; i++) {
        if (availables[i]) appendList(&result, i);
    }
    return result;
}

// Tim ô trong tiep theo co mien gia tri nho nhat
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

// Kiem tra xem sudoku da day chua?
int isFilledSudoku(Sudoku sudoku) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            if (sudoku.cells[i][j] == EMPTY) return 0;
        }
    }
    return 1;
}

// Giai Sudoku bang thuat toan quay lui
int exploredCounter = 0;
int sudokuBackTracking(Sudoku* sudoku) {
    if (isFilledSudoku(*sudoku)) return 1;
    Coord position = getNextMinDomainCell(*sudoku);
    List availables = getAvailableValues(position, *sudoku);
    if (availables.size == 0) return 0;

    int j;
    for (j = 0; j < availables.size; j++) {
        int value = availables.data[j];
        sudoku->cells[position.x][position.y] = value;
        exploredCounter++;
        if (sudokuBackTracking(sudoku)) return 1;
        sudoku->cells[position.x][position.y] = EMPTY;
    }
    free(availables.data); // Gi?i phong bo nho
    return 0;
}

// Giai quyet bai toan sudoku 
Sudoku solveSudoku(Sudoku sudoku) {
    int i, j;
    initConstrains(&sudoku.constrains);
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            if (sudoku.cells[i][j] != EMPTY) {
                ListCoord history;
                initListCoord(&history);
                Coord pos = {i, j};
                spreadConstrainsFrom(pos, &sudoku.constrains, &history);
                free(history.data);
            }
        }
    }
    exploredCounter = 0;
    if (sudokuBackTracking(&sudoku)) printf("Solved\n");
    else printf("Can not solve\n");
    printf("Explored %d states\n", exploredCounter);
    return sudoku;
}

// Khoi tao sudoku voi gia tri dau vao
void initSudokuWithValues(Sudoku* sudoku, int values[NB_ROWS][NB_COLUMNS]) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            sudoku->cells[i][j] = values[i][j];
        }
    }
    initConstrains(&sudoku->constrains);
}

// In bang Sudoku
void printSudoku(Sudoku sudoku) {
    int i, j;
    for (i = 0; i < NB_ROWS; i++) {
        for (j = 0; j < NB_COLUMNS; j++) {
            if (sudoku.cells[i][j] == EMPTY) printf(". ");
            else printf("%d ", sudoku.cells[i][j]);
            if (j == 2 || j == 5) printf("| ");
        }
        printf("\n");
        if (i == 2 || i == 5) printf("------+-------+------\n");
    }
    printf("\n");
}

// Bang dau vao
int inputs1[9][9] = {
    {0, 6, 1, 0, 0, 7, 0, 0, 3},
    {0, 9, 2, 0, 0, 3, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 8, 5, 3, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 5, 0, 4},
    {5, 0, 0, 0, 0, 8, 0, 0, 0},
    {0, 4, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 6, 0, 8, 0, 0},
    {6, 0, 0, 0, 0, 0, 0, 0, 0}
};
// Main
int main() {
    Sudoku sudoku;
    initSudokuWithValues(&sudoku, inputs1);
    printf("Initial Sudoku:\n");
    printSudoku(sudoku);
    Sudoku result = solveSudoku(sudoku);
    printf("Solved Sudoku:\n");
    printSudoku(result);
    return 0;
}
