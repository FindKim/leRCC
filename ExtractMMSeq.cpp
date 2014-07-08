//
//  ExtractSigOrf.cpp
//  
//
//  Created by Kim Ngo June 23, 2014.

//  Parses minmax files (.fasta.mm.mm) into an array of name & seq pairs
//	minmax (.fasta.mm) files are from Aaron's pruning results
//	the .fasta.mm.mm are results from mmsort which sorted original files

#include "ExtractMMSeq.h"
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <stdexcept> // out of range exception

using namespace std;

ExtractMMSeq :: ExtractMMSeq (const string& filename) {

	if (valid_file_extension(filename)) {
//		cout << "Reading " << filename << endl;
		parse_file(filename);
//		cout << "Finished reading " << filename << endl;
	}
//	} else
//		cout << filename << " is an invalid file." << endl;
}


vector<pair<string, string> > ExtractMMSeq :: get_mm_orfeome() {
	return mm_orfeome;
}


// Reads file and parses every 2 lines into pair of id & seq; adds to v
void ExtractMMSeq :: parse_file(const string& filename) {

	string id, seq;
	ifstream file(filename.c_str());
	pair<string, string> id_seq_pair;

	while (!file.eof()) {
		getline(file, id);
		getline(file, seq);
		id_seq_pair = make_pair(id, seq);
		add_id_seq(id_seq_pair);
	}
}


// Returns T if valid file ext .fasta.mm.mm
bool ExtractMMSeq :: valid_file_extension(const string& filename) {

	ifstream file(filename.c_str());

	try {
		if (filename.compare(filename.size()-12, filename.size(), ".fasta.mm.mm") == 0) {
			return true;

		} else {return false;}

	} catch (const out_of_range) {return false;}
}


// Adds id&seq pair to mm_orfeome vec
void ExtractMMSeq :: add_id_seq(pair<string, string>& id_seq) {
	mm_orfeome.push_back(id_seq);
}
