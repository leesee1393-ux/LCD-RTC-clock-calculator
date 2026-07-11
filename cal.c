/*
cal.c
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cal.h"

char temp[BUF_SIZE];

// =====================================================
// STACK 구현
// =====================================================
void init_stack(Stack *s) { s->top = -1; }
int is_empty_stack(Stack *s) { return (s->top == -1); }
int is_full_stack(Stack *s) { return (s->top >= STACK_SIZE - 1); }

void push(Stack *s, int value) {
	if (!is_full_stack(s)) s->data[++(s->top)] = value;
}

int pop(Stack *s) {
	if (!is_empty_stack(s)) return s->data[(s->top)--];
	return -1;
}

int peek(Stack *s) {
	if (!is_empty_stack(s)) return s->data[s->top];
	return -1;
}

// =====================================================
// QUEUE 구현 (필요시 활용)
// =====================================================
void init_queue(Queue *q) { q->front = -1; q->rear = -1; }
int is_empty_queue(Queue *q) { return (q->front == q->rear && q->front == -1); }
int is_full_queue(Queue *q) { return ((q->rear + 1) % QUEUE_SIZE == q->front); }

void enqueue(Queue *q, char value) {
	if (!is_full_queue(q)) {
		if (q->front == -1) q->front = 0;
		q->rear = (q->rear + 1) % QUEUE_SIZE;
		q->data[q->rear] = value;
	}
}

char dequeue(Queue *q) {
	char value = 0;
	if (!is_empty_queue(q)) {
		value = q->data[q->front];
		if (q->front == q->rear) { q->front = -1; q->rear = -1; }
		else q->front = (q->front + 1) % QUEUE_SIZE;
	}
	return value;
}

// =====================================================
// 연산자 우선순위
// =====================================================
int get_priority(char op) {
	if (op == '+' || op == '-') return 1;
	if (op == '*' || op == '/') return 2;
	if (op == '(') return 0;
	return -1;
}

// =====================================================
// 중위 표기법 → 후위 표기법 변환 
// =====================================================
void infix_to_postfix(char *infix, char *postfix) {
	Stack op_stack;
	init_stack(&op_stack);
	
	int i = 0, j = 0;
	char token;
	
	while ((token = infix[i]) != '\0') {
		if (token == ' ') { i++; continue; }
		
		if (isdigit(token)) {
			postfix[j++] = token;
			if (isdigit(infix[i+1])) postfix[j++] = ' ';
		}
		else if (token == '(') {
			push(&op_stack, token);
		}
		else if (token == ')') {
			while (!is_empty_stack(&op_stack) && peek(&op_stack) != '(') {
				postfix[j++] = pop(&op_stack);
				postfix[j++] = ' ';
			}
			pop(&op_stack); // '(' 제거
		}
		else if (token == '+' || token == '-' || token == '*' || token == '/') {
			while (!is_empty_stack(&op_stack) &&
			get_priority(peek(&op_stack)) >= get_priority(token)) {
				postfix[j++] = pop(&op_stack);
				postfix[j++] = ' ';
			}
			push(&op_stack, token);
		}
		i++;
	}
	
	while (!is_empty_stack(&op_stack)) {
		postfix[j++] = pop(&op_stack);
		postfix[j++] = ' ';
	}
	
	if (j > 0) postfix[j-1] = '\0';
	else postfix[0] = '\0';
}

// =====================================================
// 후위 표기법 계산
// =====================================================
int evaluate_postfix(char *postfix) {
	Stack val_stack;
	init_stack(&val_stack);
	
	int i = 0;
	while (postfix[i] != '\0') {
		if (postfix[i] == ' ') { i++; continue; }
		
		if (isdigit(postfix[i])) {
			int num = 0;
			while (isdigit(postfix[i])) {
				num = num * 10 + (postfix[i] - '0');
				i++;
			}
			push(&val_stack, num);
		}
		else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/') {
			int op2 = pop(&val_stack);
			int op1 = pop(&val_stack);
			int res = 0;
			
			if (postfix[i] == '+') res = op1 + op2;
			else if (postfix[i] == '-') res = op1 - op2;
			else if (postfix[i] == '*') res = op1 * op2;
			else if (postfix[i] == '/') res = (op2 != 0) ? op1 / op2 : 0;
			
			push(&val_stack, res);
			i++;
		}
		else {
			i++;
		}
	}
	return pop(&val_stack);
}

// =====================================================
// 초기화 및 메인 계산 함수 
// =====================================================
void init_cal(void) {
	temp[0] = '\0';
}

int calculator(char *buff) {
	char postfix[BUF_SIZE];
	infix_to_postfix(buff, postfix);
	return evaluate_postfix(postfix);
}