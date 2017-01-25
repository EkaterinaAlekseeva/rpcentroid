#pragma once
#include <vector>
#include <ilcplex/ilocplex.h>

template <typename T1>
void InitializationVector(unsigned N, std::vector<T1>& myVector)
{ for (int i = 0; i < N; i++)
{  T1 Value=0;
	myVector.push_back(Value);
}
};

template <typename T1>
void InitializationMatrix2D(unsigned N, unsigned M, std::vector<std::vector<T1>>& Matrix2D)
{ for (unsigned i = 0; i < N; i++)
{  std::vector<T1> myVector ;
for (unsigned j = 0; j < M; j++)
{  T1 Value=0;
	myVector.push_back(Value);
}
Matrix2D.push_back(myVector); 
}
};

template <typename T1>
void InitializationMatrix3D(unsigned N, unsigned M, unsigned K, std::vector<std::vector<std::vector<T1>>>& Matrix3D)
{ for (int i = 0; i < N; i++)
{  std::vector<std::vector<T1>> myMatrix;
 for (int j = 0; j < M; j++)
 {  std::vector<T1> myVector;
		for (int k =0; k < K; k++)
		{ T1 Value=0;
		myVector.push_back(Value); }
		myMatrix.push_back(myVector);
}
Matrix3D.push_back(myMatrix); 
}
};
/******************* structures for the variables for CPLEX *****************************************************Var*/
/* array of continuous variables */
template <typename T1>
void InitializationIloVarVector(IloArray<T1>& myVectorVar, IloEnv env, IloInt N, double BoundLow, double BoundUp)
{ for (int i = 0; i < N; i++)
{  myVectorVar[i]=T1(env, BoundLow, BoundUp); }
};

template <typename T1>
void InitializationIloVarMatrix2D(IloArray<IloArray<T1>>& myMatrix2D, IloEnv env, IloInt N, IloInt M,  double BoundLow, double BoundUp)
{ for(int i=0; i<N; i++)
{myMatrix2D[i]=IloArray<T1>(env, M);
    for (int j = 0; j < M; j++)
	{myMatrix2D[i][j]=T1(env,BoundLow, BoundUp);}
   }
};