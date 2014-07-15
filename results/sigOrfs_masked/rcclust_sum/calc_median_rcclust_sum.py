#!/usr/bin/python

import os
import re
import math

def median(myList):
	myList.sort()
	
	# If even list, avg middle two
	if len(myList)%2 == 0:
		i = len(myList)/2
		avg = float(myList[i-1] + myList[i])/2
		median = avg
	
	# If odd list, middle number
	else:
		i = int(math.floor(len(myList)/2))
		median = myList[i]
	return median
'''
myList = [2, 50, 2, 40]
print myList
sorted(myList)
print myList
med = median(myList)
print med
'''
pattern = re.compile('sig_pruned_masked_p.*_rcclust_sum\.txt')
#pattern = re.compile('sig_pruned_masked_p\.05_rcclust_length\.txt')
LENGTH,SIG_COUNT,NSIG_COUNT = range(3)	# 0, 1, 2

file_out = open('/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_sum/median_rrclust_sum.txt', 'w') # overwrites if exits

file_out.write('Calculated median rare codon cluster %min sum for significant and non-significant sequences differing by p-Value cut offs.')
file_out.write('\n')
file_out.write('\n')

for path, subdirs, files in os.walk("/afs/crc.nd.edu/user/k/kngo/leRCC/results/sigOrfs_masked/rcclust_sum"):

	for filename in files:
		if pattern.match(filename):
			print filename
			file_out.write(filename)
			file_out.write('\n')

			# Lists of lengths
			sig_length_list = list()
			nsig_length_list = list()

			file_path = os.path.join(path, filename)
			with open(file_path) as f:
				header = f.readline()
				for line in f:
					line = line.strip()		# Removes newline char
					line = line.split(",")	# Splits floato array of 3
					for i in range(0, int(line[SIG_COUNT])):
						sig_length_list.append(int(line[LENGTH]))
					for i in range(0, int(line[NSIG_COUNT])):
						nsig_length_list.append(int(line[LENGTH]))

			sorted(sig_length_list, key=int)
			sorted(nsig_length_list, key=int)

			print (median(sig_length_list))
			print (median(nsig_length_list))
			sig_median = median(sig_length_list)
			nsig_median = median(nsig_length_list)
			file_out.write('sig median seq length: ' + str(round(sig_median,2)))
			file_out.write('\n')
			file_out.write('non-sig median seq length: ' + str(round(nsig_median,2)))
			file_out.write('\n')
			file_out.write('\n')
