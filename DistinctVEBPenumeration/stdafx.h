// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#include "mpi.h"

using namespace std;
#define M 4 //#columns
#define N 4  //#rows

struct VEBP_type {
	int firstNumb;
	int secondNumb;
	int thirdNumb;
	char myFile[20];
};




// TODO: reference additional headers your program requires here
