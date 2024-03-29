#include <string>
#include "sequence.format.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

//void find_fuzzy(string DNA, string id, int unitlen_p, unsigned int ssrlen_p, int insertion_p, float unmatch_ssr_p, int both_p, int up_p, unsigned int flanking_p, int unmatch_num_p, int revercomple_p, int mincopy_p );
void find_fuzzy(string DNA, string id, int unitlen_p, int insertion_p, float unmatch_ssr_p, int both_p, int up_p, unsigned int flanking_p, int unmatch_num_p, int revercomple_p, string cutoffunit_p );
//void find_fuzzy(string DNA, string id, int unitlen_p, int insertion_p, float unmatch_ssr_p, int up_p, unsigned int flanking_p, int unmatch_num_p, int revercomple_p, string cutoffunit_p ); // remove both_p
