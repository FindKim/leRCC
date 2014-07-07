//
//  AvgSeqLength.h
//  
//
//  Created by Kim Ngo July 7, 2014.

//	Calculates average of sig and non-sig sequence lengths
//  Used for T-Test to see whether the difference between the two group's
//	average lengths is unlikely to have occurred because of random chance
//	in sample selection
//	Significant sequences are from Aaron's results
//	mm_orfeome files are from minmax results

#ifndef AVGSEQLENGTH_H
#define AVGSEQLENGTH_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class AvgSeqLength {

	public:
		AvgSeqLength(vector<string>::iterator, const vector<string>&,
			const vector<pair <string, string> >&);
			// Calculates average length for sig & non-sig sequences
		vector<string>::iterator get_it_pos();
			// Returns position iterator
		float get_sig_sum_length();
			// Returns the total sum length for significant sequences
		float get_non_sig_sum_length();
			// Returns the total sum length for non-sig seqs
		int get_sig_num_seqs();
			// Returns the number of significant sequences to account for avg
		int get_non_sig_num_seqs();
			// Returns the number of non-sig sequences to account for avg
		void add_current_to_total(float&, const float&);
			// Adds current count to total running count
		static float calc_avg(const float&, const float&);
			// Returns average of sum/num of events for avg seq length
	
	private:
		float sig_sum_length;
		float non_sig_sum_length;
		int sig_num_seqs;
		int non_sig_num_seqs;
		vector<string>::iterator sigOrf_it;
			// Position of sigOrf iterator
		void set_sigOrf_it_pos(vector<string>::iterator);
			// Sets where the last position the iterator pointed to
		vector<float> parse_mm_seq(const string&);
			// Parses minmax sequence string into vector of floats, delim=,
		void calc_sum_length(vector<string>::iterator,
			const vector<string>&, const vector<pair<string, string> >&);
			// Sums up seq length & increments # of seqs for sig & non-sig seqs
	
};

#endif
