#pragma once

#include "..\include\model.h"
#include "..\include\solution.h"
#include "..\include\pareto.h"
#include "..\include\parameters.h"

class solution; 
class Pareto; 

class RPCentroid : public  Model, public instance
	{
	public: 
	solution* _solution; 


	unsigned column;  
	void update_column(unsigned nb_columns); 
	unsigned row;  
	void update_row(unsigned nb_rows); 
	void init_row_column(); 

	//int _left[MAXIMUM_ROWS][MAXIMUM_COLUMNS]; //int **_left;
	vector<vector<int>> _left; 
	void set_left_value(unsigned nb_rows, unsigned nb_columns, int value);
	void set_left_vector(unsigned nb_rows, unsigned nb_columns, vector<unsigned>& vector, int coefficient);
	void set_left_diagonal(unsigned nb_rows_columns, int coefficient);
	void set_left_transpose(unsigned nb_rows, unsigned nb_columns, vector<vector<unsigned>>& matrix, int coefficient); 

	//int _right[MAXIMUM_ROWS]; //int *_right; 
	vector<int> _right; 
	void set_right_scalar(unsigned nb_rows, vector<unsigned>& vector1, vector<unsigned>& vector2); 
	void set_right_value(unsigned nb_rows, int value);
	void set_right_vector(unsigned nb_rows, int value, int coefficient, vector<unsigned>& vector1, vector<unsigned>*vector2=NULL);
	void set_right_pareto( vector<unsigned>& vector, Pareto &pareto); 

	unsigned rows_size, colums_size; 
	void constraints_allocate(unsigned _rows_size, unsigned _colums_size); 
	void constraints_free(unsigned _rows_size, unsigned _colums_size); 

	void display(char*string); 

	~RPCentroid(); 
	RPCentroid(); 
	}; 


class S1 : public  RPCentroid
	{
	public : 
	void set_sizes(); 
	void set_objectives(); 
	void set_variables(); 
	void set_constraints(); 
	S1(solution& s); 
	~S1(); 
	}; 


class S2 : public  RPCentroid
	{
	public: 
	Pareto* pareto; 

	void set_sizes(); 
	void set_objectives(); 
	void set_variables(); 
	void set_constraints(); 
	void S2_construire(solution& s,Pareto& _pareto) ;
	S2(); 
	S2(solution& s, Pareto& pareto); 
	~S2(); 
	}; 

class AP : public  S2
	{
	public : 
	void set_objectives(); 
	void set_constraints(); 
	AP(solution& s, Pareto& pareto); 
	~AP(); 
	}; 

class SEP : public  RPCentroid
	{
	public : 
	void set_sizes(); 
	void set_objectives(); 
	void set_variables(); 
	void set_constraints(solution& s); 
	~SEP(); 
	SEP(solution& s); 
	}; 


