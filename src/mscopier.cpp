#include <iostream>
#include <string>
#include <fstream>
#include <pthread.h>

struct thread_args{

    int threadID;
    int* counterIn;
    int* counterOut;
    
    std::string* buffer;

    std::ifstream* inFS;
    std::ofstream* outFS;

    pthread_mutex_t* mutex;
    pthread_cond_t* mutexCondition;

};

void* read(void* args)
    {
        struct thread_args* myArgs = (struct thread_args *)args;

        return NULL;
    }

void* write(void* args)
    {
        struct thread_args* myArgs = (struct thread_args *)args;

        return NULL;
    }

int main(int argc, char* argv[]) {

    int counterIn = 0, counterOut = 0, nThreads; //this will need to be *2 for reader and writer

    std::ifstream inFS; 
    std::ofstream outFS; 

    pthread_mutex_t mutex;
    pthread_cond_t mutexCondition;

    // std::string readFile = argv[2];
    // std::string writeFile = argv[3];

    struct thread_args* tStructure;
    pthread_t* tid;
    std::string* buffer = new std::string[20];

//make sure correct number of arguments are passed in
    if(argc != 4)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            return EXIT_FAILURE;
        }
//convert int from string to int from passed in arguments
    try 
        {
            nThreads = std::stoi(argv[1]);
        }
    catch (std::invalid_argument)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            return EXIT_FAILURE;
        }
//assign threadcount argument passed in
    nThreads = std::stoi(argv[1]);
    nThreads *= 2;

//initalise arrays
    tStructure = new thread_args[nThreads];
    tid = new pthread_t[nThreads];
    
//this will end up needed to be in void* read(void* args)
//open read file    
    inFS.open(argv[2]);
    if(!inFS.is_open())
        {
            std::cout << "ERROR opening file : " << argv[2] << "\n";
            return EXIT_FAILURE;
        }

//open write file
    outFS.open(argv[3]);
    if(!outFS.is_open())
        {
            std::cout << "ERROR opening file : " << argv[3] << "\n";
            return EXIT_FAILURE;
        }

//populate thread structure and create threads
    for(int i = 0; i < nThreads; i++)
        {
            tStructure[i].threadID = i;
            tStructure[i].counterIn = &counterIn;
            tStructure[i].counterOut = &counterOut;
            tStructure[i].buffer = buffer;
            tStructure[i].inFS = &inFS;
            tStructure[i].outFS = &outFS;
            tStructure[i].mutex = &mutex;
            tStructure[i].mutexCondition = &mutexCondition;

            if(i % 2 == 0)
                {
                    pthread_create(&tid[i], NULL, read, &tStructure[i]);
                }
            else
                {
                    pthread_create(&tid[i], NULL, write, &tStructure[i]);
                }
        }





//scan each line of the file 
    while(std::getline(inFS, line) && count != (nThreads/2))
        {
            std::cout << line << "\n";
            //write each line to the output file
            outFS << line << "\n";
            //outFS << "\n";
            count++;
        }

    inFS.close();
    outFS.close();

    //delete inFS;
    //delete outFS;

    return 0;
}