#include "perfect.h"
void find_perfect(string DNA, string id, int unitlen_p, int ssrlen_p, unsigned int flanking_p, int up_p, int revercomple_p ){

	unsigned int DNA_len=DNA.length();

	if( up_p == 1 ){
		transform( DNA.begin(), DNA.end(), DNA.begin(), ::toupper );
	}

	for ( unsigned int start=0; start < DNA_len; ++start) {

		for (short ssr_len=1; ssr_len<=unitlen_p; ++ssr_len){

			short repeat=1;

			for (;;++repeat){
                                short match=0;
                                for (short base=0;base<ssr_len; ++base){
					char left(DNA[start+base]);
					char right(DNA[start+ssr_len*repeat+base]);
					if( left != right ){
						match=1;
						break;
					}
					else if( left == 'N' ){
						match=1;
						break;
					}
                                }
                                if( match == 1 ){
                                        break;
                                }
                        }

			// output format: Chr/Scaffold	Start	End	SSR	Repeat	SSR_region SSR_region_flanking
			if( ssr_len*repeat >= ssrlen_p && repeat > 1){
				std::ios::sync_with_stdio(false);
				std::cin.tie(0);
                                string ssr_region;
                                string ssr;
				string ssr_region_flanking;
				string ssr_region_flanking_right;
				string ssr_region_flanking_left;
                                ssr_region=DNA.substr(start,repeat*ssr_len);
                                ssr=DNA.substr(start,ssr_len);
				if( revercomple_p > 0 ){
					string ssr_recom;
					ssr_recom=ssr;
					reverse_complement(ssr_recom);
					ssr+="\t";
					ssr+=ssr_recom;
				}
				if( flanking_p == 0 ){
					cout << id << "\t" << start+1 << "\t" << start+ssr_len*repeat << "\t" << ssr << "\t" << repeat << "\t" << repeat*ssr_len << "\t" << ssr_region << "\n";
				}
				else{
					if( (start >  flanking_p)  &&  (start + ssr_len*repeat + flanking_p) < DNA_len ){
						ssr_region_flanking_left = DNA.substr ( start - flanking_p , flanking_p );
						ssr_region_flanking_right = DNA.substr ( start+ssr_len*repeat , flanking_p );
						ssr_region_flanking = DNA.substr( start - flanking_p , ssr_len*repeat+2*flanking_p );
						cout << id << "\t" << start+1 << "\t" << start+ssr_len*repeat << "\t" << ssr << "\t" << repeat << "\t" << repeat*ssr_len << "\t" << ssr_region << "\t" << ssr_region_flanking << "\t" << ssr_region_flanking_left << "\t" << ssr_region_flanking_right << "\n";
					}
					else if( start <= flanking_p  && start + ssr_len*repeat + flanking_p < DNA_len ){
						ssr_region_flanking_left = DNA.substr ( 0 , start );
                                                ssr_region_flanking_right = DNA.substr ( start+ssr_len*repeat , flanking_p );
						ssr_region_flanking=DNA.substr( 0, start+ssr_len*repeat + flanking_p);
						cout << id << "\t" << start+1 << "\t" << start+ssr_len*repeat << "\t" << ssr << "\t" << repeat << "\t" << repeat*ssr_len << "\t" << ssr_region << "\t" << ssr_region_flanking <<"\t" << ssr_region_flanking_left << "\t" << ssr_region_flanking_right << "\n";
					}
					else if( start > flanking_p  && start + ssr_len*repeat + flanking_p >= DNA_len ){
						ssr_region_flanking=DNA.substr( start - flanking_p, DNA_len-(start-flanking_p) );
						ssr_region_flanking_left = DNA.substr ( start - flanking_p  , flanking_p );
                                                ssr_region_flanking_right = DNA.substr ( start+ssr_len*repeat , DNA_len - start+ ssr_len*repeat );
						cout << id << "\t" << start+1 << "\t" << start+ssr_len*repeat << "\t" << ssr << "\t" << repeat << "\t" << repeat*ssr_len << "\t" << ssr_region << "\t" << ssr_region_flanking << "\t" << ssr_region_flanking_left << "\t" << ssr_region_flanking_right << "\n";
					}
					else if( start <= flanking_p  &&  start + ssr_len*repeat + flanking_p >= DNA_len ){
						ssr_region_flanking_left = DNA.substr ( 0 , start );
						ssr_region_flanking_right = DNA.substr ( start+ssr_len*repeat , DNA_len - start+ ssr_len*repeat );
						cout << id << "\t" << start+1 << "\t" << start+ssr_len*repeat << "\t" << ssr << "\t" << repeat << "\t" << repeat*ssr_len << "\t" << ssr_region << "\t" << DNA << "\t" << ssr_region_flanking_left << "\t" << ssr_region_flanking_right << "\n";
					}

				}
				start+=ssr_len*repeat-1;
                                break;
                        }
                }
        }
}