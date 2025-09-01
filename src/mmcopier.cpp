#include <pthread.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib> 
#include <stdexcept>

//structure for each thread
struct mcThread
    {
        int threadID;

        //each thread has its own file so no need for locks
        std::ifstream inFS;
        std::ofstream outFS;
        std::string sourceDir;
        std::string destDir;

    };

void *fileReader(void* args);

int main(int argc, char* argv[]) {

    int nThreads, success;
   
    std::string fileName = "/source.txt";
    std::string sourceDir = "";
    std::string destDir = "";

    struct mcThread* thread_args;
    pthread_t* threadArrayID;

    
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
            nThreads = std::atoi(argv[1]);
        }
    catch (std::invalid_argument& e)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            std::cout << "./mmcopier n source_dir desination_dir \n";
            std::cout << "Where n is >= 2 and <= 10\n";
            return EXIT_FAILURE;
        }

    //check the correct number of threads were entered
    if(nThreads < 2 || nThreads > 10)
        {
            std::cout << "You've entered incorrect number of threads,\n";
            std::cout << "Please try again with a number between 2 and 10.\n";
            return EXIT_FAILURE;
        }

    //set up dir variables
    sourceDir = argv[2];
    destDir = argv[3];
    
    //initalise size of the array for thread arguments and ID
    thread_args = new mcThread[nThreads];
    threadArrayID = new pthread_t[nThreads];
    
    //make sure the directories exist
    std::ifstream outFS;
    std::ifstream inFS;
    outFS.open(destDir);
    if(!outFS.is_open())
        {
            std::cout << "ERROR : Destination direcetory either doesnt exist or is not in this location\n";
            std::cout <<"Please try again\n";
            return EXIT_FAILURE;
        }
    outFS.close();
    inFS.open(sourceDir);
    if(!inFS.is_open())
        {
            std::cout << "ERROR : Source direcetory either doesnt exist or is not in this location\n";
            std::cout <<"Please try again\n";
            return EXIT_FAILURE;
        }
    inFS.close();

    //populate the arguments we'll pass into the thread
    for(int i = 0; i < nThreads; i++)
        {
            //set up temp variables to be passed in as source and dest file names/locations            
            std::string threadFile = fileName;
            std::string threadSDir = sourceDir;
            std::string threadDDir = destDir;

            //update file name with each index required
            threadFile.insert(7, std::to_string(i+1)); 
            threadSDir += threadFile;
            threadDDir += threadFile;

            //populate required thread arguments
            thread_args[i].threadID = i;
            thread_args[i].sourceDir = threadSDir;
            thread_args[i].destDir = threadDDir;

            //create thread
            success = pthread_create(&threadArrayID[i], NULL, fileReader, &thread_args[i]);

            //make sure thread was created successuflly
            if(success)
                {
                    std::cout << "Failed to create thread number : " << i << "\n";
                }
        }

    //make sure threads finish executing before the end of the program
    for(int i = 0; i < nThreads; i++)
        {
            success = pthread_join(threadArrayID[i], NULL);
            if(success)
                {
                    std::cout << "ERROR: pthread join failed\n";
                }
        }

    //clear memory
    delete [] thread_args;
    delete [] threadArrayID;

    return EXIT_SUCCESS;
}

//thread function
void *fileReader(void* args)
    {
        struct mcThread* myArgs = (struct mcThread *)args;
        std::string line;

        //open files     
        myArgs->inFS.open(myArgs->sourceDir);
        myArgs->outFS.open(myArgs->destDir);

        //make sure files open without error
        if(!myArgs->inFS.is_open())
            {
                std::cout << "ERROR : unable to open file in : " << myArgs->sourceDir << "\n";
                std::cout << "thread : " << myArgs->threadID << "\n";
                return NULL;
            }

        if(!myArgs->outFS.is_open())
            {
                std::cout << "ERROR : unable to open  destination direcetory : " << myArgs->destDir << "\n";
                std::cout << "thread : " << myArgs->threadID << "\n";
                return NULL;
            }
        //write to output file until the end of input file
        while(std::getline(myArgs->inFS, line))
            {
                myArgs->outFS << line << "\n";
            }
        //close files
        myArgs->inFS.close();
        myArgs->outFS.close();
        
        return NULL;
    }