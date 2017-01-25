#pragma once

#include "..\include\solution.h"
#include "..\include\instance.h"
#include "..\include\solution.h"


#define MULTIGENETIC_INFEASIBLE 1 
#define MULTIGENETIC_WORST_LEADER 2 

#define MONO_LOCAL_SEARCH 1 
#define MULTI_LOCAL_SEARCH 2 

class MultiGenetic 
	{
	public: 
	static unsigned size; 
	static unsigned generations;
	static unsigned cro;


	short type; 

	Pareto *pareto; 
	//MultiLocalSearch multi_local_search; 

	//Population population;
	void initialize_pareto(solution& leader_solution); 

	int get_effective(unsigned index); 

	int minimum; 
	int maximum; 
	void set_minimum_maximum(); 

	unsigned total; 
	void set_total(); 

	unsigned select_parent(unsigned parent1); 
	solution* crossover(unsigned parent1, unsigned parent2); 
	Pareto* multi_local_search(solution* current_solution);
	Pareto*compute(solution& leader_solution, short _type, short _ls_type, Pareto *_pareto=NULL); 
	MultiGenetic(); 
	}; 

