#pragma once
#include <iostream>
#include <cstdlib> 

#ifdef NDEBUG 
//#define Log(s) {}
#define Log(s) fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, (s))
#define Error(s) {}
#else
#define Log(s) fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, (s))
#define Error(s) {fprintf(stderr, "Error: ");\
				Log(s);\
				exit(-1);}
#endif