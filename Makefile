CC=gcc
OBJ= stack.o postfix.o
INCLUDE=
LIB=-lm
DEFINES= -D_STACK_DATA_CHAR 
CFLAGS= -c -g -std=c99 -Wall -Werror $(DEFINES) $(INCLUDE) $(LIB)

postfixevalexam: stack.o postfix.o try.o
	$(CC) -g -std=c99 $(OBJ) try.o -o postfixevalexam $(LIB)

postfix_exam: stack.o postfix.o postfix_exam.o
	$(CC) -g -std=c99 $(OBJ) postfix_exam.o -o postfix_exam -lm

postfix_exam.o: stack.h postfix.h postfix_exam.c
	$(CC) $(CFLAGS) postfix_exam.c -lm
		
try.o: stack.h postfix.h try.c
	$(CC) $(CFLAGS) try.c 
		
postfix.o: stack.h postfix.h postfix.c
	$(CC) $(CFLAGS) postfix.c
	
stack.o: stack.h stack.c
	$(CC) $(CFLAGS) stack.c

clean:
	rm *.o
