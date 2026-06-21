#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty 0
#define goal 6
#define Maxlength 100

typedef struct {
  int x;
  int y;
} State;

void makeNullState(State *state) {
  state->x = 0;
  state->y = 0;
}

void print_State(State state) {
  printf("\n   X:%d --- Y:%d", state.x, state.y);
}

int goalcheck(State state) {
  return (state.x == goal || state.y == goal);
}

int pourWaterFullX(State cur_state, State *result) {
  if (cur_state.x < tankcapacity_X) {
    result->x = tankcapacity_X;
    result->y = cur_state.y;
    
    return 1;
  }
  
  return 0;
}

int pourWaterFullY(State cur_state, State *result) {
    if (cur_state.y < tankcapacity_Y) {
    result->x = cur_state.x;
    result->y = tankcapacity_Y;
    
    return 1;
  }
  
  return 0;
}

int pourWaterEmptyX(State cur_state, State *result) {
  if (cur_state.x > 0) {
    result->x = empty;
    result->y = cur_state.y;
    
    return 1;
  }
  
  return 0;
}

int pourWaterEmptyY(State cur_state, State *result) {
  if (cur_state.y > 0) {
    result->x = cur_state.x;
    result->y = empty;
    
    return 1;
  }
  
  return 0;
}

int pourWaterXY(State cur_state, State *result) {
  if (cur_state.x > 0 && cur_state.y < tankcapacity_Y) {
    result->x = fmax(cur_state.x - (tankcapacity_Y - cur_state.y), empty);
    result->y = fmin (cur_state.x+cur_state.y, tankcapacity_Y);
    
    return 1;
  }
  
  return 0;
}

int pourWaterYX(State cur_state, State *result) {
  if (cur_state.y > 0 && cur_state.x < tankcapacity_X) {
    result->x = fmin(cur_state.x+cur_state.y, tankcapacity_X);
    result->y = fmax(cur_state.y - (tankcapacity_X - cur_state.x), empty);
    
    return 1;
  }
  
  return 0;
}

int call_operator(State cur_state, State *result, int option) {
  switch (option) {
    case 1: return pourWaterFullX (cur_state, result) ;
    case 2: return pourWaterFullY (cur_state, result) ;
    case 3: return pourWaterEmptyX (cur_state, result) ;
    case 4: return pourWaterEmptyY (cur_state, result) ;
    case 5: return pourWaterXY (cur_state, result) ;
    case 6: return pourWaterYX (cur_state, result) ;
    default: printf ("Error calls operators") ;
      return 0;
  }
}


const char* action[] = {"First State", "pour Water Full X", "pour Water Full Y",
"pour Water Empty X", "pour Water Empty Y",
"pour Water X to Y", "pour Water Y to X"} ;


typedef struct Node {
	State state;
	struct Node* Parent;
	int no_function;
};

typedef struct {
	Node* Elements[Maxlength];
	int Top_idx;
} Stack;

void makeNull_Stack(Stack *stack) {
	stack->Top_idx = Maxlength;
}

int empty_Stack(Stack stack) {
	return stack.Top_idx == Maxlength;
}

int full_Stack(Stack stack) {
	return stack.Top_idx == 0;
}

Node* top(Stack stack) {
	if (!empty_Stack(stack))
		return stack.Elements[stack.Top_idx];
		
	return NULL;
}

void pop(Stack *stack) {
	if (!empty_Stack(*stack))
		stack->Top_idx += 1;
	else printf("Error! Stack is empty");
}

void push(Node* x, Stack *stack) {
	if (full_Stack(*stack))
		printf("Error! Stack is full");
	else {
		stack->Top_idx -= 1;
		stack->Elements[stack->Top_idx] = x;
	}
}

int compareStates(State s1, State s2) {
    return (s1.x == s2.x && s1.y == s2.y);
}

int find_State(State state, Stack openStack) {
	while (!empty_Stack(openStack)) {
		if (compareStates(top(openStack)->state, state))
			return 1;
		pop(&openStack);
	}
	return 0;
}

Node* DFS_Algorithm(State state) {
	Stack Open_DFS;
	Stack Close_DFS;
	makeNull_Stack(&Open_DFS);
	makeNull_Stack(&Close_DFS);
	
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	push(root, &Open_DFS);
	
	while(!empty_Stack(Open_DFS)) {
		Node* node = top(Open_DFS);
		pop(&Open_DFS);
		push(node, &Close_DFS);
		
		if (goalcheck(node->state))
			return node;
		int opt;
		
		for(opt = 1; opt <= 6; opt++) {
			State newstate;
			makeNullState(&newstate);
			
			if (call_operator(node->state, &newstate, opt)) {
				if (find_State(newstate, Close_DFS) || find_State(newstate, Open_DFS))
					continue;
					
				Node* newNode = (Node*) malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = opt;
				push(newNode, &Open_DFS);
			}
		}
	}
	
	return NULL;
}

void print_WaysToGetGoal(Node* node) {
	Stack stackPrint;
	makeNull_Stack(&stackPrint);
	
	while(node->Parent != NULL) {
		push(node, &stackPrint);
		node = node->Parent;
	}
	
	push(node, &stackPrint);
	
	int no_action = 0;
	
	while (!empty_Stack(stackPrint)) {
		printf("\nAction %d: %s", no_action, action[top(stackPrint)->no_function]);
		print_State(top(stackPrint)->state);
		pop(&stackPrint);
		no_action++;
	}
}

int main() {
	State cur_state = {0, 0};
	Node* p = DFS_Algorithm(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}
