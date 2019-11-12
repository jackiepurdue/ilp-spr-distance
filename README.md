# ILP Formulation for Computing SPR Distances of Binary Rooted Phylogenetic Trees

## Summary

This code was developed for a project on the practical limits of an integer linear program for computing SPR distances of binary rooted phylogenetic trees for an Algorithm Engineering course project.

This project involves a discussion of the practical performance limits, and an examination of new constraints for an Integer Linear Program (ILP).
The ILP computes the minimum subtree prune and regraft (SPR) distance between two binary rooted phylogenetic trees. Several factors, such as the unpredictable behaviour of the branch and bound algorithm, make it difficult to draw conclusions about the performance of the ILP, but some features of the problem allow for analysis.

The cost of prepossessing the GLPK matrix is found to be costly. Experiments are performed to find the point at which the cost of generating the matrix begins to pay off.

This limit seems to occur for problem instances which have SPR distances above six, with a possible increase to this bound as the problem size increases.
Also, a new constraint is tested and found to have no significant effect on the performance of GLPK.

## Build:

-cmake the directory
-cd into one of the built subdirectories (src for the main program)
-make

-Dependency: GLPK
-Dependency: Google Benchmarks, for benchmarking

## Run:

./cspr < tree_file.txt

Tree file examples in tests/trees directory

//TODO:: Make sure no infinite loops-- handle all parsing errors
//TODO:: Refactor code/tests
//TODO:: Investigate invalid reads from valgrind-- no idea what this is about
//TODO:: Make option to set memory usage

## Scripts:
the root directory contains scripts which can modify directory of tree files and format them for benchmarking with this
program

## Warnings:
- If you are wondering about the strange pattern used for the classes, I was experimenting with C++ and the "PIML" pattern.
I was also experimenting with immutable trees, and they found their way in to this project (they are, by the way, no longer immutable).
- Memory allocation  for GLP matrix not very thoughtfuly tested but seems to work.
- As per the TODOs above, make sure there are no spaces in Newick trees and make sure they end with semicolons.
- Running statistics.c will result in error if rspr is not in $PATH as 'rspr'

## Code Sources:
- Sparse table implementation from https://gist.github.com/Break-Neck/9570328 and adapted to return its indicies

## References:

Yufeng Wu, A practical method for exact computation of subtree prune and regraft distance, Bioinformatics, Volume 25, Issue 2, 15 January 2009, Pages 190–196, https://doi.org/10.1093/bioinformatics/btn606

Whidden, C., Beiko, R.G., Zeh, N.  Computing the SPR Distance of Binary
Rooted Trees in O(2^k n) Time. (In Preparation). 2013.

Whidden, C., Beiko, R.G. Zeh, N.  Fixed-Parameter and Approximation
Algorithms for Maximum Agreement Forests of Multifurcating Trees.
(Submitted). 2013.

Whidden, C., Zeh, N., Beiko, R.G.  Supertrees based on the subtree
prune-and-regraft distance. Syst. Biol. 63 (4): 566-581. 2014.
doi:10.1093/sysbio/syu023.

Whidden, C., Beiko, R.G., Zeh, N. Fixed-Parameter Algorithms for Maximum
Agreement Forests. SIAM Journal on Computing 42.4 (2013), pp. 1431-1466.
Available at http://epubs.siam.org/doi/abs/10.1137/110845045

Whidden, C., Beiko, R.G., Zeh, N. Fast FPT Algorithms for Computing
Rooted Agreement Forests: Theory and Experiments. Experimental Algorithms.
Ed. by P. Festa. Vol. 6049. Lecture Notes in Computer Science. Springer
Berlin Heidelberg, 2010, pp. 141-153. Available at
http://link.springer.com/chapter/10.1007/978-3-642-13193-6_13

Whidden, C., Zeh, N. A Unifying View on Approximation and FPT of
Agreement Forests. In: WABI 2009. LNCS, vol. 5724, pp. 390.401.
Springer-Verlag (2009).
