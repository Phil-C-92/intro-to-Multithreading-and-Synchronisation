#include <pthread.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    int threadCount;
    std::vector<fs::directory_entry>sortedDir;

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
            threadCount = std::stoi(argv[1]);
        }
    catch (std::invalid_argument)
        {
            std::cout << "Incorrect arguments passed in. Please try again using\n";
            std::cout << "./mmcopier n source_dir desination_dir \n";
            std::cout << "Where n is >= 2 and <= 10\n";

            return 0;
        }

//Set up paths to direcotries
    fs::path sourceDir = fs::absolute(argv[2]);
    fs::path destDir = fs::absolute(argv[3]);

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

//copy required number of files
    for(int i = 0; i < threadCount; i++)
        {
            fs::path originFile = sortedDir[i].path();
            fs::path destFile = destDir / sortedDir[i].path().filename();

            try {
                fs::copy_file(originFile, destFile, fs::copy_options::overwrite_existing); 
                }
            catch(fs::filesystem_error) 
                {
                std::cout << "Error with directory...\n";
                }
        }


    std::cout << "Arg 1: " << argv[0] << "\n";
    std::cout << "Arg 2: " << argv[1] << "\n";
    std::cout << "Arg 3: " << argv[2] << "\n";
    std::cout << "Arg 4: " << argv[3] << "\n";

    threadCount = std::stoi(argv[1]);

    std::cout << "Thread count : " << threadCount << "\n";

    return 0;

}