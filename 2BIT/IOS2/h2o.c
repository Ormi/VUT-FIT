/**
 *
 * File		h2o.c
 * Author	Michal Ormos
 * E-mail:	xormos00@stud.fit.vutbr.cz
 * Date		16.4.2015
 * Brief	1BIT Operating Systems Project 2: Building H2O
 *
 *	The program is using for demostrated processes synchronization 
 *  caries basic using of semaphore, shared memory.
 *
 */										

#include "h2o.h"

/* Semaphores declaration for mutual exclusion */
sem_t *sem_bond,
	  *sem_bond_h,
	  *sem_bond_o,
	  *sem_create,
	  *sem_finish,
	  *sem_file_protect,
	  *sem_end_bond,
	  *sem_end;

/* Mutual memory initial declaration */
int shm_line_counter = 0,
	shm_h_id = 0,
	shm_o_id = 0,
	shm_ph_id = 0,
	shm_po_id = 0,
	shm_ready_id = 0,
	shm_bond_counter = 0,
	shm_b_size = 0,
	shm_n_size = 0;

/* Mutual memory variables | initial declaration */
int *o_counter = NULL,
	*h_counter = NULL,
	*line_counter = NULL,
	*bond_h = NULL,
	*bond_o = NULL,
	*bond_ready = NULL,
	*bond_counter = NULL,
	*n_size = NULL,
	*b_size = NULL;

/* Global variables */
FILE *fp;

int main(int argc, char **argv) {

int get_id=0;
int p_time=0;
int i;
Tparam params;

	if ((argc < 2) || (argc > 5)) {
		fprintf(stderr, "Not enought or too much argument.\n");
		return EXIT_FAILURE;   
	}
	
	/* Get arguments to variables */
	params.N = atoi(argv[1]);
	params.GH = atoi(argv[2]);
	params.GO = atoi(argv[3]);
	params.B = atoi(argv[4]); 

	/* Check right interval of variables */
	if((argc!=5) || (params.N<=0) || 
		(params.GH<0 || params.GH>5000) || 
		(params.GO<0 || params.GO>5000) || 
		(params.B<0 || params.B>5000))
	{
		fprintf(stderr, "Invalid value of arguments\n");
		exit(EXIT_ERROR);
 	}

	/* Existing check of output file */
	fp = fopen("h2o.out", "w");
	if(fp==NULL)
	{
		fprintf(stderr, "Unable to access h2o.out\n");		
		exit(2);
	}

	/* Inicialization of semaphores*/
	init_var();

	*o_counter=1;
	*h_counter=1;
	*line_counter=0;
	*bond_h=0;
	*bond_o=0;
	*bond_ready=0;
	*bond_counter=0;
	*n_size=params.N,
	*b_size=params.B;

	/*  Array of processes and their inicialization */
	int childrens[params.N*3];
	for (int i = 0; i < params.N*3; i++) {
		childrens[i] = 0;
	}	
	
	get_id = fork();

	/* Parent | making hydrogen */
	if(get_id == 0) { //parent
		for(i = 0; i < params.N*2; i++) {

			p_time = (random() % (params.GH + 1));
			usleep(p_time * 1000);

			get_id = fork();

			if(get_id == 0) {
				create_hydrogen(params);
				break;
			} else if(get_id > 0) {
				childrens[i] = get_id;
			} else {
				fprintf(stderr, "Error creating hydrogen. \n");
				exit(EXIT_WRONG);
			}
		}
	/* Childrens | making oxygen */
	} else if(get_id > 0) {
		for(i = 0; i < params.N; i++) { 

			p_time = (random() % (params.GO + 1));
			usleep(p_time * 1000);

			get_id = fork();

			if(get_id == 0) {
				create_oxygen(params);
				break;
			} else if(get_id > 0) {
				childrens[i] = get_id;
			} else {
				fprintf(stderr, "Error creating oxygen. \n");
				exit(EXIT_WRONG);
			}
		}
	} else {
		fprintf(stderr, "Error creating child\n");
		exit(1);
	}


	/* Release childrens */
	for(i = 0; i < (params.N*3); i++) {
		waitpid(childrens[i], NULL, 0);
	}

free_var();

return EXIT_SUCCESS;
}

void create_hydrogen()
{
	sem_wait(sem_finish);
	int	process_id = *h_counter;	
	(*h_counter)++;
	sem_post(sem_finish	);

	sem_wait(sem_create);

	sem_wait(sem_finish);	
	(*bond_h)++;	

	sem_wait(sem_file_protect);	
	fprintf(fp, "%d: H %d: started\n", ++(*line_counter) ,process_id);	
	sem_post(sem_file_protect);		
	fflush(fp);

	sem_post(sem_finish);	


	/** If there is 2 hydrogen and one oxygen make last atom readty 
	 * otherwise waiting for other atoms 
	 * sem_bond is relase after waiting or after success boding in function bond 
	 */
	sem_wait(sem_bond);

	if(((*bond_o) >= 1) && ((*bond_h) >= 2)) {
		sem_wait(sem_file_protect);
		fprintf(fp, "%d: H %d: ready\n", ++(*line_counter), process_id);
		sem_post(sem_file_protect);			
		fflush(fp);		

		(*bond_o) -= 1;
		(*bond_h) -= 2;	

		/* Release 3 resources for 2 hydrogen and 1 oxygen */
		sem_post(sem_create);

		sem_post(sem_bond_h);
		sem_post(sem_bond_h);
		sem_post(sem_bond_o);		
		
	} else {
		sem_wait(sem_file_protect);
		fprintf(fp, "%d: H %d: waiting\n", ++(*line_counter), process_id);	
		sem_post(sem_file_protect);	
		fflush(fp);

		sem_post(sem_create);
		sem_post(sem_bond);		
	}

	sem_wait(sem_bond_h);
	bond(process_id, 0);
}

/* Same as hydrogen */
void create_oxygen()
{
	sem_wait(sem_finish);
	int	process_id = *o_counter;	
	(*o_counter)++;
	sem_post(sem_finish);

	sem_wait(sem_create);
	sem_wait(sem_finish);

	(*bond_o)++;

	sem_wait(sem_file_protect);	
	fprintf(fp, "%d: O %d: started\n", ++(*line_counter), process_id);	
	sem_post(sem_file_protect);	
	fflush(fp);

	sem_post(sem_finish);	
	
	sem_wait(sem_bond);
	
	if(((*bond_o) >= 1) && ((*bond_h) >= 2)) {	
		sem_wait(sem_file_protect);
		fprintf(fp, "%d: O %d: ready\n", ++(*line_counter), process_id);
		sem_post(sem_file_protect);			
		fflush(fp);	

		(*bond_o) -= 1;
		(*bond_h) -= 2;	

		sem_post(sem_create);

		sem_post(sem_bond_h);
		sem_post(sem_bond_h);
		sem_post(sem_bond_o);	

	} else {
		sem_wait(sem_file_protect);		
		fprintf(fp, "%d: O %d: waiting\n", ++(*line_counter), process_id);		
		sem_post(sem_file_protect);			
		fflush(fp);

		sem_post(sem_create);
		sem_post(sem_bond);	
	}

	sem_wait(sem_bond_o);
	bond(process_id, 1);
}

/** Functino for printing "begin bonding", "bonded", "finished"
 *	atom 0 = hydrogen, atom 1 = oxygen 
 *  b_size is B value for sleep proccess
 * 	bond_counter is for counting atoms to 3 so we know when start next part
 */
void bond(int process_id, int atom) {

int p_time;

sem_wait(sem_finish);

/* Begin bonding part */
	if(atom == 0) {
		sem_wait(sem_file_protect);
		fprintf(fp, "%d: H %d: begin bonding\n", ++(*line_counter), process_id);
			p_time = (random() % (*(b_size)+ 1));
			usleep(p_time * 1000);
		sem_post(sem_file_protect);			
		fflush(fp);
		(*bond_counter)++;
	}else if(atom == 1) {	
		sem_wait(sem_file_protect);
		fprintf(fp, "%d: O %d: begin bonding\n", ++(*line_counter), process_id);
			p_time = (random() % (*(b_size) + 1));
			usleep(p_time * 1000);
		sem_post(sem_file_protect);			
		fflush(fp);		
		(*bond_counter)++;
	}

	/* Test for 3 successful "begin bonding" atoms */
	if((*bond_counter) == 3) {
		(*bond_counter)=0;	
		sem_post(sem_end_bond);
		sem_post(sem_end_bond);
		sem_post(sem_finish);
	} else {		
		sem_post(sem_finish);
		sem_wait(sem_end_bond);
	}

/* Bonded part */
	if(atom == 0) {
		sem_wait(sem_finish);

		sem_wait(sem_file_protect);		
		fprintf(fp, "%d: H %d: bonded\n", ++(*line_counter), process_id);
		sem_post(sem_file_protect);		
		fflush(fp);
		(*bond_counter)++;
	}else if(atom == 1) {
		sem_wait(sem_finish);

		sem_wait(sem_file_protect);
		fprintf(fp, "%d: O %d: bonded\n", ++(*line_counter), process_id);	
		sem_post(sem_file_protect);	
		fflush(fp); 	
		(*bond_counter)++;	
	}
	/* Test for 3 successful "bonded" atoms */
	if((*bond_counter) == 3) {
		(*bond_counter)=0;
		sem_post(sem_bond);
	}			

	sem_post(sem_finish);
	sem_wait(sem_finish);	

	/* Atoms counter for final "finished" stage */
	(*bond_ready)++;
	if ((*bond_ready) == (*n_size)*3) {
		for (int i = 0; i < (*n_size)*3; ++i)
		{
			sem_post(sem_end);		
		}
	}

	sem_post(sem_finish);
	sem_wait(sem_end);

/* Finished part */
	if(atom == 0) {
		sem_wait(sem_finish);

		sem_wait(sem_file_protect);
		fprintf(fp, "%d: H %d: finished\n", ++(*line_counter), process_id);	
		sem_post(sem_file_protect);	
		fflush(fp);

		sem_post(sem_finish);
	}
	if(atom == 1) {
		sem_wait(sem_finish);

		sem_wait(sem_file_protect);
		fprintf(fp, "%d: O %d: finished\n", ++(*line_counter), process_id);
		sem_post(sem_file_protect);
		fflush(fp);

		sem_post(sem_finish);		
	}

}

void init_var(){
/* Important declaration of semaphores */
	/* Declaration semaphores variables */
	if((sem_create = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
	   (sem_bond = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
	   (sem_bond_h = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
	   (sem_bond_o = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||	   
	   (sem_finish = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
	   (sem_file_protect = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||	   
	   (sem_end_bond = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||
	   (sem_end = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED ||	   	   
	   /* Declaration of semaphores */
	   sem_init(sem_create, 1, 1) == -1 ||	   
	   sem_init(sem_bond, 1, 1) == -1 || 
	   sem_init(sem_bond_h, 1, 0) == -1 || 
	   sem_init(sem_bond_o, 1, 0) == -1 ||
	   sem_init(sem_end_bond, 1, 0) == -1 ||	   
	   sem_init(sem_end, 1, 0) == -1 ||
	   sem_init(sem_file_protect, 1, 1) == -1 ||
	   sem_init(sem_finish, 1, 1) == -1) {
    		fprintf(stderr, "Failed in creating semaphores\n");
    		exit(EXIT_ERROR);
    		
    		  /* Declaration of help variables for shmat */
	} else if((shm_line_counter = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 || 
			  (shm_h_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
			  (shm_o_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
			  (shm_ph_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
			  (shm_po_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||
			  (shm_ready_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||			  
			  (shm_bond_counter = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||		
			  (shm_n_size = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||	
			  (shm_b_size = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1 ||				  			  				  		  
			  /* Counter declaration */
			  (o_counter = (int *) shmat(shm_o_id, NULL, 0)) == NULL ||
			  (h_counter = (int *) shmat(shm_h_id, NULL, 0)) == NULL ||
			  (line_counter = (int *) shmat(shm_line_counter, NULL, 0)) == NULL ||
			  (bond_h = (int *) shmat(shm_ph_id, NULL, 0)) == NULL ||
			  (bond_o = (int *) shmat(shm_po_id, NULL, 0)) == NULL ||
			  (bond_counter = (int *) shmat(shm_bond_counter, NULL, 0)) == NULL ||	
			  (n_size = (int *) shmat(shm_n_size, NULL, 0)) == NULL ||	
			  (b_size = (int *) shmat(shm_b_size, NULL, 0)) == NULL ||				  		  	
			  (bond_ready = (int *) shmat(shm_ready_id, NULL, 0)) == NULL) {		
				  fprintf(stderr, "Failed to allocate mutual memory\n");
				  exit(EXIT_ERROR);
	}

}

void free_var(){

	/* Close/release file */
	fclose(fp);

	   /* Release semaphores */
	if(sem_destroy(sem_bond) == -1 ||
	   sem_destroy(sem_bond_h) == -1 ||
	   sem_destroy(sem_bond_o) == -1 ||	   	   
	   sem_destroy(sem_create) == -1 ||
	   sem_destroy(sem_end_bond) == -1 ||
	   sem_destroy(sem_end) == -1 ||
	   sem_destroy(sem_file_protect) == -1 ||
	   sem_destroy(sem_finish) == -1) {
		   fprintf(stderr, "Failed to clear semaphores.\n");
		   exit(EXIT_ERROR);	
	}
	   /* Release shared memory variables */
	if(shmctl(shm_line_counter, IPC_RMID, NULL) == -1 ||
	   shmctl(shm_h_id, IPC_RMID, NULL) == -1 ||
	   shmctl(shm_o_id, IPC_RMID, NULL) == -1 ||
	   shmctl(shm_ph_id, IPC_RMID, NULL) == -1 ||
	   shmctl(shm_po_id, IPC_RMID, NULL) == -1 ||
	   shmctl(shm_ready_id, IPC_RMID, NULL) == -1 ||	
	   shmctl(shm_bond_counter, IPC_RMID, NULL) == -1 ||	
	   shmctl(shm_n_size, IPC_RMID, NULL) == -1 ||
	   shmctl(shm_b_size, IPC_RMID, NULL) == -1 ||	   
	   /* Relese shared memory */
	   shmdt(line_counter) == -1 ||
	   shmdt(o_counter) == -1 ||
	   shmdt(h_counter) == -1 ||
	   shmdt(bond_o) == -1 ||
	   shmdt(bond_h) == -1 || 
	   shmdt(bond_counter) == -1 || 	
	   shmdt(n_size) == -1 || 
	   shmdt(b_size) == -1 || 
	   shmdt(bond_ready) == -1) {
		  fprintf(stderr, "Failed to free allocated mutual memory.\n");
		  exit(EXIT_ERROR);
	}

}







