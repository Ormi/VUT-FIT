/******************************************************************************/
/*                                                                            */
/*           	       IZP Projekt 2 - Iteration calculations                   */
/*                         Michal Ormos  (xormos00)                           */
/*                              November 2014                                 */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

//For using strtod (means string to double)
#include <stdlib.h>

#include <math.h>

//For using strcmp
#include <string.h>

#define NUM_ITERATIONS 8

#define RANGE_AB(arg1, arg2) ((arg1) > 0 && (arg2) <= 14)
#define RANGE_B(y) ((y) > 0 && (y) <= 1.4) 
#define RANGE_C(c) ((c) > 0 && (c) <= 100) 
#define RANGE_A(x) ((x) > 0 && (x) <= 1.4) 

#define C_ERROR "Value for -c X can be set only for (0 < X <= 100).\n"
#define AB_ERROR "Value for --tan A N M can be set only for (0 < N <= M < 14).\n"
#define A_ERROR "Value for -c X -m A B can be set only for (0 < A <= 1.4 < π/2).\n"
#define B_ERROR "Value for -c X -m A B can be set only for (0 < B <= 1.4 < π/2).\n"


void print_help()
{
printf(
"Program se spouští v následující podobě:\n"
"'./proj2 --help'\n"
"nebo\n"
"'./proj2 --tan A N M'\n'"
"nebo\n"
"'./proj2 [-c X] -m A [B]'\n"
"\n"
"--help způsobí, že program vytiskne nápovědu používání programu a skončí.\n"
"\n"
"--tan srovná přesnosti výpočtu tangens úhlu A (v radiánech) mezi voláním tan z\
matematické knihovny, a výpočtu tangens pomocí Taylorova polynomu a zřetězeného\
 zlomku.\n"
"Argumenty N a M udávají, ve kterých iteracích iteračního výpočtu má srovnání\
 probíhat. 0 < N <= M < 14\n"
"\n"
"-m vypočítá a změří vzdálenosti.\n"
" Úhel α (viz obrázek) je dán argumentem A v radiánech. Program vypočítá a\
 vypíše vzdálenost měřeného objektu. 0 < A <= 1.4 < π/2.\n"
" Pokud je zadán, úhel β udává argument B v radiánech. Program vypočítá a\
 vypíše i výšku měřeného objektu. 0 < B <= 1.4 < π/2\n"
" Argument -c nastavuje výšku měřicího přístroje c pro výpočet. Výška c je dána\
 argumentem X (0 < X <= 100). Argument je volitelný - implicitní z\
výška  je 1.5 metrů.\n");
}

/**
 *Function for calculating tangens with Taylor series
 *Numerators and Denomerators is set default for 13 iterations
 */
double taylor_tan(double x, unsigned int n)
{
	double element = 0;
	double sum = 0;
	double exp; //Exponent of x in numerator
	unsigned int i = 0; //Iterations caunter
	/**
 	 *Values for numerator and denomerator of fraction (first 13)
   */
	long int num_tor[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582,\
 443861162, 18888466084, 113927491862, 58870668456604};
  long int deno_tor[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875,\
 10854718875, 1856156927625, 194896477400625, 49308808782358125,\
 3698160658676859375};
	exp = x;
	while(i < n) {
		element = ((num_tor[i]*exp)/(deno_tor[i]));
		i++;
		exp = exp*x*x; 
		sum = sum + element;
	}   
	return sum;
}

/**
 *Function for calculating tangens with chained fractions
 *Variables = Right and Left value of fractions
 */
double cfrac_tan(double x, unsigned int n)
{
	double r_value;
	double l_value;
	/**
   *Inicialization of last (in this calculation first) value
   *The last value of chained fraction for n iterations
   */
	r_value = (2*(n-1)+1)-(x*x/(2*n+1));

	for(n = n-1; n >= 1; n--) {
		l_value = (2*(n-1)+1)-(x*x/r_value);
		r_value = l_value;
	}
	return x/r_value;
}

/**
 *Function which printf result in shape :
 *I M T TE C CE
 *I- number of iterations; M- angle from math.h; T- reslut of taylor_tan
 *TE- absolute error between taylor_tan and math.h; C- result from cfrac_tan
 *CE- absolute error betwenn cfrac_tan and math.h
 */
void print_tan(int arg1, int arg2, double x)
{
	int i;
	double te;
	double ce;
		for (i=arg1; i <= arg2; i++) {
				te = tan(x)-taylor_tan(x,i);
				if (te < 0) te *= -1;
				ce = tan(x)-cfrac_tan(x,i);
				if (ce < 0) ce *= -1;
			printf("%d %e %e %e %e %e\n", i, tan(x), taylor_tan(x,i),te, cfrac_tan(x,i), ce);
		}
}

/**
 *Function which calculate distance and height of item from inptu of tangens
 *For calculating distance we need only one angle (the bottom one)
 *For calculating height we need second angle and distance
 *NUM_ITERATIONS is set for 13, for better accuracy
 */
void calculate_item(double c, double x, double y)
{
	double distance;
	double height;
	distance = c / cfrac_tan(x,NUM_ITERATIONS);
	if (y != 0) {
		height = ((cfrac_tan(y,NUM_ITERATIONS) * distance) + c);
		printf("%.10e\n%.10e\n", distance, height);
	} else {
		printf("%.10e\n", distance);
	}
}

int test_number(char **endptr)
{
	if (**endptr != '\0') {
		fprintf(stderr, "Use numbers for defining tangens and height! Please use '--help'.\n");
		exit(0);
	} else {
		return EXIT_SUCCESS;
	}
}

/**
 *Reading arguments and executing bad inputs
 *Main calling every function
 */
int arguments(int argc, char **argv)
{
	double x;
	double y;
	int arg1;
	int arg2;
	double c = 1.5;
	int error = 1;
	char *endptr = '\0';
	if (argc < 2) {
		fprintf(stderr, "Not enought argument. Please use '--help'.\n");
		return EXIT_FAILURE;   
	}
	
	if (strcmp(argv[1], "--help") == 0) {
		print_help();
		error = 0;
	}
	
	//For input './proj2 --tan A N M'
	if ((argc == 5) && (strcmp(argv[1], "--tan") == 0)){
		x = strtod(argv[2], &endptr);
		test_number(&endptr);
		if (!RANGE_A(x)) {
		fprintf(stderr, A_ERROR);
			return EXIT_FAILURE;   
		}
		arg1 = strtod(argv[3], &endptr);
		test_number(&endptr);
		arg2 = strtod(argv[4], &endptr);
		test_number(&endptr);
		if ((!RANGE_AB(arg1, arg2)) || (arg1 > arg2)) {
			fprintf(stderr, AB_ERROR);
			return EXIT_FAILURE;   
		}
		print_tan(arg1, arg2, x);
		error = 0;
	}
	
	//For input './proj2 -m A B'
	if ((argc == 4) && (strcmp(argv[1], "-m") == 0)){
		x = strtod(argv[2], &endptr);
		test_number(&endptr);
		if (!RANGE_A(x)) {
			fprintf(stderr, A_ERROR);
			return EXIT_FAILURE;   
		}
		y = strtod(argv[3], &endptr);
		test_number(&endptr);
		if (!RANGE_B(y)) {
			fprintf(stderr, B_ERROR);
			return EXIT_FAILURE;   
		}
		calculate_item(c, x, y);
		error=0;
	}
	
	//For input './proj2 -m A'
	if ((argc == 3) && (strcmp(argv[1], "-m") == 0)){
		x = strtod(argv[2], &endptr);
		test_number(&endptr);
		if (!RANGE_A(x)) {
			fprintf(stderr, A_ERROR);
			return EXIT_FAILURE;   
		}
		y = 0;
		calculate_item(c, x, y);
		error=0;
	}
	
	//For input './proj2 -c X -m A B'
	if ((argc == 6) && (strcmp(argv[1], "-c") == 0) && (strcmp(argv[3], "-m")) == 0){
		c = strtod(argv[2], &endptr);
		test_number(&endptr);
		if (!RANGE_C(c)) {
			fprintf(stderr, C_ERROR);
			return EXIT_FAILURE;   
		}
		x = strtod(argv[4], &endptr);    
		test_number(&endptr); 
		if (!RANGE_A(x)) {
			fprintf(stderr, A_ERROR);
			return EXIT_FAILURE;   
		}   
		y = strtod(argv[5], &endptr);
		test_number(&endptr); 
		if (!RANGE_B(y)) {
			fprintf(stderr, B_ERROR);
			return EXIT_FAILURE;   
		}
		calculate_item(c, x ,y);
		error=0;
	}      

	//For input './proj2 -c X -m A'
	if ((argc == 5) && (strcmp(argv[1], "-c") == 0) && (strcmp(argv[3], "-m")) == 0){
		c = strtod(argv[2], &endptr);
		test_number(&endptr);
		if (!RANGE_C(c)) {
			fprintf(stderr, C_ERROR);
			return EXIT_FAILURE;
		}
		x = strtod(argv[4], &endptr);
		test_number(&endptr);
		if (!RANGE_A(x)) {
			fprintf(stderr, A_ERROR);
			return EXIT_FAILURE;   
		}   
		y = 0;
		calculate_item(c, x ,y);
		error=0;
	}

	if (error == 1) {
		fprintf(stderr, "Bad input! Please use '--help'\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	arguments(argc, argv);
	return EXIT_SUCCESS;
}

/*******************/
/*                 */
/*   End of proj2  */
/*                 */
/*******************/
