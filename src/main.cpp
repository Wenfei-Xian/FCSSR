#include "main.h"

int main( int argc, char *argv[] ){
	
	if(argc ==  1 ){
		return usage();
	}

	if( strcmp(argv[1], "fuzzy" ) == 0 ){
		return fuzzy_model( argc-1, argv+1 );
	}
	else if( strcmp( argv[1], "perfect") == 0 ){
		return perfect_model( argc-1, argv+1 );
	}
//	else if( strcmp( argv[1], "annotation") == 0 ){
//		return annotation_module( argc-1, argv+1 );
//	}
//	else if( strcmp( argv[1], "align") == 0 ){
//		return align_module( argc-1, argv+1 );
//	}
	else{
		cerr << "Can't recognized command" << " " << argv[1] << endl;
		return usage();
	}

}

int usage(){
	cout << "This program consists of two SSR searching modules: perfect and fuzzy" << endl;
	cout << "Version:1.0, March,2023" << endl;
	cout << "Usage: FC_SSR <command> [options]" << endl;
	cout << "Command:" << endl;
	cout << "	perfect: perfect model" << endl;
	cout << "	fuzzy: fuzzy model" << endl;
	//cout << "	annotation: SSR location annotation" << endl;
	//cout << "	alignment: aligned all the SSR according to the gfa file" << endl;
	return 0;
}

/*
int annotation_module( int argc, char *argv[] ){
	int c;
	string bed;
	string gff;
	int para=0;

	while( (c=getopt(argc, argv, "b:g:")) >= 0 ){
		if( c== 'b' ){
			bed=optarg;
			para++;
		}
		else if( c== 'g'){
			gff=optarg;
			para++;
		}
	}

	if( para != 2 ){
		cout << "Usage: FC_SSR annotation [options]" << endl;
		cout << "arguments:" << endl;
		cout << "	-g string        gene location file in gff format" << endl;
		cout << "	-b string        SSR location file in bed format" << endl;
		return -1;
	}

	ifstream bedfile( bed );
	ifstream gfffile( gff );

	if( !bedfile.good() ){
		cerr << "Can't open " << bedfile << endl;
		return -1;
	}
	else if ( !gfffile.good() ){
		cerr << "Can't open " << gfffile << endl;
		return -1;
	}

	annotation_modules( gfffile, bedfile );

}
*/

int perfect_model( int argc, char *argv[] ){
	int a;
	int unitlen_p=6;
	//int ssrlen_p=10;
	int up_p=1;
	//int mincopy_p=2;
	string fasta;
	int para=0;
	unsigned int flanking_p=0;
	int revercomple_p=0;
	string cutoffunit_p="10,5,4,3,2,2";

	while(( a=getopt(argc, argv, "f:l:L:s:u:r:n:c:")) >= 0){ // :一个:表示必须有后接参数，两个::表示可选参数，没有:表示不用接参数
		if( a == 'f'){
			fasta = optarg;
			para++;
		}
		else if( a == 'l'){
			unitlen_p = atof(optarg);//atof 字符串变浮点
			para++;
		}
		//else if( a == 'L'){
		//	ssrlen_p = atof(optarg);
		//	para++;
		//}
		else if( a == 's'){
			flanking_p = atof(optarg);
			para++;
		}
		else if( a == 'u' ){
			up_p = atof(optarg);
			para++;
		}
		else if( a == 'r' ){
			revercomple_p = atof(optarg);
			para++;
		}
		//else if( a == 'n' ){
		//	mincopy_p = atof(optarg);
		//	para++;
		//}
		else if( a == 'c' ){
			cutoffunit_p = optarg;
			para++;
		}
	}

	if( para == 0 ){ // defaule !=6
		cout << "Usage: FC_SSR perfect " << endl;
		//cout << "except -f XX.fa, other parameter don't contain space: -f6" << endl;
	        cout << "options:" << endl;
        	cout << "       -f string     fasta format file (mandatory)" << endl;
       		cout << "       -l int        maximum length of SSR unit (default value: 6)" << endl;
        	//cout << "       -L int        minimum length of SSRs region (default value: 10)" << endl;
		//cout << "       -n int        minimum copy number (default value: 2)" << endl;
		cout << "       -c string     lenght cutoff for each unit, the number of cutoff should be the same as SSR unit (default value: 10,5,4,3,2,2)" << endl;
		cout << "       -s int        whether output the flanking sequence of SSR region (default value: 0 -> don't output; length of flanking sequences -> output)" << endl;
		cout << "       -u int        whether replace all letters with uppercase letters (default value: 1 -> replace; 0 -> don't replace)" << endl;
		cout << "       -r int        whether output the reverse complement sequence (default value: 0 -> don't output; 1 -> output)" << endl; 
		return -1;
	}
	
	ifstream input( fasta );

	if( !input.good() ){
		cerr << "Can't open " << fasta << endl;
		return -1;
	}

	int num=count(cutoffunit_p.begin(),cutoffunit_p.end(),',');
	if( num+1 != unitlen_p ){
		cerr << "Maximum lenght of SSR unit is: " << unitlen_p << ", but the number of lenght cutoff for each unit is: " << num+1 << endl;
		return -1;
	}

	if( flanking_p > 0 && revercomple_p > 0){
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" "unit_reverse_complement" << "\t" << "Repeats" << "\t" << "SSR_region_length" << "\t" <<  "SSR_region" << "\t" << "SSR_region_flanking" << endl;
	}
	else if( flanking_p > 0 && revercomple_p == 0 ){
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" << "Repeats" << "\t" << "SSR_region_length" << "\t" << "SSR_region" << "\t" << "SSR_region_flanking" << endl;
	}
	else if( flanking_p == 0 && revercomple_p > 0 ){
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" "unit_reverse_complement" << "\t" << "Repeats" << "\t" << "SSR_region_length" << "\t" << "SSR_region" << endl;
	}
	else {
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" << "Repeats" << "\t" << "SSR_region_length" << "\t" << "SSR_region" << endl;
	}

	string line, id, DNA;
	while( getline(input, line) ){
		if( line.empty() ){
			continue;
		}
		if( line[0] == '>' ){
			if( !id.empty() ){
				//find_perfect( DNA, id, unitlen_p, ssrlen_p, flanking_p, up_p, revercomple_p,mincopy_p,cutoffunit_p);
				find_perfect( DNA, id, unitlen_p, flanking_p, up_p, revercomple_p, cutoffunit_p);
			}
			id=line.substr(1);
			DNA.clear();
		}
		else{
			DNA+=line;
		}
	}
	if( !id.empty() ){
		//find_perfect( DNA, id, unitlen_p, ssrlen_p, flanking_p, up_p, revercomple_p,mincopy_p,cutoffunit_p);
		find_perfect( DNA, id, unitlen_p, flanking_p, up_p, revercomple_p, cutoffunit_p);
	}
	return 0;
}

int fuzzy_model( int argc, char *argv[] ){
	
	int a;
	int unitlen_p=6;
	//int ssrlen_p=10;
	int insertion_p=1;
	float unmatch_ssr_p=0.1f;
	int up_p=0;
	int both_p=0;
	int unmatch_num_p=1;
	int flanking_p=0;
	int revercomple_p=0;
	//int mincopy_p=2;
	string cutoffunit_p="10,5,4,3,2,2";

	string fasta;

	int para=0;
	
	while(( a=getopt( argc, argv, "f:l:L:i:P:b:m:s:u:r:n:c:")) >= 0 ){

		if( a == 'f' ){
			fasta=optarg;
			para++;
		}
		else if( a == 'c' ){
			cutoffunit_p=optarg;
			para++;
		}
		else if( a == 'l'){
			unitlen_p=atof(optarg);
			para++;
		}
		//else if( a == 'L'){
		//	ssrlen_p=atof(optarg);
		//	para++;
		//}
		else if( a == 'i'){
			insertion_p=atof(optarg);
			para++;
		}
		else if( a == 'P' ){
			unmatch_ssr_p=atof(optarg);
			para++;
		}
		else if( a == 'u'){
			up_p=atof(optarg);
			para++;
		}
		else if( a == 'b'){
			both_p=atof(optarg);
			para++;
		}
		else if( a == 'm' ){
			unmatch_num_p=atof(optarg);
			para++;
		}
		else if( a == 's'){
			flanking_p=atof(optarg);
			para++;
		}
		else if( a == 'r'){
			revercomple_p=atof(optarg);
			para++;
		}
		//else if( a == 'n'){
		//	mincopy_p=atof(optarg);
		//	para++;
		//}

	}

        if( para == 0 ){
                cout << "Usage: FC_SSR perfect [options]" << endl;
                cout << "arguments:" << endl;
		//cout << "except -f XX.fa, other parameter don't contain space: -f6" << endl;
                cout << "	-f string     fasta format file (mandatory)" << endl;
                cout << "	-l int        maximum length of SSR unit (default value: 6)" << endl;
                //cout << "	-L int        minimum length of SSRs region (default value: 10)" << endl;
		cout << "	-i int        maximum length of insertion allowed within unit (default value: 1)" << endl;
		//cout << "	-n int        minimum copy number (default value: 2)" << endl;
		cout << "	-c string     lenght cutoff for each unit, the number of cutoff should be the same as SSR unit (default value: 10,5,4,3,2,2)" << endl;
		cout << "	-m int        maximum length of unmatch within unit (default value: 1)" << endl;
		cout << "	-b int        whether allow deletion/mismatch and insertion retain in one unit, 1 allow, 0 not allow (default value: 0)" << endl;
		cout << "	-P int        maximum unmatch(inserted+mismatch+deleted) percentage whold SSRs region (default value: 0.1)" << endl;
		cout << "	-u int        whether to replace all letters with uppercase letters (default value: 1 -> replace; 0 -> don't replace)" << endl;
		cout << "	-s int        whether output the flanking sequnce of SSR region (default value: 0 -> don't output; length of flanking sequences -> output)" << endl;
		cout << "	-r int        whether output the reverse complement sequence (defalut value: 0 -> don't output; 1 -> output)" << endl; 
		return -1;
        }

	ifstream input( fasta );
	if( !input.good() ){
		cerr << "Can't open " << fasta << endl;
		return -1;
	}

	int num=count(cutoffunit_p.begin(),cutoffunit_p.end(),',');
	if( num+1 != unitlen_p ){
		cerr << "Maximum lenght of SSR unit is: " << unitlen_p << ", but the number of lenght cutoff for each unit is: " << num+1 << endl;
		return -1;
	}


	if( flanking_p > 0 && revercomple_p > 0){
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" <<  "unit_reverse_complement"<< "\t" << "Repeats" << "\t" << "Deletion_base_sum " << "\t" << "Insertion_base_sum" << "\t" << "Mismatch_base_sum" << "\t" << "SSR_region_length" << "\t" << "SSR_region" << "\t" << "SSR_region_flanking" << endl;
	}
	else if( flanking_p > 0 && revercomple_p == 0 ){
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" << "Repeats" << "\t" << "Deletion_base_sum " << "\t" << "Insertion_base_sum" << "\t" << "Mismatch_base_sum" << "\t" << "SSR_region_length" << "\t" << "SSR_region" << "\t" << "SSR_region_flanking" << endl;
	}
	else if( flanking_p == 0 && revercomple_p > 0 ){
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" << "SSR_reverse_complement" << "\t" <<  "Repeats" << "\t" << "Deletion_base_sum " << "\t" << "Insertion_base_sum" << "\t" << "Mismatch_base_sum" << "\t" << "SSR_region_length" << "\t" << "SSR_region" << endl;
	}
	else{
		cout << "Chr/Scaffold" << "\t" << "Start" << "\t" << "End" << "\t" << "unit" << "\t" << "Repeats" << "\t" << "Deletion_base_sum " << "\t" << "Insertion_base_sum" << "\t" << "Mismatch_base_sum" << "\t" << "SSR_region_length" << "\t" << "SSR_region"  << endl;
	}

	string line, id, DNA;

        while( getline(input, line) ){
                if( line.empty() ){
                        continue;
                }
                if( line[0] == '>' ){
                        if( !id.empty() ){
				//find_fuzzy( DNA, id, unitlen_p, ssrlen_p, insertion_p, unmatch_ssr_p,both_p, unmatch_num_p, up_p, flanking_p );
        			//find_fuzzy( DNA, id, unitlen_p, ssrlen_p, insertion_p, unmatch_ssr_p, both_p, up_p, flanking_p, unmatch_num_p, revercomple_p,mincopy_p );
				find_fuzzy( DNA, id, unitlen_p, insertion_p, unmatch_ssr_p, both_p, up_p, flanking_p, unmatch_num_p, revercomple_p, cutoffunit_p );
				//find_fuzzy( DNA, id, unitlen_p, insertion_p, unmatch_ssr_p, up_p, flanking_p, unmatch_num_p, revercomple_p, cutoffunit_p );
			}
                        id=line.substr(1);
                        DNA.clear();
                }
                else{
                        DNA+=line;
                }
        }
        if( !id.empty() ){
		//find_fuzzy( DNA, id, unitlen_p, ssrlen_p, insertion_p, unmatch_ssr_p, both_p, up_p, flanking_p, unmatch_num_p, revercomple_p,mincopy_p );
		//find_fuzzy( DNA, id, unitlen_p, ssrlen_p, insertion_p, unmatch_ssr_p, both_p, unmatch_num_p, up_p, flanking_p);
		find_fuzzy( DNA, id, unitlen_p, insertion_p, unmatch_ssr_p, both_p, up_p, flanking_p, unmatch_num_p, revercomple_p, cutoffunit_p );
		//find_fuzzy( DNA, id, unitlen_p, insertion_p, unmatch_ssr_p, up_p, flanking_p, unmatch_num_p, revercomple_p, cutoffunit_p );
        }
	return 0;
}
