#!/usr/bin/python

import os

os.system("sed -i 's/_p.*\.txt/_p\.0001\.txt/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.0001\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p.*\.txt/_p\.001\.txt/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.001\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p.*\.txt/_p\.01\.txt/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.01\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p.*\.txt/_p\.05\.txt/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value \.05\"/g' main.cpp")
os.system("make")
os.system("./leRCC")

os.system("sed -i 's/_p.*\.txt/_p1e-05\.txt/g' main.cpp")
os.system("sed -i 's/p-Value .*\"/p-Value 1e-05\"/g' main.cpp")
os.system("make")
os.system("./leRCC")
