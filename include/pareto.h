#pragma once

#include <string.h>
#include <vector>
#include "..\include\solution.h"

class solution; 

class Pareto : public std::vector<solution*>
	{
	public: 
	//.....
	bool updated; 
	Pareto(solution* _solution); 
	solution& operator=(const solution& _solution); 
	//.....
	bool my_erase(solution* s); 
	bool add(solution* s); 
	bool update(solution _solution); 
	bool update(Pareto* population); 

	unsigned keep_efficient(); 
	unsigned replace_by_efficient_solution(); // the idea is to replace all inefficient solution in Pareto front by efficient ones found by SEP model
	bool is_feasible(); //toutes les solutions NE SONT PAS infeasable
	solution* worst_leader(); 
	solution* worst_efficient_leader(); 
	Pareto(); 
	~Pareto(); 
	}; 

