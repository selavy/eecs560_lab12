eecs560_lab12
=============

Lab 12: Tarjan's Algorithm for finding strongly connected vertices
Objective: Find the articulation points in an undirected graph.

Nota Bene: There seems to be a mistake with the first input file
because it is not a connected graph.

BUILD
=====
"make"

Nota Bene: You will need a c++11 compiler because the code uses lambdas

RUN
===
(after running make successfully)
"./lab12 [FILE]"

The output was verified with the Boost.Graph library.  That code
is in "./Verification/".  CMake, and the Boost libraries are required
to build that code.  To build:
"cd Verification"
"mkdir build && cd build && cmake .."
"make"

The output from the verification program can be redirected to a file,
which can be visualized using GraphViz.

Testing was done with both Thursday and Tuesday lab's test data, which
is in input.txt and input2.txt, respectively.