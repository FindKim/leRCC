#
# Histogram of the %min sum of rare codon clusters divided by bin length
# Plots to histograms: results for significant sequences and non-sig
#
#


clear
reset

set terminal postscript
set output "sig_pruned_masked_p1e-05_rcclust_sum.ps"

set xtics nomirror out
set ytics nomirror out
set xrange [0:4000]
set yrange [0:30]
#set auto x
#set auto y

set xlabel "%Min Sum for Rare Codon Clusters"
set ylabel "Number of Occurrences"

set style fill solid border -1
set style data histogram
set datafile separator ","

set title "Number of occurrences for rare codon cluster %min sums\nusing significant co-occurrence sequences across 76 ORFeome\np-Value 1e-05"
plot 'sig_pruned_masked_p1e-05_rcclust_sum.txt' u ($2) title columnheader lc rgb "#000000"

#set auto x
set xrange [0:4000]
set yrange [0:5000]

set title "Number of occurrences for rare codon cluster %min sums\nusing non-significant co-occurrence sequences across 76 ORFeome\np-Value 1e-05"
plot '' u ($3) title columnheader

#pause -1
