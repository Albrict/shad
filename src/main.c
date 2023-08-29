#include "shad.h"
#include <stdlib.h>

int main(const int argc, const char **argv)
{
    if (argc < 2)
        init(NULL);
    else
        init(argv[1]);
    run();
    quit();
}
