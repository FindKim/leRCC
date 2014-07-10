//
//  Main.cpp
//  
//	Driver program for Sequence class
//	Reads file; parses file; prints name, description, and sequence
//
//  Created by Kim Ngo on June 20, 2014.
//
//

#include "ExtractMMSeq.h"
#include "ExtractSigOrf.h"
#include "RunLength.h"
#include "MinValue.h"
#include "AvgSeqLength.h"
#include "StdDevLength.h"
#include "RunSum.h"
#include <iostream>
#include <iomanip>	// setprecision for decmial places
#include <string>
#include <vector>
#include <math.h>		// pow()
#include <fstream>	// Output file
#include <dirent.h>	// Traverse directory
#include <algorithm> // Sort
#include <limits>		// numeric_limits to find max value of type
//#include <regex>		// Regex to check for valid file ext WHY DOESN'T THE UNIV SUPPORT C++11?!

const string DIRECTORY = "/afs/crc.nd.edu/user/k/kngo/orig_fasta/subset/";
const string SIGORF_FILE = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_unmasked/sigOrfs_p1e-05.txt";
const string OUTPUTFILE_CLUSTER_LENGTH = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_length/sig_pruned_masked_p1e-05_rcclust_length.txt";
const string OUTPUTFILE_CLUSTER_SUM = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_sum/sig_pruned_masked_p1e-05_rcclust_sum.txt";
const string OUTPUTFILE_MINVALUE = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/minvalue/sig_pruned_masked_p1e-05_minvalue.txt";
const string OUTPUTFILE_AVG_LENGTH_T_TEST = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/avg_seq_length_t_test/sigOrfs_avg_seq_length_t_test_p1e-05.txt";

using namespace std;


// Checks if provided directory exists
bool directory_exists(const char* pzPath) {
	if(pzPath == NULL) return false;
	
	DIR *pDir;
	bool bExists = false;
	
	pDir = opendir(pzPath);
	
	if(pDir != NULL) {
		bExists = true;
		(void) closedir (pDir);
	}
	return bExists;
}


// Traverses directory and creates a vector of filenames
vector<string> traverse_directory(string& directory) {

	vector<string> mmfiles;
	DIR *dpdf;
	struct dirent *epdf;
	dpdf = opendir(directory.c_str());
	if (dpdf) {
		while(true) {
			epdf = readdir(dpdf);
			if (epdf == NULL) break;
			string file = directory;
			file.append(string(epdf->d_name));	// Absolute path
			mmfiles.push_back(file);
		}
		closedir(dpdf);
		sort(mmfiles.begin(), mmfiles.end());
	}
	return mmfiles;
}


// Prints total sig & non-sig runs
void print_total_run (const vector<int>& tot_sig_runs, const vector<int>& tot_non_sig_runs) {
/*
	vector<int>::const_iterator tot_sig_it = tot_sig_runs.begin();
	vector<int>::const_iterator tot_non_sig_it = tot_non_sig_runs.begin();
	for (int i = 0; tot_sig_it != tot_sig_runs.end(); i++, ++tot_sig_it, ++tot_non_sig_it) {
		cout << i << " sig: " << *tot_sig_it << " ";
		cout << "non-sig: " << *tot_non_sig_it << endl;
	}
*/
// Specify print values
	for (int i = 0; i < 25; i++) {
		cout << i << " sig: " << tot_sig_runs[i] << " ";
		cout << "non-sig: " << tot_non_sig_runs[i] << endl;
	}
}


// Resizes vector with 0 as values
void resize_vector(vector<int> &sum, const vector<int> &runs) {
	// Increases size if needed
	if (runs.size() != sum.size()) {
		int resize = runs.size();
		sum.resize(resize, 0);
//		cout << "MAIN ONE RESIZED TO " << resize << endl;
	}
}


// Creates a column-based output file separated by ','
// last argument is what I am calculating--length or abs min value
void create_outputfile(const string& filename, const vector<int>& sig_runs, const vector<int>& non_sig_runs, string& type) {

	cout << "Creating " << filename << "..." << endl;
	
	ofstream ofile;
	ofile.open (filename.c_str());
	
	if (ofile.is_open()) {
	
		ofile << type << ",Sig,Non-Sig" << endl;
		vector<int>::const_iterator sig_it = sig_runs.begin();
		vector<int>::const_iterator non_sig_it = non_sig_runs.begin();
		for (int i = 0; sig_it != sig_runs.end();
		i++, ++sig_it, ++non_sig_it) {
			
			ofile << i << "," << *sig_it << "," << *non_sig_it << endl;
		}
		ofile.close();
		cout << filename << " has been created." << endl;

	} else cout << "Unable to open " << filename << endl;
}


// Creates output file that writes # elements, avg, variance, std dev,
// t value, and determines if diff btwn avg are significantly different
void create_outputfile_t_test(const string& outputfile, const float& sig_tot_num_seq, const float& non_sig_tot_num_seq, const float& sig_avg, const float& non_sig_avg, const float& sig_var, const float& non_sig_var, const float& sig_std_dev, const float& non_sig_std_dev, const float& t_value) {

	cout << "Creating " << outputfile << "..." << endl;
	ofstream ofile;
	ofile.open (outputfile.c_str());
	
	if (ofile.is_open()) {
		ofile << "# t test comparing average sequence lengths of significant & non-sig seqs\n# p-Value 1e-05" << endl << endl;
		ofile << setw(12) << "Variable" << setw(12) << "Sig" << setw(12) << "Non-Sig" << endl;

		ofile << setw(12) << "n elements" << setw(12)
			<< sig_tot_num_seq << setw(12) << non_sig_tot_num_seq << endl;

		ofile.precision(2); // fixed format for two decimal places
		ofile << fixed;
		
		ofile << setw(12) << "Average" << setw(12)
			<< sig_avg << setw(12) << non_sig_avg << endl;

		ofile << setw(12) << "Variance" << setw(12)
			<< sig_var << setw(12) << non_sig_var << endl;

		ofile << setw(12) << "Std Dev" << setw(12)
			<< sig_std_dev << setw(12) << non_sig_std_dev << endl;
		
		ofile << endl;
		ofile << "t Value: " << t_value << endl;
		ofile << endl;

		float degree_of_freedom = (sig_tot_num_seq + non_sig_tot_num_seq)-2;
		if (degree_of_freedom > 120)
			if (t_value > 3.29)
				ofile << "A significant difference at probability = 0.001" << endl;
			else if (t_value > 2.58)
				ofile << "A significant difference at probability = 0.01." << endl;
			else if (t_value > 1.96)
				ofile << "A significant difference at probability = 0.05" << endl;
			else if (t_value > 1.65)
				ofile << "A significant difference at probability = 0.1" << endl;
			else
				ofile << "No siginificant difference." << endl;
		
		else
			ofile << "Select a smaller degree of freedom value from t table." << endl;

		ofile.close();
		cout << outputfile << " has been created." << endl;

	} else cout << "Unable to open " << outputfile << endl;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ---------------------------CALCULATES RARE CODON CLUSTER LENGTHS

int main() {

	int i = 0;
	string directory = DIRECTORY;
	string sigOrf_file = SIGORF_FILE;
	string outputfile_length = OUTPUTFILE_CLUSTER_SUM;

	vector<string> mmfiles;
	vector<int> tot_sig_sums(300, 0);
	vector<int> tot_non_sig_sums(300, 0);
	
	ExtractSigOrf sigOrf(sigOrf_file);
	vector<string> sigOrf_v = sigOrf.get_sigOrf();
	vector<string>::iterator sigOrf_it = sigOrf_v.begin();

	if (directory_exists(directory.c_str())) {
	
		// Traverses directory and returns vector of filenames
		mmfiles = traverse_directory(directory);
		cout << "Calculating rare codon cluster %min sums..." << endl;
		
		// Iterates through each file
		vector<string>::iterator file_it = mmfiles.begin();
		for (file_it; file_it != mmfiles.end(); ++file_it) {

			ExtractMMSeq mm(*file_it);

			// Valid file with extension ".fasta.mm.mm"--sorted .mm file
			if (mm.valid_file_extension(*file_it)) {;

// COUNTS RARE CODON CLUSTER LENGTHS
				RunSum compare(sigOrf_it, sigOrf_v, mm.get_mm_orfeome());

				vector<int> sr = compare.get_sig_sums();
				vector<int> nsr = compare.get_non_sig_sums();				
				if (tot_sig_sums.size() < sr.size() || tot_non_sig_sums.size() < nsr.size()) {
					resize_vector (tot_sig_sums, sr);
					resize_vector (tot_non_sig_sums, nsr);
//					print_total_run(tot_sig_sums, tot_non_sig_sums);
				}
				compare.add_sums (tot_sig_sums, sr);
				compare.add_sums (tot_non_sig_sums, nsr);
				sigOrf_it = compare.get_it_pos();
//				compare.print_runs(sr, nsr);
//				print_total_run(tot_sig_runs, tot_non_sig_runs);
//				cout << *sigOrf_it << endl;
			}
		}
	}
	cout << "Finished calculations." << endl;
	cout << "--------------------------------------------" << endl;
	
	string label = "%Min Sum";
	create_outputfile(outputfile_length, tot_sig_sums, tot_non_sig_sums, label);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ---------------------------CALCULATES RARE CODON CLUSTER LENGTHS
/*
int main() {

	int i = 0;
	string directory = DIRECTORY;
	string sigOrf_file = SIGORF_FILE;
	string outputfile_length = OUTPUTFILE_LENGTH;

	vector<string> mmfiles;
	vector<int> tot_sig_runs(300, 0);
	vector<int> tot_non_sig_runs(300, 0);
	
	ExtractSigOrf sigOrf(sigOrf_file);
	vector<string> sigOrf_v = sigOrf.get_sigOrf();
	vector<string>::iterator sigOrf_it = sigOrf_v.begin();

	if (directory_exists(directory.c_str())) {
	
		// Traverses directory and returns vector of filenames
		mmfiles = traverse_directory(directory);
		
		// Iterates through each file
		vector<string>::iterator file_it = mmfiles.begin();
		for (file_it; file_it != mmfiles.end(); ++file_it) {

			ExtractMMSeq mm(*file_it);

			// Valid file with extension ".fasta.mm.mm"--sorted .mm file
			if (mm.valid_file_extension(*file_it)) {;

// COUNTS RARE CODON CLUSTER LENGTHS
				RunLength compare(sigOrf_it, sigOrf_v, mm.get_mm_orfeome());

				vector<int> sr = compare.get_sig_runs();
				vector<int> nsr = compare.get_non_sig_runs();				
				if (tot_sig_runs.size() < sr.size() || tot_non_sig_runs.size() < nsr.size()) {
					resize_vector (tot_sig_runs, sr);
					resize_vector (tot_non_sig_runs, nsr);
//					print_total_run(tot_sig_runs, tot_non_sig_runs);
				}
				compare.add_runs (tot_sig_runs, sr);
				compare.add_runs (tot_non_sig_runs, nsr);
				sigOrf_it = compare.get_it_pos();
//				compare.print_runs(sr, nsr);
//				print_total_run(tot_sig_runs, tot_non_sig_runs);
//				cout << *sigOrf_it << endl;
			}
		}
	}
	cout << "--------------------------------------------" << endl;
	
	string length = "Length";
	create_outputfile(outputfile_length, tot_sig_runs, tot_non_sig_runs, length);
}
*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// -------------------------------------COUNTS MIN VALUE OCCURENCES
/*
int main() {

	int i = 0;
	string directory = DIRECTORY;
	string sigOrf_file = SIGORF_FILE;
	string outputfile_minvalue = OUTPUTFILE_MINVALUE;

	vector<string> mmfiles;
	vector<int> tot_sig_min(101, 0);
	vector<int> tot_non_sig_min(101, 0);
	
	ExtractSigOrf sigOrf(sigOrf_file);
	vector<string> sigOrf_v = sigOrf.get_sigOrf();
	vector<string>::iterator sigOrf_it = sigOrf_v.begin();
//	sigOrf.print_sigOrf();

	if (directory_exists(directory.c_str())) {
	
		// Traverses directory and returns vector of filenames
		mmfiles = traverse_directory(directory);
		
		// Iterates through each file
		vector<string>::iterator file_it = mmfiles.begin();
		for (file_it; file_it != mmfiles.end(); ++file_it) {

			ExtractMMSeq mm(*file_it);

			// Valid file with extension ".fasta.mm.mm"--sorted .mm file
			if (mm.valid_file_extension(*file_it)) {
//				cout << *file_it << endl;

// COUNTS OCCURENCES OF MIN VALUES
				MinValue mv(sigOrf_it, sigOrf_v, mm.get_mm_orfeome());
				vector<int> sm = mv.get_sig_min();
				vector<int> nsm = mv.get_non_sig_min();
				
				mv.add_mins (tot_sig_min, sm);
				mv.add_mins (tot_non_sig_min, nsm);
				sigOrf_it = mv.get_it_pos();
//				cout << *sigOrf_it << endl;
			}
		}
	}
	cout << "--------------------------------------------" << endl;
	
//	print_total_run(tot_sig_min, tot_non_sig_min);
	string minvalue = "Abs Min Value";
	create_outputfile(outputfile_minvalue, tot_sig_min, tot_non_sig_min, minvalue);
}
*/
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ---------------------------------------CALCULATES AVG SEQ LENGTH
// ---------------------------------------CALCULATES STD_DEV LENGTH
// -------------------------------------------------COMPUTES T TEST
/*
int main() {

	long double sig_tot_sum_length = 0;
	long double non_sig_tot_sum_length = 0;
	float sig_tot_num_seq = 0;
	float non_sig_tot_num_seq = 0;

	string directory = DIRECTORY;
	string sigOrf_file = SIGORF_FILE;
	string outputfile = OUTPUTFILE_AVG_LENGTH_T_TEST;

	vector<string> mmfiles;
	
	ExtractSigOrf sigOrf(sigOrf_file);
	vector<string> sigOrf_v = sigOrf.get_sigOrf();
	vector<string>::iterator sigOrf_it = sigOrf_v.begin();

	if (directory_exists(directory.c_str())) {
	
		// Traverses directory and returns vector of filenames
		mmfiles = traverse_directory(directory);
		
		cout << "Computing average for " << SIGORF_FILE << endl;
		
		// Iterates through each file
		vector<string>::iterator file_it = mmfiles.begin();
		for (file_it; file_it != mmfiles.end(); ++file_it) {

			ExtractMMSeq mm(*file_it);

			// Valid file with extension ".fasta.mm.mm"--sorted .mm file
			if (mm.valid_file_extension(*file_it)) {;

// CALCULATES AVG SEQ LENGTH
				AvgSeqLength avg(sigOrf_it, sigOrf_v, mm.get_mm_orfeome());
				
				sig_tot_sum_length += avg.get_sig_sum_length();
				non_sig_tot_sum_length += avg.get_non_sig_sum_length();
				sig_tot_num_seq += avg.get_sig_num_seqs();
				non_sig_tot_num_seq += avg.get_non_sig_num_seqs();
				
				sigOrf_it = avg.get_it_pos();
				
				if (non_sig_tot_sum_length > numeric_limits<long double>::max())
					cout << "-------------------------------------------------\nTOO LARGE, CHECK CALCULATIONS\n------------------------------------------------" << endl;


//				cout << "next sigOrf is " << *sigOrf_it << endl;
//				cout << *file_it << endl;
//				cout << "Sig Length:\t" << avg.get_sig_sum_length() << "\t" << sig_tot_sum_length << endl;
//				cout << "Sig num:\t" << avg.get_sig_num_seqs() << "\t" << sig_tot_num_seq << endl;
//				cout << "Nsig Length:\t" << avg.get_non_sig_sum_length() << "\t" << non_sig_tot_sum_length << endl;
//				cout << "Nsig num:\t" << avg.get_non_sig_num_seqs() << "\t" << non_sig_tot_num_seq << endl;
//				cout << endl;
			}
		}
	}
	cout << "Computing variance for " << SIGORF_FILE << endl;

	float sig_variance_numerator = 0;
	float non_sig_variance_numerator = 0;
	float sig_variance = 0;
	float non_sig_variance = 0;
	float sig_std_dev = 0;
	float non_sig_std_dev = 0;
	long double sigma_d = 0;
	float t_value = 0;

	float sig_avg = AvgSeqLength::calc_avg (sig_tot_sum_length, sig_tot_num_seq);
	float non_sig_avg = AvgSeqLength::calc_avg (non_sig_tot_sum_length, non_sig_tot_num_seq);

//	cout << "SIG AVG:\t" << sig_avg << endl;
//	cout << "SIG #SEQ:\t" << sig_tot_num_seq << endl;
//	cout << "NSIG AVG:\t" << non_sig_avg << endl;
//	cout << "NSIG #SEQ:\t" << non_sig_tot_num_seq << endl;

// CALCULATES STD DEVIATION
	// Iterates through each file
	sigOrf_it = sigOrf_v.begin();
	vector<string>::iterator file_it = mmfiles.begin();
	for (file_it; file_it != mmfiles.end(); ++file_it) {

		ExtractMMSeq mm(*file_it);

		// Valid file with extension ".fasta.mm.mm"--sorted .mm file
		if (mm.valid_file_extension(*file_it)) {
			
			StdDevLength t(sigOrf_it, sigOrf_v, mm.get_mm_orfeome(), sig_avg, non_sig_avg);
			sig_variance_numerator += t.get_sig_var_numerator_sum();
			non_sig_variance_numerator += t.get_non_sig_var_numerator_sum();
						
			sigOrf_it = t.get_it_pos();
		}
	}

	cout << "Computing t test for " << SIGORF_FILE << endl;

	sig_variance = StdDevLength::calc_variance (sig_variance_numerator, sig_tot_num_seq);
	non_sig_variance = StdDevLength::calc_variance  (non_sig_variance_numerator, non_sig_tot_num_seq);
	
	// sigma_d = sqrt(variance of difference btwn means)
	sigma_d = StdDevLength::calc_sigma_d (sig_variance, sig_tot_num_seq, non_sig_variance, non_sig_tot_num_seq);
	
	// sig avg, non-sig avg, sigma_d
	t_value = StdDevLength::calc_t_value (sig_avg, non_sig_avg, sigma_d);

	create_outputfile_t_test(outputfile, sig_tot_num_seq, non_sig_tot_num_seq, sig_avg, non_sig_avg, sig_variance, non_sig_variance, pow(sig_variance, 0.5), pow(non_sig_variance, 0.5), t_value);
}
*/

