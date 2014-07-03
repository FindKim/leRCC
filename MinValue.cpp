//
//  MinValue.cpp
//  
//
//  Created by Kim Ngo July 2, 2014.

//	Counts number of occurences for each min value
//  Compares min counts from significant sequences with non-sig seqs
//	SigOrf files are from Aaron's pruning results
//	mm_orfeome files are from minmax results

#include "MinValue.h"

#include <string>
#include <vector>
#include <utility>	// pair
#include <iostream>	// cout
#include <sstream>	//stringstream peak, ignore
#include <cmath>		// abs & floor
#include <stdlib.h>

const int NUM_CODON_WINDOW = 17;
const int NUM_MASK_AA	= 50 - NUM_CODON_WINDOW;

using namespace std;

MinValue :: MinValue (vector<string>::iterator sigOrf_it, const vector<string>& sigOrf_v, const vector<pair<string, string> >& id_seq) {

	set_sig_min(101);
	set_non_sig_min(101);
	count_min(sigOrf_it, sigOrf_v, id_seq);
//	cout << endl << endl << "The next organism is " << *sigOrf << endl;
}


// Instantiates sig_run vector n 0's
void MinValue :: set_sig_min(int n) {
	vector<int> temp(n, 0);
	sig_min = temp;
}
void MinValue :: set_non_sig_min(int n) {
	vector<int> temp(n, 0);
	non_sig_min = temp;
}

// Returns # of min for sig seqs
vector<int> MinValue :: get_sig_min() {
	return sig_min;
}


// Returns # of min for non-sig seqs
vector<int> MinValue :: get_non_sig_min() {
	return non_sig_min;
}


// Parses minmax sequence string into vector of floats
vector<float> MinValue :: parse_mm_seq(const string& mm_seq) {

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



// Returns the position the significant organism iterator was last at
vector<string>::iterator MinValue :: get_it_pos() {
	return sigOrf_it;
}


// Sets where the last position the iterator pointed to
void MinValue :: set_sigOrf_it_pos(vector<string>::iterator it) {
	sigOrf_it = it;
}


// Adds the count of each run length to the total sum
void MinValue :: add_mins (vector<int>& sum, const vector<int>& min) {

	vector<int>::iterator sum_it = sum.begin();
	vector<int>::const_iterator min_it = min.begin();
	
	for (min_it; min_it != min.end(); ++sum_it, ++min_it) {
		*sum_it += *min_it;
	}
}


// Prints vector of sig & non-sig min
void MinValue :: print_min (const vector<int>& sm, const vector<int>& nsm) {
/*
	vector<int>::const_iterator sm_it = sm.begin();
	vector<int>::const_iterator nsm_it = nsm.begin();
	for (int i = 0; sm_it != sm.end(); i++, ++sm_it, ++nsm_it) {
		cout << i << " sig: " << *sm_it << " ";
		cout << "non-sig: " << *nsm_it << endl;
	}
*/	for (int i = 75; i < 101; i++) {
		cout << i << "\t" << "sig: " << sm[i] << "\t";
		cout << "non-sig: " << nsm[i] << endl;
	}
}


// Rounds decimal min values to nearest integer
float MinValue :: round(const float& num) {
//	cout << num << " -> " << floor(num+0.5) << endl;
	return floor(num+0.5);
}


// Increments (non)sig_min if vec of sig seqs has consecutive mins
// The address of the bin is the length of the run
// Value in the addr is the number of min of that length
void MinValue :: count_min(vector<string>::iterator sigOrf, const vector<string>& sigOrf_v, const vector< pair< string, string> >& id_seq) {

	// Iterates through pairs in the vector id_seq
	vector< pair< string, string> >::const_iterator id_seq_it = id_seq.begin();	

	for (id_seq_it; id_seq_it != id_seq.end(); ++id_seq_it) {

		// Parse the minmax values string by ','
		vector<float> mm_number_v;
		mm_number_v = parse_mm_seq(id_seq_it->second);

		// Masks the first 50 amino acids considering 17 codon windows
		// Iterates through min max values and counts for min min
		if (mm_number_v.size() > NUM_MASK_AA) {	// Prevents running off vec

			vector<float>::const_iterator it = mm_number_v.begin();
			it += NUM_MASK_AA;

			// Iterates through each minmax value
			// Increments address of min value for every occurence
			for (it; it != mm_number_v.end(); ++it) {
				if (*it < 0) {
					int address = round(abs(*it));
//					cout << *it << " -> " << address << endl;
					if(*sigOrf == id_seq_it->first)
						sig_min[address]++;
//						cout << address << " " << sig_min[address] << endl;
					else non_sig_min[address]++;
					
//					if (address == 100)
//						cout << id_seq_it->first << endl;
				}
			}
		}
		
		// Iterates to next significant sequence once that sequence matches
		// Doesn't matter if it contains min value or is longer than 50 aa
		if (*sigOrf == id_seq_it->first && sigOrf+1 != sigOrf_v.end())
			++sigOrf;
	}
	set_sigOrf_it_pos(sigOrf);
}
