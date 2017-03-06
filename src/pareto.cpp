
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\include\pareto.h"
#include <random>
#include <vector>
using namespace std; 

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

Pareto::Pareto()
	{
	}


Pareto::Pareto(solution* _solution)
	{
	if(_solution) update(*_solution); 
	}

/*
Supprimer TOUTES les solution de "pareto" qui sont dominées par "s"; 
*/
bool Pareto::my_erase(solution* s)
	{
	bool maj=false; 
	for(iterator i=begin(); i<end(); i++)
		{
		solution* pareto_s=(*i); 
		if(*s==*pareto_s)  break; 
		else if(*s>=*pareto_s) //i.e. *s>*pareto_s
			{
			erase(i);  
			updated=true;
			maj=true;
			delete pareto_s; 
			if(size()) i=begin(); // vérifier s'il y a pas un autre point a supprimer 
			else break; 
			}
		}
	return maj; 			
	}

/*
ESSAYER d'ajouter la solution s au pareto 
*/
bool Pareto::add(solution* s)
	{
	for(iterator i=begin(); i<end(); i++)
		if((**i)>=*s)  
			return false; 
	push_back(new solution(*s)); 

	#ifdef	ALGORITHM
	cout<<" Pareto::add: follower solution was added to Pareto : "; 
	for (unsigned ii=0;ii<instance::nb_facilities; ii++)	
			if (s->follower[ii]==1) cout << ii << " " ;
	cout << endl;
	#endif
	updated=true; 
	return true; 
	}

/*
ESSSAYER de mettre a jour le pareto avec la solution "s"; 
*/
bool Pareto::update(solution s)
	{
	my_erase(&s); 
	return add(&s); 
	}

/*
ESSSAYER de mettre a jour le pareto avec le pareto "s"; 
*/
bool Pareto::update(Pareto* pareto)
	{
	bool retour=false; 
	for(iterator i=pareto->begin(); i<pareto->end();i++)
		retour=retour||update(**i); 
	return retour; 
	}


Pareto::~Pareto()
	{
		this; 
	/*
	for(unsigned i=0; i<size(); i++)
		if(at(i)) 
			if(at(i)->customers.size()!=instance::nb_customers)
				cout<<"ddddddddddddddddd"<<endl; 
			else 
				delete at(i);
	*/
	clear(); 
	}

unsigned Pareto::keep_efficient()
	{
    for(unsigned i=0; i<size();i++)
		if(!at(i)->is_efficient()) 
			{
		    delete at(i); 
			at(i)=NULL; 
			}
    for(unsigned i=0; i<size()-1;i++)
    for(unsigned j=i+1; j<size();j++)
		if(at(i)<at(j))
			{
			solution*tmp=at(i); 
			at(i)=at(j);
			at(j)=tmp; 
			}
    for(unsigned i=0; i<size();i++)
		if(at(i)==NULL)
			this->erase(this->begin()+i,end()); 
	return size();  
	}

unsigned Pareto::replace_by_efficient_solution()
	{
    for(unsigned i=0; i<size();i++)
	{
		solution *newEfficientSol=at(i)->get_efficient(); 
		if( newEfficientSol != NULL ) 
			{
		    delete at(i); 
			at(i)=NULL; 
			at(i)=newEfficientSol;
			}
	}
    for(unsigned i=0; i<size()-1;i++)
    for(unsigned j=i+1; j<size();j++)
		if(at(i)<at(j))
			{
			solution*tmp=at(i); 
			at(i)=at(j);
			at(j)=tmp; 
			}
    for(unsigned i=0; i<size();i++)
		if(at(i)==NULL)
			this->erase(this->begin()+i,end()); 
	return size();  
	}

bool Pareto::is_feasible()
	{
	for(iterator i=begin(); i<end(); i++)
		if((*i)->is_infeasible())
			return false; 
	return true; 
	}

solution* Pareto::worst_leader()
	{
	if(size()==0) return NULL; 
	solution*retour= *begin(); 
	for(iterator i=begin(); i<end(); i++)
		if(retour->leader_objective>(*i)->leader_objective)
			retour=*i; 
	return new solution(*retour); 
	}
 solution* Pareto::worst_efficient_leader()
	{
	//choisir une solution la premiere solution inéfficace rencontré 
	/*
	//le mettre en commentaire permet de réduire le temps de moitité 
	for(iterator i=begin(); i<end();i++) 
		if(!(*i)->is_efficient()) 
			return (*i); 
	*/
	//choisir la solution efficace ayant le plus petit objectif 
	solution*retour=*begin(); 
    for(iterator i=(begin()+1); i<end();i++) 
			if(retour->leader_objective > (*i)->leader_objective)
				retour=*i; 
	return retour; 
	}
