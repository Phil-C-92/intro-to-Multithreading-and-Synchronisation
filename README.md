# intro-to-Multithreading-and-Synchronisation
Intro to some basic examples of multithreaded programs and synchronisation in C++. An example of concurrency is shown with mmcopier; mscopier requires synchronicity utilising mutex locks in order to function correctly. The pthreads library or POSIX threads are used in the programs listed. 

# mmcopier
This file when compiled and run with the following command:

 ./mmcopier n ../source_dir ../desination_dir  

where n (2 <= n <= 10) is the number of files, corresponding to the number of threads created and number of files to be copied from "source_dir", to the target directory "destination_dir".

mmcopier is a rudimentary example of concurrency with each individual thread running at the same time. Each thread is copying all lines from one file into another independently before terminating. 

# mscopier 
When compiled and run with the following command: 

./mscopier n ../read.txt ../write.txt  

where n (2 <= n <= 10) is the number of threads that will be created for each function - read and write. 