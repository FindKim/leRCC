//
//  RunSum.h
//  
//
//  Created by Kim Ngo July 9, 2014.

//	Calculates sum %min value every min cluster
//  Compares min runs from significant sequences with non-sig seqs
//	SigOrf files are from Aaron's pruning results
//	mm_orfeome files are from minmax results

#ifndef RUNSUM_H
#define RUNSUM_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class RunSum {

	public:
		RunSum(vector<string>::iterator, const vector<string>&,
			const vector< pair< string, string> >&);
			// Calculates sum min values for consecutive min runs (rc cluster)
			// Compares sum of runs for sigorfeomes and non-sig
		vector<int> get_sig_sums();			// Returns sum of runs for sig seqs
		vector<int> get_non_sig_sums();	// Returns sum of runs for non-sig
		vector<string>::iterator get_it_pos();	// Returns position iterator
		void add_sums(vector<int>&, const vector<int>&);
			// Adds elements in second vector to first vector
		void print_sums(const vector<int>&, const vector<int>&);

	private:
		vector<int> sig_sums;
			// v address reps sum for rc cluster & stores the # of occurences
		vector<int> non_sig_sums;
			// v address reps sum for rc cluster & stores the # of occurences
		vector<string>::iterator sigOrf_it;
			// Position of sigOrf iterator
		void calc_run_sum(vector<string>::iterator, const vector<string>&,
			const vector< pair< string, string> >&);
			// Increments (non)sig_sum if rc cluster
			// The address of the bin is the sum of the run
			// Value in the addr is the sum number for rc cluster run

		void set_sig_sums(int);		// Initializes sig_run vec with 300 0's
		void set_non_sig_sums(int); //""
		void set_sigOrf_it_pos(vector<string>::iterator);
			// Sets where the last position the iterator pointed to
		vector<float> parse_mm_seq(const string&);
			// Parses minmax sequence string into vector of floats, delim=,
		float round(const float&);
			// Round float to nearest integer
};

#endif
