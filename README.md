# intro-to-Multithreading-and-Synchronisation
Intro to some basic examples of multithreaded programs and synchronisation

# mmcopier
This file when compiled with the following command:

 ./mmcopier n ../source_dir ../desination_dir  

where n (2 <= n <= 10) is the number of files, which corresponds to the number of threads used, to be copied under source_dir, to the target directory destination_dir.

# structure of thread
Each thread will have a threadID, index, path to source directory and destination directoy. The index is simply which number thread was created