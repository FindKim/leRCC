//
//  SeqLength.h
//  
//
//  Created by Kim Ngo July 7, 2014.

//	Calculates average of sig and non-sig sequence lengths
//  Used for T-Test to see whether the difference between the two group's
//	average lengths is unlikely to have occurred because of random chance
//	in sample selection
//	Significant sequences are from Aaron's results
//	mm_orfeome files are from minmax results

#ifndef SEQLENGTH_H
#define SEQLENGTH_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class SeqLength {

	public:
		SeqLength(vector<string>::iterator, const vector<string>&,
			const vector<pair <string, string> >&);
			// Calculates average length for sig & non-sig sequences
		vector<string>::iterator get_it_pos();
			// Returns position iterator
		vector<int> get_sig_num_occ();
		vector<int> get_non_sig_num_occ();
		void add_num_occ_v(vector<int>&, const vector<int>&);
		void print_num_occ(const vector<int>&, const vector<int>&ns);

	private:
		vector<int> sig_num_occ;
		vector<int> non_sig_num_occ;
		void set_sig_num_occ(int);
		void set_non_sig_num_occ(int);
		vector<string>::iterator sigOrf_it;
			// Position of sigOrf iterator
		void set_sigOrf_it_pos(vector<string>::iterator);
			// Sets where the last position the iterator pointed to
		vector<float> parse_mm_seq(const string&);
			// Parses minmax sequence string into vector of floats, delim=,
		void calc_num_occ(vector<string>::iterator, const vector<string>&, const vector<pair<string, string> >&);

};

#endif
