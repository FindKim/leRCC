//
//  ExtractSigOrf.h
//  
//
//  Created by Kim Ngo June 6, 2014.

//  Parses significant orfeome files into an array of sequence names
//	SigOrf files are from Aaron's pruning results


#include "ExtractSigOrf.h"
#include <vector>
#include <fstream> 	// reading file; check valid file
#include <iostream>	// getline
#include <utility>	// pair, make_pair
#include <string>
#include <stdlib.h> // atoi

using namespace std;

ExtractSigOrf :: ExtractSigOrf (string filename) {

	if (validFile(filename)) {
		string line;
		ifstream file(filename.c_str());
		pair<string, int> name_number_pair;
		
		while (getline(file, line)) {
			name_number_pair = createPair(line);
			sigOrf.push_back(name_number_pair);
		}
		
//		vector< pair<string, int> >::iterator it = sigOrf.begin();
//		for (it; it != sigOrf.end(); ++it) {
//			cout << it->first << ", " << it->second << endl;
//		}

	} else {
		cout << filename << " is not a valid file." << endl;
	}
}
ExtractSigOrf :: ExtractSigOrf(){}


// With an organism name, returns sequence number
int ExtractSigOrf :: get_seq_num (string organism_name) {

	vector< pair<string, int> >::iterator it = sigOrf.begin();
	for (it; it != sigOrf.end(); ++it) {
		if (it->first == organism_name) {
			cout << it->first << ", " << it->second << endl;
			return it->second;
		}
	}
	return -1;
}


// Parses line to pair of name & sequence #
pair<string, int> ExtractSigOrf :: createPair (string line) {

	string org_name;
	string seq_num_str;
	int seq_num;
	int position = line.find("|");
	
	if (position != string::npos) {	// Delimiter is found
		org_name = line.substr(0, position);
		seq_num_str = line.substr(position+1, line.size());
		seq_num = atoi(seq_num_str.c_str());
//		cout << org_name << " " << seq_num_str << " " << seq_num << endl;
		
		return make_pair (org_name, seq_num);

	} else {
		return make_pair ("Empty", 0);
	}
}


// Test for valid file
bool ExtractSigOrf :: validFile (string filename) {

	ifstream file(filename.c_str());
	
	if (file.good()
	&& filename.compare(filename.size()-4, filename.size(), ".txt") == 0
	&& filename.compare(0, 7, "sigOrfs") == 0) {
	
		return true;
	}
	return false;
}
