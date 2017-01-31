#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\include\solution.h"
#include <random>
#include <vector>
#include <memory>
using namespace std; 

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

#define MINIMUM(a,b) ((a<b)?a:b)

int compteur=0; 
int compteur_total=0; 

int model_S1_is_infeasible =0;
int model_S2_has_solution =0;
int model_SEP_has_solution =0;
int model_AP_has_solution =0;

//===================================================
//===================================================
//===================================================

leader_solution::~leader_solution() 
	{
#ifdef MEMORY 
		compteur--; 
		std::cout<<".............................Delete solution " <<compteur<<endl<<flush; 
#endif 
	}

leader_solution::leader_solution() 
	{
		leader.resize(instance::nb_facilities); 
#ifdef MEMORY 
		compteur_total++; 
		compteur++; 
		std::cout<<".............................New solution " <<compteur<<endl<<flush; 
#endif 
		for (unsigned i=0;i<instance::nb_facilities; i++)
			leader[i]=0;
		for (unsigned i=0;i<instance::nb_open_leaders; i++)
			{
			unsigned index; 
			do index=rand()% instance::nb_facilities; 
			while (leader[index]);
			leader[index]=1; 
			}
		//debug print
		/* cout << "constructeure leader_solution ";
		for (unsigned i=0;i<instance::nb_facilities; i++)
			if (leader[i]==1)
			 cout << i << " " ;
		cout << endl; */
	}

leader_solution::leader_solution(leader_solution& _solution)
	{
		for (unsigned i=0;i<instance::nb_facilities; i++)
			leader[i]=_solution.leader[i]; 
	}


//===================================================
//===================================================
//===================================================
void follower_solution::follower_intialize() 
	{
	for (unsigned i=0;i<instance::nb_facilities; i++)
		follower[i]=0;
	for (unsigned i=0;i<instance::nb_open_followers; i++)
		{
		unsigned index; 
		do index=rand()% instance::nb_facilities; 
		while (leader[index] || follower[index]);
		follower[index]=1; 
		}
	//debug print

		for (unsigned i=0;i<instance::nb_facilities; i++)
			if ((follower[i]==1) && (leader[i]==1))
				cout << "constructeure follower_solution : " << i << " Errrorrrrr " << endl ;

	}


follower_solution::follower_solution() 
	{
	follower.resize(instance::nb_facilities);

	follower_potential_facilities.resize(instance::nb_facilities); 
	for(unsigned i=0; i<follower_potential_facilities.size();i++)
		follower_potential_facilities[i].resize(instance::nb_customers); 

	follower_intialize(); // we need generate RANDOM follower solution only for Genetic algo
	}

follower_solution::follower_solution(leader_solution& _solution)  
	{
	leader_solution::leader_solution(_solution); 
	follower_solution::follower_solution(); 

		for (unsigned i=0;i<instance::nb_facilities; i++)
			if ((follower[i]==1) && (leader[i]==1))
			 cout << i << " Errrorrrrr\n" ;
	}

void follower_solution::set_follower_potential_facilities()
	{
	for(int c=0;c<instance::nb_customers;c++)
		{
		float distance=FLT_MAX; 
		for(int f=0;f<instance::nb_facilities;f++)
			if(leader[f])
				distance=MINIMUM(distance,instance::distances[f][c]); 
		for(int f=0;f<instance::nb_facilities;f++)
			follower_potential_facilities[f][c]=(instance::distances[f][c]<distance); 
		}
	}

float follower_solution::utilitity(float weight)
	{
	return weight*follower_objective1+(1-weight)*follower_objective2; 
	}



//===================================================
//===================================================
//===================================================


solution::solution() 
	{
	resize(); 
	neighbours_visited=false; 
	update_objectives(); 
	}

solution::solution(const solution& _solution)
	{
	resize(); 
	*this=_solution; 
	}

solution::solution(leader_solution& _solution) 
	{
	resize(); 
	follower_solution::follower_solution(_solution); 
	update_objectives(); 
	}


solution::solution(Model*model, solution*s)
	{
	customers.resize(instance::nb_customers); 
	solution::solution(*s); 
	for(unsigned i=0;i<instance::nb_facilities; i++)
		leader[i]=s->leader[i]; 
	
	for(unsigned i=0;i<instance::nb_facilities; i++)
		follower[i]=model->solutions_get(i); 
	for(unsigned i=0;i<instance::nb_customers; i++)
		customers[i]=model->solutions_get(instance::nb_facilities+i); 

	#ifdef ALGORITHM 
		cout << "after 'solution(Model*model, solution*s)' Leader solution : " ;
		for (unsigned i=0;i<instance::nb_facilities; i++)	
			if (leader[i]==1) cout << i<< " " ;
		cout << endl;
	#endif

	update_objectives(); 
	}


solution& solution::operator=(const solution& _solution) 
	{
	neighbours_visited=_solution.neighbours_visited; 
    leader_infeasibility=_solution.leader_infeasibility;  
	leader_objective=_solution.leader_objective;  
	follower_objective1=_solution.follower_objective1;  
	follower_objective2=_solution.follower_objective2;  
 	//memcpy(leader, _solution.leader, sizeof(unsigned)*MAX_FACILITIES);
	leader=_solution.leader; 
 	//memcpy(follower, _solution.follower, sizeof(unsigned)*MAX_FACILITIES);
	follower=_solution.follower; 
 	//memcpy(customers, _solution.customers, sizeof(unsigned)*MAX_CUSTOMERS);
	customers=_solution.customers; 
    return *this;
	}

bool solution::operator==(const solution& s)
	{
	bool objective1=(follower_objective1 == s.follower_objective1); 
	bool objective2=(follower_objective2 == s.follower_objective2);  
	return objective1 && objective2; 
	}

bool solution::operator>=(const solution& s)
	{
	bool objective1=(follower_objective1 >= s.follower_objective1);
	bool objective2=(follower_objective2 >= s.follower_objective2);  
	return objective1 && objective2; 
	}

void solution::set_leader_objective()
	{
	leader_objective=0; 
	for(unsigned c=0; c<instance::nb_customers; c++)
		leader_objective+=instance::profit_leader[c]*customers[c]; 
	for(unsigned f=0; f<instance::nb_facilities; f++)
		leader_objective-=instance::costs[f]*leader[f]; 
		
	}

void solution::set_leader_infeasibility()
	{
	leader_infeasibility=0; 
	for(int c=0;c<instance::nb_customers;c++)
		leader_infeasibility+=instance::profit_leader[c]*customers[c]; 
	leader_infeasibility-=instance::leader_minimum_profit; 
	}


void solution::set_follower_objectives()
	{
	follower_objective1=0; 
	for(unsigned c=0; c<instance::nb_customers; c++)
		follower_objective1+=instance::profit_follower1[c]*(1-customers[c]); 

	follower_objective2=0; 
	for(unsigned c=0; c<instance::nb_customers; c++)
		follower_objective2+=instance::profit_follower2[c]*(1-customers[c]); 
	}

bool solution::leader_constraints_satisfied()
	{
	unsigned sum=0; 
	for(unsigned c=0; c<instance::nb_customers; c++)
		sum+=instance::profit_leader[c]*customers[c]; 
	return (sum>=instance::leader_minimum_profit); 
	}

void solution::set_customers()
	{
	for(int c=0;c<instance::nb_customers;c++)
		{
		float min_distance=FLT_MAX; 
		unsigned min_facilty; 
		for(int f=0;f<instance::nb_facilities;f++)
			if(leader[f]||follower[f])
				{
				float distance=instance::distances[f][c];
				if(distance<min_distance) {min_distance=distance;min_facilty=f;}
				}
		customers[c]=leader[min_facilty]; 
		}
	}

void solution::update_objectives()
	{
	set_follower_potential_facilities(); 
	set_customers(); 
	set_follower_objectives(); 
	set_leader_infeasibility(); 
	if(leader_constraints_satisfied())
		set_leader_objective();
	else 
		leader_objective=INT_MIN; 
	}

//===================================================
//===================================================
//===================================================

bool solution::is_efficient()
	{
	
		#ifdef ALGORITHM 
			cout << "SEP model: to check is_efficient of Follower solution : ";
				for (unsigned i=0;i<instance::nb_facilities; i++)
					{
						if (this->follower[i]==1)
						cout << i << " " ;
					}
				cout << endl;
		#endif 
	SEP sep(*this);
	
	if (sep.solve("sep","max")==true)
	{
		if (sep.objective_get()==0)
		{
			#ifdef ALGORITHM 
			cout << "OFV in SEP =0 => solution is efficient" << endl;
			#endif 
			return true;
		}
		else
			{
				solution *retourSEPefficient=new solution(&sep,this); 
				#ifdef ALGORITHM 
				cout << "SEP OFV =" << sep.objective_get() << "=> solution is not efficient." << endl; 
				cout << "New efficient solution is :" << endl;
				cout << "Leader: " ;
				for (unsigned i=0;i<instance::nb_facilities; i++)
					{
						if (retourSEPefficient->leader[i]==1)
						cout << i << " " ;
					}
				cout << " Follower : " ;
				for (unsigned i=0;i<instance::nb_facilities; i++)
					{
						if (retourSEPefficient->follower[i]==1)
						cout << i << " " ;
					}
				cout << endl << endl;
				#endif 
				// return new solution(sep,this);
				return false;
			}
	}
	else
	{
#ifdef ALGORITHM 
		cout << "SEP does not have any feasible solution. It is ERROR" << endl;
#endif 
		return false;
	}
	/// fin de corrige par K.
	}

solution* solution::get_efficient()
	{
	//Tester (Using CPLEX/SEP model) si cette solution est efficace  

#ifdef ALGORITHM 
	cout << " SEP model: get_efficient solution   " << endl;
#endif
	SEP sep(*this); 
	sep.solve("sep","max"); //always true 
	if(sep.objective_get()==0) 
		{
		#ifdef ALGORITHM 
			cout << "solution is  efficient" << endl;
		#endif 
		return NULL; // si cette solution est efficace (ignorer le voisin, et passer un autre voir pour leader ) 
		}
	else {
		solution *retourSEP=new solution(&sep,this);
		model_SEP_has_solution++;
		#ifdef ALGORITHM 
		cout << "SEP has found a new efficient solution :" << endl;
		for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
			if (retourSEP->follower[ii]==1) cout << ii << " " ;
		cout << endl;
		#endif 
	return retourSEP;
	}
	}

bool solution::is_infeasible()
	{
#ifdef ALGORITHM 
	cout << " S1 model : to check if there alternative infeasible solution " << endl;
#endif 
	S1 s1(*this); 
	return s1.solve("s1","min");
	//S1*infeasible_solution=new solution(s1,leader_neighbour); 
	}

solution* solution::get_infeasible(Pareto* pareto)
	{
	//vérifier (using CPLEX/S2 model) si l'une des solution du followers qui se trouvent dans pareto est infeasable 
	//infeasable = ne vérifie les contraintes du leader
#ifdef ALGORITHM 
	cout << "S2 model checks if there is solution that dominates current Pareto efficient solutions and is infeasible" << endl;
#endif 
	S2 s2(*this,*pareto); 
		
	solution*retour; 
	if(s2.solve("s2","min")==false)
	{
		retour=NULL; 
		#ifdef ALGORITHM 
		cout << "S2 does not have feasible solution " << endl;
		#endif 
	}
	else 
		{
		model_S2_has_solution++;
		retour=new solution(&s2,this); // get one infeasible solution 
		#ifdef ALGORITHM 
			cout << "S2 has feasible solution :" << endl;
			for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
				if (this->follower[ii]==1) cout << ii << " " ;
			cout << endl;
		#endif 
		}
	return retour; 
	}

solution* solution::make_leader_worst(Pareto* pareto)
	{
	//vérifier (using CPLEX/AP model). Y a-t-il (1) une nouvelle solution du follower, (2) qui est a un cout objective-leader plus petit que '*this', et (3) qui dominie 'pareto' 
	#ifdef ALGORITHM
	cout << " AP model : Y a-t-il " <<endl;
	cout << "(1) une nouvelle solution du follower, "<< endl;
	cout << "(2) qui est a un cout objective-leader " << endl;
	cout << "(3) qui dominie Pareto actuel " << endl;
	#endif
	cout << "Leader before resolution  : ";
	for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
	{if (this->leader[ii]==1) cout << ii << " " ;}
	cout << endl;
	AP ap(*this,*pareto); 
	if(ap.solve("ap","min")==false) 
		{  
		#ifdef ALGORITHM
		cout << "AP is not feasible " << endl;
		#endif
		return NULL; //aucune solution 
		}
	else 
		{
		solution *retourAP=new solution(&ap,this); 
		#ifdef ALGORITHM
		cout << "AP has a solution \n";
		cout << "Leader: ";
		for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
		{if (retourAP->leader[ii]==1) cout << ii << " " ;}
		cout << " Follower : ";
		for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
		{if (retourAP->follower[ii]==1) cout << ii << " " ;}
		cout << endl;
		#endif
		model_AP_has_solution++;
		return retourAP; 
		}
	}

/*
bool solution::swap_another(unsigned*follower_leader,unsigned& maximum)
{
	bool ok=(follower_leader[swap_index1]==follower_leader[swap_index2])&&(--maximum>0);  
	if(follower_leader==leader)
		return ok; 
	else if(follower_leader==follower)
		return ok||(leader[swap_index1]==1)||(leader[swap_index2]==1); 
}

bool solution::swap(unsigned*follower_leader,unsigned maximum)
	{
	do 
		{
		swap_index1=rand()% instance::nb_facilities; 
 		swap_index2=rand()% instance::nb_facilities; 
		}
	while (swap_another(follower_leader,maximum));
	if(maximum>=1)
		{
		unsigned swap_tmp=follower_leader[swap_index1];
		follower_leader[swap_index1]=follower_leader[swap_index2];  		
		follower_leader[swap_index2]=swap_tmp;
		update_objectives(); 
		return true; 
		}
	else 
		return false; 
	}

*/


void solution::resize()
	{
	follower.resize(instance::nb_facilities);
	customers.resize(instance::nb_customers); 
	leader.resize(instance::nb_facilities); 
	}

