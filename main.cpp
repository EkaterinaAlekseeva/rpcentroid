#include <C:\Users\Ekaterina\Desktop\Ekaterina\cplex_test\cplex_test1\include\model.h>
#include <C:\Users\Ekaterina\Desktop\Ekaterina\cplex_test\cplex_test1\include\instance.h>
#include <C:\Users\Ekaterina\Desktop\Ekaterina\cplex_test\cplex_test1\include\solution.h>
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <fstream>

ILOSTLBEGIN
using namespace std;

int main()
	{
		/*
			Model m(1,1,0,3); 
	
			m.variables_initialize_infinity(); 
	
			int coeffcient[]={0,0}; 
			m.objective_initialize(coeffcient); 
	
			int left[MAX_CONSTRAINTS][MAX_VARIABLES];
		
			left[0][0]=-2;left[0][1]=-1; 
			left[1][0]=-1;left[1][1]=-4; //-4=>solution, -2=> no solution  
			left[2][0]=1;left[2][1]=2; 

			int right[]={-14,-33,20}; 

			m.constraints_initialize(left,right); 
			if(m.solve("test","min")) 
				{
				cout<<m.cplex->getStatus()<<endl; 
				cout<<"objective="<<m.objective_get()<<endl; 
				float solutions[2]; 
				m.solutions_get(solutions); 
				cout<<"x1="<<solutions[0]<<endl; 
				cout<<"x2="<<solutions[1]<<endl;
				}
			cin.get(); 

		//model
		solution _solution; 
		short _number_variables1; 
		short _number_variables2;  
		short _number_constraint1;  
		short _number_constraint2; 
		int _coeffcient[MAX_VARIABLES]; 

//================  model 1: S1(leader solution, one efficient follower solution) ======================================

		// LHS of equality constraints
		//contraint : sum_i (y_i)=r)
		for(int j=0;j<instance::nb_facilities;j++) _left[0][j]=1; 
		for(int j=instance::nb_facilities;j<_number_variables1;j++) _left[0][j]=0; 
		//contraint : -sum_j(w2_j*z_j=...)
		for(int j=0;j<instance::nb_facilities;j++) _left[1][j]=0; 
		for(int j=instance::nb_facilities;j<_number_variables1;j++) 
			_left[1][j]=-instance::profit_follower1[j-instance::nb_facilities]; 
		//contraint :
		for(int j=0;j<instance::nb_facilities;j++) _left[2][j]=0; 
		for(int j=instance::nb_facilities;j<_number_variables1;j++) 
			_left[2][j]=-instance::profit_follower2[j-instance::nb_facilities]; 
		// RHS of equality constraints
		_right[0]=instance::nb_open_followers; 
		int sum(0);
		for(int j=0;j<instance::nb_customers;j++) 
			sum+=instance::profit_follower1[j]*(_solution.customers[j]); 
		_right[1]=-sum; 
		sum=0;
		for(int j=0;j<instance::nb_customers;j++) 
			sum+=instance::profit_follower2[j]*(_solution.customers[j]); 
		_right[2]=-sum;

		//inequality constraints. contraints must be as "<="
		//  constraint sum_j(w1_jz_j) <= W -1  <==> failed upper level constraint
		// LHS 
		for(int j=0;j<instance::nb_facilities;j++) _left[3][j]=0; 
		for(int j=instance::nb_facilities;j<_number_variables1;j++) 
			_left[3][j]=instance::profit_leader[j-instance::nb_facilities]; 
		// RHS
			_right[3]=instance::leader_minimum_profit-1;

		// constraints: for each j  - (sum_{i \in I_j} (y_i) + z_j) <= -1  <==> lower level constraint to do customers choice
		// LHS 
		for (int j = 0; j < instance::nb_customers; j++)
		{		
			for(int i=0;i<instance::nb_facilities;i++) 
				_left[3+1+j][j]=-_solution.follower_potential_facilities[i][j];		 
			for(int i=instance::nb_facilities;i<_number_variables1;i++) 
				_left[3+1+j][i]=-(i==j); 
		}
		// RHS
		for (int j = 0; j < instance::nb_customers; j++)
			_right[3+1+j]=-1;
		
		// constraints: for each i  y_i <= 1-x_i  <==> lower level constraint to define where follower opens his facilities
		// LHS
		for (int i = 0; i < instance::nb_facilities; i++)
		{	for(int j=0;j<instance::nb_facilities;j++) 
				_left[3+instance::nb_customers+i+1][j]=(j==i);		  
		for(int j=instance::nb_facilities; j< instance::nb_customers; j++) 
				_left[3+instance::nb_customers+i+1][j]=0;		  
		}
		// RHS
		for (int i = 0; i < instance::nb_facilities; i++)
			_right[3+instance::nb_customers+i+1]=1-_solution.leader[i];	
		
		// constraints to guarante boolean type:   y_i <= 1 for each i
		// LHS
		for (int i = 0; i < instance::nb_facilities; i++)
		{
			for(int j=0;j<instance::nb_facilities;j++) 
		 		_left[3+instance::nb_customers+instance::nb_facilities+i+1][j]=(j==i);	
			for(int j=instance::nb_facilities;j<instance::nb_customers ;j++) 
		 		_left[3+instance::nb_customers+instance::nb_facilities+i+1][j]=0;	
		
			_right[3+instance::nb_customers+instance::nb_facilities+i+1]=1;	
		}

//========= model 2 : S2(leader solution, current vector<solution*> front of follower solutions ) ======================================
		_number_variables1=instance::nb_facilities+instance::nb_customers+Pareto_size; //integer variables
		_number_variables2=0;   // real variables
		_number_constraint1=3;   // equality constraints
		_number_constraint2=instance::nb_facilities+instance::nb_customers+1; //inequality variables
		//... 
		for(int i=0;i<MAX_VARIABLES;i++) 
			_coeffcient[i]=0; 


//===============================================================

		//model 3 */
		return 0 ;
	}
	




/*
static void
populatebyrow (IloModel model, IloNumVarArray var, IloRangeArray con );
int main (void) {
IloEnv env ;
try {
	IloModel model(env) ;
	IloNumVarArray var ( env ) ;
	IloRangeArray con ( env ) ;
	populatebyrow ( model , var , con ) ;
	IloCplex cplex (model);
	cplex.solve();
env.out( ) << "Solution status= " << cplex.getStatus() << endl;
env.out() << "Solution value= " << cplex.getObjValue() << endl;
IloNumArray vals(env);
cplex.getValues (vals, var);
env.out() << "Values =" << vals << endl;
cplex.getSlacks (vals, con);
env.out( ) << "Slacks = " << vals << endl;
cplex.exportModel ("ipex1.lp");
cin.get();

}

catch(IloException& e)
{ cerr << "Concert exception caught : " << e << endl; cin.get();
}
catch (...) {
cerr << "Unknown exception caught " << endl; cin.get();
}
env.end ( );
return 0 ;
} // END main

static void populatebyrow ( IloModel model , IloNumVarArray x , IloRangeArray c ) {
IloEnv env = model.getEnv ( ) ;
x.add (IloNumVar(env, 0.0 , IloInfinity, ILOINT));
x.add (IloNumVar(env, 0.0 , IloInfinity, ILOINT));
model.add (IloMaximize(env, 1.00*x[0] + 0.64*x[1]));
//c.add(50*x[0] + 31*x[1]<=250);
c.add(3*x[0] - 2*x[1]<=-250);
c.add(3*x[0]-2*x[1]>=-4);
model.add(c) ;
} // END p o p ul a t e by r ow
*/