#include <pthread.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>

namespace fs = std::filesystem;

//structure for each thread
struct mcThread
    {
        int threadID;
        int index;
        fs::path sDir;
        fs::path dDir;
    };

//thread function
void *fileReader(void* args)
    {
        struct mcThread* myArgs = (struct mcThread *)args;
        //int threadID = myArgs->threadID;

        //std::cout << "Thread id : " << threadID << " created and passed into func\n";

        fs::path sDir = myArgs->sDir;
        fs::path dDir = myArgs->dDir;

         try {
                fs::copy_file(sDir, dDir, fs::copy_options::overwrite_existing); 
            }
        catch(fs::filesystem_error) 
            {
            std::cout << "Error with directory...\n";
            }

        return NULL;
    }


int main(int argc, char* argv[]) {

    int nThreads, success;
    std::vector<fs::directory_entry>sortedDir;
    struct mcThread* thread_args;
    pthread_t* threadArrayID;
    

    std::cout << "Number of arguments passed in: " << argc << "\n";

//make sure correct number of arguments are passed in
    if(argc != 4)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            std::cout << "./mmcopier n source_dir desination_dir \n";
            std::cout << "Where n is >= 2 and <= 10\n";
            return 0;
        }
//convert int from string to int from passed in arguments
    try 
        {
            nThreads = std::stoi(argv[1]);
        }
    catch (std::invalid_argument)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            std::cout << "./mmcopier n source_dir desination_dir \n";
            std::cout << "Where n is >= 2 and <= 10\n";
            return 0;
        }
//assign threadcount argument passed in
    nThreads = std::stoi(argv[1]);

//set up paths to direcotries
    fs::path sourceDir = fs::absolute(argv[2]);
    fs::path destDir = fs::absolute(argv[3]);
    // fs::path* sDir = &sourceDir;
    // fs::path* dDir = &destDir;


//populate vector with files
    for(const fs::directory_entry& file : fs::directory_iterator(sourceDir))
        {
            std::string fname = file.path().filename().string();
            if(!fname.empty() && fname[0] != '.')
                sortedDir.push_back(file);
        }

//sort alphabetically and ensure index 10 is at the back
    std::sort(sortedDir.begin(), sortedDir.end());
    fs::directory_entry temp = sortedDir[1];
    sortedDir.erase(sortedDir.begin() + 1);
    sortedDir.push_back(temp);

//initalise size of the array for thread arguments and ID
    thread_args = new mcThread[nThreads];
    threadArrayID = new pthread_t[nThreads];

//populate the arguments we'll pass in and create thread
    for(int i = 0; i < nThreads; i++)
        {
            thread_args[i].threadID = i;
            thread_args[i].index = i;
            thread_args[i].sDir = sortedDir[i].path();
            thread_args[i].dDir = destDir / sortedDir[i].path().filename();

            success = pthread_create(&threadArrayID[i], NULL, fileReader, &thread_args[i]);
            if(success)
                {
                    std::cout << "Failed to create thread number : " << i << "\n";
                }
        }

    for(int i = 0; i < nThreads; i++)
        {
            success = pthread_join(threadArrayID[i], NULL);
            if(success)
                {
                    std::cout << "ERROR: pthread join failed\n";
                }
        }

    std::cout << "Thread count : " << nThreads << "\n";

    delete [] thread_args;
    delete [] threadArrayID;

    return 0;

}