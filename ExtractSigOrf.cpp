//
//  ExtractSigOrf.cpp
//  
//
//  Created by Kim Ngo June 20, 2014.

//  Parses significant orfeome files into an array of sequence names
//	SigOrf files are from Aaron's pruning results


#include "ExtractSigOrf.h"
#include <vector>
#include <algorithm> // sort
#include <fstream> 	// reading file; check valid file
#include <iostream>	// getline
#include <utility>	// pair, make_pair
#include <string>
#include <stdlib.h> // atoi
#include <sstream>	// ostringstream to convert num to string
#include <stdexcept> // out of range exception

using namespace std;

ExtractSigOrf :: ExtractSigOrf (string filename) {

	if (validFile(filename)) {
		string line;
		ifstream file(filename.c_str());
		pair<string, int> name_number_pair;

		while (getline(file, line)) {
//			v_sigOrf.push_back(line);
			name_number_pair = createPair(line);
			sigOrf.push_back(name_number_pair);
		}
		file.close();

	} else {
		cout << filename << " is not a valid file." << endl;
	}
	sort_vector_pair(sigOrf);
}


ExtractSigOrf :: ExtractSigOrf(){}


// Returns vector of sigOrf strings
vector<string> ExtractSigOrf :: get_sigOrf() {
	return v_sigOrf;
}

// Prints all significant names & seq #
void ExtractSigOrf :: print_sigOrf() {

	vector<string>::iterator it = v_sigOrf.begin();
	for (it; it != v_sigOrf.end(); ++it) {
		cout << *it << endl;
	}
}


// With an organism name, returns sequence number
int ExtractSigOrf :: get_seq_num (string organism_name) {

	vector< pair<string, int> >::iterator it = sigOrf.begin();
	for (it; it != sigOrf.end(); ++it) {
		if (it->first == organism_name) {
//			cout << it->first << ", " << it->second << endl;
			return it->second;
		}
	}
	return -1;
}


// After vector is in numerical order
// Compiles organism name & number into one string for future comparison
string ExtractSigOrf :: pair_to_string (pair<string, int> name_num_pair) {

	ostringstream ss;
	string name_num = name_num_pair.first;
	ss << name_num_pair.second;
	name_num.append("|");
	name_num.append(ss.str());
	return name_num;
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


// Sorts vector pair alphabetically
void ExtractSigOrf :: sort_vector_pair (vector< pair<string, int> > sigOrf) {
	string name_num;

	sort (sigOrf.begin(), sigOrf.end());
	for (vector<pair<string,int> >::iterator it = sigOrf.begin(); it != sigOrf.end(); ++it) {
//		cout << it->first << ' ' << it->second << endl;
		name_num = pair_to_string(*it);
		v_sigOrf.push_back(name_num);
	}
//	for (vector<string>::iterator it = v_sigOrf.begin(); it!= v_sigOrf.end(); ++it) {
//		cout << *it << endl;
//	}
}


// Test for valid file
bool ExtractSigOrf :: validFile (string filename) {

	ifstream file(filename.c_str());

	try {
		if (file.good() &&
		filename.compare(filename.size()-4, filename.size(), ".txt") == 0) {
			return true;

		} else { return false; }

	} catch (const out_of_range) {return false;}
}
