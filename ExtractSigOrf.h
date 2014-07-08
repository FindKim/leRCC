//
//  ExtractSigOrf.h
//  
//
//  Created by Kim Ngo June 20, 2014.

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
		vector<string> get_sigOrf(); // Returns vector of sigOrf strings
		void print_sigOrf();			// Prints all significant names & seq #

	private:
		vector<string> v_sigOrf;
			// Vector that contains all organism name & seq# in sigorf cutoff
		vector<int> v_sigOrf_seq_num;
			// Vector that contains all seq#;
		vector< pair<string, int> > sigOrf;
			// Pair of organism name and sequence number

		string pair_to_string (pair<string, int>);	// Converts pair to str
		pair<string, int> createPair(string);	// Parse line to pair: name & #
		int get_seq_num(string);	// Returns sig oseq # w/organism name
		bool validFile(string);		// Returns T if valid file w/"sigOrfs*.txt"
		void sort_vector_pair(vector< pair<string, int> >);
			// Sorts orgs alphabetically
};

#endif /* defined(____ExtractSigOrf__) */
