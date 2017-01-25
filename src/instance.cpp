
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\include\instance.h"
#include <random>
#include <string>
#include <iostream>
#include <fstream>
using namespace std; 

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

#define MAX_RANDOM1 7000
#define MAX_RANDOM2 100

unsigned instance::big_number;

unsigned instance::nb_facilities;
unsigned instance::nb_customers; 
unsigned instance::nb_open_leaders; // p
unsigned instance::nb_open_followers; // r

//unsigned instance::facilities_positions[MAX_FACILITIES][2]; //positions 
vector<vector<unsigned>> instance::facilities_positions; //positions 
//unsigned instance::customers_positions[MAX_CUSTOMERS][2]; //positions 
vector<vector<unsigned>> instance::customers_positions; //positions 
//float instance::distances[MAX_FACILITIES][MAX_CUSTOMERS];//distances  
vector<vector<float>> instance::distances;//distances  

//unsigned instance::costs[MAX_FACILITIES]; //cost for opening each facility 
vector<unsigned> instance::costs; //cost for opening each facility 

unsigned instance::minimum_follower12;  
unsigned instance::maximum_follower2;

//unsigned instance::profit_leader[MAX_CUSTOMERS]; 
//unsigned instance::profit_follower1[MAX_CUSTOMERS]; 
//unsigned instance::profit_follower2[MAX_CUSTOMERS]; 

vector<unsigned> instance::profit_leader; 
vector<unsigned> instance::profit_follower1; 
vector<unsigned> instance::profit_follower2; 


unsigned instance::leader_minimum_profit; //leader minimum profit  (lead should earn more than W)


void instance::intialize(unsigned _facilities_number,unsigned _customers_number,unsigned _leaders_number,unsigned _followers_number)
	{
	//...
	nb_facilities=_facilities_number; 
	nb_customers=_customers_number;
	nb_open_leaders=_leaders_number;
	nb_open_followers=_followers_number; 

	//...
	set_distances(); 
	set_profits(); 
	set_costs_and_leader_minimum_profit(); 
	} 

void instance::readInitialDataFromFile(unsigned _facilities_number,unsigned _customers_number,unsigned _leaders_number,unsigned _followers_number)
	{
	// set dimensions
	nb_facilities=_facilities_number; 
	nb_customers=_customers_number;
	nb_open_leaders=_leaders_number;
	nb_open_followers=_followers_number; 

	//set vectors 
	facilities_positions.resize(instance::nb_facilities); 
	for(unsigned i=0; i<facilities_positions.size();i++)
		facilities_positions[i].resize(2); 

	customers_positions.resize(instance::nb_customers); 
	for(unsigned i=0; i<customers_positions.size();i++)
		customers_positions[i].resize(2); 

	profit_leader.resize(nb_customers); 
	profit_follower1.resize(nb_customers); 
	profit_follower2.resize(nb_customers);
	costs.resize(nb_facilities); 
	
	string filename=to_string(nb_facilities)+".txt";
	ifstream input("C:\\Users\\Ekaterina\\Dropbox\\Bilevel_MultiCriteriaFollower\\program - Copy\\TestInstancesFromPreviousPaper\\"+filename);
	if (input) {
		int flag(0);
		unsigned aux_i(0);
	for( string line; getline( input, line ); )
		{
			if	(line == "facility1(i)") {flag=1; aux_i=0;}
			else
				if	(line == "facility2(i)") {flag=2; aux_i=0;}
				else
					if	(line == "client1(j)") {flag=3; aux_i=0;}
					else
						if	(line == "client2(j)") {flag=4; aux_i=0;}
						else
							if	(line == "rld(j)") {flag=5; aux_i=0;}
							else
								if	(line == "rfl1(j)") {flag=6; aux_i=0;}
								else
									if	(line == "rfl2(j)") {flag=7; aux_i=0;}

			if (line != "facility1(i)" & flag==1)
				{ if (aux_i<nb_facilities)
					{
						facilities_positions[aux_i][0]= stoi(line);
						aux_i++;		
					} else
						cout << "Index in facilities_positions0 is out of range reading " << filename << "\n";
				}
			if (line != "facility2(i)" & flag==2)
				{ if (aux_i<nb_facilities)
					{
						facilities_positions[aux_i][1]= stoi(line);
						aux_i++;		
					} else
						cout << "Index in facilities_positions1 is out of range reading " << filename << "\n";
				}
			if (line != "client1(j)" & flag==3)
			{ if (aux_i<nb_customers)
					{
						customers_positions[aux_i][0]= stoi(line);
						aux_i++;		
					} else
						cout << "Index in customers_positions0 is out of range reading " << filename << "\n";
				}
			if (line != "client2(j)" & flag==4)
				{ if (aux_i<nb_customers)
					{
						customers_positions[aux_i][1]= stoi(line);
						aux_i++;		
					} else
						cout << "Index in customers_positions1 is out of range reading " << filename << "\n";
				}
			if (line != "rld(j)" & flag==5)
				{ if (aux_i<nb_customers)
					{
						profit_leader[aux_i]= stoi(line);
						aux_i++;		
					} else
						cout << "Index in profit_leader is out of range reading " << filename << "\n";
				}
			if (line != "rfl1(j)" & flag==6)
				{ if (aux_i<nb_customers)
					{
						profit_follower1[aux_i]= stoi(line);
						aux_i++;		
					} else
						cout << "Index in profit_follower1 is out of range reading " << filename << "\n";
				}
			if (line != "rfl2(j)" & flag==7)
				{ if (aux_i<nb_customers)
					{
						profit_follower2[aux_i]= stoi(line);
						aux_i++;		
					} else
						cout << "Index in profit_follower2 is out of range reading " << filename << "\n";
				}
			
		}
	distances.resize(instance::nb_facilities); 
	for(unsigned i=0; i<distances.size();i++)
		distances[i].resize(instance::nb_customers); 
	
	for (unsigned i=0;i<nb_facilities; i++) 
		for (unsigned j=0;j<nb_customers; j++) 
			{
			unsigned x1=facilities_positions[i][0]; 
			unsigned y1=facilities_positions[i][1]; 
			unsigned x2=customers_positions[j][0]; 
			unsigned y2=customers_positions[j][1]; 
			distances[i][j]=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)); 
			} 

	for (unsigned i=0;i<nb_facilities; i++) 
			costs[i]=0.5*profit_leader[i];
	//init leader_minimum_profit
	leader_minimum_profit=0; 
	for (unsigned i=0;i<nb_facilities; i++) 
		leader_minimum_profit+=costs[i];
	leader_minimum_profit=nb_open_leaders*leader_minimum_profit/nb_facilities; 

	input.close();
	} else
		 cout << "Couldn't open " << filename << " for reading\n";

	} 


void instance::set_distances(){
//unsigned instance::facilities_positions[MAX_FACILITIES][2]; //positions 
facilities_positions.resize(instance::nb_facilities); 
for(unsigned i=0; i<facilities_positions.size();i++)
	facilities_positions[i].resize(2); 

//unsigned instance::customers_positions[MAX_CUSTOMERS][2]; //positions 
customers_positions.resize(instance::nb_customers); 
for(unsigned i=0; i<customers_positions.size();i++)
	customers_positions[i].resize(2); 

//float instance::distances[MAX_FACILITIES][MAX_CUSTOMERS];//distances  
distances.resize(instance::nb_facilities); 
for(unsigned i=0; i<distances.size();i++)
	distances[i].resize(instance::nb_customers); 

//unsigned instance::costs[MAX_FACILITIES]; //cost for opening each facility 
costs.resize(instance::nb_facilities); 

	for (unsigned i=0;i<nb_facilities; i++) 
		{
 		facilities_positions[i][0]=rand()%MAX_RANDOM1; 
 		facilities_positions[i][1]=rand()%MAX_RANDOM1; 
		}
	for (unsigned j=0;j<nb_customers; j++) 
		{
 		customers_positions[j][0]=rand()%MAX_RANDOM1; 
 		customers_positions[j][1]=rand()%MAX_RANDOM1; 
		}

	for (unsigned i=0;i<nb_facilities; i++) 
		for (unsigned j=0;j<nb_customers; j++) 
			{
			unsigned x1=facilities_positions[i][0]; 
			unsigned y1=facilities_positions[i][1]; 
			unsigned x2=customers_positions[j][0]; 
			unsigned y2=customers_positions[j][1]; 
			distances[i][j]=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)); 
			}
	} 

void instance::set_profits(){

	profit_leader.resize(nb_customers); 
	profit_follower1.resize(nb_customers); 
	profit_follower2.resize(nb_customers); 

	minimum_follower12=MAX_RANDOM2;  
	for (unsigned j=0;j<nb_customers; j++)
		{ 
		profit_leader[j]=rand()%(MAX_RANDOM2-1)+1;
		if(minimum_follower12>profit_leader[j]) minimum_follower12=profit_leader[j]; 
		}
	maximum_follower2=0;  
	for (unsigned j=0;j<nb_customers; j++) 
		{
		profit_follower1[j]=rand()%(MAX_RANDOM2-1)+1; 
		if(maximum_follower2<profit_follower1[j]) maximum_follower2=profit_follower1[j]; 
		if(minimum_follower12>profit_follower1[j]) minimum_follower12=profit_follower1[j]; 
		}
	for (unsigned j=0;j<nb_customers; j++) 
		profit_follower2[j]=maximum_follower2-profit_follower1[j]+1; 
	} 
 
void instance::set_costs_and_leader_minimum_profit() {
	//init costs
	for (unsigned i=0;i<nb_facilities; i++) 
		costs[i]=rand()%minimum_follower12;
	//init leader_minimum_profit
	leader_minimum_profit=0; 
	for (unsigned i=0;i<nb_facilities; i++) 
		leader_minimum_profit+=costs[i];
	leader_minimum_profit=nb_open_leaders*leader_minimum_profit/nb_facilities; 
	} 


void instance::set_big_number(unsigned _big_number)
	{
	big_number=_big_number;
	}

