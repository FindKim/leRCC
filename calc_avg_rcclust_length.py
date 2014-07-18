#!/usr/bin/python

import os
import re

pattern = re.compile('sig_pruned_masked_p.+_length\.txt')
#pattern = re.compile('sig_pruned_masked_p\.05_rcclust_length\.txt')
LENGTH,SIG_COUNT,NSIG_COUNT = range(3)	# 0, 1, 2

file_out = open('/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_length/avg_rcclust_length.txt', 'w') # overwrites if exits

file_out.write('Calculated average length for rare codon clusters differing by p-Value cut offs.')
file_out.write('\n')
file_out.write('\n')

for path, subdirs, files in os.walk("/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_length/"):

	for filename in files:
		if pattern.match(filename):
			print filename
			file_out.write(filename)
			file_out.write('\n')

			# Avg variables
			sig_sum_length = 0
			sig_num_size_list = 0

			nsig_sum_length = 0
			nsig_num_size_list = 0

			file_path = os.path.join(path, filename)
			with open(file_path) as f:
				header = f.readline()
				for line in f:
					line = line.strip()		# Removes newline char
					line = line.split(",")	# Splits floato array of 3

					sig_sum_length += float(line[SIG_COUNT])*float(line[LENGTH])
					sig_num_size_list += float(line[SIG_COUNT])
					nsig_sum_length += float(line[NSIG_COUNT])*float(line[LENGTH])
					nsig_num_size_list += float(line[NSIG_COUNT])

			sig_avg_length = sig_sum_length / sig_num_size_list
			nsig_avg_length = nsig_sum_length / nsig_num_size_list

			print (sig_num_size_list)
			print (nsig_num_size_list)
			file_out.write('sig avg length: ' + str(round(sig_avg_length,2)))
			file_out.write('\n')
			file_out.write('non-sig avg length: ' + str(round(nsig_avg_length,2)))
			file_out.write('\n')
			file_out.write('\n')
