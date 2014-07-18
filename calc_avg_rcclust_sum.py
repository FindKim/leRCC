#!/usr/bin/python

import os
import re

pattern = re.compile('sig_pruned_masked_p.+_rcclust_sum\.txt')
#pattern = re.compile('sig_pruned_masked_p\.05_rcclust_sum\.txt')
SUM,SIG_COUNT,NSIG_COUNT = range(3)	# 0, 1, 2

file_out = open('/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_sum/avg_rcclust_sum.txt', 'w') # overwrites if exits

file_out.write('Calculated average %min sum for rare codon clusters differing by p-Value cut offs.')
file_out.write('\n')
file_out.write('\n')

for path, subdirs, files in os.walk("/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_sum"):

	for filename in files:
		if pattern.match(filename):
			print filename
			file_out.write(filename)
			file_out.write('\n')

			# Avg variables
			sig_sum_sum = 0
			sig_num_size_list = 0

			nsig_sum_sum = 0
			nsig_num_size_list = 0

			file_path = os.path.join(path, filename)
			with open(file_path) as f:
				header = f.readline()
				for line in f:
					line = line.strip()		# Removes newline char
					line = line.split(",")	# Splits floato array of 3
					sig_sum_sum += float(line[SIG_COUNT])*float(line[SUM])
					sig_num_size_list += float(line[SIG_COUNT])
					nsig_sum_sum += float(line[NSIG_COUNT])*float(line[SUM])
					nsig_num_size_list += float(line[NSIG_COUNT])

			sig_avg_sum = sig_sum_sum / sig_num_size_list
			nsig_avg_sum = nsig_sum_sum / nsig_num_size_list

			print sig_num_size_list
			print nsig_num_size_list
			'''
			print (str(sig_avg_sum) + " = " + str(sig_sum_sum) + " / " + str(sig_num_size_list))
			print (str(nsig_avg_sum) + " = " + str(nsig_sum_sum) + " / " + str(nsig_num_size_list))
			print "\n"
			'''
			file_out.write('sig avg sum: ' + str(round(sig_avg_sum,2)))
			file_out.write('\n')
			file_out.write('non-sig avg sum: ' + str(round(nsig_avg_sum,2)))
			file_out.write('\n')
			file_out.write('\n')
