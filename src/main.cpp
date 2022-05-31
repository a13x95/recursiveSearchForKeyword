
#include "../inc/fileSearch.hpp"
#include "../inc/timer.hpp"

/// @brief Main function
///
/// @param
///     argc - int which stores number of command-line arguments passed by the user
/// @param
///     argv - array of character pointers listing all the arguments
///
int main(int argc, char* argv[])
{
    Timer* timer = new Timer();

    std::string file = "";
    std::string toSearch = "";
    if (argc == 3) {
        file = argv[1];
        toSearch = argv[2];
        FileSearch searchObj(file, toSearch);
        searchObj.process();
    } else {
        std::cout << "Wrong number of arguments provided!" << std::endl;
    }

    delete timer;

    return 0;
}