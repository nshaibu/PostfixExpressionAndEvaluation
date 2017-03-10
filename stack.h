#ifndef _STACK_LINKED_LIST_H
#define _STACK_LINKED_LIST_H

#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern int errno;

#ifdef _STACK_DATA_INT
	typedef int stackData_t;
#endif
#ifdef _STACK_DATA_STRING
	typedef char* stackData_t;
#endif
#ifdef _STACK_DATA_CHAR
	typedef char stackData_t;
#endif
#ifdef _STACK_DATA_FLOAT
	typedef float stackData_t;
#endif
#ifdef _STACK_DATA_DOUBLE
	typedef double stackData_t;
#endif
	
	//typedef unsigned int w_char32_t;
		
	enum { STACKOVERFLOW, STACKUNDERDLOW, STACKEMPTY };
		
	typedef struct node {
		stackData_t data;
		struct node* next;
	}Node_t, *NodePtr_t;
		
	typedef struct {
		NodePtr_t top;
	} stack_t;
		
	stack_t* init_stack();
	void push(stack_t *, stackData_t value);
	stackData_t pop(stack_t *);
	bool is_stack_empty(stack_t *);
	stackData_t peek(stack_t *);
		
	void MSGERROR(int err);

#endif
