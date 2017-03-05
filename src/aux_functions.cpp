#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include "..\include\aux_functions.h"
#include <time.h>
#include <string.h>
#include <iostream>
using namespace std; 

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
std::string AuxFunctions::currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
       strftime(buf, sizeof(buf), "%Y-%m-%d ; %X", &tstruct);
return buf;
}
