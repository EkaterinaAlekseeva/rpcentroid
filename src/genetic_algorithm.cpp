
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\include\genetic_algorithm.h"

using namespace std; 

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

unsigned MultiGenetic::size; 
unsigned MultiGenetic::generations;




int MultiGenetic::get_effective(unsigned index)
	{
		if(type==MULTIGENETIC_INFEASIBLE)
			return pareto->at(index)->leader_infeasibility; 
		else if(type==MULTIGENETIC_WORST_LEADER)
			return pareto->at(index)->leader_objective; 
	}

void MultiGenetic::set_minimum_maximum()
	{
	minimum=INT_MAX; 
	maximum=INT_MIN;
	for(unsigned i=0; i<size; i++)
		{
		int objective=get_effective(i); //.... 
		if(objective<minimum) minimum=objective; 
		if(objective>maximum) maximum=objective; 
		}
	}

unsigned MultiGenetic::select_parent(unsigned parent1)
	{
	int selected=0; 
	for(unsigned i=0; i<pareto->size(); i++)
		if((i!=parent1)&&(get_effective(i)<get_effective(selected)))
			selected=i; 
	return selected; 
	}

solution* MultiGenetic::crossover(unsigned parent1, unsigned parent2) 
	{
	solution* s1=pareto->at(parent1); 
	solution* s2=pareto->at(parent2); 
	solution* s=new solution(*s1); 
	for(int f=0; f<instance::nb_facilities; f++)
		if(s2->follower[f])
			s->follower[f]=1; 
	unsigned total=0; 
	for(int f=0; f<instance::nb_facilities; f++)
		total+=s->follower[f]; 
	while(total>instance::nb_open_followers)
		{
		unsigned f=rand()%instance::nb_facilities;
		if(s->follower[f])
				{
				s->follower[f]=0; 
				total--; 
				}
		}
	#ifdef ALGORITHM 
		cout << "Child :" << endl;
		for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
			if (s->follower[ii]==1) cout << ii << " " ;
		cout << endl;
	#endif
	return s; 
	}

Pareto* MultiGenetic::multi_local_search(solution* current_solution)
	{
	solution* tmp=new solution(*current_solution); 
	Pareto* pareto=new Pareto();
	pareto->update(*tmp); 
	delete tmp; 

	while(pareto->updated)
		{
		pareto->updated=false; 
		Pareto *tmp=new Pareto(NULL); //create temporary pareto 
		//visit neighbours
		for(unsigned i=0; i<pareto->size(); i++)
			{
			solution* current=pareto->at(i); 
			if(current->neighbours_visited) continue; 
			current->neighbours_visited=true; 

			#ifdef	ALGORITHM
				cout << "Take FOLLOWER solution (center of neighbor) " << i << " from Pareto  :" << endl; 	
				for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
					{
						if (current->follower[ii]==1)
								 cout << ii << " " ;
					}
				cout << "; L-OFV =  " << current->leader_objective ; 
				cout << " F-OFV1 =  " << current->follower_objective1 ; 
				cout << " F-OFV2 =  " << current->follower_objective2 << endl ; 
				//check
				for (unsigned ii=0;ii<instance::nb_facilities; ii++)
					if ((current->follower[ii]==1) && (current->leader[ii]==1))
						cout << ii << " Error at genetic algo \n" << endl ;
			#endif
			unsigned swap_index1(0);
			unsigned swap_index2(0);
			static unsigned tmpFollowerSolution[MAX_FACILITIES]; 
			unsigned index=0;
			unsigned neighborNumber=0;
			for(unsigned c=0; c<instance::nb_facilities; c++)
				if(current->follower[c]==1){
					tmpFollowerSolution[index]=c;
					index++;
				}
			for(unsigned c=0; c<instance::nb_facilities; c++)
				if(current->follower[c]==0){
					tmpFollowerSolution[index]=c;
					index++;
				}
				//main loop for neighborhood 
			solution* neighbour= new solution(*current);
			for(unsigned c1=0; c1<instance::nb_open_followers ; c1++)
				for(unsigned c2=instance::nb_open_followers; c2<instance::nb_facilities; c2++)
			{ 
			float pr=(double)rand() / (RAND_MAX + 1.0);
			if (pr<=NEIGHB_RATE_MLS && neighbour->leader[tmpFollowerSolution[c2]]==0 ){
				neighborNumber++;
				swap_index1= tmpFollowerSolution[c1];
				swap_index2= tmpFollowerSolution[c2]; 
					#ifdef	ALGORITHM
						/*
					cout << " swap_index1 (facility number to be closed) ="<< swap_index1 << endl; 				
					cout << " swap_index2 (facility number to be open) ="<< swap_index2 << endl; 				*/
					cout << "Leader : " ;
					for (unsigned ii=0;ii<instance::nb_facilities; ii++)
						{
						if (neighbour->leader[ii]==1) cout << ii << " " ;
						}
					unsigned myCounter(0);
						for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
							if (neighbour->follower[ii]==1)
									 myCounter++ ;
				/*
					cout << "FOLLOWER before swap " <<  endl; 	
						for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
							if (neighbour->follower[ii]==1)	 cout << ii << " " ;
						cout << endl;*/
					#endif
					unsigned swap_tmp=neighbour->follower[swap_index1];
					neighbour->follower[swap_index1]=neighbour->follower[swap_index2];  		
					neighbour->follower[swap_index2]=swap_tmp;
					#ifdef	ALGORITHM
						unsigned myCounterAfter(0);
						for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
							if (neighbour->follower[ii]==1)
									 myCounterAfter++ ;
						if ( myCounterAfter != FOLLOWERS_NUMBER ) cout << "Error: after swap follower opens : " << myCounterAfter << endl; 	
					#endif

				neighbour->update_objectives(); 

				#ifdef	ALGORITHM
					cout << " FOLLOWER Neighbour "  << neighborNumber << " : "; 	
					for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
						if (neighbour->follower[ii]==1)
								 cout << ii << " " ;
					cout << "; L-OFV =  " << neighbour->leader_objective ; 
					cout << " F-OFV1 =  " << neighbour->follower_objective1 ; 
					cout << " F-OFV2 =  " << neighbour->follower_objective2 << endl ;
					//check
					for (unsigned ii=0;ii<instance::nb_facilities; ii++)
					{
						if ((neighbour->follower[ii]==1) && (neighbour->leader[ii]==1))
							cout << ii << " Error at neighborhood at genetic algo \n" << endl ;
					}
				#endif
				tmp->update(*neighbour);
				delete neighbour;
				neighbour= new solution(*current);
				swap_index1=0;
				swap_index2=0;
				/*
				cout << "(center of neighbor) FOLLOWER solution"  << endl; 	
				for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
					if (neighbour->follower[ii]==1)
							 cout << ii << " " ;
				cout << endl;*/
				}
			} //new swap
			}
		//update Pareto
		//cout << " Updating Pareto with temporary pareto "  << endl; 
		for(unsigned i=0; i<tmp->size(); i++)
			pareto->update(*tmp->at(i)); 
		#ifdef	ALGORITHM
		cout << " Pareto size after multi_local_search =" << pareto->size(); 
		cout << " Pareto solutions : " << endl; ; 
		for(unsigned i=0; i<pareto->size(); i++)
		{
			for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
					if (pareto->at(i)->follower[ii]==1)
							 cout << ii << " " ;
				cout << "; L-OFV =  " << pareto->at(i)->leader_objective ; 
				cout << " F-OFV1 =  " << pareto->at(i)->follower_objective1 ; 
				cout << " F-OFV2 =  " << pareto->at(i)->follower_objective2 << endl ; 
				cout << endl;
		}
		#endif
		//delete temporary pareto 
		delete tmp; 
		}
	return pareto; 
	}


Pareto*MultiGenetic::compute(solution& leader_solution, short _type, short _ls_type, Pareto *_pareto) 
	{
	//MultiLocalSearch *multi_local_search=new MultiLocalSearch(); 
	type=_type; 
	if(_pareto)
	{
		pareto=new Pareto(*_pareto); 
		#ifdef	ALGORITHM
			cout << " begin of algorithm2 , Genetic Algo, _pareto->size= " << pareto->size() << endl ;
			if (pareto->size() >=1)
			{
			cout << " Pareto solutions : " << endl; ; 
			for(unsigned i=0; i<pareto->size(); i++)
				{
				for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
						if (pareto->at(i)->follower[ii]==1)
								 cout << ii << " " ;
					cout << "; L-OFV =  " << pareto->at(i)->leader_objective ; 
					cout << " F-OFV1 =  " << pareto->at(i)->follower_objective1 ; 
					cout << " F-OFV2 =  " << pareto->at(i)->follower_objective2 << endl ; 
					cout << endl;
				}
			}
		#endif ALGORITHM
	}
	else
		{
			#ifdef	ALGORITHM
			//check if leader and follower solutions have common open facilities
					for (unsigned ii=0;ii<instance::nb_facilities; ii++)
					{
						if ((leader_solution.follower[ii]==1) && (leader_solution.leader[ii]==1))
							cout << ii << " Error in MultiGenetic::compute" << endl ;
					}
			#endif
		pareto=multi_local_search(&leader_solution); 
		}
	int tmp=generations; 
	while((tmp--)>0 && (pareto->size()>=1))
			{
			#ifdef	ALGORITHM
			cout<<"MultiGenetic. Generation " << tmp << endl<<flush; 
			#endif
			solution* enfant1=pareto->at(0); 
			if(pareto->size()>1)
			{
				#ifdef	ALGORITHM
				cout << " pareto size = " << pareto->size() << ". Choice of parents " << endl;
				#endif
				unsigned parent1=rand()%pareto->size(); 
				unsigned parent2=select_parent(parent1); 
				solution* enfant1=crossover(parent1,parent2); 
			}				
			if(_ls_type==MULTI_LOCAL_SEARCH)
				{
				#ifdef	ALGORITHM
				cout << "multi_local_search applied to enfant starts" << endl;
				#endif
				Pareto *enfants=multi_local_search(enfant1); 
				if(pareto->update(enfants)) 
					tmp=generations;
				else if(pareto->size()==1) {
					tmp=0;
					#ifdef	ALGORITHM
					cout << " Pareto has not been updated. Pareto size =" << pareto->size() << ". Genetic stop " << endl;
					#endif

				}
				}
			}
	#ifdef	ALGORITHM
	cout << "end of MultiGenetic. Pareto size =" << pareto->size() << endl << endl; 
	#endif
	return pareto; 
	}


MultiGenetic::MultiGenetic()
	{
	type=77; 
	}
