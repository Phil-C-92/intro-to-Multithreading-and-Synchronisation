#include <pthread.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

int main(int argc, char* argv[]) {

    int threadCount;

    std::cout << "Number of arguments passed in: " << argc - 1 << "\n";

    if(argc < 4)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            std::cout << "./mmcopier n source_dir desination_dir \n";
            std::cout << "Where n is >= 2 and <= 10\n";

            return 0;
        }

    try 
        {
            threadCount = std::stoi(argv[1]);
        }
    catch (std::invalid_argument)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            std::cout << "./mmcopier n source_dir desination_dir \n";
            std::cout << "Where n is >= 2 and <= 10\n";

            return 0;
        }

    

    std::cout << "Arg 1: " << argv[0] << "\n";
    std::cout << "Arg 2: " << argv[1] << "\n";
    std::cout << "Arg 3: " << argv[2] << "\n";
    std::cout << "Arg 4: " << argv[3] << "\n";

    int threadCount = std::stoi(argv[1]);

    std::cout << "Thread count : " << threadCount << "\n";

    return 0;

}