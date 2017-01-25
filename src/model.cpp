
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\include\model.h"
#include <memory>
//===============================================================
//===============================================================
//===============================================================
int compteur2=0; 
int compteur_total2=0; 
int nb_model_SEP=0;

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

Model::Model()
	{
	variables.resize(MAX_VARIABLES); 
#ifdef MEMORY 
	compteur2++;  
	compteur_total2++; 
	std::cout<<"========================New model " <<compteur2<<endl<<flush; 
	std::cout<<"=======================compteur_total2 " <<compteur_total2<<endl<<flush; 
#endif 
	}


void Model::Model_construire(short _number_variables1,short _number_variables2, short _number_constraint1, short _number_constraint2)
	{
	nb_var_int=_number_variables1;    
	nb_var_real=_number_variables2; 
	nb_var=_number_variables1+_number_variables2; 
	nb_const_eq=_number_constraint1;  
	nb_const_leq=_number_constraint2;  
	nb_const=_number_constraint1+_number_constraint2;  
	}

Model::Model(short _number_variables1,short _number_variables2, short _number_constraint1, short _number_constraint2)
	{
	Model_construire(_number_variables1,_number_variables2,_number_constraint1,_number_constraint2); 
	}


void Model::variables_initialize(const vector<int>& left,const vector<int>& right)
	{

	for(int i=0;i<nb_var;i++)
		{
		IloNumVar::Type type=(i<nb_var_int)?(IloNumVar::Int):(IloNumVar::Float); 
	//	auto tmp=make_shared<IloNumVar>(env,left[i],right[i],type); 

		IloNumVar* tmp=new IloNumVar(env,left[i],right[i],type); 
		variables[i]=*tmp; 
		delete tmp; 
		}
	}


void Model::variables_initialize_infinity()
	{
	//int left[MAX_VARIABLES]; 
	vector<int> left; left.resize(MAX_VARIABLES);  
	//int right[MAX_VARIABLES]; 
	vector<int> right; right.resize(MAX_VARIABLES);  
	for(short i=0; i<nb_var; i++)
		{
			left[i]=SHRT_MIN; 
			right[i]=SHRT_MAX; 
		}
	variables_initialize(left,right); 
	}

void Model::variables_initialize_bool()
	{
	//int left[MAX_VARIABLES]; 
	vector<int> left; left.resize(MAX_VARIABLES);  
	//int right[MAX_VARIABLES]; 
	vector<int> right; right.resize(MAX_VARIABLES);  
	for(short i=0; i<nb_var; i++)
		{
			left[i]=0; 
			right[i]=1; 
		}
	variables_initialize(left,right); 
	}

void Model::objective_initialize(vector<int>& coefficients)
	{
	objective=new IloExpr(env);
	//auto objective = make_shared<IloExpr>(env);
	for (short i = 0; i < nb_var; i++)
		{
		IloNumVar variable=variables[i];  
		int coefficient=coefficients[i];  
		(*objective)+= coefficient*variable; 
		}
	}

void Model::objective_initialize_zero()
	{
	//int coefficients[MAX_VARIABLES]; 
	vector<int> coefficients; coefficients.resize(MAX_VARIABLES);  
	for(short i=0; i<nb_var; i++) coefficients[i]=0; 
	objective_initialize(coefficients); 
	}

void Model::constraints_initialize(vector<vector<int>>& left,vector<int>& right)
	{
		ilomodel=new IloModel(env);
		//auto ilomodel = make_shared<IloModel>(env);
		for (short i = 0; i < nb_const; i++)
			{
			//the right side 
			IloExpr exprRight(env);
			exprRight+=right[i];
			//the left side 
			IloExpr exprLeft(env);	
			for (short j = 0; j < nb_var; j++)
				{
				IloNumVar variable=variables[j];  
			    double coefficient=left[i][j]; 
				exprLeft+=coefficient*variable; 
				}
			//adding the constraint
			if(i<nb_const_eq)
				ilomodel->add(exprLeft==exprRight); 
			else
				ilomodel->add(exprLeft<=exprRight); 
			//clear 
			exprRight.end(); //usefull??? 
			exprLeft.end(); //usefull??? 
			}
	}


unsigned Model::solve(string name, string min_max)
	{
		//min or max
		IloObjective aa; 
		if(min_max=="min")
			aa=IloMinimize(env, *objective); 
		else 
			aa=IloMaximize(env, *objective);
		ilomodel->add(aa);
		//resolution 
		cplex=new IloCplex(*ilomodel); 
		//auto cplex = make_shared<IloCplex>(*ilomodel);
		#ifndef CPLEX  
		cplex->setOut(env.getNullStream()); 
		freopen("DeleteMe.txt", "w", stderr); 
		#endif 
		string name_str=name+".lp";
		if (name=="sep"){
			nb_model_SEP++ ;
			name_str=name+to_string(nb_model_SEP)+".lp";
		} 		
	//	cout<<"before exported"<<endl<<flush; 
	//	cplex->exportModel(name_str.c_str());  
	//	cout<<"exported"<<endl<<flush; 
		cplex->setParam(IloCplex::EpGap,0);
		cplex->setParam(IloCplex::EpAGap,0);
		cplex->solve();
		#ifdef ALGORITHM 
		cout << name_str << " model; Solution status = " << cplex->getStatus() << endl;
		#endif 
		return (cplex->getStatus()!=IloAlgorithm::Infeasible); 
	}

float Model::objective_get() 
	{
	return cplex->getObjValue(); 
	}

float Model::solutions_get(int index) 
	{
	return cplex->getValue(variables[index]); 
	}

void Model::solutions_get(float*solutions) 
	{
	for(auto i = 0; i < nb_var; i++)
		solutions[i]=solutions_get(i);
	}


Model::~Model()
	{

#ifdef MEMORY 
	compteur2-=1;  
	std::cout<<"=============================Delete model " <<compteur2<<endl<<flush; 
#endif 
	objective->end(); 
	delete objective; 
	ilomodel->end(); 
	delete ilomodel; 
	cplex->end(); 
	delete cplex; 
	env.end();
	}

//===============================================================
//===============================================================
//===============================================================


