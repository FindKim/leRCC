#
# Histogram of the number of rare codon clusters divided by bin length
# Plots to histograms: results for significant sequences and non-sig
#
#


clear
reset

set terminal postscript
###		set output "sigOrfs_masked/sig_pruned_masked_p.0001_length.ps"
set output "sigOrfs_masked/sig_pruned_masked_p.0001_minvalue.ps"

set xtics 0, 20 nomirror out
set ytics nomirror out
###			set xrange [0:150]
set xrange [0:100]
set auto y
###		set xlabel "Rare Codon Cluster Length"
set xlabel "Absolute %Min Value"
set ylabel "Number of Occurrences"

set style fill solid border -1
set style data histogram
set datafile separator ","

###		set title "Number of occurrences for rare codon cluster lengths\nusing significant co-occurrence sequences across 76 ORFeome\np-Value .0001"
###		plot 'sigOrfs_masked/sig_pruned_masked_p.0001_length.txt' u ($2) 
###		title columnheader lc rgb "#000000"

###		set ytics add (50000, 100000)
###		set title "Number of occurrences for rare codon cluster lengths\nusing non-significant co-occurrence sequences across 76 ORFeome\np-Value .0001"
###		plot '' u ($3) title columnheader


set title "Number of occurrences for absolute %Min value\nusing significant co-occurrence sequences across 76 ORFeome\np-Value .0001"
plot 'sigOrfs_masked/sig_pruned_masked_p.0001_minvalue.txt' u ($2) title columnheader lc rgb "#000000"

set ytics add (50000, 100000)
set title "Number of occurrences for absolute %Min value\nusing significant co-occurrence sequences across 76 ORFeome\np-Value .0001"
plot '' u ($3) title columnheader


#pause -1
