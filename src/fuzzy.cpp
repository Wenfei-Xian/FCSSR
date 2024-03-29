#include "fuzzy.h"

//void find_fuzzy(string DNA, string id, int unitlen_p, unsigned int ssrlen_p, int insertion_p, float unmatch_ssr_p, int both_p, int up_p, unsigned int flanking_p, int unmatch_num_p, int revercomple_p, int mincopy_p ){
void find_fuzzy(string DNA, string id, int unitlen_p, int insertion_p, float unmatch_ssr_p, int both_p, int up_p, unsigned int flanking_p, int unmatch_num_p, int revercomple_p, string cutoffunit_p ){
//void find_fuzzy(string DNA, string id, int unitlen_p, int insertion_p, float unmatch_ssr_p, int up_p, unsigned int flanking_p, int unmatch_num_p, int revercomple_p, string cutoffunit_p ){ //remove both_p
	
	if( up_p == 1){ // up_p : whether transfer all the character to upper, default: 0
		transform( DNA.begin(), DNA.end(), DNA.begin(), ::toupper);
	}

	int cutoff_array_main[unitlen_p]={0};
	istringstream iss(cutoffunit_p);
	string token;
	char split=',';
	int pos=0;
	while( getline(iss, token, split) ){
		int cutoff = stoi(token);
		cutoff_array_main[pos]=cutoff;
		pos++;
	}

        unsigned int DNA_len=DNA.length();
	int insertion=insertion_p; // insertion_p : maximum length of insertion within unit

        for ( unsigned int start=0; start < (DNA_len-unitlen_p-6) ; start++){

                for (unsigned short ssr_len=1; ssr_len<= unitlen_p ; ssr_len++){ // unitlen_p : longest length of unit, default: 6

			/*
			int add_match=0;
                        if( ssr_len > 1){
                                char add_left;
                                char add_right;
                                //int add_match=0;
                                for ( short add=0; add < ssr_len+insertion_p; add++ ){
                                        add_left=(DNA[start + add]);
                                        add_right=(DNA[start + ssr_len + insertion_p + add]);
                                        if( add_left != add_right ){
                                                break;
                                        }
                                        else{
                                                add_match++;
                                        }
                                }

                                //if( add_match == ssr_len+insertion_p ){
                                //        continue;
                                //}
                        }
			if( add_match == ssr_len+insertion_p ){
				continue;
			}
			*/

			int sim=1;
			if( ssr_len > 1 ){
				char add_left(DNA[start]);
				for ( short j=1; j<ssr_len; ++j ){
					char add_right(DNA[start+j]);
					if( add_left == add_right ){
						sim++;
					}
				}
			}
			if( sim == ssr_len ){
				continue;
			}


			unsigned int ssrlen_p=cutoff_array_main[ssr_len-1]*ssr_len;
			unsigned short repeat=1;
			unsigned int start_new=start+ssr_len;

			string ssr_region; // used for output

			string left_unit=DNA.substr( start, ssr_len ); // used for output
			
			ssr_region+=left_unit; // used for output
			ssr_region+=" "; // used for output

			unsigned int delete_sumbase=0;
			unsigned int insert_sumbase=0;
			unsigned int mismatch_sumbase=0;

                        for ( ; ;++repeat ){

                                short match=0; // total matched base number between unit and next unit;
                                short rightstart=0;
                                int insertion_change=insertion; // maximum insertion length within next unit;
				int matchright[ssr_len+insertion_p]={0};
				int site[ssr_len]={0};

				//in nature, the frequency of mismatch is higher than insertion and deletion
				//So first, program will check, whether mismatch happen.
			
				for ( short base = 0; base < ssr_len; base++ ){
					char left(DNA[start+base]);

					if( left == 'N' ){
						break;
					}

					char right(DNA[start_new+rightstart]);
					
					//cout << start << "\t" << ssr_len << "\t" << left << "\t" << right << endl;

					if( left == right ){
						match++;
						matchright[match]=rightstart;
					}
					rightstart++;

				}
				//cout << start << "\t" << ssr_len << "\t" << match << endl;

				// if mismatch is not suitable in this case, insertion and deletion will be considered
				if( (ssr_len == 1 && match == 0 ) || (match < ssr_len - unmatch_num_p && ssr_len > 1) ){
					
					rightstart=0;
					match=0;
					insertion_change=insertion;
					
					for( short i=0; i < ssr_len+insertion_p; ++i){
						matchright[i]=0;
					}

                                	for ( short base = 0; base < ssr_len; base++ ){// compare unit and next unit
                                        
						char left(DNA[start+base]);
						if( left == 'N' ){
							break;
						}
                                       		char right;

                                        	for( short ins=0; ins <= insertion_change; ins++ ){ // for each base comparision, how many insertion quota left.
                                                	right = (DNA[start_new+rightstart+ins]);
						
							if( left != right ){
								continue;
							}
							else if( left == right ){
								site[base]=1;
                                                        	match++;
                                                        	matchright[match]=rightstart+ins;
                                                        	rightstart+=(ins+1);
								if( base == 0 ){
									insertion_change-=ins;
								}
								else if( site[base-1]==1 ){
                                                        		insertion_change-=ins;
								}
                                                        	break;
                                                	}
                                        	}
					
                                        	if( match < base-unmatch_num_p ){ // match = longest common sequence (LCS); if LCS shorter than base - maximum allowed, which mean loop was no meaningful, so stop it
							break;
                                        	}
					}
				}	
				
				if( ssr_len == 1 ){

					// two conditions will break the loop
					// 1) unit level
                                        if( match == 0 ){ // Finished, when ssr_len == 1
                                                break;
                                        }
					// 2) ssr region level
					else{ // when unit length = 1, unmatch will be considered as insertion. Eventhough insertion is the same as mismatch.
						insert_sumbase+=matchright[1];
						if( (start_new - start) < ssrlen_p && insert_sumbase > ssrlen_p*unmatch_ssr_p ){
							insert_sumbase-=matchright[1];
							break;
						}
						else if( (start_new - start ) >= ssrlen_p && insert_sumbase > (start_new - start) * unmatch_ssr_p ){
							insert_sumbase-=matchright[1];
							break;
						}
					}

                                        ssr_region+=DNA.substr(start_new,matchright[match]+1);
					ssr_region+=" ";
                                        start_new+=matchright[match]+1;

                                }
                                else{ // ssr_len > 1
					
					// two condition will break the loop 
					//1) unit level
					if( match < ssr_len-unmatch_num_p ){ // match number less than cutoff
						break;
					}
					else if( insertion_change < insertion_p && match < ssr_len && both_p == 0 ){ // match number more than cutoff but two types of unmatch exists
						break;
					}

					//2) whole region level
					else if( match == ssr_len ){ //if match == ssr_len, which mean that no mismatch and no deletion there
						insert_sumbase+=(matchright[match]-(ssr_len-1)); // could be 0 or larger than 0
						if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > ssrlen_p*unmatch_ssr_p && (start_new - start) < ssrlen_p){
							insert_sumbase-=(matchright[match]-(ssr_len-1));
							//cout << start << "\t" << ssr_len << "\t" << "(delete_sumbase + mismatch_sumbase + insert_sumbase) > ssrlen_p*unmatch_ssr_p" << endl;
							break;
						}
						else if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > (start_new - start)*unmatch_ssr_p && (start_new - start) >= ssrlen_p ){
							insert_sumbase-=(matchright[match]-(ssr_len-1));
							//cout << start << "\t" << ssr_len << "\t" << "(delete_sumbase + mismatch_sumbase + insert_sumbase) > ssrlen_p*unmatch_ssr_p" << endl;
							break;
						}
					}
					else if( matchright[match] == ssr_len-1 ){ // if match < ssr_len, maybe mismatch or deletion 
						mismatch_sumbase+=(ssr_len-match);
						if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > ssrlen_p*unmatch_ssr_p && (start_new - start) < ssrlen_p ){
							mismatch_sumbase-=(ssr_len-match);
							break;
						}
						else if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > (start_new - start)*unmatch_ssr_p && (start_new - start) >= ssrlen_p ){
							mismatch_sumbase-=(ssr_len-match);
							break;
						}
					}
					else if( matchright[match] < ssr_len-1 ){ // delete allowed
						delete_sumbase+=(ssr_len-match);
						if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > ssrlen_p*unmatch_ssr_p && (start_new - start) < ssrlen_p ){
							delete_sumbase-=(ssr_len-match);
							break;
						}
						else if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > (start_new - start)*unmatch_ssr_p && (start_new - start) >= ssrlen_p ){
							delete_sumbase-=(ssr_len-match);
							break;
						}
					}

					ssr_region+=DNA.substr(start_new,matchright[match]+1);
					ssr_region+=" ";
					start_new+=matchright[match]+1;

                                }
                        }

			//if( ssr_len * repeat >= ssrlen_p && (start_new - start) >= ssrlen_p && repeat >= mincopy_p ){
			if( repeat >= cutoff_array_main[ssr_len-1] ){
				if( revercomple_p > 0 ){
					string ssr_recom;
					ssr_recom=left_unit;
					reverse_complement(ssr_recom);
					left_unit+="\t";
					left_unit+=ssr_recom;
				}
				
				if( flanking_p == 0 ){
					cout << id << "\t" << start+1 << "\t" << start_new << "\t" << left_unit << "\t" << repeat << "\t" << delete_sumbase << "\t" << insert_sumbase << "\t" << mismatch_sumbase << "\t" << start_new-start ;
					cout << "\t" << ssr_region ;
					cout << "\n";
					start+=(start_new-start);
                                	break;
				}
				else if( flanking_p > 0 ){
					string flanking_seq;
					string flanking_seq1;
					string flanking_seq2;
					if( start > flanking_p && start_new + flanking_p < DNA_len ){
						flanking_seq=DNA.substr( start - flanking_p , 2 * flanking_p + start_new - start );
						flanking_seq1=DNA.substr( start - flanking_p, flanking_p );
						flanking_seq2=DNA.substr( start_new, flanking_p );
						cout << id << "\t" << start+1 << "\t" << start_new << "\t" << left_unit << "\t" << repeat << "\t" << delete_sumbase << "\t" << insert_sumbase << "\t" << mismatch_sumbase << "\t" << start_new-start;
						cout << "\t" << ssr_region ;
						cout << "\t" << flanking_seq;
						cout << "\t" << flanking_seq1;
						cout << "\t" << flanking_seq2;
						cout << "\n";
						start+=(start_new-start);
						break;
					}
					else if ( start <= flanking_p && start_new + flanking_p < DNA_len ){
						flanking_seq=DNA.substr( 0, start+start_new-start+flanking_p );
						flanking_seq1=DNA.substr( 0, start );
						flanking_seq2=DNA.substr( start_new, flanking_p );
						cout << id << "\t" << start+1 << "\t" << start_new << "\t" << left_unit << "\t" << repeat << "\t" << delete_sumbase << "\t" << insert_sumbase << "\t" << mismatch_sumbase << "\t" << start_new-start;
						cout << "\t" << ssr_region ;
                                                cout << "\t" << flanking_seq;
						cout << "\t" << flanking_seq1;
						cout << "\t" << flanking_seq2;
                                                cout << "\n";
                                                start+=(start_new-start);
                                                break;
					}
					else if ( start > flanking_p && start_new + flanking_p >= DNA_len ){
						flanking_seq=DNA.substr( start-flanking_p, flanking_p+start_new-start+DNA_len-start_new );
						flanking_seq1=DNA.substr( start-flanking_p, flanking_p );
						flanking_seq2=DNA.substr( start_new, DNA_len - start_new );
						cout << id << "\t" << start+1 << "\t" << start_new << "\t" << left_unit << "\t" << repeat << "\t" << delete_sumbase << "\t" << insert_sumbase << "\t" << mismatch_sumbase << "\t" << start_new-start ;
						cout << "\t" << ssr_region ;
						cout << "\t" << flanking_seq;
						cout << "\t" << flanking_seq1;
						cout << "\t" << flanking_seq2;
						cout << "\n";
						start+=(start_new-start);
						break;
					}
					else if ( start <= flanking_p && start_new+flanking_p >= DNA_len ){
						flanking_seq1=DNA.substr( 0, start );
						flanking_seq2=DNA.substr( start_new, DNA_len - start_new );
						cout << id << "\t" << start+1 << "\t" << start_new << "\t" << left_unit << "\t" << repeat << "\t" << delete_sumbase << "\t" << insert_sumbase << "\t" << mismatch_sumbase << "\t" << start_new-start ;
						cout << "\t" << ssr_region ;
						cout << "\t" << flanking_seq1;
						cout << "\t" << flanking_seq2;
						cout << "\t" << DNA;
						cout << "\n";
						start+=(start_new-start);
						break;
					}
				}
			}
                }
        }
}
