#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

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
    // printf("%d pushed to stack\n", item);
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
    // printf("%d pushed to stack\n", item);
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

//Expression Evaluation===============================//
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
		// printf("i=%d\n", i);fflush(stdout);
		if(expression[i] == ' ') continue;

		if (expression[i] >= '0' && expression[i] <= '9')
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

//End of expression evaluation===================================

//Message Queue==================================================
struct msgbuf{
	long mtype;
	char expression[128];
};

int open_queue(key_t keyval)
{
	int qid;
	if((qid = msgget(keyval, IPC_CREAT|0660)) == -1)
	{
		return -1;
	}
	return qid;
}

int send_message(int qid, struct msgbuf *qbuf)
{
	int result, length;

	length = sizeof(struct msgbuf) - sizeof(long);

	if((result = msgsnd(qid, qbuf, length, 0)) == -1) {
		return -1;
	}
	return result;
}

int read_message(int qid, long type, struct msgbuf *qbuf)
{
	int result, length;
	length = sizeof(struct msgbuf) - sizeof(long);

	if((result = msgrcv(qid, qbuf, length, type, 0)) == -1) { //can change ? IPC_NOWAIT
		return -1;
	}
	return result;
}



int main()
{
	// printf("%o\n", IPC_CREAT);
	// printf("%o\n", IPC_EXCL);
	// printf("%o\n", IPC_NOWAIT);
	// printf("%o\n", IPC_CREAT|0660);
	// return 0;

	//Child Process Identifier
	pid_t cpid;
	
	//Message Queue Variables
	int qid;
	key_t msgkey;

	//generate key
	msgkey = ftok(".", 'a');

	//open/create queue
	if((qid = open_queue(msgkey)) == -1)
	{
		perror("open queue");
		exit(1);
	}

	cpid = fork();
	
	if(cpid != 0) //Parent's job
	{
		printf("Nhap bieu thuc tinh toan: ");
		char expr[128];
		fgets(expr, sizeof(expr), stdin);
		expr[strlen(expr)] = '\0';
			// a += i;
			// b += i;
		struct msgbuf sending;
		sending.mtype = 11;
		strcpy(sending.expression, expr);
		send_message(qid, &sending);
			// int status;
			// wait(NULL);
		wait(NULL);
		struct msgbuf re_receive;
		re_receive.mtype = 10;
		read_message(qid, 10, &re_receive);

		printf("\n\nGia tri bieu thuc: %s \n", re_receive.expression );
		
	}
	else //Child's job
	{
		struct msgbuf receiving;
		read_message(qid, 11, &receiving);

		int res = evaluate(receiving.expression);

			//eval(receiving.expression);

		struct msgbuf resend;
		resend.mtype = 10;
		sprintf(resend.expression, "%d", res);
		send_message(qid, &resend);
	}
	
	// if(cpid != 0) wait(NULL);
}