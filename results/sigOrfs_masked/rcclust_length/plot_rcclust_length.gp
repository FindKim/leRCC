#
# Histogram of the number of rare codon clusters divided by bin length
# Plots to histograms: results for significant sequences and non-sig
#
#


clear
reset

set terminal postscript
set output "sig_pruned_masked_p1e-05_rcclust_length.ps"

set xtics nomirror out
set ytics nomirror out
set xrange [0:100]
set auto y
set xlabel "Rare Codon Cluster Length"
set ylabel "Number of Occurrences"

set style fill solid border -1
set style data histogram
set datafile separator ","

set title "Number of occurrences for rare codon cluster lengths\nusing significant co-occurrence sequences across 76 ORFeome\np-Value 1e-05"
plot 'sig_pruned_masked_p1e-05_rcclust_length.txt' u ($2) title columnheader lc rgb "#000000"

set ytics add (50000, 100000)
set title "Number of occurrences for rare codon cluster lengths\nusing non-significant co-occurrence sequences across 76 ORFeome\np-Value 1e-05"
plot '' u ($3) title columnheader

#pause -1
