//#define _STACK_DATA_CHAR

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "postfix.h"

int MEMGROWTHSIZE=10;
char *prompt="[infix To posfix]~> ";
int err_bt=0;

#define MAXBACKETSIZE 20

char ret[MAXBACKETSIZE]; //get operator_eval output
//char result[MAXBACKETSIZE]; //get the result of the operation from eval_postfix_exp

enum { 
	COS_MATH_EXP,
	SIN_MATH_EXP, 
	LOG_MATH_EXP, 
	EXP_MATH_EXP, 
	TAN_MATH_EXP, 
	ABS_MATH_EXP 
};

void ERRORBRACKET(int err) {
	if (err==1) fprintf(stderr, "%s\n", "Brackets Mismatch:left bracket missing "); 
	else if (err == 2) fprintf(stderr, "%s\n", "Brackets Mismatch: right bracket missing"); 
}

void turn_pop_chars_inorder(char *arr, int len){
	int y=0;
	if (len > 1){
		char tmp[len+1];
		for (int i=len-1; i>=0; i--) tmp[y++]=arr[i];
		tmp[y]='\0';
		strncpy(arr, tmp, len+1);
	}	
}

double str_to_double(char *arr){
	errno=0;
	double result=strtod(arr, NULL);
	if ((errno == ERANGE && (result == HUGE_VAL)) || (errno != 0 && result == 0)){
		perror("strtol");
		exit(EXIT_FAILURE);
	}
	
	return result;
}

int operator_eval(char op, double bk1, double bk2, char set[]){
		
	double ans=0;
	switch(op){
		case '+':
			ans=bk1 + bk2;
			break;
		case '-':
			ans=bk2 - bk1;
			break;
		case '*':
			ans=bk2 * bk1;
			break;
		case '/':
			ans=bk2 / bk1;
			break;
		case '%':
			ans=(int)bk2 % (int)bk1;
			break;
		case '^':
			ans=pow(bk2, bk1);
			break;
		case 's':
			ans=sin(bk1);
			break;
		case 'c':
			ans=cos(bk1);
			break;
		case 'l':
			ans=log10(bk1);
			break;	
		case 'e':
			ans=exp(bk1);
			break;	
		case 't':
			ans=tan(bk1);
			break;
		case 'a':
			ans=(bk1 < 0) ? -bk1:bk1;
			break;	
	} 
	if (sprintf(set, "%g", ans) > 0) return 0;
	else return 1;
	
}

int precedence_table(char c){
	int test=0;
	switch(c){
		case '(':
			test=0;
			break;
		case '-':
		case '+':
			test=1;
			break;
		case '*':
		case '/':
		case '%':
			test=2;
			break;
		case '^':
			test=3;
			break;
		default:
			test=-1;
	}
	return test;
}

char* cmdline(const char *prompt){
	char *arr=(char*)malloc(MEMGROWTHSIZE*sizeof(char));
	if (arr == NULL) MSGERROR(errno);
	int inc=MEMGROWTHSIZE;
	int i=0;
		
	printf("%s ", prompt);
	while((arr[i++]=getchar()) != '\n'){
		if (i == inc) {
			arr=(char*)realloc(arr, (inc+=1)*sizeof(char));
			if (arr == NULL) {
				free(arr);
				MSGERROR(errno);
			}
		}
	}
	
	arr[--i]='\0';
		
	if(match_brackets(arr, &err_bt)) return arr;
	else {
		free(arr);
		return NULL;
	}
}

int CHECK(char *arr){
	int ret=-1;
	if (strcmp(arr, "cos")==0) ret=0;
	else if (strcmp(arr, "sin")==0) ret=1;
	else if (strcmp(arr, "log")==0) ret=2;
	else if (strcmp(arr, "exp")==0) ret=3;
	else if (strcmp(arr, "tan")==0) ret=4;	
	else if (strcmp(arr, "abs")==0) ret=5;
	return ret;
}

bool other_operators_test(char op[], int len){
	op[len+1]='\0';
	bool test=false;
		
		
	switch(CHECK(op)){
		case COS_MATH_EXP:
				test=true;
				break;
		case SIN_MATH_EXP:		
				test=true;
				break;
		case LOG_MATH_EXP:			
				test=true;
				break;
		case EXP_MATH_EXP:
				test=true;
				break;
		case TAN_MATH_EXP:			
				test=true;
				break;
		case ABS_MATH_EXP:	
				test=true;
				break;
	}
	
	return test;
}

bool match_brackets(char *arr, int *errcode){
	int num_of_left_bkt=0, num_of_right_bkt=0;
	*errcode=0;
	
	for (int i=0; i<strlen(arr); i++){
		if (arr[i]=='(') ++num_of_left_bkt;
		else if (arr[i]==')') ++num_of_right_bkt;
	}
		
	if (num_of_left_bkt < num_of_right_bkt) *errcode=1;
	else *errcode=2;
	
	if (num_of_left_bkt != num_of_right_bkt) 
		return false;
	else return true;
}


void callback_infix_to_postfix_exp(void){
	
	char *resp=NULL, *output=NULL, other_op[6];
	bool ret_of_other_op_test=false;
		
	error:
	while ((resp=cmdline(prompt)) && resp[0] != 'q' ){
		output=(char*)malloc((strlen(resp)+1) * sizeof(char));
		if (output==NULL) MSGERROR(errno);
				
		char ch;
		stack_t* stack=init_stack();
		int j=0, len_op=0, p=0, other_op_len=0;
				
		for (int i=0; i<strlen(resp); i++){
			if (isspace(resp[i]) || isdigit(resp[i])) output[j++]=resp[i];
			else if (resp[i]=='(') push(stack, '(');
			else if (resp[i]==')') { 
				while ((ch=pop(stack)) != '(') output[j++]=ch;
				
				if (ret_of_other_op_test) 
					while (p < other_op_len) output[j++]=other_op[p++];
								
				len_op=0;
			}
			else if (isalpha(resp[i])) { 
				other_op[len_op++]=resp[i];
				other_op_len=len_op;
								
				if ((ret_of_other_op_test=other_operators_test(other_op, len_op)))
								len_op=0;
			}
			else {
				while (!is_stack_empty(stack) && precedence_table(peek(stack)) >= precedence_table(resp[i]) ) output[j++]=pop(stack);
				push(stack, resp[i]);
			}
		}
		
		while(!is_stack_empty(stack)) output[j++]=pop(stack);
		output[j]='\0';
				
		if (strcmp(resp, "clear")==0){
			#ifdef WIN32
				system("cls");
			#else
				system("clear");
			#endif	
			continue;
		} else if (strcmp(output, "help") == 0 || output[0] == 'h'){
			printf("\t%s\n\t%s\n\t%s\n", "clear --  To clear the screen", "help/h -- To display help", "quit/q -- To exit");
			continue;
		}
				
		printf("%s\n", output);
		free(resp);
	}
		
	if (resp == NULL){
		ERRORBRACKET(err_bt);
		free(resp);
		goto error;
	} else free(resp);
	
}

char* infix_to_postfix(char *arr){
	char* output=(char*)malloc((strlen(arr)+1) * sizeof(char));
	if (output==NULL) MSGERROR(errno);
	
	bool ret_of_other_op_test=false;		
	char ch, other_op[6];
	int j=0, len_of_other_op=0, p=0;
		
	stack_t* stack=init_stack();
		
	for (int i=0; i<strlen(arr); i++){
			
		if (isspace(arr[i]) || isdigit(arr[i]) || arr[i]=='.') output[j++]=arr[i];
		else if (arr[i]=='(') push(stack, '(');
		else if (arr[i]==')') { 
			while ((ch=pop(stack)) != '(') output[j++]=ch;
				
			if (ret_of_other_op_test) while (p < len_of_other_op) output[j++]=other_op[p++];
		}
		else if (isalpha(arr[i])) { 
			other_op[len_of_other_op++]=arr[i];
			ret_of_other_op_test=other_operators_test(other_op, len_of_other_op);
		}
		else {
			while (!is_stack_empty(stack) && precedence_table(peek(stack)) >= precedence_table(arr[i]) ) output[j++]=pop(stack);
			push(stack, arr[i]);
		}
	}
		
	while(!is_stack_empty(stack)) output[j++]=pop(stack);
	output[j]='\0';	
	
	free(arr);
	return output;
}

void eval_postfix_exp(char *arr, char result[]){
#if 0
	space chars are used as the delimitor for the numbers on the stack. so
	a number is a string of contiguous chars with no spaces in between. if there 
	are spaces between the string of chars on the stack then they are different numbers .
#endif
	
	char tmp_backet1[MAXBACKETSIZE], tmp_backet2[MAXBACKETSIZE], ch; 
	int j=0, x=0, len=0, n;
	stack_t *stack=init_stack();
		
	for (int i=0; i<strlen(arr); i++){
		if (isdigit(arr[i]) || isspace(arr[i]) || arr[i]=='.') push(stack, arr[i]);
		else if (isalpha(arr[i])) {
			x=0, n=0;
			while (!is_stack_empty(stack) && isspace(peek(stack))) ch=pop(stack); //remove spaces from the stack
			while (!is_stack_empty(stack) && (ch=pop(stack)) != ' ') tmp_backet1[x++]=ch; //get string of chars as num from stack
			tmp_backet1[x]='\0';
			
			turn_pop_chars_inorder(tmp_backet1, strlen(tmp_backet1));
			double value=str_to_double(tmp_backet1);
						
			if (operator_eval(arr[i], value, 0, ret)==0) {
			
				if (!is_stack_empty(stack)) {
					if (isdigit(ch=peek(stack))) push(stack, ' '); //separate nums
					while(n != strlen(ret)) push(stack, ret[n++]);
				}
				else {
					push(stack, ' '); //separate nums
					while(n != strlen(ret)) push(stack, ret[n++]);	
				}		
			}
			i+=2; //read pass alphas
		}
		else {
			x=0;
			while (!is_stack_empty(stack) && isspace(peek(stack))) ch=pop(stack); //remove spaces from the stack
			while (!is_stack_empty(stack) && (ch=pop(stack)) != ' ') tmp_backet1[x++]=ch; //get string of chars as num from stack
			while (!is_stack_empty(stack) && isspace(peek(stack))) ch=pop(stack); //remove spaces from the stack
			while (!is_stack_empty(stack) && (ch=pop(stack)) != ' ') tmp_backet2[j++]=ch;//get string of chars as num from stack
			tmp_backet1[x]='\0';
			tmp_backet2[j]='\0';
			
									
			turn_pop_chars_inorder(tmp_backet1, strlen(tmp_backet1));
			turn_pop_chars_inorder(tmp_backet2, strlen(tmp_backet2));
					
			double bkt1=str_to_double(tmp_backet1);
			double bkt2=str_to_double(tmp_backet2);
					
			
			j=0; x=0; n=0;
			if (operator_eval(arr[i], bkt1, bkt2, ret)==0) {
				
				if (!is_stack_empty(stack)){
					if (isdigit(ch=peek(stack))) push(stack, ' '); //separate nums
					while(n != strlen(ret)) push(stack, ret[n++]);
				}
				else {
					push(stack, ' '); //separate nums
					while(n != strlen(ret)) push(stack, ret[n++]);
				}
			}
		}
	}
	
	x=0;
	while(!is_stack_empty(stack) && (ch=pop(stack))) result[x++]=ch;
	result[x]='\0';
	
	free(arr);
	x=0;
	if ((len=strlen(result)) > 1) { 
		char result_of_computation[len+1];
		for (int i=len-1; i>=0; i--) result_of_computation[x++]=result[i];
		result_of_computation[x]='\0';
		strncpy(result, result_of_computation, len+1);
	}
}


