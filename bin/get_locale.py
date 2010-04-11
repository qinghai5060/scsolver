#!/usr/bin/env python

import sys

if len(sys.argv) < 2:
    sys.exit(1)

file = open(sys.argv[1], 'r')
lines = file.readlines()
file.close()
words = lines[1].split("\t")
print words[9]
