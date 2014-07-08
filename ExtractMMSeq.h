//
//  ExtractSigOrf.h
//  
//
//  Created by Kim Ngo June 23, 2014.

//  Parses minmax files (.fasta.mm.mm) into an array of name & seq pairs
//	minmax (.fasta.mm) files are from Aaron's pruning results
//	the .fasta.mm.mm are results from mmsort which sorted original files

#ifndef EXTRACTMMSEQ_H
#define EXTRACTMMSEQ_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class ExtractMMSeq {

	public:
		ExtractMMSeq(const string&);
			// Reads and parses file argument into a vector of pairs: id & seq
		vector<pair<string, string> > get_mm_orfeome();
			// Returns vector of id & seq
		bool valid_file_extension(const string&);
			// Returns T if valid file ext .fasta.mm.mm

	private:
		vector<pair<string, string> > mm_orfeome;
			// vector of id & seq pairs from an orfeome file
		void parse_file(const string&);
			// Reads file and parses every 2 lines into pair of id & seq;add to v
		void add_id_seq(pair<string, string>&);
			// Adds id&seq pair to mm_orfeome vec

};

#endif
