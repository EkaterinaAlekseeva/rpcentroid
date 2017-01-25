#pragma once

#include "..\include\solution.h"
#include "..\include\instance.h"
#include "..\include\genetic_algorithm.h"
#include "..\include\rpcentroid.h"

class Movement
	{
	public:
	unsigned i1;
	unsigned i2;
	Movement(unsigned _i1, unsigned _i2);
	bool equal(unsigned _i1, unsigned _i2);
	};


class TabuList : public std::vector<Movement>
	{
	public:
	unsigned maximum_size;
	TabuList(unsigned _maximum_size);
	//void add(solution* s);
	bool update(solution* s); 
	bool belong(solution* s); 
	};

class TabuSearch
	{
	public:
	Pareto*algorithm1(solution* leader_neighbour);
	solution*algorithm2(solution* leader_neighbour,Pareto* follower_potential_pareto); 
	solution* compute(unsigned nb_iterations, unsigned list_tabu_size);
	};
