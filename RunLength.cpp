//
//  RunLength.cpp
//  
//
//  Created by Kim Ngo June 24, 2014.

//	Counts number of min runs
//  Compares min runs from significant sequences with non-sig seqs
//	SigOrf files are from Aaron's pruning results
//	mm_orfeome files are from minmax results

#include "RunLength.h"

#include <string>
#include <vector>
#include <utility>	// pair
#include <iostream>	// cout
#include <sstream> //stringstream peak, ignore

using namespace std;

RunLength :: RunLength (vector<string>::iterator sigOrf, vector<pair<string, string> > id_seq) {

	set_sig_runs(100);
	set_non_sig_runs(100);
	count_runs(sigOrf, id_seq);
}


// Instantiates sig_run vector n 0's
void RunLength :: set_sig_runs(int n) {
	vector<int> temp(n, 0);
	sig_runs = temp;
}
void RunLength :: set_non_sig_runs(int n) {
	vector<int> temp(n, 0);
	non_sig_runs = temp;
}

// Returns # of runs for sig seqs
vector<int> RunLength :: get_sig_runs() {
	return sig_runs;
}


// Returns # of runs for non-sig seqs
vector<int> RunLength :: get_non_sig_runs() {
	return non_sig_runs;
}


// Parses minmax sequence string into vector of floats
vector<float> RunLength :: parse_mm_seq(string mm_seq) {

	float i;
	vector<float> mm_number_v;	// str parsed into a vector of floats
	stringstream ss(mm_seq);
	
	while (ss >> i) {
		mm_number_v.push_back(i);
		if (ss.peek() == ',')
			ss.ignore();
	}
	return mm_number_v;
}


// Increments (non)sig_runs if vec of sig seqs has consecutive mins
// The address of the bin is the length of the run
// Value in the addr is the number of runs of that length
void RunLength :: count_runs(vector<string>::iterator sigOrf, vector< pair< string, string> > id_seq) {

	int min_run_count = 0;

	// Iterates through pairs in the vector id_seq
	vector< pair< string, string> >::iterator id_seq_it = id_seq.begin();	

//	while (id_seq_it->first != ">Aaeo|60") {
//		++id_seq_it;
//	}++sigOrf;
//	for (id_seq_it; id_seq_it->first != ">Aaeo|100"; ++id_seq_it) {	

	for (id_seq_it; id_seq_it != id_seq.end(); ++id_seq_it) {

		// parse the minmax values string by ','
		vector<float> mm_number_v = parse_mm_seq(id_seq_it->second);
		vector<float>::iterator it = mm_number_v.begin();
		for (it; it != mm_number_v.end(); ++it) {

			min_run_count = 0;						// Reset count every max value break
			while (*it < 0) {							// Continuous min values
//				cout << *it << " ";
				min_run_count++;
				if (it+1 != mm_number_v.end())	// Prevents from running off vec
					++it;
				else break;
			}

			// If id matches significant orfeome, increment accordingly
			if (*sigOrf == id_seq_it->first) {
				sig_runs[min_run_count]++;
				++sigOrf;
//				cout << "Significant: " << sig_runs[min_run_count] << endl;
//				cout << "---------------------------------SIGNIFICANT!!-------------------------------" << endl;
//				cout << "next significant seq: " << *sigOrf << endl << endl;

			} else {
				non_sig_runs[min_run_count]++;
//				cout << "Not significant: " << non_sig_runs[min_run_count] << endl;
			}
		}
	}
}
