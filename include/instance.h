#pragma once

#include "..\include\parameters.h"
#include <vector>
using namespace std; 

class instance 
	{
	public: 
	static unsigned big_number;

	static unsigned nb_facilities;
	static unsigned nb_customers; 
	static unsigned nb_open_leaders; // p
	static unsigned nb_open_followers; // r

	//static unsigned facilities_positions[MAX_FACILITIES][2]; //positions 
	static vector<vector<unsigned>> facilities_positions; //positions 
	//static unsigned customers_positions[MAX_CUSTOMERS][2]; //positions 
	static vector<vector<unsigned>> customers_positions; //positions 
	//static float distances[MAX_FACILITIES][MAX_CUSTOMERS];//distances  
	static vector<vector<float>> distances;//distances  

	//static unsigned costs[MAX_FACILITIES]; //cost for opening each facility 
	static vector<unsigned> costs; //cost for opening each facility 

	static unsigned minimum_follower12;  
	static unsigned maximum_follower2;

	//static unsigned profit_leader[MAX_CUSTOMERS]; 
	static vector<unsigned> profit_leader; 
	//static unsigned profit_follower1[MAX_CUSTOMERS]; 
	static vector<unsigned> profit_follower1; 
	//static unsigned profit_follower2[MAX_CUSTOMERS]; 
	static vector<unsigned> profit_follower2; 
	
	static unsigned leader_minimum_profit; //leader minimum profit  (lead should earn more than W)

	static void intialize(unsigned _facilities_number,unsigned _customers_number,unsigned _leaders_number,unsigned _followers_number); 
	static void readInitialDataFromFile(unsigned _facilities_number,unsigned _customers_number,unsigned _leaders_number,unsigned _followers_number);
	//instance(); 
	static void set_distances(); 
	static void set_big_number(unsigned _big_number); 
	static void set_profits(); 
	static void set_costs_and_leader_minimum_profit(); 
	}; 

