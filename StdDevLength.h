//
//  StdDevLength.h
//  
//
//  Created by Kim Ngo July 7, 2014.

//	Calculates standard deviation of sig and non-sig sequence lengths
//  Computes T-Test to see whether the difference between the two group's
//	average lengths is unlikely to have occurred because of random chance
//	in sample selection
//	Significant sequences are from Aaron's results
//	mm_orfeome files are from minmax results

#ifndef STDDEVLENGTH_H
#define STDDEVLENGTH_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class StdDevLength {

	public:
		StdDevLength(vector<string>::iterator, const vector<string>&,
			const vector<pair <string, string> >&, const float&, const float&);
			// Iterator for sig seqs, vector of sig seqs, pair of id & seq,
			// sig avg length, non sig avg length
			// Calculates the numerator for variance: sum (len-avg_len)^2
		float get_sig_var_numerator_sum();
		float get_non_sig_var_numerator_sum();
		vector<string>::iterator get_it_pos();
			// Returns position iterator
		static float calc_variance(const float&, const float&);
			// variance numerator, # of elements
			// Calculates & returns variance for sample data
			// sigma^2 = sum((len-avg_len)^2)/tot-1
		static long double calc_sigma_d (const float&, const float&, const float&, const float&);
			// Calculates & returns sqrt(variance of diff btwen means)
			// sigma_d = sqrt[(var_1^2)/n_2 +  (var_2^2)/n_2]
		static float calc_t_value(const float&, const float&, const float&);
			// sig avg len, non sig avg len, sqrt(variance of diff btwn means)
			// Calculates and returns t value
	
	private:	
		float sig_var_numerator_sum;
		float non_sig_var_numerator_sum;
		
		vector<string>::iterator sigOrf_it;
			// Position of sigOrf iterator
		void set_sigOrf_it_pos(vector<string>::iterator);
			// Sets where the last position the iterator pointed to
		vector<float> parse_mm_seq(const string&);
		void calc_variance_numerator(vector<string>::iterator,
			const vector<string>&, const vector<pair <string, string> >&,
			const float&, const float&);
			// calculates variance numerator, sums num for each seq
			// sigma^2 = sum((len-avg_len)^2)/tot-1

};

#endif
