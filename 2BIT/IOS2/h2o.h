/**
 *
 * File		h2o.h
 * Author	Michal Ormos
 * E-mail:	xormos00@stud.fit.vutbr.cz
 * Date		16.4.2015
 * Brief	.h file suitable for h2o.c
 *
 *	The program is using for demostrated processes synchronization 
 *  caries basic using of semaphore, shared memory.
 *
 */	

#include <stdio.h> // printf()
#include <stdlib.h> //exit()
#include <time.h> //using srand(time(NULL))
#include <unistd.h> //using fork()
#include <semaphore.h> //sem_open()
#include <sys/types.h> // using waitpid |
#include <sys/wait.h>  //            <--| 
#include <sys/mman.h> //for mman() function
#include <sys/shm.h> //for shmat	

#define EXIT_OK 0
#define EXIT_WRONG 1
#define EXIT_ERROR 2

/* Arguments/Paramteters declaration */
typedef struct params {
	int N,
		GH,
		GO,
		B;
} Tparam;

/* Declarate prototypes of functions */
void create_oxygen();
void create_hydrogen();
void free_var();
void init_var();
int increase_id(int *id);
void bond(int p_id, int atom);
void bond_end(int p_id, int atom);
void bond_finish(int n);