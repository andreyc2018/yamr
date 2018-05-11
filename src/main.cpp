#include "filedivider.h"
#include "logger.h"
#include <iostream>

int main(int argc, char const** argv)
{
    try {
        const std::string app =
                std::string(argv[0]).substr(std::string(argv[0]).rfind("/") + 1);
        if (argc < 4) {
            std::cout << "usage: "
                      << app
                      << " <file> <mnum> <rnum>\n"
                         "where:\n"
                         "  file - file with input data\n"
                         "  mnum - number of mappers\n"
                         "  rnum - number of reducers\n";
            exit(1);
        }
        if (argc > 4) {
            gLogger->set_level(spdlog::level::debug);
        }

        const std::string file(argv[1]);
        int mnum = std::stoi(argv[2]);
        int rnum = std::stoi(argv[3]);
        std::cout << "Starting " << app << " with " << mnum << " mappers and "
                  << rnum << " reducers.\n";


    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
