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
	return count;
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
    if(empRowCurrent < ROWS - 1) // Ði?u ki?n dúng
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
    if(empColCurrent > 0)
    {
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
    if(empColCurrent < COLS - 1)
    {
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
		default: printf("Khong the goi ham callOperator");
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
    return list.size == 0;
}

int fullList(List list)
{
	return list.size == MaxLength;
}

Node* element_at(int p, List list)
{
	if(p < 1 || p > list.size)
	{
		printf("Loi");
		return NULL;
	}
	return list.Elements[p - 1];
}

void pushList(Node* x, int position, List *list)
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

Node* findState(State state, List list, int *position)
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

Node* bestFirstSearch(State state, State goal)
{
	List Open_BFS;
	List Close_BFS;
	makeNullList(&Open_BFS);
	makeNullList(&Close_BFS);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_function = 0;
	root->heuristic = heuristic1(root->state, goal);
	pushList(root, Open_BFS.size + 1, &Open_BFS);
	while(!emptyList(Open_BFS))
	{
		Node* node = element_at(1, Open_BFS);
		deleteList(1, &Open_BFS);
		pushList(node, Close_BFS.size + 1, &Close_BFS);
		if(goalcheck(node->state, goal))
		{
			printf("Tim thay trang thai dich");
			return node;
		}
		int opt;
		for(opt = 1; opt <= MAX_OPERATOR; opt++)
		{
			State newstate;
			newstate = node->state;
			if(callOperator(node->state, &newstate, opt))
			{
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->parent = node;
				newNode->no_function = opt;
				newNode->heuristic = heuristic1(newstate, goal);
				int pos_Open, pos_Close;
				Node* nodeFoundOpen = findState(newstate, Open_BFS, &pos_Open);
				Node* nodeFoundClose = findState(newstate, Close_BFS, &pos_Close);
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL)
				{
					pushList(newNode, Open_BFS.size + 1, &Open_BFS);
				}
				else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic)
				{
					deleteList(pos_Open, &Open_BFS);
					pushList(newNode, pos_Open, &Open_BFS);
				}
				else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic)
				{
					deleteList(pos_Close, &Close_BFS);
					pushList(newNode, Open_BFS.size + 1, &Open_BFS);
				}
				sortList(&Open_BFS);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node)
{
	List listPrint;
	makeNullList(&listPrint);
	while(node->parent != NULL)
	{
		pushList(node, listPrint.size + 1, &listPrint);
		node = node->parent;
	}
	pushList(node, listPrint.size + 1, &listPrint);
	int no_action = 0, i;
	for(i = listPrint.size; i > 0; i--)
	{
		printf("\n Hanh dong %d: %s", no_action, action[element_at(i, listPrint)->no_function]);
		printState(element_at(i, listPrint)->state);
		no_action++;
	}
}

int main()
{
	State state;
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
	Node* p = bestFirstSearch(state, goal);
	print_WaysToGetGoal(p);
	return 0;
}
