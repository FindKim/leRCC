//
//  RunLength.h
//  
//
//  Created by Kim Ngo June 24, 2014.

//	Counts number of min runs
//  Compares min runs from significant sequences with non-sig seqs
//	SigOrf files are from Aaron's pruning results
//	mm_orfeome files are from minmax results

#ifndef RUNLENGTH_H
#define RUNLENGTH_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class RunLength {

	public:
		RunLength(vector<string>::iterator, vector< pair< string, string> >);
			// Calculates min value runs
			// Compares length of runs for sigorfeomes and non-sig
		vector<int> get_sig_runs();			// Returns # of runs for sig seqs
		vector<int> get_non_sig_runs();	// Returns # of runs for non-sig seqs
	
	private:
		vector<int> sig_runs;	// min value runs for significant seqs in bins
		vector<int> non_sig_runs;	// min value runs for non-sig seqs in bins
		void count_runs(vector<string>::iterator, vector< pair< string, string> >);
			// Increments (non)sig_runs if vec of sig seqs has consecutive mins
			// The address of the bin is the length of the run
			// Value in the addr is the number of runs of that length
		
		void set_sig_runs(int);			// Initializes sig_run vec with 100 0's
		void set_non_sig_runs(int); //""
		vector<float> parse_mm_seq(string);
			// Parses minmax sequence string into vector of floats, delim=,
};

#endif