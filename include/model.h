#pragma once

#undef free 

#include <ilcplex/ilocplex.h>
#include <vector>
#include <string>
#include <memory>
#include "..\include\parameters.h"

using namespace std;

//===============================================================
//===============================================================
//===============================================================


class Model 
	{
	public:
	// dimensions 
	short nb_var_int; //int variables  
	short nb_var_real;  //float variables 
	short nb_var; //int variables  
	short nb_const_eq; //equality  
	short nb_const_leq; //inferiority 
	short nb_const; //inferiority 

	//int efficiency;  

	// Ilog variables 
	IloEnv env;
	//IloNumVar variables[MAX_VARIABLES];  
	vector<IloNumVar> variables;  
	IloExpr *objective; 
	IloModel *ilomodel; 
	IloCplex *cplex;

	//Methods 
	////// Attention to the sequence of VARIABLES and corresponded COEFFICIENTS: 
	////// 1. All integer variables; 2 All real variables  
	void Model_construire(short _number_variables1,short _number_variables2, short _number_constraint1, short _number_constraint2); 
	Model(short _number_variables1, short _number_variables2, short _number_constraint1, short _number_constraint2);
	Model(); 

	void variables_initialize(const vector<int>& left,const vector<int>& right); 
	void variables_initialize_infinity(); 
	void variables_initialize_bool(); 

	void objective_initialize(vector<int>& coefficients); 
	void objective_initialize_zero(); 

	void constraints_initialize(vector<vector<int>>& left, vector<int>& right); 

	unsigned solve(string name, string min_max); 
	float objective_get(); 
	float solutions_get(int index); 
	void solutions_get(float*solutions); 
	~Model();
	}; 


//===============================================================
//===============================================================
//===============================================================

