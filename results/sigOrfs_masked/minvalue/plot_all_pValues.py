#!/usr/bin/python

import os

os.system("sed -i 's/_p1e-05/_p\.0001/g' plot_minvalue.gp")
os.system("sed -i 's/p-Value .*\"/p-Value \.0001\"/g' plot_minvalue.gp")
os.system("gnuplot plot_minvalue.gp")
os.system("ps2pdf sig_pruned_masked_p.0001_minvalue.ps sig_pruned_masked_p.0001_minvalue.pdf")

os.system("sed -i 's/_p\.0001/_p\.001/g' plot_minvalue.gp")
os.system("sed -i 's/p-Value .*\"/p-Value \.001\"/g' plot_minvalue.gp")
os.system("gnuplot plot_minvalue.gp")
os.system("ps2pdf sig_pruned_masked_p.001_minvalue.ps sig_pruned_masked_p.001_minvalue.pdf")

os.system("sed -i 's/_p\.001/_p\.01/g' plot_minvalue.gp")
os.system("sed -i 's/p-Value .*\"/p-Value \.01\"/g' plot_minvalue.gp")
os.system("gnuplot plot_minvalue.gp")
os.system("ps2pdf sig_pruned_masked_p.01_minvalue.ps sig_pruned_masked_p.01_minvalue.pdf")

os.system("sed -i 's/_p\.01/_p\.05/g' plot_minvalue.gp")
os.system("sed -i 's/p-Value .*\"/p-Value \.05\"/g' plot_minvalue.gp")
os.system("gnuplot plot_minvalue.gp")
os.system("ps2pdf sig_pruned_masked_p.05_minvalue.ps sig_pruned_masked_p.05_minvalue.pdf")

os.system("sed -i 's/_p\.05/_p1e-05/g' plot_minvalue.gp")
os.system("sed -i 's/p-Value .*\"/p-Value 1e-05\"/g' plot_minvalue.gp")
os.system("gnuplot plot_minvalue.gp")
os.system("ps2pdf sig_pruned_masked_p1e-05_minvalue.ps sig_pruned_masked_p1e-05_minvalue.pdf")

os.system("rm *.ps")
