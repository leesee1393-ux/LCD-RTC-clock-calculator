/*
cal.h
*/
#ifndef CAL_H_
#define CAL_H_

#define BUF_SIZE 32
#define STACK_SIZE 32
#define QUEUE_SIZE 32

// STACK 구조체 정의
typedef struct {
	int data[STACK_SIZE];
	int top;
} Stack;

// QUEUE 구조체 정의
typedef struct {
	char data[QUEUE_SIZE];
	int front;
	int rear;
} Queue;

void init_cal(void);
int calculator(char *buff);

// STACK 연산 함수 선언
void init_stack(Stack *s);
int is_empty_stack(Stack *s);
int is_full_stack(Stack *s);
void push(Stack *s, int value);
int pop(Stack *s);
int peek(Stack *s);

// QUEUE 연산 함수 선언
void init_queue(Queue *q);
int is_empty_queue(Queue *q);
int is_full_queue(Queue *q);
void enqueue(Queue *q, char value);
char dequeue(Queue *q);

// 중위→후위 변환 및 계산 함수 선언
void infix_to_postfix(char *infix, char *postfix);
int get_priority(char op);
int evaluate_postfix(char *postfix);

#endif /* CAL_H_ */