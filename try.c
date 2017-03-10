#include <stdio.h>
#include "postfix.h"


int main(){
	char *out=NULL, *zout=NULL, var[10];
	while ((out=cmdline("Nafiu~"))){
		if (out[0]=='q') {
			free(out);
			break;
		}
		zout=infix_to_postfix(out);
		eval_postfix_exp(zout, var);
		printf("%s\n", var);
		//free(zout);
	}
}
