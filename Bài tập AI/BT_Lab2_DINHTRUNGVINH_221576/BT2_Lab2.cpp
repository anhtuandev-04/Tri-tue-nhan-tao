#include<stdio.h>
#include<stdlib.h>
#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define MaxLength 500

const char* action[] = {"First State", "Move cell EMPTY to UP", "Move cell EMPTY to DOWN", "Move cell EMPTY to LEFT", "Move cell EMPTY to RIGHT"};

typedef struct
{
	int eightPuzzel[ROWS][COLS];
	int emptyRow;
	int emptyCol;
} State;

void printState(State state)
{
	int row, col;
	printf("\n-------\n");
	for(row = 0; row < ROWS; row++)
	{
		for(col = 0; col < COLS; col++)
			printf("|%d", state.eightPuzzel[row][col]);
		printf("| \n");
	}
	printf("-------\n");
}

//heuristic 1
int heuristic1(State state, State goal)
{
	int row, col, count = 0;
	for(row = 0; row < ROWS; row++)
		for(col = 0; col < COLS; col++)
			if(state.eightPuzzel[row][col] != goal.eightPuzzel[row][col])
				count++;
	return count;
}

//heuristic 2
int heuristic2(State state, State goal)
{
	int count = 0;
	int row, col, row_g, col_g;
	for(row = 0; row < ROWS; row++)
	{
		for(col = 0; col < COLS; col++)
		{
			if(state.eightPuzzel[row][col] != EMPTY)
			{
				for(row_g = 0; row_g < ROWS; row_g++)
				{
					for(col_g = 0; col_g < COLS; col_g++)
					{
						if(state.eightPuzzel[row][col] != goal.eightPuzzel[row][col])
						{
							count += abs(row - row_g) + abs(col - col_g);
							col_g = COLS;
							row_g = ROWS;
						}
					}
				}
			}
		}
	}
}

int compareStates(State state1, State state2)
{
	if(state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
		return 0;
	int row, col;
	for(row = 0; row < ROWS; row++)
		for(col = 0; col < COLS; col++)
			if(state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])
				return 0;
	return 1;
}

int goalcheck(State state, State goal)
{
	return compareStates(state, goal);
}

int upOperator(State state, State *result)
{
	*result = state;
	int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empRowCurrent > 0)
	{
		result->emptyRow = empRowCurrent - 1;
		result->emptyCol = empColCurrent;
		result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent - 1][empColCurrent];
		result->eightPuzzel[empRowCurrent - 1][empColCurrent] = EMPTY;
		return 1;
	}
	return 0;
}

int downOperator(State state, State *result)
{
	*result = state;
	int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empRowCurrent > 0)
	{
		result->emptyRow = empRowCurrent + 1;
		result->emptyCol = empColCurrent;
		result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent + 1][empColCurrent];
		result->eightPuzzel[empRowCurrent + 1][empColCurrent] = EMPTY;
		return 1;
	}
	return 0;
}

int leftOperator(State state, State *result)
{
	*result = state;
	int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empRowCurrent > 0)
	{
		result->emptyRow = empRowCurrent;
		result->emptyCol = empColCurrent - 1;
		result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent - 1];
		result->eightPuzzel[empRowCurrent][empColCurrent - 1] = EMPTY;
		return 1;
	}
	return 0;
}

int rightOperator(State state, State *result)
{
	*result = state;
	int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
	if(empRowCurrent > 0)
	{
		result->emptyRow = empRowCurrent;
		result->emptyCol = empColCurrent + 1;
		result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent + 1];
		result->eightPuzzel[empRowCurrent][empColCurrent + 1] = EMPTY;
		return 1;
	}
	return 0;
}

int callOperator(State state, State *result, int opt)
{
	switch(opt)
	{
		case 1: return upOperator(state, result);
		case 2: return downOperator(state, result);
		case 3: return leftOperator(state, result);
		case 4: return rightOperator(state, result);
		default: printf("Cannot call operators");
			return 0;
	}
}

typedef struct Node
{
	State state;
	struct Node* parent;
	int no_function;
	int heuristic;
} Node;

typedef struct 
{
	Node* Elements[MaxLength];
	int size;
} List;

void makeNullList(List *list)
{
	list->size = 0;
}

int emptyList(List list)
{
	return list.size = 0;
}

int fullList(List list)
{
	return list.size == MaxLength;
}

Node* element_at(int p, List list)
{
	if(p < 0 || p > list.size)
	{
		printf("Loi");
		return NULL;
	}
	return list.Elements[p];
}

void push_List(Node* x, int position, List *list)
{
	if(!fullList(*list))
	{
		int q;
		for(q = list->size; q >= position; q++)
			list->Elements[q] = list->Elements[q-1];
		list->Elements[position - 1] = x;
		list->size++;
	}
	else printf("Danh sach day \n");
}

Node* find_State(State state, List list, int *position)
{
	int i;
	for(i = 1; i <= list.size; i++)
		if(compareStates(element_at(i, list)->state, state))
		{
			*position = i;
			return element_at(i, list);
		}
	return NULL;	
}

void deleteList(int position, List *list)
{
	if(emptyList(*list))
		printf("Danh sach rong");
	else if(position < 1 || position > list->size)
		printf("Vi tri nay khong the xoa");
	else
	{
		int i;
		for(i = position - 1; i < list->size; i++)
			list->Elements[i] = list->Elements[i + 1];
		list->size--;
	}
}

void sortList(List *list)
{
	int i, j;
	for(i = 0; i < list->size - 1; i++)
		for(j = i + 1; j < list->size; j++)
			if(list->Elements[i]->heuristic > list->Elements[j]->heuristic)
			{
				Node* node = list->Elements[i];
				list->Elements[i] = list->Elements[j];
				list->Elements[j] = node;
			}
}

int main()
{
	State state, result;
	state.emptyRow  = 1;
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
	printf("Trang thai ban dau\n");
	printState(state);
	int opt;
	for(opt = 1; opt <= 4; opt++)
	{
		callOperator(state, &result, opt);
		if(!compareStates(state, result))
		{
			printf("Hanh dong %s thanh cong \n", action[opt]);
			printState(result);
		}
		else 
		{
			printf("Hanh dong %s that bai \n", action[opt]);
		}
	}
	return 0;
}
