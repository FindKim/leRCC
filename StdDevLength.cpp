//
//  StdDevLength.cpp
//  
//
//  Created by Kim Ngo July 7, 2014.

//	Calculates standard deviation of sig and non-sig sequence lengths
//  Computes T-Test to see whether the difference between the two group's
//	average lengths is unlikely to have occurred because of random chance
//	in sample selection
//	Significant sequences are from Aaron's results
//	mm_orfeome files are from minmax results

#include "StdDevLength.h"

#include <string>
#include <vector>
#include <math.h>		// pow()
#include <utility>	// pair
#include <iostream>	// cout
#include <sstream>	// stringstream peak, ignore
#include <fstream>	// output file

using namespace std;

// Calculates the numerator for variance: sum (len-avg_len)^2
StdDevLength :: StdDevLength (vector<string>::iterator sigOrf_it, const vector<string>& sigOrf_v, const vector<pair<string, string> >& id_seq, const float& sig_avg_len, const float& non_sig_avg_len) {

	sig_var_numerator_sum = 0;
	non_sig_var_numerator_sum = 0;	

	calc_variance_numerator(sigOrf_it, sigOrf_v, id_seq, sig_avg_len, non_sig_avg_len);
}


float StdDevLength :: get_sig_var_numerator_sum() {
	return sig_var_numerator_sum;
}
float StdDevLength :: get_non_sig_var_numerator_sum() {
	return non_sig_var_numerator_sum;
}


// Sets where the last position the iterator pointed to
void StdDevLength :: set_sigOrf_it_pos(vector<string>::iterator it) {
	sigOrf_it = it;
}
// Returns the position the significant organism iterator was last at
vector<string>::iterator StdDevLength :: get_it_pos() {
	return sigOrf_it;
}


// Parses minmax sequence string into vector of floats
vector<float> StdDevLength :: parse_mm_seq(const string& mm_seq) {

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


// calculates variance numerator, sums num for each seq
// sigma^2 = sum((len-avg_len)^2)/tot-1
void StdDevLength :: calc_variance_numerator(vector<string>::iterator sigOrf, const vector<string>& sigOrf_v, const vector<pair<string, string> >& id_seq, const float& sig_avg_len, const float& non_sig_avg_len) {

	// Iterates through pairs in the vector id_seq
	vector< pair< string, string> >::const_iterator id_seq_it
		= id_seq.begin();	
	for (id_seq_it; id_seq_it != id_seq.end(); ++id_seq_it) {
	
		vector<float> mm_seq_v;
		mm_seq_v = parse_mm_seq(id_seq_it->second);
		float len = mm_seq_v.size();
		
		if (*sigOrf == id_seq_it->first && sigOrf+1 != sigOrf_v.end()) {

			sig_var_numerator_sum += pow((len - sig_avg_len),2);
			sigOrf++;
//			cout << "((" << len << " - " << sig_avg_len << ")^2) = " << pow((len - sig_avg_len),2) << "\t" << sig_var_numerator_sum << endl;
		} else
			non_sig_var_numerator_sum += pow((len - non_sig_avg_len),2);
	}
	set_sigOrf_it_pos(sigOrf);
}


// Calculates & returns variance for sample data
// sigma^2 = sum((len-avg_len)^2)/tot-1
float StdDevLength :: calc_variance(const float& variance_numerator, const float& num_seq) {
//	cout << "variance = " << variance_numerator << " / " << num_seq << "-1 = " << variance_numerator/(num_seq-1) << endl;
	return variance_numerator/(num_seq-1);
}


// Calculates & returns sqrt(variance of diff btwen means)
// sigma_d = sqrt[(var_1^2)/n_2 +  (var_2^2)/n_2]
long double StdDevLength :: calc_sigma_d (const float& sig_var, const float& sig_n, const float& non_sig_var, const float& non_sig_n) {

//	cout << "sigma d = (" << sig_var << "^2 / " << sig_n << " + " << non_sig_var << "^2 / " << non_sig_n << ")^.05 = " << pow( (pow(sig_var,2)/sig_n + pow(non_sig_var,2)/non_sig_n), 0.5) << endl; 
	return (long double)pow( (pow(sig_var,2)/sig_n + pow(non_sig_var,2)/non_sig_n), 0.5);
}


// Calculates & returns t value
float StdDevLength :: calc_t_value(const float& sig_avg_len, const float& non_sig_avg_len, const float& sigma_d) {

	float t = (sig_avg_len - non_sig_avg_len)/sigma_d;
	if (t < 0)
		t = (non_sig_avg_len - sig_avg_len)/sigma_d;
	
//	cout << "t = (" << sig_avg_len << " - " << non_sig_avg_len << ") / " << sigma_d << " = " << t << endl;
	return t;
}
