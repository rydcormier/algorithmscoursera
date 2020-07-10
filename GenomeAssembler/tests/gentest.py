#!/usr/bin/env python3
#
# Generate a set of simulated error-prone reads from
# a simultaed genome.
# 
import random

num_reads = 1618;
read_length = 100;
fpin = '/Users/ryan/Repositories/algorithmscoursera/Genome Assembly Programming Challenge/Programming Assignment 3/genome2.txt'
fpout = '/Users/ryan/Repositories/algorithmscoursera/Genome Assembly Programming Challenge/Programming Assignment 3/reads2.txt'

# get the genome
with open( fpin, 'r' ) as f:
    s = f.read().strip()

length = len( s)

# the alphabet  
sigma = 'ACGT'

# double s to provide circular reads
s += s
reads = []
i = 0;

# start with generating reads at fixed intervals--using a prime 
# number--to ensure full coverage. Flip one "neucleotide" in the 
# read to simulate a 1% error rate.
while len( reads ) < 1000:
    read = s[ i : i + read_length ]
    pos = random.randrange( read_length )
    c = read[ pos ]
    while c == read[ pos ]:
        c = random.choice( sigma )
    reads.append( read[ : pos ] + c + read[ pos + 1 : ] )
    i = ( ( i + 137 ) % length )
    
# Generate the rest of the reads randomly; still flipping one neucleotide.
while len( reads ) < num_reads:
    i = random.randrange( length )
    read = s[ i : i + read_length ]
    pos = random.randrange( read_length )
    c = read[ pos ]
    while c == read[ pos ]:
        c = random.choice( sigma )
    reads.append( read[ : pos ] + c + read[ pos + 1 : ] )
    
# write the reads to a file
with open( fpout, 'w' ) as f:
    for read in reads:
        f.write( read + '\n' )
        
exit(0)