
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\include\tabu_search.h"
#include "..\include\parameters.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <time.h>
using namespace std;

extern int compteur, compteur2, compteur_total, compteur_total2, model_S2_has_solution, model_SEP_has_solution, model_AP_has_solution,
	nb_model_SEP, flagIsParetoFrontCheckedForEfficiency; 

int flagIsParetoFrontCheckedForEfficiency=0;
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

int main()
	{
		//working version
	#ifdef RANDOM 
	//srand(10);
	for(int i = 0;   i < 10;i++ )
      printf( "  %6d\n", rand() );
	std::cout<<flush; 
	int i; cin>>i; 
	return i; 
	#endif

	// read data from file
	
	FILE *stream ;
	/*
	ofstream myfile;
	myfile.open("DebugPrint.txt");
    myfile << "Coucou" ;
	myfile.close();
	*/
	
	//if((stream = freopen("DebugPrint.txt", "w", stderr)) == NULL) exit(-1);
	if((stream = freopen("DebugPrint.txt", "w", stdout)) == NULL) exit(-1);

	unsigned _facilities_number=FACILITIES_NUMBER,_customers_number=CUSTOMERS_NUMBER,_leaders_number=LEADERS_NUMBER, _followers_number=FOLLOWERS_NUMBER; 

	#ifdef ALGORITHM 
	cout << "Instance: \n" ;
	cout	<< "KOLI = " << _facilities_number << "; KOLJ = " <<_customers_number << "; p_leader =" 
	<< _leaders_number << "; r_follower = " << _followers_number << "\n";
	#endif 

	instance::intialize(_facilities_number,_customers_number,_leaders_number,_followers_number); 
	//instance::readInitialDataFromFile(_facilities_number,_customers_number,_leaders_number,_followers_number); 
	//debug print

	#ifdef ALGORITHM 

	cout << "Matrix D " << "\n "; 
	for (unsigned i=0;i<_facilities_number; i++) 
	{
		for (unsigned j=0;j<_customers_number; j++) 		
			cout <<instance::distances[i][j] << " "; 
	cout << "\n "; 
	}

	cout << "leader_minimum_profit = " << instance::leader_minimum_profit << "\n "; 
	cout << "leader_profit :" ; 
	 for (unsigned j=0;j<_customers_number; j++) 		
			cout <<instance::profit_leader[j] << " "; 
	 cout << "\n "; 

	 cout << "leader costs = " << " ";
	 for (unsigned i=0;i<_facilities_number; i++) 
		cout << instance:: costs[i] << " " ; 
	 cout << "\n "; 

	 cout << "profit_follower1 :" ; 
	 for (unsigned j=0;j<_customers_number; j++) 		
			cout <<instance::profit_follower1[j] << " "; 
	 cout << "\n "; 
	 cout << "profit_follower2 :" ; 
	 for (unsigned j=0;j<_customers_number; j++) 		
			cout <<instance::profit_follower2[j] << " "; 	 
	#endif 
	//end of debug print

	 unsigned tmp=0; 
	for (unsigned i=0; i<instance::nb_customers; i++)
		tmp+=instance::profit_follower1[i]+instance::profit_follower2[i]; 
	instance::set_big_number(tmp); 

	#ifdef ALGORITHM 
	cout << "\n "; 
	cout << "big_number =" << instance::big_number << "\n "; 
	#endif 

	clock_t tStart = clock(); 

	TabuSearch ts; 
	MultiGenetic::generations=NB_GeneticAlgo_Generations; 
	solution* s=ts.compute(100,8); //Algorithm parameters: ts.compute(nb_iterations, list_tabu_size)

	#ifdef MEMORY
	std::cout<<"#Solution objects total "<<compteur_total<<endl; 
	std::cout<<"#Solution objects "<<compteur<<endl; 
	std::cout<<"#Model objects total "<<compteur_total2<<endl; 
	std::cout<<"#Model objects "<<compteur2<<endl; 
	#endif

	cout << "model_S2_has_solution = " << model_S2_has_solution << endl;
	cout << "model_SEP_has_solution = " << model_SEP_has_solution << endl;
	cout << "model_AP_has_solution =" << model_AP_has_solution << endl;

	cout<< "rand = " << rand() <<endl; 

	
	cout << " TIME taken (in sec) : " << (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
	
	_CrtDumpMemoryLeaks();
	int retour; 
	cin>> retour; return retour; 
	
	}
/*


class C 
{
public: 
int i; 
C(){i=333;};
};



class A 
{
public: 
int i; 
	A(int _i){i=_i;};
};

class B 
{
public: 
C c; 
A a; 
	B(): a(c.i){cout<<"B"<<endl;};
};


int main()
{
B b; 
cout<<b.a.i<<endl; 
int i; cin>>i; cout<<i; 
}
*/