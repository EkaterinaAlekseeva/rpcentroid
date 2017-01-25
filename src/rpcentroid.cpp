#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "..\include\rpcentroid.h"
#include "..\include\instance.h"

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

//============================================================================================
void RPCentroid::update_column(unsigned nb_columns)
	{
	column+=nb_columns; 
//	cout<<"column "<<column<<endl<<flush; 
	}

void RPCentroid::update_row(unsigned nb_rows)
	{
	row+=nb_rows; 
	column=0; 
	}

void RPCentroid::init_row_column()
	{
	row=column=0; 
	}

RPCentroid::~RPCentroid()
	{
	//free left 
	/*
	for(int i = 0; i < rows_size; i++)
		{
		for(int j = 0; j < colums_size; j++) 
			free(_left);
		//free right 
		free(_right); 
		}
	*/
	}

void RPCentroid::constraints_allocate(unsigned _rows_size, unsigned _colums_size)
	{
	rows_size=_rows_size; 
	colums_size=_colums_size;
	/*
	//malloc left 
	_left = (int **)malloc(_rows_size * sizeof(int*));
	for(int i = 0; i < rows_size; i++) 
		_left[i] = (int *)malloc(colums_size* sizeof(int));
	
	for(int i = 0; i < rows_size; i++) 
		for(int j = 0; j < colums_size; j++) 
			_left[i][j]=0;
	//malloc right 
	_right = (int *)malloc(rows_size* sizeof(int));
	for(int i = 0; i < rows_size; i++) 
			_right[i]=0;
	*/
	}

//============================================================================================
void RPCentroid::set_left_value(unsigned nb_rows, unsigned nb_columns, int value)
	{
	//	cout << "set_left_value \n" ; 
	for(unsigned r=0; r<nb_rows;r++)
		for(unsigned c=0; c<nb_columns;c++)
		{
			_left[row+r][column+c]=value; 
		//	cout << " row = " << row+r << " column = " << column+c << " \n" ;
		}
	update_column(nb_columns); 
	}

void RPCentroid::set_left_vector(unsigned nb_rows, unsigned nb_columns, vector<unsigned>& vector, int coefficient)
	{
	for(unsigned r=row; r<row+nb_rows;r++)
		for(unsigned c=column; c<column+nb_columns;c++)
			_left[r][c]=coefficient*vector[c-column]; 
	update_column(nb_columns); 
	}

void RPCentroid::set_left_diagonal(unsigned nb_rows_columns, int coefficient)
	{
	//	cout << "set_left_diagonal " << "\n"; 
		//this->rows_size; this->colums_size; this->_left; 
	for(unsigned r=0; r<nb_rows_columns;r++)
		for(unsigned c=0; c<nb_rows_columns;c++)
		{
			_left[row+r][column+c]=coefficient*(r==c); 
		//	cout << "row = " << row+r << "column = " << column+c << "\n"; 
		}
	update_column(nb_rows_columns); 
	}	


void RPCentroid::set_left_transpose(unsigned nb_rows, unsigned nb_columns, vector<vector<unsigned>>& matrix, int coefficient)
	{
	for(unsigned r=0; r<nb_columns;r++)
		{
		for(unsigned c=0; c<nb_rows;c++)
			{
			_left[row+r][column+c]=coefficient*(matrix[c][r]); 
			// cout << "left[" << row+r << "," << column+c << "]=" << matrix[c][r] << " ";
			}
		// cout << "\n";
		}
	update_column(nb_columns);  
	}	


RPCentroid::RPCentroid()
	{
	//int _left[MAXIMUM_ROWS][MAXIMUM_COLUMNS]; //int **_left;
	_left.resize(MAXIMUM_ROWS); 
	for(unsigned i=0; i<_left.size();i++)
		_left[i].resize(MAXIMUM_COLUMNS); 

	//int _right[MAXIMUM_ROWS]; //int *_right; 
	_right.resize(MAXIMUM_ROWS); 
	}


//============================================================================================
void RPCentroid::set_right_scalar(unsigned nb_rows, vector<unsigned>& vector1, vector<unsigned>& vector2)
	{
	_right[row]=0; 
	for(unsigned r=0; r<nb_rows;r++)
		_right[row]+=vector1[r]*vector2[r]; 
	update_row(1); 
	}

void RPCentroid::set_right_value(unsigned nb_rows, int value)
	{
	//	cout << "set_right_value \n";
	for(unsigned r=0; r<nb_rows;r++)
	{
		_right[row+r]=value; 
		// cout << " row =" << row+r << "\n";
	}
	update_row(nb_rows); 
//	cout << " row Updated  =" << row << "\n";
	}

void RPCentroid::set_right_vector(unsigned nb_rows, int value, int coefficient, vector<unsigned>& vector1, vector<unsigned>* vector2)
	{
	for(unsigned r=0; r<nb_rows;r++)
		_right[row+r]=value+coefficient*vector1[r]*(vector2?vector2->at(r):1); 
	update_row(nb_rows); 
	}



void RPCentroid::set_right_pareto(vector<unsigned>& vector, Pareto &pareto)
	{
	for(unsigned r=0; r<pareto.size();r++)
		{
		_right[row+r]=big_number-1;
		for(unsigned c=0; c<nb_customers;c++)
			_right[row+r]+=vector[c]*pareto[r]->customers[c]; 
		}
	update_row(pareto.size()); 
	}


void RPCentroid::display(char*string)
	{
	cout<<"=========================="<<endl; 
	cout<<*string<<"===="<<endl; 
	cout<<"=========================="<<endl;
	cout<<"Left("<<nb_var_int<<","<<nb_var_real<<")"<<endl; 
	for(int i = 0; i < rows_size; i++)
		{
		for(int j = 0; j < colums_size; j++) 
			cout<<_left[i][j]<<" ";
		cout<<endl; 
		}
	cout<<"Right("<<nb_const_eq<<","<<nb_const_leq<<")"<<endl; 
	for(int i = 0; i < rows_size; i++)
			cout<<_right[i]<<" ";
	cout<<endl; 
	cout<<"=========================="<<endl; 
	}


//... 
//============================================================================================
void S1::set_sizes() 
	{
	nb_var_int=nb_facilities+nb_customers; //integer variables
	nb_var_real=0;   // real variables
	nb_const_eq=3;   // equality constraints
	nb_const_leq=2*(nb_facilities)+2*(nb_customers)+1; //inequality constraints including type of variables
	//Model::Model(nb_var_int,nb_var_real,nb_const_eq,nb_const_leq); 
	//nb_var=nb_var_int+nb_var_real; 
	//nb_const=nb_const_eq+nb_const_leq;  
	Model_construire(nb_var_int,nb_var_real,nb_const_eq,nb_const_leq); 
	constraints_allocate(nb_const_eq+nb_const_leq,nb_var_int+nb_var_real); 
	}

void S1::set_objectives() 
	{
	objective_initialize_zero(); 
	} 

void S1::set_variables() 
	{
	variables_initialize_bool(); 
	}
 
void S1::set_constraints() 
	{
	init_row_column(); 
	//equation 1
	set_left_value(1,nb_facilities,1);
	set_left_value(1,nb_customers,0);
	set_right_value(1,nb_open_followers); 
	//equation 2
	set_left_value(1,nb_facilities,0);
	set_left_vector(1,nb_customers,profit_follower1,-1);
	set_right_vector(1,0,-1,profit_follower1); 
	//equation 3
	set_left_value(1,nb_facilities,0);
	set_left_vector(1,nb_customers,profit_follower2,-1);
	set_right_vector(1,0,-1,profit_follower2); 
	//equation 4
	set_left_value(1,nb_facilities,0);
	set_left_vector(1,nb_customers,profit_leader,+1);
	set_right_value(1,leader_minimum_profit-1); 
	//other equation nb_customers equations 
	set_left_transpose(nb_customers,nb_facilities,_solution->follower_potential_facilities,-1);
	set_left_diagonal(nb_customers,-1); 
	set_right_value(nb_customers,-1); 
	//other equation nb_facilites equations 
	set_left_diagonal(nb_facilities,1);
	set_left_value(nb_facilities,nb_customers,0);
	set_right_vector(nb_facilities,1,-1,_solution->leader); 
	constraints_initialize(_left,_right); 
	}

S1::S1(solution& s)
	{
	_solution=&s; 
	set_sizes(); 
	set_variables(); 
	set_objectives(); 
	set_constraints(); 
	}

S1::~S1() 
	{
	//RPCentroid::~RPCentroid();
	}

//============================================================================================
void S2::set_sizes() 
	{
	nb_var_int=nb_facilities+nb_customers+2*pareto->size(); //integer variables
	nb_var_real=0;   // real variables
	nb_const_eq=1;   // equality constraints
	nb_const_leq=1+nb_facilities+nb_customers+3*pareto->size(); //inequality constraints including type of variables
	//Model(nb_var_int,nb_var_real,nb_const_eq,nb_const_leq);
	//nb_var=nb_var_int+nb_var_real; 
	//nb_const=nb_const_eq+nb_const_leq; 
	Model_construire(nb_var_int,nb_var_real,nb_const_eq,nb_const_leq); 
	constraints_allocate(nb_const_eq+nb_const_leq,nb_var_int+nb_var_real); 
	}
 
void S2::set_objectives() 
	{
	objective_initialize_zero(); 
	}
 
void S2::set_variables() 
	{
	variables_initialize_bool(); 
	}
 
void S2::set_constraints() 
	{
	init_row_column(); 
	//equation 1
	set_left_value(1,nb_facilities,1);
	set_left_value(1,nb_customers,0);
	set_left_value(1,pareto->size(),0);
	set_left_value(1,pareto->size(),0);
	set_right_value(1,nb_open_followers); 
	//equation 2
	set_left_value(1,nb_facilities,0);
	set_left_vector(1,nb_customers,profit_leader,1);
	set_left_value(1,pareto->size(),0);
	set_left_value(1,pareto->size(),0);
	set_right_value(1,leader_minimum_profit-1); 
	//other nb_customers equations
/*	cout << "\n "; 
	cout << "_solution->follower_potential_facilities" << "\n" ;
	for(int c=0;c<nb_customers;c++)
	{
		for (unsigned i=0;i<nb_facilities; i++) 
			cout <<_solution->follower_potential_facilities[i][c] << " "; 
	cout << "\n "; 
	}*/
	#ifdef ALGORITHM 
	cout << "_solution->leader in S2: " << "\n" ;
	for (unsigned i=0;i<nb_facilities; i++) 
		{	
			if(_solution->leader[i]==1)
				cout << i << " "; 
		}
	cout << "\n ";
	#endif
	/*
	cout << "_solution->follower : " << "\n" ;
		for (unsigned i=0;i<nb_facilities; i++) 
			cout <<_solution->follower[i] << " "; 
	cout << "\n "; 
	*/
	set_left_transpose(nb_customers,nb_facilities,_solution->follower_potential_facilities,-1);
	set_left_diagonal(nb_customers,-1); 
	set_left_value(nb_customers,pareto->size(),0);
	set_left_value(nb_customers,pareto->size(),0);
	set_right_value(nb_customers,-1); 
	//other nb_facilitiess equations
	set_left_diagonal(nb_facilities,1);
	set_left_value(nb_facilities,nb_customers,0);
	set_left_value(nb_facilities,pareto->size(),0);
	set_left_value(nb_facilities,pareto->size(),0);	
	set_right_vector(nb_facilities,1,-1,_solution->leader); 
	//other first third pareto equations
	set_left_value(pareto->size(),nb_facilities,0);
	set_left_vector(pareto->size(),nb_customers,profit_follower1,1);
	set_left_diagonal(pareto->size(),big_number); 
	set_left_value(pareto->size(),pareto->size(),0);
	set_right_pareto(profit_follower1,*pareto); 
	//other second third pareto equations
	set_left_value(pareto->size(),nb_facilities,0);
	set_left_vector(pareto->size(),nb_customers,profit_follower2,1);
	set_left_value(pareto->size(),pareto->size(),0);
	set_left_diagonal(pareto->size(),big_number); 
	set_right_pareto(profit_follower2, *pareto); 
	//other third third pareto equations
	set_left_value(pareto->size(),nb_facilities,0);
	set_left_value(pareto->size(),nb_customers,0);
	set_left_diagonal(pareto->size(),-1); 
	set_left_diagonal(pareto->size(),-1); 
	set_right_value(pareto->size(),-1); 
	constraints_initialize(_left,_right); 
}

void S2::S2_construire(solution& s,Pareto& _pareto)
	{
	pareto=&_pareto; 
	_solution=&s; 
	set_sizes(); 
	set_variables();  
	set_objectives(); 
	set_constraints();  
	}

S2::S2() 
	{
	}

S2::S2(solution& s,Pareto& _pareto)
	{
	S2_construire(s,_pareto); 
	}

S2::~S2() 
	{
	//RPCentroid::~RPCentroid();
	}

//============================================================================================
 
void AP::set_objectives() 
	{
	unsigned i=0; 
	unsigned limit=0; 
	//int coefficients[MAX_VARIABLES]; 
	vector<int> coefficients; coefficients.resize(MAX_VARIABLES);  

	for(limit+=nb_facilities; i<limit; i++) 
		coefficients[i]=0; 
	for(limit+=nb_customers; i<limit; i++) 
		coefficients[i]=profit_leader[i-nb_facilities]; 
	for(limit+=2*pareto->size(); i<limit; i++) 
		coefficients[i]=0; 
	objective_initialize(coefficients); 
	}
 
 
void AP::set_constraints() 
	{
	solution*worst_feasible_solution=pareto->worst_efficient_leader(); 
	unsigned cost=(worst_feasible_solution)?worst_feasible_solution->leader_objective:UINT_MAX; 
	//_right[1]=_solution->leader_objective-1; 
	_right[1]=cost-1; 
	for(unsigned i=0; i<nb_facilities; i++)
		_right[1]+=costs[i]*_solution->leader[i]; 
	constraints_initialize(_left,_right); 

	#ifdef ALGORITHM 
		cout << "_solution->leader in AP :" << endl;
		for (unsigned ii=0;ii<nb_facilities; ii++)	
			if (_solution->leader[ii]==1) cout << ii << " " ;
		cout << endl;
	#endif
	}

AP::AP(solution& s,Pareto& pareto)
	{
	S2_construire(s,pareto); 
	delete ilomodel; 
	AP::set_objectives(); 
	AP::set_constraints(); 
	}

AP::~AP()
	{
	//RPCentroid::~RPCentroid();
	}

//============================================================================================
 
void SEP::set_sizes()
	{
	nb_var_int=nb_facilities+nb_customers; //integer variables
	nb_var_real=2;   // real variables
	nb_const_eq=3;   // equality constraints
	nb_const_leq=nb_facilities+nb_customers; //inequality constraints including type of variables
	//Model(nb_var_int,nb_var_real,nb_const_eq,nb_const_leq); 
	//nb_var=nb_var_int+nb_var_real; 
	//nb_const=nb_const_eq+nb_const_leq;  
	#ifdef	ALGORITHM
	//cout << "nb_var_int = " << nb_var_int << "; nb_var_real = " << nb_var_real 
		//<< "; nb_const_eq = " << nb_const_eq << "; nb_const_leq = " << nb_const_leq << endl;
	#endif 
	Model_construire(nb_var_int,nb_var_real,nb_const_eq,nb_const_leq); 
	constraints_allocate(nb_const_eq+nb_const_leq,nb_var_int+nb_var_real); 
	}

void SEP::set_objectives() 
	{
	//int coefficients[MAX_VARIABLES]; 
	vector<int> coefficients; coefficients.resize(MAX_VARIABLES);  

	for(short i=0; i<nb_var-2; i++) coefficients[i]=0; 
	coefficients[nb_var-1]=1; 
	coefficients[nb_var-2]=1;
	objective_initialize(coefficients); 
	}

void SEP::set_variables()
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
	right[nb_var-1]=INT_MAX; 
	right[nb_var-2]=INT_MAX;
	variables_initialize(left,right); 
	}

void SEP::set_constraints(solution& s) 
	{
	init_row_column(); 
	//equation 1
	set_left_value(1,nb_facilities,1);
	set_left_value(1,nb_customers,0);
	set_left_value(1,1,0);
	set_left_value(1,1,0);
	set_right_value(1,nb_open_followers); 
	//equation 2
	set_left_value(1,nb_facilities,0);
	set_left_vector(1,nb_customers,profit_follower1,1);
	set_left_value(1,1,1);
	set_left_value(1,1,0);
	set_right_scalar(nb_customers,profit_follower1,s.customers); 
	//equation 2
	set_left_value(1,nb_facilities,0);
	set_left_vector(1,nb_customers,profit_follower2,1);
	set_left_value(1,1,0);
	set_left_value(1,1,1);
	set_right_scalar(nb_customers,profit_follower2,s.customers); 
	//other equation nb_customers equations 
	s.set_follower_potential_facilities(); 
	/*
	cout << "\n "; 
	cout << "s.follower_potential_facilities" << "\n" ;
	for(int c=0;c<nb_customers;c++)
	{
		for (unsigned i=0;i<nb_facilities; i++) 
			cout <<s.follower_potential_facilities[i][c] << " "; 
	cout << "\n "; 
	}
	*/
	#ifdef ALGORITHM 
	cout << "_solution->leader in SEP : ";
		for (unsigned i=0;i<nb_facilities; i++) 
		{
			if(s.leader[i]==1)
			cout << i << " ";
		} 
	cout << "\n ";
	cout << "_solution->follower in SEP : " ;
		for (unsigned i=0;i<nb_facilities; i++) 
		{ if(s.follower[i]==1)
			cout << i << " "; 
		}
	cout << "\n "; 
	#endif
	set_left_transpose(nb_customers,nb_facilities,s.follower_potential_facilities,-1);  
	set_left_diagonal(nb_customers,-1); 
	set_left_value(nb_customers,2,0);
	set_right_value(nb_customers,-1); 
	//other equation nb_facilites equations 
	set_left_diagonal(nb_facilities,1);
	set_left_value(nb_facilities,nb_customers,0);
	set_left_value(nb_facilities,2,0);
	set_right_vector(nb_facilities,1,-1,s.leader); 
	constraints_initialize(_left,_right); 
	}

SEP::SEP(solution& s) 
	{
	set_sizes(); 
	set_variables(); 
	set_objectives(); 
	set_constraints(s); 
//	display("step 2"); 
	}

	
SEP::~SEP() 
	{
	//RPCentroid::~RPCentroid();
	}
