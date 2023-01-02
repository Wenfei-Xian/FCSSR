#include "fuzzy.h"

void find_fuzzy(string DNA, string id, int unitlen_p, unsigned int ssrlen_p, int insertion_p, float unmatch_ssr_p, int both_p, int up_p, unsigned int flanking_p, int unmatch_num_p, int revercomple_p ){

	if( up_p == 1){ // up_p : whether transfer all the character to upper, default: 0
		transform( DNA.begin(), DNA.end(), DNA.begin(), ::toupper);
	}

        unsigned int DNA_len=DNA.length();
	int insertion=insertion_p; // insertion_p : maximum length of insertion within unit

        for ( unsigned int start=0; start < (DNA_len-unitlen_p) ; start++){

                for (unsigned short ssr_len=1; ssr_len<= unitlen_p ; ssr_len++){ // unitlen_p : longest length of unit, default: 6

			//if( start+(ssr_len*2+insertion_p) > DNA.length() ){ // at least two repeats will be output, if start+ssr_len * 2 > DNA.length, which will be no meaning
			//	break;
			//}
			
			//if( ssr_len > 1){ // avoid AAAAA as a unit
			//	char add_left;
			//	char add_right;
			//	int add_match=0;
			//	for ( short add=0; add < ssr_len+insertion_p; add++ ){
			//		add_left=(DNA[start + add]);
			//		add_right=(DNA[start + ssr_len + insertion_p + add]);
			//		if( add_left != add_right ){
			//			break;
			//		}
			//		else{
			//			add_match++;
			//		}
			//	}
			
			//	if( add_match == ssr_len+insertion_p ){
			//		continue;
			//	}
			//}
			
			unsigned short repeat=1;
			unsigned int start_new=start+ssr_len;

			string ssr_region; // used for output

			//string left_unit;
			//for( int site0=start; site0 < ssr_len; ++site0 ){
			//	cout << site0 << endl;
			//	left_unit+=DNA[site0];
			//}

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
				int matchright[ssr_len+insertion_p];
				int site[ssr_len]={0};

                                for ( short base=0; base<ssr_len; base++ ){// compare unit and next unit
                                        
					char left;
                                        char right;

                                        for( short ins=0; ins <= insertion_change; ins++ ){ // for each base comparision, how many insertion quota left.

                                                left = (DNA[start+base]);
                                                right = (DNA[start_new+rightstart+ins]);
						
						if( left != right ){
							continue;
						}
						else if( left == 'N' ){
							break;
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
						else if( (start_new - start) >= ssrlen_p && insert_sumbase > (start_new - start) * unmatch_ssr_p ){
							insert_sumbase-=matchright[1];
							break;
						}
					}

					//string unit_next;
					//for( int site=start_new; site<matchright[match]+1; ++site ){
					//	unit_next=DNA[site];
					//}

                                        ssr_region+=DNA.substr(start_new,matchright[match]+1);
                                        
					//ssr_region+=unit_next;
					ssr_region+=" ";
                                        start_new+=matchright[match]+1;

                                }
                                else{ // ssr_len > 1
					// two condition will break the  loop 
					//1) unit level
					if( match < ssr_len-unmatch_num_p ){ // match number less than cutoff
						break;
					}
					else if( insertion_change < insertion_p && match < ssr_len ){ // match number more than cutoff but two types of unmatch exists
						break;
					}

					//2) whole region level
					else if( match == ssr_len ){ // insertion allowed
						insert_sumbase+=(matchright[match]-(ssr_len-1));
						if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > ssrlen_p*unmatch_ssr_p && (start_new - start) < ssrlen_p){
							insert_sumbase-=(matchright[match]-(ssr_len-1));
							break;
						}
						else if( (delete_sumbase + mismatch_sumbase + insert_sumbase) > (start_new - start)*unmatch_ssr_p && (start_new - start) >= ssrlen_p ){
							insert_sumbase-=(matchright[match]-(ssr_len-1));
							break;
						}
					}
					else if( matchright[match] == ssr_len-1 ){ // mismatch allowed
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

					//string unit_next;
					//for( int site2 = start_new; site2 < matchright[match]+1; ++site2 ){
					//	unit_next+=DNA[site2];
					//}
					ssr_region+=DNA.substr(start_new,matchright[match]+1);
					//ssr_region+=unit_next;
					ssr_region+=" ";
					start_new+=matchright[match]+1;

                                }
                        }

			if( ssr_len * repeat >= ssrlen_p && (start_new - start) >= ssrlen_p && repeat > 1 ){
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