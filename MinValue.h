//
//  MinValue.h
//  
//
//  Created by Kim Ngo July 2, 2014.

//	Counts number of occurences for each min value
//  Compares min counts from significant sequences with non-sig seqs
//	SigOrf files are from Aaron's pruning results
//	mm_orfeome files are from minmax results

#ifndef MINVALUE_H
#define MINVALUE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class MinValue {

	public:
		MinValue(vector<string>::iterator, const vector<string>&,
			const vector< pair< string, string> >&);
			// Calculates min value runs
			// Compares length of runs for sigorfeomes and non-sig
		vector<int> get_sig_min();
			// Returns # of occ per min value for sig seqs
		vector<int> get_non_sig_min();
			// Returns # of occ per min vale for non-sig seqs
		vector<string>::iterator get_it_pos();
			// Returns position iterator
		void add_mins(vector<int>&, const vector<int>&);
			// Adds elements in second vector to first vector
		void print_min(const vector<int>&, const vector<int>&);

	private:
		vector<int> sig_min;
			// # of occ for min values for significant seqs
		vector<int> non_sig_min;
			// # of occ for min values for non-sig seqs
		vector<string>::iterator sigOrf_it;
			// Position of sigOrf iterator
		void count_min(vector<string>::iterator, const vector<string>&,
			const vector< pair< string, string> >&);
			// Increments (non)sig_min if vec for every (non)sig seqs min value
			// The address of the bin is the %min value
			// Value in the addr is the number of occurences for that value
		float round(const float&);
			// Rounds decimal min values to nearest integer

		void set_sig_min(int);		// Initializes sig_run vec with 1-100 0's
		void set_non_sig_min(int); //""
		void set_sigOrf_it_pos(vector<string>::iterator);
			// Sets where the last position the iterator pointed to
			// Used to determine next significant sequence
		vector<float> parse_mm_seq(const string&);
			// Parses minmax sequence string into vector of floats, delim=,
};

#endif
