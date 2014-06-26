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
vector<string> traverse_directory(string &directory) {

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

/*
	vector<int> tot_sig_runs(100, 0);
	vector<int> tot_non_sig_runs(100, 0);
	
//	vector<ExtractMMSeq>::iterator mm_it = mm_v.begin();
	vector<RunLength>::iterator rl_it = rl_v.begin();
	for (rl_it; rl_it != rl_v.end(); ++rl_it) {
		vector<int> sig_runs = rl_it->get_sig_runs();
		vector<int> non_sig_runs = rl_it->get_non_sig_runs();
		
		vector<int>::iterator sig_runs_it = sig_runs.begin();
		vector<int>::iterator non_sig_runs_it = non_sig_runs.begin();
		vector<int>::iterator tot_sig_runs_it = tot_sig_runs.begin();
		vector<int>::iterator tot_non_sig_runs_it = tot_non_sig_runs.begin();
		
		for (; sig_runs_it != sig_runs.end(); ++sig_runs_it, ++non_sig_runs_it, ++tot_sig_runs_it, ++tot_non_sig_runs_it) {
			*tot_sig_runs_it += *sig_runs_it;
			*tot_non_sig_runs_it += *non_sig_runs_it;
		}
	}
*/


int main() {

	int i = 0;
	string directory = "/afs/crc.nd.edu/user/k/kngo/orig_fasta/";
	vector<string> mmfiles;
	
	ExtractSigOrf sigOrf("/afs/crc.nd.edu/user/k/kngo/leRCC/sigOrfs/sigOrfs_p.05.txt");
	vector<string> sigOrf_v = sigOrf.get_sigOrf();
	vector<string>::iterator sigOrf_it = sigOrf_v.begin();
	vector<RunLength> rl_v;
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
				vector<int>::iterator sr_it = sr.begin();
				vector<int>::iterator nsr_it = nsr.begin();
/*				for (sr_it, nsr_it; sr_it != sr.end(); ++sr_it, ++nsr_it) {
					cout << "sig: " << *sr_it << "\t";
					cout << "non-sig: " << *nsr_it << endl;
				}
*/				sigOrf_it = compare.get_it_pos();
//				rl_v.push_back(compare);
			}
		}
	}
	/*
	vector<int> tot_sig_runs(100, 0);
	vector<int> tot_non_sig_runs(100, 0);
	
//	vector<ExtractMMSeq>::iterator mm_it = mm_v.begin();
	vector<RunLength>::iterator rl_it = rl_v.begin();
	for (rl_it; rl_it != rl_v.end(); ++rl_it) {
		vector<int> sig_runs = rl_it->get_sig_runs();
		vector<int> non_sig_runs = rl_it->get_non_sig_runs();
		
		vector<int>::iterator sig_runs_it = sig_runs.begin();
		vector<int>::iterator non_sig_runs_it = non_sig_runs.begin();
		vector<int>::iterator tot_sig_runs_it = tot_sig_runs.begin();
		vector<int>::iterator tot_non_sig_runs_it = tot_non_sig_runs.begin();
		
		for (; sig_runs_it != sig_runs.end(); ++sig_runs_it, ++non_sig_runs_it, ++tot_sig_runs_it, ++tot_non_sig_runs_it) {
			*tot_sig_runs_it += *sig_runs_it;
			*tot_non_sig_runs_it += *non_sig_runs_it;
		}
	}
*/}
