## algorithmscoursera

My coursework for the *Algorithms and Data Structures* specialization \
through UC San Diego and the National Research University Higher School \
of Economics. The course order is:
1. Algorithmic Toolbox
2. Data Structures
3. Algorithms on Graphs
4. Algorithms on Strings
5. Advanced Algorithms and Complexity
6. Keystone Project: Genome Assembly Programming Challenge

### Most Recent Work

[phi_X174_error_free_overlap.cpp](https://github.com/rydcormier/algorithmscoursera/blob/master/Genome%20Assembly%20Programming%20Challenge/Programming%20Assignment%201/phiX174_error_free_overlap.cpp)

From 1000 simulated error-free reads randomly drawn from 5,386-nucleotide \
long phi X174 genome, this program performs the task of Genome Assembly and \ 
returns the original circular genome. 

This implementation uses a generalized suffix tree to first solve the \
*All-Pairs Suffix-Prefix* problem, thus calculating the edge weights of \
an overlap graph. It then constructs a Hamilton path in a greedy fashion: \
for each read,select an outgoing edge of maximum weight. The label of this \
path-minus any overlap from the last read and the first-is returned, which \
is the circular genome from which the reads came.
