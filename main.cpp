//
//  Main.cpp
//  
//	Driver program for Sequence class
//	Reads file; parses file; prints name, description, and sequence
//
//  Created by Kim Ngo on 14-3-15.
//
//

#include "ExtractMMSeq.h"
#include "ExtractSigOrf.h"
#include "RunLength.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
//#include <regex>		// Regex to check for valid file ext WHY DOESN'T THE UNIV SUPPORT C++11?!
#include "stdlib.h"	// System command
#include <dirent.h>
#include <utility>	// pair, make_pair
#include <stdexcept> // out of range exception

using namespace std;

bool DirectoryExists(const char* pzPath) {
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


int main() {

//	string file;
//	cin >> file;
	ExtractSigOrf sigOrf("/afs/crc.nd.edu/user/k/kngo/leRCC/sigOrfs/sigOrfs_p.05.txt");
//	sigOrf.print_sigOrf();

	ExtractMMSeq mm("/afs/crc.nd.edu/user/k/kngo/orig_fasta/Aaeo.fasta.mm.mm");

	vector<string> sigOrf_v = sigOrf.get_sigOrf();
	vector<string>::iterator sigOrf_it = sigOrf_v.begin();
//	for (sigOrf_it; sigOrf_it != sigOrf_v.end(); ++sigOrf_it)
		RunLength compare(sigOrf_it, mm.get_mm_orfeome());
}
