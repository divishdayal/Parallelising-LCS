This project contains the sequential as well as parrallel(multi-threaded) code for calculating LCS(longest common subsequence) 
of various documents and finally printing the intersection in these documents where each line is considered as one sequence for calculating LCS/intersection.

There are 1000 documents in the folder. You can test for any number of it. Max lines in any file is around 1000.
Problem 1 has been implemented using PThread library.

To compile :
	Sequential
		gcc pc.c
		time ./a.out <no of documents> <value of K>

	Parallel
		gcc parallel_pc.c -lpthread
		time ./a.out <no of documents> <value of K> <no of threads>

The files should be in the same folder and named as 1.txt, 2.txt and so on.

Both the codes give out put as -
LCS of k pairs printed
intersection lines
docs included in the k files selected
total intersections found


results : 

N	K	Sequential	Parallel	Speedup
70	7	11.92		6.02(16)	1.98
70	7			6.02(4)		1.98
70	7			6.32(3)		1.88
70	7			6.86(2)		1.737
100	15	36.31		18.45(16)	1.96
100	15			18.13(4)	2
100	15			19.09(3)	1.9
100	15			20.63(2)	1.76

NOTE : In parallel column, brackets tell the number of threads used to execute. Tested on MacBook Air(1.6 gHz core i5, 8gb Ram)
