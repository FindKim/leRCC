#!/usr/bin/python

import os
import re

pattern = re.compile('sig_pruned_masked_p.+_results\.txt')
LENGTH,SIG_COUNT,NSIG_COUNT = range(3)	# 0, 1, 2

file_out = open('/afs/crc.nd.edu/user/k/kngo/leRCC/sigOrfs_masked/std_dev_rcclust_length.txt', 'w') # overwrites if exits

file_out.write('Calculated standard deviation for rare codon cluster length differing by p-Value cut offs.')
file_out.write('\n')
file_out.write('\n')

for path, subdirs, files in os.walk("/afs/crc.nd.edu/user/k/kngo/leRCC/sigOrfs_masked/"):

	for filename in files:
		if pattern.match(filename):
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
			
			f.close();
			
			# Std deviation variables
			sig_sum_val_avg_sqr_diff = 0; # std deviation: sum += (val-avg)^2
			nsig_sum_val_avg_sqr_diff = 0; # std deviation: sum += (val-avg)^2
			
			with open(file_path) as f:
#				print file_path
				header = f.readline()
				for line in f:
					line = line.strip()
					line = line.split(",")
					
					sig_sum_val_avg_sqr_diff += ((float(line[LENGTH])-sig_avg_length)**2)*float(line[SIG_COUNT])
					nsig_sum_val_avg_sqr_diff += ((float(line[LENGTH])-nsig_avg_length)**2)*float(line[NSIG_COUNT])

					'''					
					print "sig"
					print '(' + line[LENGTH] + '-' + str(sig_avg_length) + ')^2 *' + line[SIG_COUNT] + ' = ' + str(((float(line[LENGTH])-sig_avg_length)**2)*float(line[SIG_COUNT]))
					print str(sig_sum_val_avg_sqr_diff)
					print "non-sig"
					print '(' + line[LENGTH] + '-' + str(nsig_avg_length) + '^2) *' + line[NSIG_COUNT] + ' = ' + str(((float(line[LENGTH])-nsig_avg_length)**2)*float(line[NSIG_COUNT]))
					print str(nsig_sum_val_avg_sqr_diff)
					print '\n'
					'''
					
				sig_std_dev = (sig_sum_val_avg_sqr_diff/(sig_num_size_list-1))**(0.5)
				nsig_std_dev = (nsig_sum_val_avg_sqr_diff/(nsig_num_size_list-1))**(0.5)
				
				file_out.write('sig std dev: ' + str(round(sig_std_dev,2)))
				file_out.write('\n')
				file_out.write('non-sig std dev: ' + str(round(nsig_std_dev,2)))
				file_out.write('\n')
				file_out.write('\n')
				
				
