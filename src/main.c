#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "shad.h"

static struct shad_opts parse_arguments(const int argc, const char **argv);

int main(const int argc, const char **argv)
{
    const struct shad_opts opts = parse_arguments(argc, argv);
    init(opts);
    run();
    quit();
}

static struct shad_opts parse_arguments(const int argc, const char **argv)
{

    const char *flags = "h:w:f:o:";
    int c;

    struct shad_opts opts;
    opts.canvas_height = 0;
    opts.canvas_width = 0;
    if (argc == 2) {
        opts.filename = argv[1];
        return opts;
    } else {
        while((c = getopt(argc, (char *const*)argv, flags)) != -1) {
            switch(c) {
            case 'h':
                opts.canvas_height = strtol(optarg, NULL, 10);
                break;
            case 'w':
                opts.canvas_width = strtol(optarg, NULL, 10);
                break;
            case 'f':
                opts.filename = optarg;
                break;
            case 'o':
                opts.filename = optarg;
                break;
            }
        }
        return opts;
    }
}
