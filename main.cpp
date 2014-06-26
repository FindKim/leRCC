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
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>	// Traverse directory
#include <stdexcept> // out of range exception
#include <algorithm> // Sort
//#include <regex>		// Regex to check for valid file ext WHY DOESN'T THE UNIV SUPPORT C++11?!

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

	vector<int>::const_iterator tot_sig_it = tot_sig_runs.begin();
	vector<int>::const_iterator tot_non_sig_it = tot_non_sig_runs.begin();
	for (int i = 0; tot_sig_it != tot_sig_runs.end(); i++, ++tot_sig_it, ++tot_non_sig_it) {
		cout << i << " sig: " << *tot_sig_it << " ";
		cout << "non-sig: " << *tot_non_sig_it << endl;
	}

// Specify print values
/*	for (int i = 0; i < 100; i++) {
		cout << i << " sig: " << tot_sig_runs[i] << " ";
		cout << "non-sig: " << tot_non_sig_runs[i] << endl;
	}
*/
}


// Resizes vector with 0 as values
void resize_vector(vector<int> &sum, const vector<int> &runs) {
	// Increases size if needed
	if (runs.size() != sum.size()) {
		int resize = runs.size();
		sum.resize(resize, 0);
		cout << "MAIN ONE RESIZED TO " << resize << endl;
	}
}


int main() {

	int i = 0;
	string directory = "/afs/crc.nd.edu/user/k/kngo/orig_fasta/";
	vector<string> mmfiles;
	vector<int> tot_sig_runs(300, 0);
	vector<int> tot_non_sig_runs(300, 0);
	
	ExtractSigOrf sigOrf("/afs/crc.nd.edu/user/k/kngo/leRCC/sigOrfs/sigOrfs_p.05.txt");
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
//				cout << *sigOrf_it << endl;
			}
		}
	}
	cout << "--------------------------------------------" << endl;
	print_total_run(tot_sig_runs, tot_non_sig_runs);
}
