//
// Created by Ataberk Dönmez on 22.04.2022.
//

#include <getopt.h>
#include <iostream>

void processArgs(int argc, char** argv) {
    const char* const short_opts = "p:";
    const option long_opts[] = {
            {"prefix", required_argument, nullptr, 'p'},
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
            case 'p':
                std::cout << optarg << std::endl;
                break;
    }

}


int main(int argc, char **argv) {


}