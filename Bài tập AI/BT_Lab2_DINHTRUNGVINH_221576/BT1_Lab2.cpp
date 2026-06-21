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
