#!/usr/bin/python

import os

os.system("sed -i 's/_p1e-05/_p\.0001/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.0001\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p\.0001/_p\.001/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.001\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p\.001/_p\.01/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.01\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p\.01/_p\.05/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.05\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p\.05/_p1e-05/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value 1e-05\"/g' main.cpp")
os.system("make")
os.system("./leRCC")
