#ifndef _POSTFIX_EXP_H
#define _POSTFIX_EXP_H

#ifndef _STACK_DATA_CHAR
#define _STACK_DATA_CHAR
#endif

#include "stack.h"
#include <string.h>

//int MEMGROWTHSIZE;
extern int err_bt;


//int CHECK(char *);
void ERRORBRACKET(int err);
int operator_eval(char , double, double, char[]);
char* cmdline(const char *prompt); //use free after using this function
bool match_brackets(char *arr, int *errcode);
void callback_infix_to_postfix_exp(void); //just a call back function just include it a .c/.cpp file and u have a full postfix/infix translator
char* infix_to_postfix(char*); //use free after using this function
int precedence_table(char c);
void eval_postfix_exp(char*, char[]); 

//other operators like trigs logarithms etc
bool other_operators_test(char[], int);

#endif
