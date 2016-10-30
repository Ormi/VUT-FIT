/******************************************************************************/
/*                                                                            */
/*           	         IZP Projekt 3 - Průchod bludištěm                      */
/* 	            		       Michal Ormoš  (xormos00)                           */
/*                              November 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

//for EXIT_SUCCESS and EXIT_FAILURE
#include <stdlib.h>

//for strlen and strcat
#include <string.h>

//for bool
#include <stdbool.h>

//for atof
#include <ctype.h>

#define FALSE 0
#define TRUE 1

typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

typedef enum {
	LEFT, DOWN, RIGHT, UP
} Direction;

typedef enum {
	NORMAL, REVERSE
} Triangle;

typedef enum {
	L_ROT, R_ROT
} Path;

typedef enum {
	HELP, TEST, LPATH, RPATH
} Arguments;

void print_help()
{
	printf(
   "IZP Projekt 3 - Průchod bludištěm\n"                     
   "Michal Ormoš  (xormos00)\n"
   "November 2014\n"
	 "\n"
	 "Program sa spusta v nasledujucej podobe\n"
	 "'./proj3 --help'\n"
	 "Program vypise napovedu k pouzivaniu programu a skonci\n"
	 "\n"
	 "'./proj3 --test sobor.txt'\n"
	 "Skotroluje, ze subor dany druhym argumentom programu obsahuje spravnu"
	 "definiciu mapy bludiska. V pripade, ze odpoveda vypise 'Valid', v opacnom"
	 "pripade vypise 'Invalid'\n"
	 "\n"
	 "'./proj3 --rpath R C bludiste.txt'\n"
	 "Hlada priechod bludiskom na vstupu na riadku R a stlpci C. Cestu hlada"
	 "pomocou pravidla pravej ruky\n"
	 "\n"
	 "'./proj3 --lpath R C bludiste.txt'\n"
	 "Hlada priechod bludiskom na vstupu na riadku R a stlpci C. Cestu hlada"
	 "pomocou pravidla lavej ruky\n"
	 "\n");
}
/**
 *Inicialization of structure Map
 *(if we have *map we use -> ; if we have map we use .)
 */
void init_map(Map *map)
{
	map->rows = 0;
	map->cols = 0;
	map->cells = NULL;
}

/**
 *Free the memory which map take with malloc
 */
void free_map(Map *map)
{
	free(map->cells);
}

/**
 *Test if one number in map array is only from 0 to 7 included
 *Return EXIT_FAILURE if is and program is closed by EXIT_FAILUREs
 */
int test_item(char c)
{
	if((c < 48) || (c > 55)) {
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

/**
 *Test if there is the right sum of numbers in map, so they can potentially
 *create a map. Sum of numbers divorce map->cells = map-> rows
 */
int test_count(Map *map, double i)
{
	i = (i / map->rows);
	if(i != (double)map->cols) {
		return EXIT_FAILURE;
	}
return EXIT_SUCCESS;
}

int load_map(char **argv, Map *map)
{
	FILE *f;
	int i = 0;
	int size;

	if ((strcmp(argv[1], "--rpath") == 0) || (strcmp(argv[1], "--lpath") == 0)) {
		f = fopen(argv[4], "r");
	}
	if (strcmp(argv[1], "--test") == 0) {
		f = fopen(argv[2], "r");
	}

	if (f == NULL)
	{
		perror(argv[0]);
		fprintf(stderr, "Perror");
		return 1;
	} else {
	}

	fscanf(f, "%d", &map->rows);
	fscanf(f, "%d", &map->cols);

	size = (map->rows* map->cols) +1;

	map->cells = malloc(size);

	while(fscanf(f, "%s", &map->cells[i]) == 1)
	{
		if(test_item(map->cells[i]) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		i++;
	}

	if (test_count(map, i) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	fclose(f);

return EXIT_SUCCESS;
}

/**
 *Function which findout if we can go through wall of field
 *'z - 48' is for ASCII exchange to int
 *border can be left / right / updown
 *function return FALSE - we can't go through wall
 *return TRUE - we can go throught wall
 */
bool isborder(Map *map, int r, int c, int border)
{
	int i, z;

	i = ((r-1)*(map->cols)+(c-1));

	z = map->cells[i];

	i = z - 48;

	switch(border)
	{
		case 0 : {
			if ((i & 1) == 1) {
				return FALSE; 
			} else {
				return TRUE;
			}
			break;
		}

		case 2 : {
			if ((i & 2) == 2) {
				return FALSE; 
			} else {
				return TRUE;
			}
			break;
		}

		case 3 : {
			if ((i & 4) == 4) {
				return FALSE; 
			} else {
				return TRUE;
			}
			break;
		}

		case 1 : {
			if ((i & 4) == 4) {
				return FALSE; 
			} else {
				return TRUE;
			}
			break;
		}
		default :
			fprintf(stderr, "Case error\n");
	}
return EXIT_SUCCESS;
}

/**
 *Determinate when we "look" at the begging of the game
 *This is strict specified by implementation of program
 *Return value of 'Direction' from begining position data and orientation(L/R)
 */
int start_border(Map *map, int x, int y, int leftright)
{
if (leftright == RIGHT) {
	
	if((y == 1) && (x == 1) && (isborder(map, x, y, UP))) {
		return UP;
	}	
	if((y == 1) && (x == 1) && (!isborder(map, x, y, UP))) {
		return LEFT;
	}
	if((y == map->cols) && (x == 1) && (isborder(map, x, y, UP))) {
		return UP;
	}
	if((y == map->cols) && (x == map->rows) && (!isborder(map, x, y, DOWN))) {
		return RIGHT;
	}


	if((y >= 1) && (y <= map->cols) && (x == 1)) {
		return UP;
	}
	if((y == 1) && ((x % 2) == 0)) {
		return DOWN;
	}	
	if((y == 1) && ((x % 2) != 0)) {
		return RIGHT;
	}	
	if(((y >= 1) && (y <= map->cols)) && (x == map->rows)) {
		return DOWN;
	}	
	if((y == map->cols) && ((x % 2) != 0)) {
		return UP;
	}	
	if((y == map->cols) && ((x % 2) == 0)) {
		return LEFT;
	}
}


if (leftright == LEFT) {

	if((y == map->cols) && (x == 1) && (isborder(map, x, y, UP))) {
		return RIGHT;
	}
	if((y == map->cols) && (x == 1) && (!isborder(map, x, y, UP))) {
		return RIGHT;
	}
	if((y >= 1) && (y <= map->cols) && (x == 1)) {
		return UP;
	}	
	if((y == 1) && ((x % 2) == 0)) {
		return LEFT;
	}	
	if((y == 1) && ((x % 2) != 0)) {
		return LEFT;
	}
	if((y >= 1) && (y <= map->cols) && (x == map->rows)) {
		return DOWN;
	}	
	if((y == map->cols) && ((x % 2) != 0)) {
		return RIGHT;
	}	
	if((y == map->cols) && ((x % 2) == 0)) {
		return RIGHT;
	}
}
return EXIT_FAILURE;
}

/**
 *Checking triangle whether is NORMAL or REVERSE
 *Because shape of maze we can do it with from remainder of 2
 */
int check_triangle(int row, int col)
{
Triangle t;

	if((row % 2) == 0) {
		if((col % 2) == 0) {
			t = REVERSE;
		} else {
			t = NORMAL;
		}
	}
	if((row % 2) != 0) {
		if((col % 2) != 0) {
			t = REVERSE;
		} else {
			t = NORMAL;
		}
	}
return t;
}

/**
 *Input is from which direction we 'arrived' 
 *and what shape triangle is(Normal/Reverse)
 *Output is on which direction we 'look' and should go
 *check_direction_left work similarly
 */
Direction check_direction_right(Direction d, Triangle t)
{
	if((d == LEFT) && (t == NORMAL))
		return DOWN;

	if((d == DOWN) && (t == NORMAL))
		return RIGHT;

	if((d == RIGHT) && (t == NORMAL))
		return LEFT;

	if((d == LEFT) && (t == REVERSE))
		return RIGHT;

	if((d == RIGHT) && (t == REVERSE))
		return UP;

	if((d == UP) && (t == REVERSE))
		return LEFT;

return EXIT_SUCCESS;
}

Direction check_direction_left(Direction d, Triangle t)
{
	if((d == LEFT) && (t == NORMAL))
		return RIGHT;

	if((d == DOWN) && (t == NORMAL))
		return LEFT;

	if((d == RIGHT) && (t == NORMAL))
		return DOWN;

	if((d == LEFT) && (t == REVERSE))
		return UP;

	if((d == RIGHT) && (t == REVERSE))
		return LEFT;

	if((d == UP) && (t == REVERSE))
		return RIGHT;

return EXIT_SUCCESS;
}

/**
 *Functions which 'rotating' if we encounter on wall
 *Using check_direction_left/right for define direction of move
 *check_route_left work similarly
 */
Direction check_route_right(Map *map, int row, int col, Direction d, Triangle t)
{
	for(int i = 1; i < 4; i++)
	{
		d = check_direction_right(d,t);
			if(isborder(map, row, col, d) == TRUE) {
				return d;
			}
	}
return EXIT_FAILURE;
}

Direction check_route_left(Map *map, int row, int col, Direction d, Triangle t)
{
	for(int i = 1; i < 4; i++)
	{
		d = check_direction_left(d,t);
			if(isborder(map, row, col, d) == TRUE) {
				return d;
			}
	}
return EXIT_FAILURE;
}

/**
 *Most important recursion function
 *First 'if' stop program when soulution is find = we are out of maze
 *Second 'if' determinates if which rotate wit rule of righ hand or left hand
 *Third compound 'if' making moves and recursion calling
 */
int find_route(Map *map, int row, int col, Direction d, Path path)
{
Direction direction;

	if ((row > map->rows) || (col > map->cols) || (row < 1) || (col < 1)) {
		return EXIT_SUCCESS;
	}

	printf("%d,%d\n", row, col);

	Triangle t = check_triangle(row, col);
		if (path == R_ROT) {
			direction = check_route_right(map, row, col, d, t);
		} else {
			direction = check_route_left(map, row, col, d, t);
		}

	if(direction == DOWN) {
		find_route(map, row+1, col, UP, path);
	} else 
	if (direction == UP) { 
		find_route(map, row-1, col, DOWN, path);
	} else
	if (direction == LEFT) {
		find_route(map, row, col-1, RIGHT, path);
	} else
	if (direction == RIGHT) {
		find_route(map, row, col+1, LEFT, path);
	}
return EXIT_SUCCESS;
}

int test_number(char **endptr)
{
	if (**endptr != '\0') {
		fprintf(stderr, "Use numbers for defining tangens and height! Please use '--help'.\n");
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

/**
 * Determinate arguments
 */
int read_arguments(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "You must use more than 2 arguments!");
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "--help") == 0) {
		return HELP;
	}
	if (strcmp(argv[1], "--test") == 0) {
		return TEST;
	}
  if (strcmp(argv[1], "--lpath") == 0) {
		return LPATH;
	}
  if (strcmp(argv[1], "--rpath") == 0) {	
		return RPATH;
	}
return EXIT_FAILURE;
}

/**
 *In 'main' function program 
 * - in '--lpath' and '--lpath' free 'map.cells'
 * - test if in input is only numbers in argv 2 and 3
 */
int main(int argc, char **argv)
{ 
	int x,y;
	Direction direction;
	Map map;
	char *endptr = '\0';
	Arguments arg;

	arg = read_arguments(argc, argv);

	if (arg == HELP) {
	print_help();
	}

	if (arg != HELP) {
		init_map(&map);
		if (arg == TEST) {
			if (load_map(argv, &map) == EXIT_FAILURE) {
				printf("Invalid\n");
				return EXIT_FAILURE;
			} else {
				printf("Valid\n");
			}
		}
		if ((arg != TEST) && (arg != HELP)) {
			load_map(argv, &map);
			x = strtod(argv[2], &endptr);
				if (test_number(&endptr) == EXIT_FAILURE) return EXIT_FAILURE;
			y = strtod(argv[3], &endptr);
				if (test_number(&endptr) == EXIT_FAILURE) return EXIT_FAILURE;
		}
	 	if (arg == LPATH) {
			direction = start_border(&map, x, y, LEFT);
			find_route(&map, x, y, direction, L_ROT);
		}
 		if (arg == RPATH) {
			direction = start_border(&map, x, y, RIGHT);
			find_route(&map, x, y, direction, R_ROT);
		}

	free_map(&map);
 }
return EXIT_SUCCESS;

}

/*******************/
/*                 */
/*   End of proj3  */
/*                 */
/*******************/
