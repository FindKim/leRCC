//
//  Length_vs_Runs.h
//  
//
//  Created by Kim Ngo June 24, 2014.

//	Counts number of min runs
//  Compares min runs from significant sequences with non-sig seqs
//	SigOrf files are from Aaron's pruning results
//	mm_orfeome files are from minmax results

#ifndef EXTRACTSIGORF_H
#define EXTRACTSIGORF_H

#include <string>
#include <vector>
#include <pair>

using namespace std;

class Length_vs_Runs {

	public:
		Length_vs_Runs(vector<string>, vector< pair<string, string> >);
			// Calculates min value runs
			// Compares length of runs for sigorfeomes and non-sig
		vector<int> get_sig_runs();			// Returns # of runs for sig seqs
		vector<int> get_non_sig_runs();	// Returns # of runs for non-sig seqs
	
	private:
		vector<int> sig_runs;	// min value runs for significant seqs in bins
		vector<int> non_sig_runs;	// min value runs for non-sig seqs in bins
		void count_runs(vector<string>, vector<pair<string,string> >);
			// Increments (non)sig_runs if vec of sig seqs has consecutive mins
			// The address of the bin is the length of the run
			// Value in the addr is the number of runs of that length
};

#endif
