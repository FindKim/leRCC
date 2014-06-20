//
//  ExtractSigOrf.h
//  
//
//  Created by Kim Ngo June 6, 2014.

//  Parses significant orfeome files into an array of sequence names
//	SigOrf files are from Aaron's pruning results

#ifndef EXTRACTSIGORF_H
#define EXTRACTSIGORF_H

#include <string>
#include <vector>
#include <utility> // Pair

using namespace std;

class ExtractSigOrf {

	public:
		ExtractSigOrf();
		ExtractSigOrf(string);		// SigOrf filename;
			// Parses sigOrf file to individual organism & seq #
		pair<string, int> createPair(string);	// Parses line to pair of name & #
		int get_seq_num(string);	// Returns sig oseq # w/organism name
		bool validFile(string);		// Returns T if valid file w/"sigOrfs*.txt"

	private:
		vector<string> v_sigOrf_organism;
			// Vector that contains all organism name & seq# in sigorf cutoff
		vector<int> v_sigOrf_seq_num;
			// Vector that contains all seq#;
		vector< pair<string, int> > sigOrf;
			// Pair of organism name and sequence number
};

#endif /* defined(____ExtractSigOrf__) */
