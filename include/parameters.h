

//test 1
/*
#define MAX_FACILITIES 6
#define MAX_CUSTOMERS MAX_FACILITIES 

#define MAX_VARIABLES (MAX_FACILITIES*5) 
#define MAX_CONSTRAINTS (MAX_FACILITIES*5) 

#define FACILITIES_NUMBER MAX_FACILITIES
#define CUSTOMERS_NUMBER  MAX_CUSTOMERS
#define LEADERS_NUMBER 2
#define FOLLOWERS_NUMBER LEADERS_NUMBER 
*/

//test 1
/*
#define MAX_FACILITIES 10
#define MAX_CUSTOMERS MAX_FACILITIES 

#define MAX_VARIABLES (MAX_FACILITIES*5) 
#define MAX_CONSTRAINTS (MAX_FACILITIES*5) 

#define FACILITIES_NUMBER MAX_FACILITIES
#define CUSTOMERS_NUMBER  MAX_CUSTOMERS
#define LEADERS_NUMBER 2
#define FOLLOWERS_NUMBER LEADERS_NUMBER 
*/
//test 2  

#define MAX_FACILITIES 51
#define MAX_CUSTOMERS MAX_FACILITIES 

#define MAX_VARIABLES (MAX_FACILITIES*5) 
#define MAX_CONSTRAINTS (MAX_FACILITIES*5) 

#define FACILITIES_NUMBER MAX_FACILITIES
#define CUSTOMERS_NUMBER  MAX_CUSTOMERS
#define LEADERS_NUMBER 3
#define FOLLOWERS_NUMBER LEADERS_NUMBER 

//test 4
/*
#define MAX_FACILITIES 14
#define MAX_CUSTOMERS MAX_FACILITIES 

#define MAX_VARIABLES (MAX_FACILITIES*5) 
#define MAX_CONSTRAINTS (MAX_FACILITIES*5) 

#define FACILITIES_NUMBER MAX_FACILITIES
#define CUSTOMERS_NUMBER  MAX_CUSTOMERS
#define LEADERS_NUMBER 4
#define FOLLOWERS_NUMBER LEADERS_NUMBER 
*/
//algorithm parameters 
#define NEIGHB_RATE_TS 0.3 // neighborhood rate in tabu search
#define NEIGHB_RATE_MLS 0.5 // neighborhood rate in multilocal search in Genetic algorithm
#define NB_GeneticAlgo_Generations 1

#define MAX_PARETO (2*MAX_FACILITIES)
#define MAXIMUM_ROWS (4+MAX_CUSTOMERS+MAX_FACILITIES+3*MAX_PARETO) 
#define MAXIMUM_COLUMNS (MAX_CUSTOMERS+MAX_FACILITIES+3*MAX_PARETO) 

//#define MEMORY 
#define ALGORITHM 
//#define CPLEX 
