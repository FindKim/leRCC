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
#include "SeqLength.h"
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

const string DIRECTORY = "/afs/crc.nd.edu/user/k/kngo/orig_fasta/";
const string SIGORF_FILE = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_unmasked/sigOrfs_p1e-05.txt";
const string OUTPUTFILE_CLUSTER_LENGTH = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_length/sig_pruned_masked_p1e-05_rcclust_length.txt";
const string OUTPUTFILE_CLUSTER_SUM = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_sum/sig_pruned_masked_p1e-05_rcclust_sum.txt";
const string OUTPUTFILE_MINVALUE = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/minvalue/sig_pruned_masked_p1e-05_minvalue.txt";
const string OUTPUTFILE_AVG_LENGTH_T_TEST = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/avg_seq_length_t_test/sigOrfs_avg_seq_length_t_test_p1e-05.txt";
const string OUTPUTFILE_SEQ_LENGTH = "/afs/crc.nd.edu/user/k/kngo/leRCC/results/avg_seq_length_t_test/sigOrfs_seq_length_p1e-05.txt";

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


/*
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ---------------------------CALCULATES RARE CODON CLUSTER SUM

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
	
	int snum_seq = 0;
	int nsnum_seq = 0;
	
	for (int i = 0; i < tot_non_sig_sums.size(); i++) {
		nsnum_seq += tot_non_sig_sums[i];
	}
	
	for (int i = 0; i < tot_non_sig_sums.size(); i++) {
		snum_seq += tot_sig_sums[i];
	}	
	
	cout << snum_seq << endl;
	cout << nsnum_seq << endl;
	
	cout << "Finished calculations." << endl;
	cout << "--------------------------------------------" << endl;
	
	string label = "%Min Sum";
	create_outputfile(outputfile_length, tot_sig_sums, tot_non_sig_sums, label);
}
*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ---------------------------CALCULATES RARE CODON CLUSTER LENGTHS
/*
int main() {

	int i = 0;
	string directory = DIRECTORY;
	string sigOrf_file = SIGORF_FILE;
	string outputfile_length = OUTPUTFILE_CLUSTER_LENGTH;

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
			if (mm.valid_file_extension(*file_it)) {

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
	int snum_seq = 0;
	int nsnum_seq = 0;
	
	for (int i = 0; i < tot_non_sig_runs.size(); i++) {
		nsnum_seq += tot_non_sig_runs[i];
	}
	
	for (int i = 0; i < tot_non_sig_runs.size(); i++) {
		snum_seq += tot_sig_runs[i];
	}	
	
	cout << snum_seq << endl;
	cout << nsnum_seq << endl;
	
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
// -----------------------------------CALCULATES SEQ LENGTH

int main() {

	long double sig_tot_sum_length = 0;
	long double non_sig_tot_sum_length = 0;
	float sig_tot_num_seq = 0;
	float non_sig_tot_num_seq = 0;
	
	vector<int> tot_sig_num_occ(300,0);
	vector<int> tot_non_sig_num_occ(300,0);

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
			if (mm.valid_file_extension(*file_it)) {
//				cout << *file_it << endl;
// CALCULATES SEQ LENGTH
				SeqLength avg(sigOrf_it, sigOrf_v, mm.get_mm_orfeome());

				vector<int> s = avg.get_sig_num_occ();
				vector<int> ns = avg.get_non_sig_num_occ();

				if (tot_sig_num_occ.size() < s.size() || tot_non_sig_num_occ.size() < ns.size()) {
					resize_vector (tot_sig_num_occ, s);
					resize_vector (tot_non_sig_num_occ, ns);
//					print_total_run(tot_sig_num_occ, tot_non_sig_num_occ);
				}
				avg.add_num_occ_v (tot_sig_num_occ, s);
				avg.add_num_occ_v (tot_non_sig_num_occ, ns);
				sigOrf_it = avg.get_it_pos();

				if (non_sig_tot_sum_length > numeric_limits<long double>::max())
					cout << "-------------------------------------------------\nTOO LARGE, CHECK CALCULATIONS\n------------------------------------------------" << endl;
			}
		}
	}
	string type = "Seq Length";
	create_outputfile(OUTPUTFILE_SEQ_LENGTH, tot_sig_num_occ, tot_non_sig_num_occ, type);

}


