

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\include\tabu_search.h"
#include <iostream>
#include <fstream>


#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

void my_assert(char* message,bool condition)
{
	if(condition)
	{
		cout<<message<<endl<<flush; 
		exit(0); 
	}
}

//===================================================================================
Movement::Movement(unsigned _i1, unsigned _i2)
{
	i1=_i1;
	i2=_i2;

	#ifdef	ALGORITHM
	cout << "TabuList is updated with " << i1 << "; " << i2 << endl;
	#endif 
}
bool Movement::equal(unsigned _i1, unsigned _i2)
{
	return ((i1==_i1)&&(i2==_i2))||((i2==_i1)&&(i1==_i2));
}

//===================================================================================
TabuList::TabuList(unsigned _maximum_size)
{
	maximum_size=_maximum_size;
}

bool TabuList::belong(solution* s)
{
	for(unsigned i=0; i<size(); i++)
		if(at(i).equal(s->swap_index1,s->swap_index2))
			return true;
	return false;
}


bool TabuList::update(solution* s)
{
	for(unsigned i=0; i<size(); i++)
		if(at(i).equal(s->swap_index1,s->swap_index2))
			return true;
	Movement Movement(s->swap_index1,s->swap_index2);
	this->push_back(Movement);
	if(this->size()>maximum_size)
		{
		this->erase(begin());
		#ifdef	ALGORITHM
		cout << "TabuList is reached its max size => 1st element is erased" << endl;
		#endif 
		}
	return false;
}


//===================================================================================
Pareto*  TabuSearch::algorithm1(solution* leader_neighbour)
{
	 #ifdef	ALGORITHM
		cout << " algorithm1 " << endl;
	#endif 
	MultiGenetic multi_genetic; 
	Pareto* follower_potential_pareto=multi_genetic.compute(*leader_neighbour,MULTIGENETIC_INFEASIBLE,MULTI_LOCAL_SEARCH);  //applying GA  to find a pareto for the follower
	solution * infeasible_solution=NULL; 
	do 
	{
		infeasible_solution=leader_neighbour->get_infeasible(follower_potential_pareto); 
		if(infeasible_solution==NULL) break;
		solution *tmp=infeasible_solution->get_efficient(); 
		if(tmp==NULL) break;
		follower_potential_pareto->update(*tmp); 
		delete tmp; delete infeasible_solution; 
		infeasible_solution=NULL;  //clean 
	}
	while (true);
	//returning the pareto front 
	//(infeasible_solution!=NULL) &&
	if( (follower_potential_pareto->keep_efficient()!=0) && (follower_potential_pareto->is_feasible()==true)) {
		#ifdef	ALGORITHM
			cout << "after algorithm1: follower_potential_pareto size = " << follower_potential_pareto->size() <<endl ; 	
		#endif 
		return follower_potential_pareto; 
	}
	else 
	{
		delete follower_potential_pareto;
		#ifdef	ALGORITHM
			cout << "after algorithm1: follower_potential_pareto size = NULL" <<endl ; 	
		#endif
		return NULL; 
	}
}

solution*  TabuSearch::algorithm2(solution* leader_neighbour,Pareto* follower_potential_pareto)
{ 
	#ifdef	ALGORITHM
		cout << endl << "algorithm2 " << endl;
		#endif 
	MultiGenetic multi_genetic; 
	Pareto* follower_potential_pareto2=multi_genetic.compute(*leader_neighbour,MULTIGENETIC_WORST_LEADER,MULTI_LOCAL_SEARCH,follower_potential_pareto);  //applying GA  to find a pareto for the follower
	solution * worst_solution=follower_potential_pareto2->worst_efficient_leader(); 
	do 
	{
		my_assert("Error WORST SOLUTION SHOULD EXIST!!!!",worst_solution==NULL); 

		solution *new_worst_solution=worst_solution->make_leader_worst(follower_potential_pareto2); 
		if(new_worst_solution==NULL) break;
		//delete worst_solution; à regarder ...  
		worst_solution=new_worst_solution; 
		solution *tmp=worst_solution->get_efficient(); if(tmp==NULL) break;
		follower_potential_pareto2->update(*tmp); 
		delete tmp; 
	}
	while (true);
	//returning the pareto front 
	my_assert("Error PARETO SHOULD BE FEASIBLE!!!!",follower_potential_pareto2->is_feasible()==false); 			
	solution* pessimistic_solution=NULL; 
	//if((worst_solution!=NULL) && (follower_potential_pareto2->keep_efficient()!=0)) 
	if(follower_potential_pareto2->keep_efficient()!=0) 
		pessimistic_solution=follower_potential_pareto2->worst_leader(); //à partir d'ici, nous avons un pareto front où toutes les solutions (1) sont efficaces et (2) vérifient les contraints du leader.	
	//si 
	delete follower_potential_pareto2; 
	return pessimistic_solution; 
}

solution* TabuSearch::compute(unsigned nb_iterations, unsigned list_tabu_size)
{	
	cout<< endl; 
	unsigned swap_index1(0);
	unsigned swap_index2(0);
	TabuList*tabu_list=new TabuList(list_tabu_size);
	solution* initial_solution=new solution(); //a random solution   

	#ifdef	ALGORITHM
	cout << "Initial LEADER solution is : " ; 	
	for (unsigned i=0;i<instance::nb_facilities; i++)
	{
		if (initial_solution->leader[i]==1)
			 cout << i << " " ;
	}
		cout << endl;
	//check if leader and follower solution have the common open facilities
	for (unsigned ii=0;ii<instance::nb_facilities; ii++)
	{
		if ((initial_solution->follower[ii]==1) && (initial_solution->leader[ii]==1))
		cout << ii << "Error at tabu search before calling algorithm1 \n" << endl ;
	}
	#endif 

	Pareto* follower_potential_pareto=algorithm1(initial_solution);			 
	solution* pessimistic_solution=algorithm2(initial_solution,follower_potential_pareto); 

	if(follower_potential_pareto) 
			delete follower_potential_pareto; 
	if(pessimistic_solution==NULL)	
		initial_solution->leader_objective = -instance::big_number; // initialising to a small number
	/// end of my added part
	delete pessimistic_solution; 
	#ifdef	ALGORITHM
		cout << "initial LEADER OFV is : " << initial_solution->leader_objective << endl ; 	
	#endif 

		/**	cout << "Initial FOLLOWER solution is : " ; 	
		for (unsigned i=0;i<instance::nb_facilities; i++)
			if (initial_solution->follower[i]==1)
				 cout << i << " " ;
		cout << endl; 
		cout << "initial FOLLOWER objective function values are : " << initial_solution->follower_objective1 << " ; " << initial_solution->follower_objective2 << endl ; 	*/
	/// starting from this I changed TS

	solution* current_solution =initial_solution; //changemnet de nom. meme objet. 
	solution* record_ts=new solution(*current_solution); 

	while(nb_iterations>0)  // check: criteria to stop !!! in TS we move to the neigh solution even if it is worse than the initial one 
	{
		cout << " Tabu Search. Iteration  : " << nb_iterations << "\n "; 	 
		//TabuList*tabu_list_tmp=new TabuList(neighbourhood_size);
		solution* best_neighbour=new solution(*current_solution); 
		best_neighbour->leader_objective=-instance::big_number;

		/*before new swap:
		 for(unsigned i=0; i<neighbourhood_size; i++) //visit leader neighbours
			 {
			solution* leader_neighbour=new solution(*current_solution,100);// CHECK THAT IT IS NOT in TABULIST!!!! 
		*/
		/// new swap 
		static unsigned tempSolution[MAX_FACILITIES]; 
		unsigned index=0;
		unsigned neighborNumber=0;
		for(unsigned c=0; c<instance::nb_facilities; c++)
			if(current_solution->leader[c]==1){
				tempSolution[index]=c;
				index++;
			}
		for(unsigned c=0; c<instance::nb_facilities; c++)
			if(current_solution->leader[c]==0){
				tempSolution[index]=c;
				index++;
			}

		for(unsigned c1=0; c1<instance::nb_open_leaders ; c1++)
			for(unsigned c2=instance::nb_open_leaders; c2<instance::nb_facilities; c2++)
			{ 
			float pr=(double)rand() / (RAND_MAX + 1.0);
			if (pr<=NEIGHB_RATE_TS){
				neighborNumber++;
				swap_index1= tempSolution[c1];
				swap_index2= tempSolution[c2] ;
				if(swap_index1>300 || swap_index2> 300){ 
					cout << "Error in swaps" << endl;
				}
				solution* leader_neighbour=new solution(*current_solution);
				unsigned swap_tmp=leader_neighbour->leader[swap_index1];
				leader_neighbour->leader[swap_index1]=leader_neighbour->leader[swap_index2];  		
				leader_neighbour->leader[swap_index2]=swap_tmp;
		
				if (tabu_list->belong(leader_neighbour)) {
						#ifdef ALGORITHM
						cout << "Swap facilities are (" <<swap_index1 << "," << swap_index2 << ") from TL! tabu_list = " << tabu_list->size() << endl; 
						#endif
						delete leader_neighbour; 
						continue;
				}

				leader_neighbour->follower_intialize();
				//solution::follower_solution(*leader_neighbour) ; 
				leader_neighbour->update_objectives(); 
				//end of new swap
					#ifdef ALGORITHM
					cout << "pr = " << pr << endl; 
					cout << " Looking at Neighbourhood :" << endl; 				
					//cout << "LEADER Neighbour " << i+1 << ":" << endl; 	
					cout << "LEADER Neighbour " << neighborNumber << ":" << endl; 	
					for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
					if (leader_neighbour->leader[ii]==1)
						 cout << ii << " " ;
					cout << endl; 
					#endif 
			
					Pareto* follower_potential_pareto=algorithm1(leader_neighbour);			
					solution* pessimistic_solution=algorithm2(leader_neighbour,follower_potential_pareto); 
			
					//cout << "Neighbour LEADER OFV is : " << pessimistic_solution->leader_objective << endl ; 
					if(follower_potential_pareto) 
						delete follower_potential_pareto; 
					if(pessimistic_solution==NULL)	{cout << "delete leader_neighbour " << endl; delete leader_neighbour; continue;} //à vérifier?????? 
					//choose best_neighbour			
					if(best_neighbour->leader_objective<pessimistic_solution->leader_objective)
						{
							if(swap_index1>300 || swap_index2> 300){ 
								cout << "Error2 in swaps" << endl;
							}
							swap_index1= leader_neighbour->leader[swap_index1];
							swap_index2= leader_neighbour->leader[swap_index2];
							if(swap_index1>300 || swap_index2> 300){ 
								cout << "Error3 in swaps" << endl;
							}
							delete best_neighbour; 
							best_neighbour=pessimistic_solution;
						}
					else 
						delete pessimistic_solution; 
					delete leader_neighbour;
				} }//end of exploring neighborhood 
		
		#ifdef	ALGORITHM
		cout << "=============== end of looking Neighbourhood =================" << endl; 				
		cout << "Best LEADER Neighbour is :" << endl; 	
		for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
		if (best_neighbour->leader[ii]==1)
			cout << ii << " " ;
		cout << endl;
		cout << "Best LEADER Neighbour OFV is : " << best_neighbour->leader_objective << endl ; 
		#endif 

		current_solution = best_neighbour;
		if(best_neighbour->leader_objective > record_ts->leader_objective )
			{ delete record_ts; 
			  record_ts= best_neighbour;

			#ifdef	ALGORITHM
			  cout << "Current record LEADER solution is :" << endl; 	
			 for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
				 if (record_ts->leader[ii]==1)
					cout << ii << " " ;
			cout << endl;
			cout << "Current record LEADER  OFV is : " << record_ts->leader_objective << endl ; 
			 cout << "(Current record) Follower solution is :" << endl; 	
			 for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
				 if (record_ts->follower[ii]==1)
					cout << ii << " " ;
			cout << endl;
			cout << "(Current record) Follower OFV1: " << record_ts->follower_objective1 << "; OFV2 " << record_ts->follower_objective2 << endl ; 
			#endif
			}
	if (swap_index1!=0 && swap_index2!=0) {
	best_neighbour->swap_index1=swap_index1;
	best_neighbour->swap_index2=swap_index2;
	tabu_list->update(best_neighbour);
	}
	nb_iterations--; 
	swap_index1=0;
	swap_index2=0;
	}

	//#ifdef	ALGORITHM
	cout << "========================= \n" ;  
	cout << "final record : LEADER solution is : " ; 	
	for (unsigned i=0;i<instance::nb_facilities; i++)
		if (record_ts->leader[i]==1)
			 cout << i << " " ;
		cout << endl;
	cout << "final record : Follower solution is : " ; 	
	for (unsigned i=0;i<instance::nb_facilities; i++)
		if (record_ts->follower[i]==1)
			 cout << i << " " ;
		cout << endl;
	cout << "record L-OFV is : " << record_ts->leader_objective << endl ; 	
	cout << "record F-OFVs are : " << record_ts->follower_objective1 << " ; " << record_ts->follower_objective2 << endl ; 	
	cout << "========================= \n" ; 
	// #endif
	return record_ts;
}

