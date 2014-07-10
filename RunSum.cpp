//
//  RunSum.cpp
//  
//
//  Created by Kim Ngo July 9, 2014.

//	Calculates sum %min value every min cluster
//  Compares min runs from significant sequences with non-sig seqs
//	SigOrf files are from Aaron's pruning results
//	mm_orfeome files are from minmax results

#include "RunSum.h"

#include <string>
#include <vector>
#include <utility>	// pair
#include <iostream>	// cout
#include <cmath>    // std::abs
#include <sstream> //stringstream peak, ignore

const int NUM_CODON_WINDOW = 17;
const int NUM_MASK_AA	= 50 - NUM_CODON_WINDOW;

using namespace std;

RunSum :: RunSum (vector<string>::iterator sigOrf_it, const vector<string>& sigOrf_v, const vector<pair<string, string> >& id_seq) {

	set_sig_sums(300);
	set_non_sig_sums(300);
	calc_run_sum(sigOrf_it, sigOrf_v, id_seq);
//	cout << endl << endl << "The next organism is " << *sigOrf << endl;
}


// Instantiates sig_run vector n 0's
void RunSum :: set_sig_sums(int n) {
	vector<int> temp(n, 0);
	sig_sums = temp;
}
void RunSum :: set_non_sig_sums(int n) {
	vector<int> temp(n, 0);
	non_sig_sums = temp;
}

// Returns sums for sig seqs
vector<int> RunSum :: get_sig_sums() {
	return sig_sums;
}


// Returns sums for non-sig seqs
vector<int> RunSum :: get_non_sig_sums() {
	return non_sig_sums;
}


// Parses minmax sequence string into vector of floats
vector<float> RunSum :: parse_mm_seq(const string& mm_seq) {

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
vector<string>::iterator RunSum :: get_it_pos() {
	return sigOrf_it;
}


// Sets where the last position the iterator pointed to
void RunSum :: set_sigOrf_it_pos(vector<string>::iterator it) {
	sigOrf_it = it;
}


// Adds the count of each run sum to the total sum
void RunSum :: add_sums (vector<int>& tot_sum, const vector<int>& current_sum) {

	vector<int>::iterator tot_sum_it = tot_sum.begin();
	vector<int>::const_iterator current_sum_it = current_sum.begin();

	for (current_sum_it; current_sum_it != current_sum.end();
	++tot_sum_it, ++current_sum_it)
		*tot_sum_it += *current_sum_it;
}


// Prints vector of sig & non-sig sums
void RunSum :: print_sums (const vector<int>& sr, const vector<int>& nsr) {
/*
	vector<int>::const_iterator sr_it = sr.begin();
	vector<int>::const_iterator nsr_it = nsr.begin();
	for (int i = 0; sr_it != sr.end(); i++, ++sr_it, ++nsr_it) {
		cout << i << " sig: " << *sr_it << " ";
		cout << "non-sig: " << *nsr_it << endl;
	}
*/	for (int i = 0; i < 25; i++) {
		cout << i << "\t" << "sig: " << sr[i] << "\t";
		cout << "non-sig: " << nsr[i] << endl;
	}
}


// Rounds decimal min values to nearest integer
float RunSum :: round(const float& num) {
//	cout << num << " -> " << floor(num+0.5) << endl;
	return floor(num+0.5);
}


// Increments (non)sig_sums if vec of sig seqs has consecutive mins
// The address of the bin is the length of the run
// Value in the addr is the number of sums of that length
void RunSum :: calc_run_sum(vector<string>::iterator sigOrf, const vector<string>& sigOrf_v, const vector< pair< string, string> >& id_seq) {

	float min_run_sum = 0;

	// Iterates through pairs in the vector id_seq
	vector< pair< string, string> >::const_iterator id_seq_it = id_seq.begin();	
	for (id_seq_it; id_seq_it != id_seq.end(); ++id_seq_it) {
//		cout << id_seq_it->first << endl;

		// Parse the minmax values string by ','
		vector<float> mm_number_v;
		mm_number_v = parse_mm_seq(id_seq_it->second);
//		cout << endl;

		// Masks the first 50 amino acids considering 17 codon windows
		// Iterates through min max values and sums min values for min runs
		if (mm_number_v.size() > NUM_MASK_AA) {	// Prevents running off vec

			vector<float>::const_iterator it = mm_number_v.begin();
			it += NUM_MASK_AA;	// Masks the first 50 aa

			for (it; it != mm_number_v.end(); ++it) {
				min_run_sum = 0;			// Reset sum every max value break
				while (*it < 0) {				// Continuous min values
					min_run_sum += abs(*it);	// Absolute value of min
//					cout << *it << " " << min_run_sum << endl;
					// Prevents from running off vec
					if (it+1 != mm_number_v.end())
						++it;
					else break;
				}
				
				// Rounds sum to nearest integer to have address
				if (min_run_sum > 0)
					min_run_sum = round(min_run_sum);

				if (min_run_sum > sig_sums.size()
					|| min_run_sum > non_sig_sums.size()) {
					sig_sums.resize(min_run_sum+1, 0);
					non_sig_sums.resize(min_run_sum+1, 0);
//					cout << "local resized here to " << min_run_sum+1 << endl;
				}

				// If id matches significant orfeome, increment accordingly
				if (*sigOrf == id_seq_it->first && sigOrf+1 != sigOrf_v.end()) {

					if (min_run_sum > 0)
						sig_sums[min_run_sum]++;
					++sigOrf;
//					cout << "Significant: " << min_run_sum << " " << sig_sums[min_run_sum] << endl;
//					cout << "next significant seq: " << *sigOrf << endl << endl;
				} else if (min_run_sum > 0) {
					non_sig_sums[min_run_sum]++;
//					cout << "Non-sig: " << min_run_sum << " " << non_sig_sums[min_run_sum] << endl;
				}
			}
		}
	}
	set_sigOrf_it_pos(sigOrf);
}
