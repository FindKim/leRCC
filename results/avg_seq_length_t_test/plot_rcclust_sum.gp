#
# Histogram of the %min sum of rare codon clusters divided by bin length
# Plots to histograms: results for significant sequences and non-sig
#
#


clear
reset

set terminal postscript
set output "sigOrfs_seq_length_p1e-05.ps"

set xtics nomirror out
set ytics nomirror out
set xrange [0:4000]
#set yrange [0:100]
#set auto x
set auto y

set xlabel "Sequence Length"
set ylabel "Number of Occurrences"

set style fill solid border -1
set style data histogram
set datafile separator ","

set title "Number of occurrences for significant sequence lengths\nusing significant co-occurrence sequences across 76 ORFeome\np-Value 1e-05"
plot 'sigOrfs_seq_length_p1e-05.txt' u ($2) title columnheader lc rgb "#000000"

#set auto x
set xrange [0:4000]
#set yrange [0:100]

set title "Number of occurrences for non-significant sequence lengths\nusing non-significant co-occurrence sequences across 76 ORFeome\np-Value 1e-05"
plot '' u ($3) title columnheader

#pause -1
