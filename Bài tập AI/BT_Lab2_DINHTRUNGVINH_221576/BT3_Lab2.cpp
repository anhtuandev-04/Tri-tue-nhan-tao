#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define Maxlength 500
using namespace std;
const char* action[] = {"Trang thai dau", "Di chuyen o EMPTY len TREN", "Di chuyen o EMPTY xuong DUOI", "Di chuyen o EMPTY sang TRAI", "Di chuyen o EMPTY sang PHAI"};

typedef struct {
    int eightPuzzel[ROWS][COLS];
    int emptyRow;
    int emptyCol;
} State;

void printState(State state){
    int row, col;
    printf("\n-----------\n");
    for (row = 0; row < ROWS; row++){
        for(col = 0; col < COLS; col++)
            printf("|%d ", state.eightPuzzel[row][col]);
        printf("|\n");
    }
    printf("-----------\n");
}

int compareStates(State state1, State state2){
    if(state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
        return 0;
    int row, col;
    for(row = 0; row < ROWS; row++)
        for(col = 0; col < COLS; col++)
            if(state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])
                return 0;
    return 1;
}

int goalcheck(State state, State goal) {
    return compareStates(state, goal); 
}

int upOperator(State state, State *result){
    *result = state;
    int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
    if(empRowCurrent > 0){
        result->emptyRow = empRowCurrent - 1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent - 1][empColCurrent];
        result->eightPuzzel[empRowCurrent - 1][empColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

int downOperator(State state, State *result) {
    *result = state;
    int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
    if (empRowCurrent < ROWS - 1) {
        result->emptyRow = empRowCurrent + 1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent + 1][empColCurrent];
        result->eightPuzzel[empRowCurrent + 1][empColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

int leftOperator(State state, State *result) {
    *result = state; 
    int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol; 
    if(empColCurrent > 0) { 
        result->emptyRow = empRowCurrent; 
        result->emptyCol = empColCurrent - 1; 
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent - 1]; 
        result->eightPuzzel[empRowCurrent][empColCurrent - 1] = EMPTY; 
        return 1; 
    }
    return 0; 
}

int rightOperator(State state, State *result) {
    *result = state; 
    int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol; 
    if(empColCurrent < COLS - 1) { 
        result->emptyRow = empRowCurrent; 
        result->emptyCol = empColCurrent + 1; 
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent + 1]; 
        result->eightPuzzel[empRowCurrent][empColCurrent + 1] = EMPTY; 
        return 1;
    }
    return 0; 
}

int callOperators(State state, State *result, int opt) {
    switch(opt) {
        case 1: 
            return upOperator(state, result);
        case 2: 
            return downOperator(state, result);
        case 3: 
            return leftOperator(state, result);
        case 4: 
            return rightOperator(state, result);
        default: 
            printf("Khong the goi toan tu");
            return 0;
    }
}

int heuristic1(State state, State goal) {
    int row, col, count = 0;
    for(row = 0; row < ROWS; row++)
        for(col = 0; col < COLS; col++)
            if(state.eightPuzzel[row][col] != goal.eightPuzzel[row][col])
                count++;
    return count;
}

int heuristic2(State state, State goal) {
    int count = 0;
    int row, col, row_g, col_g;
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            if (state.eightPuzzel[row][col] != EMPTY) {
                for (row_g = 0; row_g < ROWS; row_g++) {
                    for (col_g = 0; col_g < COLS; col_g++) {
                        if (state.eightPuzzel[row][col] == goal.eightPuzzel[row_g][col_g]) {
                            count += abs(row - row_g) + abs(col - col_g);
                            col_g = COLS;
                            row_g = ROWS;
                        }
                    }
                }
            }
        }
    }
    return count;
}

typedef struct Node {
    State state;
    struct Node* parent;
    int no_function;
    int heuristic;
} Node;


// Tim trang thai state co thuoc Open hoac Close hay khong?
// Luu vi tri tim duoc vao bien *position
Node* find_State(State state, vector<Node*> v, vector<Node *>::iterator *position) {
    vector<Node *>::iterator it = v.begin(); // Khoi tao iterator o dau danh sach
    if (v.size() == 0) return NULL; // Tra ve NULL neu danh sach rong

    // Lap qua cac phan tu trong danh sach cho den khi iterator dat cuoi
    while (it != v.end()) {
        if (compareStates((*it)->state, state)) { // So sanh trang thai
            *position = it; // Luu iterator cua trang thai tim duoc vao *position
            return *it; // Tra ve con tro den Node tim duoc
        }
        it = v.erase(it); // Xoa phan tu it khoi danh sach va cap nhat iterator
    }
    return NULL; // Tra ve NULL neu khong tim thay trang thai
}

// Sap xep danh sach theo trong so heuristic
// Tieu chi de sort trong vector
// Phan tu trong vector se duoc truyen lan luot vao Node* a va Node *b de so sanh
bool compareHeuristic(Node* a, Node* b) {
    return a->heuristic > b->heuristic;
}

Node* best_first_search(State state, State goal){
    vector<Node*> Open_BFS;
    vector<Node*> Close_BFS;
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->heuristic = heuristic1(root->state, goal);
    Open_BFS.push_back(root); // Dua node vao Open_BFS

    while(!Open_BFS.empty()) { // Dam bao vong lap tiep tuc khi Open_BFS khong rong
        Node* node = Open_BFS.back(); // Lay phan tu cuoi cung
        Open_BFS.pop_back(); // Xoa phan tu cuoi cung
        Close_BFS.push_back(node); // Them vao Close_BFS

        if(goalcheck(node->state, goal)) {
            cout << "Muc tieu duoc dat!" << endl;
            return node;
        }

        for(int opt = 1; opt <= MAX_OPERATOR; opt++) { // Dieu chinh vong lap cho dung
            State newstate;
            if(callOperators(node->state, &newstate, opt)) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->parent = node;
                newNode->no_function = opt;
                newNode->heuristic = heuristic1(newstate, goal);

                vector<Node*>::iterator pos_Open, pos_Close;
                Node* nodeFoundOpen = find_State(newstate, Open_BFS, &pos_Open);
                Node* nodeFoundClose = find_State(newstate, Close_BFS, &pos_Close);

                if(nodeFoundOpen == NULL && nodeFoundClose == NULL) {
                    Open_BFS.push_back(newNode);
                }
                else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic) {
                    Open_BFS.erase(pos_Open); 
                    Open_BFS.push_back(newNode);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic) {
                    Close_BFS.erase(pos_Close);
                    Open_BFS.push_back(newNode);
                }
            }
        }

        sort(Open_BFS.begin(), Open_BFS.end(), compareHeuristic); // Sap xep Open_BFS theo heuristic
    }

    return NULL; // Neu khong tim thay muc tieu
}

// Ham in ket qua cua thuat toan BFS
void print_WaysToGetGoal(Node* node) {
    vector<Node*> vectorPrint;
    // Duyet nguoc ve nut parent
    while(node->parent != NULL) {
        vectorPrint.push_back(node);
        node = node->parent;
    }
    vectorPrint.push_back(node); // Them trang thai ban dau vao cuoi vector

    // In ra the tu hanh dong di chuyen o trong
    int no_action = 0, i;
    for(i = vectorPrint.size() - 1; i >= 0; i--) { // Duyet tu trang thai ban dau
        printf("\nAction %d: %s", no_action, action[vectorPrint.at(i)->no_function]);
        printState(vectorPrint.at(i)->state);
        no_action++;
    }
}

int main() {
    State state;
    state.emptyRow = 1;
    state.emptyCol = 1;
    state.eightPuzzel[0][0] = 3;
    state.eightPuzzel[0][1] = 4;
    state.eightPuzzel[0][2] = 5;
    state.eightPuzzel[1][0] = 1;
    state.eightPuzzel[1][1] = 0;
    state.eightPuzzel[1][2] = 2;
    state.eightPuzzel[2][0] = 6;
    state.eightPuzzel[2][1] = 7;
    state.eightPuzzel[2][2] = 8;

    State goal;
    goal.emptyRow = 0;
    goal.emptyCol = 0;
    goal.eightPuzzel[0][0] = 0;
    goal.eightPuzzel[0][1] = 1;
    goal.eightPuzzel[0][2] = 2;
    goal.eightPuzzel[1][0] = 3;
    goal.eightPuzzel[1][1] = 4;
    goal.eightPuzzel[1][2] = 5;
    goal.eightPuzzel[2][0] = 6;
    goal.eightPuzzel[2][1] = 7;
    goal.eightPuzzel[2][2] = 8;

    Node* p = best_first_search(state, goal);

    print_WaysToGetGoal(p);

    return 0;
}

