#include <stdio.h>
#include "stack.h"

stack_t* init_stack(){
	stack_t* sp=(stack_t*)malloc(sizeof(stack_t));
	if (sp == NULL) MSGERROR(errno);
		
	sp->top=NULL;
	return sp;
}

void push(stack_t *s, stackData_t value){
	NodePtr_t np = (NodePtr_t)malloc(sizeof(Node_t));
	if (np == NULL) MSGERROR(STACKOVERFLOW);
	
	np->data=value;
	np->next=s->top;
	s->top=np;
}

bool is_stack_empty(stack_t *s){
	return (s->top == NULL);
}

stackData_t pop(stack_t *s){
	if (is_stack_empty(s)) MSGERROR(STACKUNDERDLOW);
	
	stackData_t data=s->top->data;
	NodePtr_t tmp=s->top;
	s->top=s->top->next;
	free(tmp);
	return data;
}

stackData_t peek(stack_t *s){
	if (is_stack_empty(s)) MSGERROR(STACKEMPTY);
	
	stackData_t data=s->top->data;
	return data;
}

void MSGERROR(int err){
	if (err == 0) fprintf(stderr, "%s\n", "StackOverflow"); 
	else if (err== 1) fprintf(stderr, "%s\n", "stackUnderflow");
	else if (err==2) fprintf(stderr, "%s\n","stack is empty"); 
	else fprintf(stderr, "%s\n", strerror(err)); 
	exit(EXIT_FAILURE); 
}
