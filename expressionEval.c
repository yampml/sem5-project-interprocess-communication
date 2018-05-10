#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Stack Integer-----------------------------------//
struct StackInt
{
	int top;
	int size;
	int* array; 
};

struct StackInt* initStack1(int size)
{
	struct StackInt* stack = (struct StackInt*) malloc(sizeof(struct StackInt));
    stack->size = size;
    stack->top = -1;
    stack->array = (int*) malloc(size * sizeof(char));
    return stack;
}

int isFull1(struct StackInt* stack)
{
	if(stack->top == stack->size - 1) return 1;
	else return 0;
}

int isEmpty1(struct StackInt* stack)
{
	if(stack->top == -1) return 1;
	else return 0;
}

void push1(struct StackInt *stack, int item)
{
	if (isFull1(stack))
	{
		printf("Stack is full!! Exit!!");
        exit(-1);
	}
    stack->array[++stack->top] = item;
    printf("%d pushed to stack\n", item);
}

int pop1(struct StackInt* stack)
{
    if (isEmpty1(stack))
    {
    	printf("Stack is Empty!! Exit!!");
    	exit(-1);
    }
    return stack->array[stack->top--];
}

int peek1(struct StackInt* stack)
{
	int k = pop1(stack);
	push1(stack, k);
	return k;
}

//End of Stack Integer-----------------------------------//

//Stack Char-----------------------------------//

struct StackChar
{
	int top;
	int size;
	char* array; 
};

struct StackChar* initStack2(int size)
{
	struct StackChar* stack = (struct StackChar*) malloc(sizeof(struct StackChar));
    stack->size = size;
    stack->top = -1;
    stack->array = (char*) malloc(size * sizeof(char));
    return stack;
}

int isFull2(struct StackChar* stack)
{
	if(stack->top == stack->size - 1) return 1;
	else return 0;
}

int isEmpty2(struct StackChar* stack)
{
	if(stack->top == -1) return 1;
	else return 0;
}

void push2(struct StackChar *stack, char item)
{
	if (isFull2(stack))
	{
		printf("Stack is full!! Exit!!");
        exit(-1);
	}
    stack->array[++stack->top] = item;
    printf("%d pushed to stack\n", item);
}

int pop2(struct StackChar* stack)
{
    if (isEmpty2(stack))
    {
    	printf("Stack is Empty!! Exit!!");
    	exit(-1);
    }
    return stack->array[stack->top--];
}

char peek2(struct StackChar* stack)
{
	char k = pop2(stack);
	push2(stack, k);
	return k;
}

//End of Stack Char-----------------------------------//


int precedence(char op1, char op2)
{
	if (op2 == '(' || op2 == ')')
		return 0;
	if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-'))
		return 0;
	else
		return 1;
}

int execOperator(int a, int b, char op)
{
	switch(op)
	{
		case '+': return a+b; break;
		case '-': return a-b; break;
		case '*': return a*b; break;
		case '/': if(b==0) {
					printf("Divide by zero!!");
					exit(-1);	
				}
				  return a/b; break;
	}
	return 0;
}

int evaluate(char expression[])
{
	

	struct StackChar* stackChar = initStack2(1000);

	struct StackInt* stackInt = initStack1(1000);

	for(int i=0; i<strlen(expression); i++)
	{
		printf("i=%d\n", i);fflush(stdout);
		if(expression[i] == ' ');
		else if (expression[i] >= '0' && expression[i] <= '9')
		{
			int k = 0;
			while (i < strlen(expression) && expression[i] >= '0' && expression[i] <= '9')
			{
				k = k*10 + (expression[i]-'0');
				i++;
			}
			if(expression[i] != ' ')i--;
			push1(stackInt, k);
		}
		else if (expression[i] == '(') push2(stackChar, expression[i]);
        else if (expression[i] == ')')
        {
            while(peek2(stackChar) != '(')
            	push1(stackInt, execOperator(pop1(stackInt), pop1(stackInt), pop2(stackChar)));
            pop2(stackChar);
        }
        else if (expression[i] == '+' || expression[i] == '-' ||
                 expression[i] == '*' || expression[i] == '/')
        {

			// printf("running here\n");fflush(stdout);
          	while(!isEmpty2(stackChar) && precedence(expression[i], peek2(stackChar)))
          		push1(stackInt, execOperator(pop1(stackInt), pop1(stackInt), pop2(stackChar)));

            push2(stackChar, expression[i]);
        }
	}

	while(!isEmpty2(stackChar)) push1(stackInt, execOperator(pop1(stackInt), pop1(stackInt), pop2(stackChar)));
	return pop1(stackInt);
}



int main()
{
	char buf[100];
	printf("Nhap bieu thuc: ");
	fgets(buf, sizeof(buf), stdin);
	buf[strlen(buf)-1] = '\0';
	int result = 0;

	printf("Ket qua: %d\n", evaluate(buf));

    // struct StackChar* stack = initStack2(100);
 
    // push2(stack, 'a');
    // push2(stack, 'b');
    // push2(stack, 'c');
    // push2(stack, 'd');
 
    // printf("%c popped from stack\n", pop2(stack));
 
    // push2(stack, 'e');
    // printf("%c popped from stack\n", pop2(stack));
 
    // printf("%c popped from stack\n", pop2(stack));
 
    // printf("%c popped from stack\n", pop2(stack));
 
    return 0;


}