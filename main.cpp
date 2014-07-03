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
#include <iostream>
#include <string>
#include <vector>
#include <fstream>	// Output file
#include <dirent.h>	// Traverse directory
#include <algorithm> // Sort
//#include <regex>		// Regex to check for valid file ext WHY DOESN'T THE UNIV SUPPORT C++11?!

const string DIRECTORY = "/afs/crc.nd.edu/user/k/kngo/orig_fasta/";
const string SIGORF_FILE = "/afs/crc.nd.edu/user/k/kngo/leRCC/sigOrfs_unmasked/sigOrfs_p1e-05.txt";
const string OUTPUTFILE_LENGTH = "sigOrfs_masked/sig_pruned_masked_p1e-05_length.txt";
const string OUTPUTFILE_MINVALUE = "sigOrfs_masked/sig_pruned_masked_p1e-05_minvalue.txt";

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
void create_output_file(const string& filename, const vector<int>& sig_runs, const vector<int>& non_sig_runs, string& type) {

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


int main() {

	int i = 0;
	string directory = DIRECTORY;
	string sigOrf_file = SIGORF_FILE;
	string outputfile_length = OUTPUTFILE_LENGTH;
	string outputfile_minvalue = OUTPUTFILE_MINVALUE;
/*	string sigOrf_file;
	string outputfile;
	cout << "sigOrf_p file\n>";
	cin >> sigOrf_file;
	cout << "output file name\n>";
	cin >> outputfile;
*/
	vector<string> mmfiles;
	vector<int> tot_sig_runs(300, 0);
	vector<int> tot_non_sig_runs(300, 0);
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
/*				
				mv.print_min (sm, nsm);
				cout << endl << "TOTAL COUNT" << endl;
				mv.print_min (tot_sig_min, tot_non_sig_min);
				cout << endl;
*/
/* COUNTS RARE CODON CLUSTER LENGTHS
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
*/
			}
		}
	}
	cout << "--------------------------------------------" << endl;
	
	string minvalue = "Abs Min Value";
	string length = "Length";
//	print_total_run(tot_sig_min, tot_non_sig_min);
	create_output_file(outputfile_minvalue, tot_sig_min, tot_non_sig_min, minvalue);
//	print_total_run(tot_sig_runs, tot_non_sig_runs);
//	create_output_file(outputfile_length, tot_sig_runs, tot_non_sig_runs, length);
}
