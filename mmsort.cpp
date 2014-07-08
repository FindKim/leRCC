//
//  mmsort.cpp
//  
//
//  Created by Kim Ngo on June 23, 2014.

//  Reads in .mm files in a given directory
//  Sorts sequences alphabetically/numerically

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>	// outfile
#include <vector>
#include <utility> // Pair, make_pair;
#include <algorithm> // Sort
#include <dirent.h>
#include <string.h> // strlen
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

bool compare_ext(const char *filename, const char *extension) {

	int file_len = strlen(filename);
	int ext_len = strlen(extension);
	for (int i = file_len-ext_len, j = 0; i < file_len; i++, j++) {
		if (filename[i] != extension[j])
			return false;
	}
	return true;
}


// Parses line to pair of name & sequence #
pair<string, int> create_name_num_pair (string line) {

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

// After vector is in numerical order
// Compiles organism name & number into one string for future comparison
string pair_to_string (pair<string, int> name_num_pair) {

	ostringstream ss;
	string name_num = name_num_pair.first;
	ss << name_num_pair.second;
	name_num.append("|");
	name_num.append(ss.str());
	return name_num;
}


pair< pair<string, int>, string> create_id_seq_pair(pair<string, int> id, string seq) {
	return make_pair(id, seq);
}


vector< pair<string, string> > compile_v(vector< pair< pair<string, int>, string> > initial_v) {
	vector< pair<string, string> > final_v;
	pair<string, string> pair_nn;
	string id;

	vector< pair< pair<string, int>, string> >::iterator it = initial_v.begin();
	for(it; it != initial_v.end(); ++it) {
		id = pair_to_string(it->first);
		pair_nn = make_pair(id, it->second);
		final_v.push_back(pair_nn);
	}
	return final_v;
}

int main() {

	string extension = ".fasta.mm";
	string path_dir = "/afs/crc.nd.edu/user/k/kngo/orig_fasta";
	string header; // String with name & seq #
	string seq; // String of sequence
	pair<string, int> pair_name_seq_num; // Name & int value of seq_num
	pair< pair<string, int>, string> pair_id_seq; // Name, int val, seq
	pair<string, string> pair_name_seq;		// First: name&#; Second: seq
	vector< pair<string, string> > orfeome;

	// Input directory to read all .fasta files
	if(DirectoryExists(path_dir.c_str())) {

		// Read all files in directory
		DIR *dpdf;
		struct dirent *epdf;
		dpdf = opendir(path_dir.c_str());
		if (dpdf != NULL) {
			while (epdf = readdir(dpdf)) {

				// Check for valid file extension
				string file = string(epdf->d_name);
				string path_file = path_dir;
				path_file.append("/");
				path_file.append(file);		// Full path to file
//				cout << path_file << " " << file << endl;

				try {
					if (compare_ext(file.c_str(), extension.c_str())) {
//					if (file.compare_ext(file.size()-8, 8, "fasta.mm") == 0) {
						vector< pair< pair<string, int>, string> > v_name_num_seq;	// Name, seq#, seq
						ifstream ifile;
						ifile.open(path_file.c_str());
						cout << "Reading " << path_file << "..." << endl;
						while(!ifile.eof()) {
			        getline(ifile, header);
			        getline(ifile, seq);
			        pair_name_seq_num = create_name_num_pair(header);
			        pair_id_seq = create_id_seq_pair(pair_name_seq_num, seq);
			        v_name_num_seq.push_back(pair_id_seq);
//			        cout << header << endl;
//			        cout << seq << endl;
						}
						cout << "Closing " << path_file << endl;
						ifile.close();

						cout << "Sorting " << path_file << endl;
						sort (v_name_num_seq.begin(), v_name_num_seq.end());
						vector< pair<string, string> > orfeome;
						orfeome = compile_v(v_name_num_seq);

						string ofilename = path_file;
						ofilename.append(".mm");
						cout << "Creating " << ofilename << endl;
						ofstream ofile;
						ofile.open (ofilename.c_str());
						if (ofile.is_open()) {

							vector< pair<string, string> >::iterator it = orfeome.begin();
							for (it; it != orfeome.end(); ++it) {
								if (it->first != "Empty|0") {
									ofile << it->first;
									ofile << endl;
									ofile << it->second;
									ofile << endl;
								}
							}
						}
						ofile.close();
						cout << ofilename << " has been created." << endl;

					}// else cout << "Unable to open " << file << endl;

	//								cout << epdf->d_name << endl;
	//								cout << file << endl;
				} catch (const out_of_range) {}
			}
		}
	}
}
