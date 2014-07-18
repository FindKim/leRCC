//
//  SeqLength.cpp
//  
//
//  Created by Kim Ngo July 7, 2014.

//	Calculates average of sig and non-sig sequence lengths
//  Used for T-Test to see whether the difference between the two group's
//	average lengths is unlikely to have occurred because of random chance
//	in sample selection
//	Significant sequences are from Aaron's results
//	mm_orfeome files are from minmax results

#include "SeqLength.h"

#include <string>
#include <vector>
#include <utility>	// pair
#include <iostream>	// cout
#include <sstream>	//stringstream peak, ignore

const int NUM_CODON_WINDOW = 17;
const int NUM_MASK_AA	= 50 - NUM_CODON_WINDOW;

SeqLength :: SeqLength (vector<string>::iterator sigOrf_it, const vector<string>& sigOrf_v, const vector<pair<string, string> >& id_seq) {

	set_sig_num_occ(300);
	set_non_sig_num_occ(300);
	calc_num_occ(sigOrf_it, sigOrf_v, id_seq);
}


// Instantiates sig_run vector n 0's
void SeqLength :: set_sig_num_occ(int n) {
	vector<int> temp(n, 0);
	sig_num_occ = temp;
}
void SeqLength :: set_non_sig_num_occ(int n) {
	vector<int> temp(n, 0);
	non_sig_num_occ = temp;
}

// Returns # of runs for sig seqs
vector<int> SeqLength :: get_sig_num_occ() {
	return sig_num_occ;
}

// Returns # of runs for non-sig seqs
vector<int> SeqLength :: get_non_sig_num_occ() {
	return non_sig_num_occ;
}


// Sets where the last position the iterator pointed to
void SeqLength :: set_sigOrf_it_pos(vector<string>::iterator it) {
	sigOrf_it = it;
}
// Returns the position the significant organism iterator was last at
vector<string>::iterator SeqLength :: get_it_pos() {
	return sigOrf_it;
}


// Parses minmax sequence string into vector of floats
vector<float> SeqLength :: parse_mm_seq(const string& mm_seq) {

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


// Adds the count of each run length to the total sum
void SeqLength :: add_num_occ_v (vector<int>& sum_v, const vector<int>& v_to_add) {

	vector<int>::iterator sum_it = sum_v.begin();
	vector<int>::const_iterator v_to_add_it = v_to_add.begin();

	for (v_to_add_it; v_to_add_it != v_to_add.end(); ++sum_it, ++v_to_add_it) {
		*sum_it += *v_to_add_it;
	}
}

// Prints vector of sig & non-sig runs
void SeqLength :: print_num_occ (const vector<int>& s, const vector<int>& ns) {
/*
	vector<int>::const_iterator s_it = s.begin();
	vector<int>::const_iterator ns_it = ns.begin();
	for (int i = 0; s_it != s.end(); i++, ++s_it, ++ns_it) {
		cout << i << " sig: " << *s_it << " ";
		cout << "non-sig: " << *ns_it << endl;
	}
*/	for (int i = 0; i < 25; i++) {
		cout << i << "\t" << "sig: " << s[i] << "\t";
		cout << "non-sig: " << ns[i] << endl;
	}
}

// Counts number of occurences for each sequence length
void SeqLength :: calc_num_occ (vector<string>::iterator sigOrf, const vector<string>& sigOrf_v, const vector<pair<string, string> >& id_seq) {

		// Iterates through pairs in the vector id_seq
	vector< pair< string, string> >::const_iterator id_seq_it
		= id_seq.begin();
	for (id_seq_it; id_seq_it != id_seq.end(); ++id_seq_it) {
	
		vector<float> mm_seq_v;
		mm_seq_v = parse_mm_seq(id_seq_it->second);
		
		// Masks the first 50 amino acids considering 17 codon windows
		// Iterates through min max values and counts for min min
		if (mm_seq_v.size() > 0) {
			
			int length = mm_seq_v.size();
			if (length >= sig_num_occ.size() || length >= non_sig_num_occ.size()) {
				sig_num_occ.resize(length+1, 0);
				non_sig_num_occ.resize(length+1, 0);
//				cout << "local resize to " << length+1 << " from " << non_sig_num_occ.size() << endl;
			}
			
			if (*sigOrf == id_seq_it->first) {
				sig_num_occ[length]++;
//				cout << *sigOrf << " " << length << " " << sig_num_occ[length] << endl;
			} else {
				non_sig_num_occ[length]++;
			}
		}
		
		if (*sigOrf == id_seq_it->first && sigOrf+1 != sigOrf_v.end())
			++sigOrf;
	}
	set_sigOrf_it_pos(sigOrf);
}
