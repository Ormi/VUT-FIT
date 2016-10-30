/******************************************************************************/
/*                                                                            */
/*           	       IZP Projekt 1 - Výpočty v tabulce 						 		        */
/* 	            		      Michal Ormoš  (xormos00)	      								    */
/*                         October-November 2014	              		  			  */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
/**
 *for isdigit()
 */
#include <ctype.h>
/**
 *for atof()
 */
#include <stdlib.h>
/**
 *for bool
 */
#include <stdbool.h>
/**
 *for strcmp
 */
#include <string.h>

#define USE_HELP "Please, use '--help'\n"

void print_help()
{
printf(
"Pouzití\n\n"
  "Ocekavany vstup: './proj1 operace výběr_buněk'\n\n"
 "operace reprezentuje jednu z následujících operací:\n"
    "   select značí operaci, která z dané tabulky pouze vybere a následně\
 vytiskne hodnoty daných buněk,\n"
    "   min značí vyhledání a následný tisk minimální hodnoty z daného rozsahu\
buněk,\n"
    "   max značí vyhledání a následný tisk maximální hodnoty z daného rozsahu\
buněk,\n"
    "   sum značí výpočet a následný tisk sumy hodnot všech vybraných buněk,\n"
 "   avg značí výpočet a následný tisk aritmetického průměru vybraných buněk.\n"
"výběr_buněk reprezentuje rozsah tabulky, nad kterým má být provedena danázn\
 operace. Rozsah je definován jedním z následujících argumentů:\n"
    "   row X značí výběr všech buněk na řádku X (X > 0),\n"
		"   col X značí výběr všech buněk ve sloupci X (X > 0),\n"
    "   rows X Y značí výběr všech buněk od řádku X (včetně) až po Y (včetně).\
 0 < X <= Y.\n"
    "   cols X Y značí výběr všech buněk od sloupce X (včetně) až po Y \
(včetně). 0 < X <= Y.\n"
    "   range A B X Y značí výběr buněk od řádku A po řádek B a od sloupce X po\
 sloupec Y (včetně daných řádků a sloupců). 0 < A <= B, 0 < X <= Y.\n"

);
}

/**
 *Function for testing length of one row
 *pos_in_row - position in one row (max. 1024)
 *Bool is support variable true or false for simplicity
 *Static is a vriable which is set only one to zero and in every entry to static
 *variable inside a function keeps its value between invocations. 
 *return exit of program if input is bigger than 1024 else return nothing.
 */
void check_line_length(bool reset) { 
	static int pos_in_row = 0; 
	if (reset) {
		pos_in_row = 0;
	}	else {
		pos_in_row++;
		if (pos_in_row > 1025) {
			fprintf(stderr, "Line is out of max line length\n");				
			exit(0);
		}	
	}
}
/**
 *Funcion for skipping spaces - skip_leading_spaces
 *Can skip more than one space chracter
 *Return character back to get_next_item if no more spaces in string
 */
char skip_leading_spaces() {
	char z;
	while ((z = getchar()) != EOF) {
		check_line_length(false);
		if((! isspace(z) )) {
			 return z;
		}
	}
	return z;
}



/**
 *Major function. 
 *Read char and process it to main.
 *Format of scope is - [min row, max row, min col, max col] - input from user
 *Return sting of characters of one cell [*data].
 *Return string containing number(1) or character(0) or exit_end_of_file(-1)
 *if there is no next number
 *You insert to 'data' loaded item - ourput to user
 */
int get_next_item(char *data, int scope[]) { 
	/**
	*row, col - counter
	*data_pos - counter for position of char in array of data
	**/
	static int row = 0;
	static int col = 0;
	char z;
	int data_pos = 0;
	int is_number = 1;

	z = skip_leading_spaces();

	/**
	 *Essential while - saving data (char, number, others) depends on occasion
	 *Return this data to function and then to main
	 *Important what while return (number(1) or character(0) or nothing(-1)) 
	 *because output %.10f or %s
	 *Chcecking end of one cell, end of file and end of line
	 */
	while (z != EOF) {
		if (z == '\n') {
			check_line_length(true); //set pos_in_row to 0 for new row
			data[data_pos++] = '\0';
		  if ((row >= scope[0]) && (row <= scope[1]) && (col >= scope[2]) && (col <= scope[3])) {
				row++;
				col = 0;
				return is_number;
			} else {
				col = 0;
				row++;
				return (-2);
			}
		}
		if( isspace(z) ) { 
			data[data_pos++] = '\0';
		  if ((row >= scope[0]) && (row <= scope[1]) && (col >= scope[2]) && (col <= scope[3])) {
				col++;
				return is_number;
			} else {
				col++;
	    	return (-2);
			}
		}
		data[data_pos] = z;
		if ( ! isdigit(z)) {
			is_number = 0;
		}
		data_pos++;
		z = getchar();
		check_line_length(false);
	}	
	return -1;
}

int main(int argc, char *argv[]) {
	char number[1024];
	int scope[4]={0,1024,0,512};
	int n;
	n = argc;
	double cache; //for calculating MIN and MAX
	double num_sum = 0.0; //for math operations - sum of numbers
	int num_counter = 0; //for math operations - total sum of numbers
	/**
	 *Identifier of operation
	 *select = 1 | min = 2| max = 3| sum = 4| avg = 5
	 *specify type of mathematical operation
	 */
	int arg1 = 0;
 	/**
	 *Identifier of select cells
	 *row = 1 | col = 2| rows = 3 | cols = 4 | range = 5
	 *specify work with cells
	 */ 
	int arg2 = 0;

/**
 *Declaration of inputs for avg1
 */
if (strcmp(argv[1],"--help")==0) {
	print_help();
	return 0;
}
if (argc<4) {
  	fprintf(stderr, "You not assigned enough arguments (4 or more is required)\n");
		printf(USE_HELP);	
		return 1;
}
if (strcmp(argv[1],"select") == 0) {
	arg1 = 1;
}
if (strcmp(argv[1], "min") == 0) {
	arg1 = 2;
}
if (strcmp(argv[1], "max") == 0) {
	arg1 = 3;
}
if (strcmp(argv[1], "sum") == 0) {
	arg1 = 4;
}
if (strcmp(argv[1], "avg") == 0) {
	arg1 = 5;
}
if (arg1 == 0) {
	fprintf(stderr, "You don't enter operation argument (first)\n");
	printf(USE_HELP);	
	return 1;
}

/**
 *Declaration of inputs for avg2
 */
if (strcmp(argv[2], "row") == 0) {
	arg2 = 1;
}
if (strcmp(argv[2], "col") == 0) {
	arg2 = 2;
}
if (strcmp(argv[2], "rows") == 0) {
	arg2 = 3;
}
if (strcmp(argv[2], "cols") == 0) {
	arg2 = 4;
}
if (strcmp(argv[2], "range") == 0) {
	arg2 = 5;
}
if (arg2 == 0) {
	fprintf(stderr, "You don't enter cell argument (second)\n");
	printf(USE_HELP);	
	return 1;
}

/**
 *Declaration of setup scope for inputs of avg2
 *There is difference betwen input of 
 *Argv is count from 0, so n=4 but argv is n minus nubmer
 */
if ((n==4) && (arg2==1)) { //for 4 arguments and ROW
	scope[0]=(atoi(argv[n-1]))-1;
	scope[1]=(atoi(argv[n-1]))-1;
} else
if ((n==4) && (arg2==2)) { //for 4 arguments and COL
	scope[2]=(atoi(argv[n-1]))-1;
	scope[3]=(atoi(argv[n-1]))-1;
} else
if ((n==5) && (arg2==3)) { //for 5 arguments and ROWS
	scope[0]=(atoi(argv[n-2]))-1;
	scope[1]=(atoi(argv[n-1]))-1;
} else
if ((n==5) && (arg2==4)) { //for 5 arguments and COLS
	scope[2]=(atoi(argv[n-2]))-1;
	scope[3]=(atoi(argv[n-1]))-1;
} else
if ((n==7) && (arg2==5)) { //for 7 arguments a RANGE
	scope[0]=(atoi(argv[n-4]))-1;
	scope[1]=(atoi(argv[n-3]))-1;
	scope[2]=(atoi(argv[n-2]))-1;
	scope[3]=(atoi(argv[n-1]))-1;
} else {
	fprintf(stderr, "Bad input! (For second argument)\n");
  printf(USE_HELP);
	return 1; 
}

/**
 *While loop for outputs from is_num (number(1) or character(0) or nothing(-1))
 */
while (1) {
	int is_num = get_next_item(number, scope);

	/**
   *Save first number of inputs for math operations to 'cache'
	 */
	if ((num_counter==0) && (is_num==1)){
		cache = atof(number);
	}
	/**
	 *Use of 'select'
	 */
	if (arg1==1) {
		switch (is_num) {
			case 1 : printf("%.10f\n", atof(number));
							break;
			case -1 : return 0;
							break;
			case 0 : printf("%s\n",number);
							break;
			default:  // item out of range, just skip
						 break;
		}
	}
	/**
	 *Use of other inputs (min, max, sum, avg)
	 */
	if (arg1>1) {
		switch (is_num) {
			case 1 : { 
					if (arg1==2) {	//MIN
						if (atof(number)<=cache) {
							cache = atof(number);
						}
					}
					if (arg1==3) {	//MAX
						if (atof(number)>=cache) {
							cache = atof(number);
						}
					}
					if (arg1==4) {	//SUM
						num_sum = num_sum + atof(number);
					}
					if (arg1==5) {	//AVG
						num_sum = num_sum + atof(number);
					}
			}
			num_counter++;
					break;
			case -1 : { 
					if ((arg1>1) && (arg1<4))	{							
						printf("%.10f\n", cache); //Write out MIN or MAX
						return 1;
					}
					if (arg1==4) {
						printf("%.10f\n", num_sum); //Write out SUM
						return 1; 
					}
					if (arg1==5) {
						printf("%.10f\n", num_sum/num_counter); //Write out AVG
							return 1;
					}
			}
					break;
			case 0 : { 
					fprintf(stderr, "Bad input (find char in mathematical operation)\n");
          printf(USE_HELP);
					return 0;
			}
					break;
			default:  // item out of range, just skip
					break;
		}
	}
	
}
return 0;
}
/*******************/
/*                 */
/*     END proj1   */
/*                 */
/*******************/
