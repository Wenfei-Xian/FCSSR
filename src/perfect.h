#include <string>
#include "sequence.format.h"
#include <cstdio>
#include <algorithm>
#include <unordered_map>
#include <nmmintrin.h>
using namespace std;

double vector_dot(uint32_t len,const float* a,const float* b);

void find_perfect(string DNA, string id, int unitlen_p, int ssrlen_p, unsigned int flanking_p, int up_p, int revercomple_p, int mincopy_p);
