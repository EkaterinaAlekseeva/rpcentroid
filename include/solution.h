#pragma once

#include <string.h>
#include <vector>
#include "..\include\instance.h"
#include "..\include\rpcentroid.h"
#include "..\include\parameters.h"

using namespace std; 

#define NONE	0
#define LEADER	1
#define FOLOWER	2

class Pareto; 


class leader_solution 
	{
	public:

	int leader_objective;  //to be computed (can be negative int value)
	int leader_infeasibility;  //to be computed (can be negative int value)
	//unsigned leader[MAX_FACILITIES]; 
	vector<unsigned> leader; 

	leader_solution(); 
	~leader_solution(); 
	leader_solution(leader_solution& _solution); 
	}; 



class follower_solution : public leader_solution
	{
	public: 
	unsigned follower_objective1;   //to be computed
	unsigned follower_objective2; //to be computed
	//unsigned follower_potential_facilities[MAX_FACILITIES][MAX_CUSTOMERS]; 
	vector<vector<unsigned>> follower_potential_facilities; 
	void follower_intialize();  
	follower_solution(); 
	follower_solution(leader_solution& _solution); 

	//unsigned follower[MAX_FACILITIES]; 
	vector<unsigned> follower; 
	void set_follower_potential_facilities(); 
	float utilitity(float weight); 
	}; 


class solution : public follower_solution
	{

	public: 
	bool neighbours_visited; 
	//unsigned customers[MAX_CUSTOMERS]; 
	vector<unsigned> customers; 
	void set_customers(); 

	solution(); //empty constructor 
	solution(const solution& _solution); //copy constructor 
	solution(leader_solution& _solution);  //constructor 
	solution(const solution& _solution, unsigned maximum, bool follower_leader=false); //swap constructor, if follower_leader=false then swap leader else swap follower
	solution(Model*model, solution*s); 

	solution& operator=(const solution& _solution); 
	void update_objectives(); 

	bool operator==(const solution& _solution); 
	bool operator>=(const solution& _solution); 

	void set_leader_objective();
	void set_leader_infeasibility(); 

	void set_follower_objectives();
	bool leader_constraints_satisfied();

	bool is_efficient(); 
	solution* get_efficient(); 
	bool is_infeasible(); // Teste egalement les solutions alternative (i.e. ayant le meme cout pour les objectifs du follower)
	solution* get_infeasible(Pareto* pareto); 
	solution* make_leader_worst(Pareto* pareto); 

	unsigned swap_index1; 
	unsigned swap_index2; 
	bool swap_another(unsigned*follower_leader,unsigned& maximum); 
	bool swap(unsigned*follower_leader, unsigned maximum=100);

	void resize(); 
}; 



