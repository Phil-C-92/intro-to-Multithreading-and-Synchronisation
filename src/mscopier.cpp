#include <iostream>
#include <string>
#include <fstream>
#include <pthread.h>

//g++ -Wall -Werror -std=c++17 -o mscopier mscopier.cpp -lpthread

struct thread_args{

    int threadID;
    int* full; 
    int* producerIndex; 
    int* consumerIndex;
    bool* finishedReading;
    std::string* buffer;

    std::ifstream* inFS;
    std::ofstream* outFS;

    pthread_mutex_t* mutex;
    pthread_cond_t* notFull;
    pthread_cond_t* notEmpty;

};

void* read(void* args)
    {
        struct thread_args* myArgs = (struct thread_args *)args;

        //make sure the file is empty
        while(!*myArgs->finishedReading)
            {

        //lock mutex           
        pthread_mutex_lock(myArgs->mutex);
        while(*myArgs->full == 20)
            {
                pthread_cond_broadcast(myArgs->notEmpty);
                pthread_cond_wait(myArgs->notFull, myArgs->mutex);
            }
        //read the next word or line from the file
        std::string nextWord;
       
        //check for end of file
        if (!(std::getline(*myArgs->inFS, nextWord))) 
            {
                *myArgs->finishedReading = true;
                //wake up all consumer threads
                pthread_cond_broadcast(myArgs->notEmpty);
                pthread_mutex_unlock(myArgs->mutex);
                return NULL;
            }
        //insert word into buffer
        myArgs->buffer[*myArgs->producerIndex] = nextWord;

        //increment producer index in buffer keeping it between 0-19
        *myArgs->producerIndex = (*myArgs->producerIndex +1) % 20;

        //update buffer counters
        *myArgs->full += 1;

        //signal consumer thread
        pthread_cond_signal(myArgs->notEmpty);

        //unlock mutex
        pthread_mutex_unlock(myArgs->mutex);

        }

        return NULL;
    }

void* write(void* args)
    {
        struct thread_args* myArgs = (struct thread_args *)args;

        //check that the file is empty
        while(!*myArgs->finishedReading || *myArgs->full > 0)
            {

        pthread_mutex_lock(myArgs->mutex);
        //check if there is 0 empty slots meaning buffer is full
        while(*myArgs->full == 0)
            {
                //check if the file has finished
                if (*myArgs->finishedReading && *myArgs->full == 0)
                    {
                        pthread_cond_broadcast(myArgs->notEmpty);
                        pthread_mutex_unlock(myArgs->mutex);
                        return NULL; 
                    }
                //if file hasnt finished then wait
                pthread_cond_wait(myArgs->notEmpty, myArgs->mutex);
            }

        if(myArgs->buffer[*myArgs->consumerIndex][0] != '\0')
            {
                //read the next word from the buffer
                std::string word = myArgs->buffer[*myArgs->consumerIndex];

                //delete word from buffer
                myArgs->buffer[*myArgs->consumerIndex].clear();

                //write word to output file
                *myArgs->outFS << word << "\n";

                //update the consumer index 
                *myArgs->consumerIndex = (*myArgs->consumerIndex +1) % 20;

                //update buffer counters
                *myArgs->full -= 1;

                //signal producer thread
                pthread_cond_signal(myArgs->notFull);
            }

            //unlock mutex
            pthread_mutex_unlock(myArgs->mutex);
        }

        return NULL;
    }

int main(int argc, char* argv[]) {

    int full = 0, producerIndex = 0, consumerIndex = 0, nThreads, success; 
    bool finRead = false;
    std::ifstream inFS; 
    std::ofstream outFS; 

    //create and initialise locks and conditions
    pthread_mutex_t mutex;
    pthread_cond_t nEmpty;
    pthread_cond_t nFull;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&nEmpty, NULL);
    pthread_cond_init(&nFull, NULL);

    struct thread_args* tStructure;
    pthread_t* tid;
    std::string* buffer = new std::string[20];

    //make sure correct number of arguments are passed in
    if(argc != 4)
        {
            std::cout << "Incorrect arguments passed in. Please try again\n";
            return EXIT_FAILURE;
        }

    //convert int from string to int from passed in arguments
    try 
        {
            nThreads = std::stoi(argv[1]);
        }
    catch (std::invalid_argument)
        {
            std::cout << "Incorrect arguments passed in. Please try again.\n";
            return EXIT_FAILURE;
        }
    //account for read and write threadcount
    nThreads *= 2;

    //initalise arrays
    tStructure = new thread_args[nThreads];
    tid = new pthread_t[nThreads];
    
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
            tStructure[i].full = &full;
            tStructure[i].producerIndex = &producerIndex;
            tStructure[i].consumerIndex = &consumerIndex;
            tStructure[i].buffer = buffer;
            tStructure[i].inFS = &inFS;
            tStructure[i].outFS = &outFS;
            tStructure[i].mutex = &mutex;
            tStructure[i].notEmpty = &nEmpty;
            tStructure[i].notFull = &nFull;
            tStructure[i].finishedReading = &finRead;

            if((i+1) % 2 == 0)
                {
                    success = pthread_create(&tid[i], NULL, write, &tStructure[i]);
                }
            else
                {
                    success = pthread_create(&tid[i], NULL, read, &tStructure[i]);
                }
            //make sure no issues creating threads    
            if (success)
                {
                    std::cout << "ERROR: pthread join failed\n";
                } 
            
        }

    for (int i = 0; i < nThreads; i++) 
        {
            success = pthread_join(tid[i], NULL);
            if (success)
                {
                    std::cout << "ERROR: pthread join failed\n";
                } 
        }

    //close files
    inFS.close();
    outFS.close();

    //clear memory
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&nEmpty);
    pthread_cond_destroy(&nFull);
    delete [] tStructure;
    delete [] tid;
    delete [] buffer;

    return EXIT_SUCCESS;
}

